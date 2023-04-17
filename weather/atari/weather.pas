program weather;
{$librarypath '../blibs/'}
// get your blibs here: https://gitlab.com/bocianu/blibs
uses atari, sysutils, crt, b_crt, fn_tcp, fn_sio, sio, fn_cookies;
{$r resources.rc}
{$i-}

const
{$i const.inc}
{$i datetime.inc}
{$i types.inc}

const
    APPKEY_CREATOR_ID = $b0c1;
    APPKEY_APP_ID = $1;
    APPKEY_CONFIG_KEY = $c0;
    APPKEY_LOCATION_KEY = $10;

var IP_api: string[15] = 'api.ipstack.com';
    OW_api: string[22] = 'api.openweathermap.org';
    OC_api: string[20] = 'api.opencagedata.com';
    
    getLine: string;    
    responseBuffer: array [0..4095] of byte absolute JSON_BUFFER;
    
    imperialCCodes: array [0..7] of string[2] = ('US', 'GB', 'IN', 'IE', 'CA', 'AU', 'HK', 'NZ');
    windDir: array [0..7] of string[2] = ('N ', 'NE', 'E ', 'SE', 'S ', 'SW', 'W ', 'NW');
    monthNames: array [0..11] of string[5] = (' Jan ', ' Feb ', ' Mar ', ' Apr ', ' May ', ' Jun' , ' Jul ', ' Aug ', ' Sep ',' Oct ',' Nov ',' Dec ');
    dowNames: array [0..6] of string[3] = ('Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat');
    weatherDescLen: byte;

    city, tmp: string[40];
    country_code: string[3];
    region_code: string[3];
    longitude, latitude: string[20];
    defaultApiKey: string[32] = '2e8616654c548c26bc1c86b1615ef7f1';
    
    descDir, descOffset, descScroll, descHSC: byte;

    curDate, sunriseDate, sunsetDate: TDateTime;
    refreshCounter, refreshFrames, unixTime: cardinal;
    timezone: integer;
    
    forecastPtrs: array [0..7] of word;

    scrWidth: byte;
    i, menuDelay: byte;
    statusDelay: word;
    timeShown: boolean;
    clockCount: word;
    k: char;
    page: byte;
    
    iconsD: array [0..8] of word = (
        0 * 10 + 0 * (40 * 24) + GFX, 
        1 * 10 + 0 * (40 * 24) + GFX, 
        2 * 10 + 0 * (40 * 24) + GFX,
        3 * 10 + 0 * (40 * 24) + GFX,
        0 * 10 + 1 * (40 * 24) + GFX,
        1 * 10 + 1 * (40 * 24) + GFX,
        2 * 10 + 1 * (40 * 24) + GFX,
        3 * 10 + 1 * (40 * 24) + GFX,
        0 * 10 + 2 * (40 * 24) + GFX
    );
    iconsN: array [0..8] of word = (
        1 * 10 + 2 * (40 * 24) + GFX,
        2 * 10 + 2 * (40 * 24) + GFX,
        2 * 10 + 0 * (40 * 24) + GFX,
        3 * 10 + 0 * (40 * 24) + GFX,
        0 * 10 + 1 * (40 * 24) + GFX,
        3 * 10 + 2 * (40 * 24) + GFX,
        2 * 10 + 1 * (40 * 24) + GFX,
        3 * 10 + 1 * (40 * 24) + GFX, 
        0 * 10 + 2 * (40 * 24) + GFX
    );

    logo: array [0..13*4-1] of byte = (
        $00, $00, $00, $00, $00, $5a, $5b, $5c, $00, $00, $00, $00, $00, 
        $40, $41, $42, $43, $44, $54, $55, $56, $4a, $4b, $4c, $4d, $4e, 
        $45, $46, $47, $48, $49, $57, $58, $59, $4f, $50, $51, $52, $53, 
        $00, $00, $00, $00, $00, $00, $5d, $5e, $5f, $00, $00, $00, $00
    );

    colorsNTSC: array [0..PALLETE_MAX] of byte;
    colorsPAL:  array [0..PALLETE_MAX] of byte;
    framesPerMinute: word;
    fps: byte;
    colors: array [0..0] of byte;
    cityColor, textColor, menuColor: byte;

    options: TOptions;  
    location: TLocation;
    theme: TTheme;
    savingEnabled: boolean;
  
{$i interrupts.inc}    
{$i json.inc}    

    
// ***************************************************** HELPERS    

procedure ScreenOff;
begin
    sdmctl := sdmctl and %11111100;
end;

procedure ScreenOn;
begin
    sdmctl := sdmctl or %00100010;
end;

procedure MergeStr(var s1:string;s2:string);
var l1,l2:byte;
begin
    l1 := Length(s1);
    l2 := Length(s2);
    s1[0] := char(l1 + l2);
    while l2>0 do begin
        s1[l1+l2] := s2[l2];
        dec(l2);
    end;
end;
    
function isCCImperial(var cc:TString):boolean;
var i:byte;
    ic:string[2];
begin
    result := false;
    for i := 0 to 7 do begin
        ic := imperialCCodes[i];
        if (ic[1] = cc[1]) and (ic[2] = cc[2]) then exit(true);
    end;
end;

function GetDirIndex(angle: word):byte;
begin
    result := Round(angle / 45.0) mod 8;
end;

function GetIconPtr(var icon:Tstring): word;
var iconNum,iconId:byte;
begin
    iconNum := (ord(icon[1])-48)*10 + (ord(icon[2])-48);
    iconId := 0;
    case iconNum of
         1: iconId := 0;
         2: iconId := 1;
         3: iconId := 2;
         4: iconId := 3;
         9: iconId := 4;
        10: iconId := 5;
        11: iconId := 6;
        13: iconId := 7;
        50: iconId := 8;
    end;
    result := iconsD[iconId];
    if icon[3] = 'n' then result := iconsN[iconId];
end;

procedure MergeCityWithCC;
begin
    getLine[0] := #0;
    MergeStr(getLine, city);    
    MergeStr(getLine, ', ');
    if options.showRegion then begin
        MergeStr(getLine, region_code);
        MergeStr(getLine, ', ');
    end;
    MergeStr(getLine, country_code);
    if Length(getLine) > 40 then setLength(getLine, 40); 
end;

procedure ConvertHPA2INHG(var tmp:string);
var p:word;
begin
    p := StrToInt(tmp);
    p := p * 3;
    Str(p, tmp);
    tmp[5]:=tmp[4];
    tmp[4]:=tmp[3];
    tmp[3]:='.';
    Inc(tmp[0]);
end;

procedure SwapUnits;
begin
    if options.units = metric then options.units := imperial else options.units := metric;
end;

function IsKeyCustom: boolean;
begin
    result := Length(options.apiKeyOW) = 32;
end;

procedure UpdateRefreshFrames;
begin
    refreshFrames := options.refreshInterval * 3000;
    refreshCounter := 0;
end;

// ********************************************************* DATA PARSERS

procedure GetTimezone;
begin
    GetJsonKeyValue('timezone_offset', tmp);
    timezone := StrToInt(tmp);    
end;

procedure ParseLocation;
begin
    GetJsonKeyValue('ip', tmp);
    GetJsonKeyValue('city', city);
    utfNormalize(city);
    GetJsonKeyValue('country_code', country_code);
    GetJsonKeyValue('region_code', region_code);
    GetJsonKeyValue('latitude', latitude);
    GetJsonKeyValue('longitude', longitude);
end;

procedure ParseWeather;
begin
    GetTimezone;
    GetJsonKeyValue('dt', tmp);
    unixTime := StrToInt(tmp) + timezone;
    UnixToDate(unixtime, curDate);
    clockCount := CurDate.second * fps;
end;

procedure ParseForecast;
var i:byte;
begin
    GetTimezone;
    FollowKey('daily');
    for i := 0 to 7 do forecastPtrs[i] := FindIndex(i);
end;

// ***************************************************** OPTIONS ROUTINES

procedure SetLoadedLocation;
begin
    city := location.city;
    region_code := location.region_code;
    country_code := location.country_code;
    latitude := location.latitude;
    longitude := location.longitude;
end;

function LoadLocation: byte;
begin
    InitCookie(APPKEY_CREATOR_ID, APPKEY_APP_ID, APPKEY_LOCATION_KEY);
    result := GetCookie(location);
end;

procedure StrCpyMax(var s1,s2:string; len:byte);
var slen: byte;
begin
    slen := Length(s1);
    if slen > len then slen := len;
    Move(@s1[1], @s2[1], slen);
    s2[0] := char(slen);
end;

function SaveLocation: byte;
begin
    location.city := city;
    location.region_code := region_code;
    location.country_code := country_code;
    StrCpyMax(latitude, location.latitude, 7);
    StrCpyMax(longitude, location.longitude, 7);

    InitCookie(APPKEY_CREATOR_ID, APPKEY_APP_ID, APPKEY_LOCATION_KEY);
    result := $ff;
    if savingEnabled then result := SetCookie(location, SizeOf(TLocation));
    if result <> 1 then savingEnabled := false;
end;


function LoadOptions: byte;
begin
    InitCookie(APPKEY_CREATOR_ID, APPKEY_APP_ID, APPKEY_CONFIG_KEY);
    result := GetCookie(options);
end;

function SaveOptions: byte;
begin
    InitCookie(APPKEY_CREATOR_ID, APPKEY_APP_ID, APPKEY_CONFIG_KEY);
    result := $ff;
    if not IsKeyCustom then options.refreshInterval := DEFAULT_REFRESH;
    if savingEnabled then result := SetCookie(options, SizeOf(TOptions));
    if result <> 1 then savingEnabled := false;
    UpdateRefreshFrames;
end;

procedure DefaultOptions;
begin
    Writeln('Saving defaults');
    options.apiKeyOW := '';
    options.refreshInterval := DEFAULT_REFRESH;
    options.units := unknown;
    options.showRegion := false;
    options.detectLocation := true;
    options.maxPrecision := 1;
    options.theme := 'DEFAULT.THM';
    ioresult := SaveOptions;
    if ioresult <> 1 then begin
        Writeln('Error saving options: ', ioresult);
    end;
end;

procedure InitOptions; 
begin
    savingEnabled := true;
    Writeln('Initializing options');
    ioresult := LoadOptions;
    if ioresult <> 1 then begin
        Writeln('No saved config found');
        DefaultOptions;
    end else begin
        if sizeof(TOptions) <> cookie.len then begin
            Writeln('Config size mismatch');
            DefaultOptions;
        end;
    end;
    UpdateRefreshFrames;
end;

// ***************************************************** NETWORK ROUTINES

function WaitAndParseRequest:byte;
var blockLen:word;
begin
    jsonEnd := 0;
    jsonRoot := 0;
    jsonStart := 0;
    result := 1;
    repeat 
        TCP_bytesWaiting := 0;
        TCP_GetStatus;
        if TCP_bytesWaiting > 0 then begin
            blockLen := TCP_bytesWaiting;
            FN_ReadBuffer(@responseBuffer[jsonEnd], blockLen);
            jsonEnd := jsonEnd + blockLen;
        end;
        pause;
    until (TCP_bytesWaiting = 0) and (jsonEnd <> 0);
    result := sioResult;
end;

function isIOError:boolean;
begin
    result := false;
    if ioResult <> 1 then begin // error
        Writeln('Connection Error: ', ioResult);
        Readkey;
        result := true;
    end;
end;

function PercentEscape(var s:string):string[50];
var cur:byte;
begin
    result[0] := #0;
    cur := 1;
    while cur<=Length(s) do begin
        case s[cur] of
            ' ': MergeStr(result,'%20');
            else begin
                Inc(result[0]);
                result[byte(result[0])] := s[cur];
            end;
        end;
        Inc(cur);
    end;
end;

procedure ComposeQuery(var s:string; askFor:byte);
begin
    if askFor = CALL_CHECKCITY then begin
        s:='/geocode/v1/json?key=c99982467d084722a38807998f450ddd&q=';
        MergeStr(s,PercentEscape(city));
        MergeStr(s,'&no_annotations=1&limit=1&language=en');
       
    end;
    if (askFor = CALL_WEATHER) or (askFor = CALL_FORECAST) then begin
        s:='/data/2.5/onecall?lat=';
        MergeStr(s,latitude);
        MergeStr(s,'&lon=');
        MergeStr(s,longitude);
        MergeStr(s,'&exclude=minutely,hourly,alerts');
        if askFor = CALL_WEATHER then MergeStr(s,',daily');
        if askFor = CALL_FORECAST then MergeStr(s,',current');
        MergeStr(s,'&units=');
        if options.units = metric then MergeStr(s,'metric')
        else MergeStr(s,'imperial');
        MergeStr(s,'&appid=');
        if IsKeyCustom then MergeStr(s, options.apiKeyOW)
        else MergeStr(s, defaultApiKey);
    end;
    if askFor = CALL_CHECKKEY then begin
        s:='/data/2.5/onecall?lat=50&lon=20&exclude=daily,minutely,hourly,alerts&appid=';
        MergeStr(s,tmp);
    end;
end;

procedure HTTPGet(var api, query:string);
var uri:string[200];
begin
    uri:='N:http://';
    MergeStr(uri, api);
    MergeStr(uri, query);
    MergeStr(uri,#0#0);
    ioResult := TCP_Connect(uri);
    if isIOError then exit;
    ioResult := WaitAndParseRequest;
    if isIOError then exit;
    TCP_Close;    
end;

procedure GetWeather;
begin
    ComposeQuery(getLine, CALL_WEATHER);
    HTTPGet(OW_api, getLine);
    ParseWeather;
end;

procedure GetForecast;
begin
    ComposeQuery(getLine, CALL_FORECAST);
    HTTPGet(OW_api, getLine);
    ParseForecast;
end;

procedure TrimAtChar(var s:string;c:char);
var i:byte;
begin
    for i:=1 to Length(s) do 
        if s[i] = c then begin
            s[0] := char(i-1);
            exit;
        end;
end; 

function GetCityLocation:boolean;
begin
    result := true;
    ComposeQuery(getLine, CALL_CHECKCITY);
    HTTPGet(OC_api, getLine);
    city[0] := #0;
    tmp[0] := #0;
    Writeln;
    if findKeyPos('total_results') <> 0 then begin
        GetJsonKeyValue('total_results', tmp);
        if tmp[1] = '1' then begin
            GetJsonKeyValue('formatted', city);
            //if Length(city) = 0 then GetJsonKeyValue('town', city);
            TrimAtChar(city,',');
            UtfNormalize(city);
            GetJsonKeyValue('ISO_3166-1_alpha-2', country_code);
            GetJsonKeyValue('state_code', region_code);
        end;
        if Length(city) = 0 then begin
            tmp := '404';
            getLine := 'City not found';
            exit;
        end;
        jsonStart := jsonRoot;
        FollowKey('geometry');
        GetJsonKeyValue('lat', latitude);
        GetJsonKeyValue('lng', longitude);
    end else begin
        tmp := '400';
        getLine := 'Communication error';
        result := false;
    end;
    jsonStart := jsonRoot;
end;

procedure GetIPLocation;
begin
    getLine:='/check?access_key=9ba846d99b9d24288378762533e00318&fields=ip,region_code,country_code,city,latitude,longitude';
    HTTPGet(IP_api, getLine);
    ParseLocation;
    if options.units = unknown then begin
        if isCCImperial(country_code) then options.units := imperial
            else options.units := metric;
        SaveOptions;
    end;
    Writeln('Your IP: ',tmp);
end;


// ***************************************************** GUI ROUTINES

procedure WriteTime(date: TDateTime);
var hour:byte;
begin
    hour := date.hour;
    if options.units = imperial then hour := hour24to12(hour) 
    else Write(' ');
    if hour < 10 then Write(0);
    Write(hour,':');
    if date.minute < 10 then Write(0);
    Write(date.minute);
    if options.units = imperial then begin
        if date.hour >= 12 then Write('pm')
            else Write('am');
    end;
end;

procedure WriteSpeedUnit;
begin
    if options.units = metric then Write('m/s')
        else Write('mph');
end;

procedure PutBitmap(src,dest:word;w,h:byte);
var row,col:byte;
begin
    row:=0;
    repeat 
        col:=0;
        repeat
            poke(dest+col,peek(src+col));
            inc(col)
        until col = w;
        inc(src,40);
        inc(dest,40);
        inc(row);
    until row = h;
end;

procedure DrawIcon(src,dest: word);
begin
    PutBitmap(src,dest,10,24);
end;

function PutChar(c:char; dest: word; color: byte):byte;
var row,bit,ic:byte;
    src,dc:word;
begin
    row := 0;
    src := FONT + (ord(c) shl 3);
    repeat 
        dc := 0;
        bit := 0;
        ic := peek(src);
        repeat 
            if (ic and 128) <> 0 then dc := dc or color;
            ic := ic shl 1;
            dc := dc shl 2;
            inc(bit);
        until bit = 8;
        poke(dest,peek(dest) or hi(dc));
        poke(dest+1,peek(dest+1) or lo(dc));
        inc(src);
        inc(dest,scrWidth);
        inc(row);
    until row = 8;
end;

procedure PutString(var s:string; dest: word; color: byte);
var i: byte;
    line: string[40];
begin
    i := 1;
    line := Atascii2Antic(s);
    while (i <= Length(line)) do begin
        PutChar(line[i], dest, color);
        inc(dest, 2);
        inc(i);
    end;
end;

procedure PutCString(var s:string;dest: word;color:byte;w:byte);
var l:byte;
begin
    l:=Length(s);
    if l < w then dest := dest + (w - l);
    PutString(s,dest,color);
end;

function PutSymbol(c:char; dest: word):byte;
var x,h:byte;
    src,off:word;
begin
    result := 3;
    h := 20;
    src:= GFX + 74 * 40;
    case c of
        '0'..'9': begin 
            x := (ord(c) - 48) * 3;
            if c = '1' then begin
                dec(result);
                inc(x);
            end;
        end;
        'F': begin
            x := 11 * 3;
        end;
        'C': begin
            x := 12 * 3;
        end;
        '^': begin
            x := 10 * 3 + 1;
            dec(result);
            h := 7;
        end;
        '.': begin
            x := 10 * 3 + 1;
            dec(result);
            h := 4;
            off := 40 * 16;
            inc(src, off);
            inc(dest, off)
        end;
        '-': begin
            x := 10 * 3 + 1;
            dec(result);
            h := 4;
            off := 40 * 8;
            inc(src, off);
            inc(dest, off)
        end;
    end;
    inc(src, x);
    PutBitmap(src, dest, result, h);
end;

procedure PrintTemperature(var s:string;dest: word);
var i,w:byte;
begin
    i:=1;
    while (i<=Length(s)) do begin
        w := putSymbol(s[i],dest);
        inc(dest,w);
        inc(i);
    end;
end;

procedure ShowUnits();
begin
    if options.units = metric then Writeln('metric')
        else Writeln('imperial');
end;

procedure ShowLocation;
begin
    Writeln('Location: ', city, ', ', country_code);
    Writeln('latitude: ', latitude);
    Writeln('longitude: ', longitude);
    Write('units: ');
    ShowUnits;
end;

procedure PromptLocation;
var reqOK,foundLocation:boolean;
    
begin
    foundLocation := false;
    CursorOn;
    repeat 
        Writeln('Search for city,country');
        Writeln('example: PARIS,FR');
        Readln(city);
        reqOk := GetCityLocation;
        if Length(city) <> 0 then begin
            foundLocation := true; 
            ShowLocation;
        end else begin
            Writeln('Request Error : ',tmp);
            Writeln(getLine);
            Writeln('Try Again!');
            Writeln;
            if not reqOk then DumpJson;
        end;
    until foundLocation;
    CursorOff;
end;

procedure ClearGfx;
begin
    FillByte(pointer(VRAM),VRAM_SIZE,0);
end;

procedure MoveDescription;
var row:byte;
    dlptr:byte;
    vptr:word;
begin
    dlptr := 51;
    vptr := VRAM + (41*40) + descOffset;
    for row:=0 to 8 do begin
        dpoke(DLIST + dlptr, vptr);
        inc(dlptr,3);
        inc(vptr,80);
    end;
end;

procedure SetDliJMP(p:byte);
var addr:byte;
    dlptr:byte;
begin
    dlptr := 51 + 27;
    addr := peek(DLIST + dlptr + 2 + p);
    poke(DLIST + dlptr, addr);
end;

procedure InitScroll;
begin
    descDir := 0;
    descOffset := 0;
    descScroll := BOUNCE_DELAY;
    descHSC := 8;
    hscrol := descHSC;
    MoveDescription;
end;

procedure ShowHeader;
begin
    ClearGfx;
    InitScroll;
    scrWidth:=80;
    PutCString(getLine, VRAM + 43 * 40 + 2,1,20);
    scrWidth:=40;
end;

procedure ShowDescription;
begin
    InitScroll;
    scrWidth := 80;
    GetJsonKeyValue('description', getLine);
    PutCString(getLine, VRAM + 43 * 40 + 2,1,20);
    PutCString(getLine, VRAM + 41 * 40 + 2,3,20); 
    weatherDescLen := Length(getLine);
    scrWidth := 40;
end;

procedure ShowMenu;
begin
    menuDelay := MENU_TIME;
    statusDelay := 0;
    timeShown := false;
    menuColor := cityColor;
    Gotoxy(1,9);
    if page = PAGE_WEATHER then 
        Write(' '+'F'*+'orecast    '+'U'*+'nits     '+'R'*+'efresh   '+'Q'*+'uit   ')
    else 
        Write(' '+'N'*+'ext      '+'B'*+'ack                '+'Q'*+'uit     ');    
end;

procedure InitGfx;
begin
    Pause;
    scrWidth := 40;
    SDLSTL := DLIST;
    if page = PAGE_WEATHER then SetDliJMP(0) else SetDliJMP(1);
    SetIntVec(iDLI, @dli);
    nmien := $c0; 
    chbas := Hi(FONT);
    ClearGfx;

    if palnts = 0 then begin
        colors := colorsNTSC;
        fps := 60;
    end else begin
        colors := colorsPAL;
        fps := 50;
    end;
    framesPerMinute := fps * 60;

    color1 := colors[C_SUN];
    color2 := colors[C_CLOUD];
    color4 := colors[C_SKY_NIGHT];
    color0 := colors[C_HEAD_NIGHT];
    cityColor := colors[C_HEAD_NIGHT];


    textColor := 15;
    menuColor := 0;
end;

procedure ShowWeather;
var tempLen:byte;
    grade:string[2];
    i,decp:byte;
    aftercoma:boolean;
begin
    page := PAGE_WEATHER;
    ScreenOff;
    InitGfx;
    // set backgrond color based on icon type
    GetJsonKeyValue('icon', tmp);
    if tmp[3] = 'd' then begin
        color4 := colors[C_SKY_DAY];
        color0 := colors[C_HEAD_DAY];
        cityColor := colors[C_HEAD_DAY];
    end;
    // icon
    DrawIcon(GetIconPtr(tmp), VRAM + 17 * 40);

    // date
    Str(curDate.day, getLine);
    MergeStr(getLine, monthNames[curDate.month - 1]);
    Str(curDate.year, tmp);
    MergeStr(getLine, tmp);
    MergeStr(getLine, ', ');
    MergeStr(getLine, dowNames[curDate.dow]);
    PutCString(getLine, VRAM + 0 * 40, 1,20);
       
    // temperature
    decp := 0;
    aftercoma := false;
    GetJsonKeyValue('temp', getLine);
    for i:=1 to Length(getLine) do begin
        if aftercoma then inc(decp);
        if getLine[i] = '.' then aftercoma:=true;
    end;
    
    if decp > options.maxPrecision then SetLength(getLine, Length(getLine)-(decp-options.maxPrecision));
    
    tempLen := 5;
    grade := '^C';
    if options.units = imperial then begin
        tempLen := 4;
        grade := '^F'
    end;
    if Length(getLine)>tempLen then setLength(getLine, tempLen); 
    if getLine[Length(getLine)] = '.' then Dec(getLine[0]);
    MergeStr(getLine, grade);
    PrintTemperature(getLine, VRAM + 17 * 40 + 12);
    
    // pressure
    GetJsonKeyValue('pressure', tmp);
    getLine := 'hPa';
    if options.units = imperial then begin
        getLine := '"Hg';
        ConvertHPA2INHG(tmp);
    end;
    i := 40 - Length(tmp) shl 1;
    PutString(tmp, VRAM + 16 * 40 + i, 3);
    PutString(getLine, VRAM + 24 * 40 + 34, 3);
    
    // desription
    ShowDescription;
    
    // bottom - TXT part
    savmsc := VRAM + (41 * 40) + (9 * 80);
 
    // city, country
    MergeCityWithCC;
    Gotoxy(21-(Length(getLine) shr 1), 1);
    Writeln(getLine);

    // wind
    GetJsonKeyValue('wind_speed', tmp);
    Gotoxy(2,3);
    Write('Wind: ',tmp,' ');
    WriteSpeedUnit;
    Write(' ');
    GetJsonKeyValue('wind_deg', tmp);
    Write(windDir[GetDirIndex(StrToInt(tmp))]);

    GetJsonKeyValue('humidity', tmp);
    Gotoxy(24,6);
    Write('Humidity: ', tmp, '%');
    GetJsonKeyValue('clouds', tmp);
    Gotoxy(24,7);
    Write('Clouds:   ', tmp, '%');

    GetJsonKeyValue('sunrise', tmp);        
    unixTime := StrToInt(tmp) + timezone;
    UnixToDate(unixtime, sunriseDate);
    Gotoxy(24,3);
    Write('Sunrise: ');
    WriteTime(sunriseDate);
    
    GetJsonKeyValue('sunset', tmp);
    unixTime := StrToInt(tmp) + timezone;
    UnixToDate(unixtime, sunsetDate);
    Gotoxy(24,4);
    Write('Sunset:  ');
    WriteTime(sunsetDate);

    GetJsonKeyValue('dew_point', tmp);
    Gotoxy(2,6);
    Write('Dew point:  ',tmp);
    Write(grade);
    
    GetJsonKeyValue('visibility', tmp);
    Gotoxy(2,7);
    unixtime := StrToInt(tmp);
    Write('Visibility: ');
    if unixtime > 1000 then begin
        unixtime := unixtime div 1000;
        write(unixtime, 'km')
    end else Write(unixtime, 'm');

    GetJsonKeyValue('feels_like', tmp);
    Gotoxy(2,4);
    Write('Feels like: ', tmp);
    Write(grade);
    
    ShowMenu;
    ScreenOn;
end;

procedure ShowDayofForecast(column:byte);
var x:byte;
    o:byte;
    prob:byte;
    grade:string[2];
begin
    grade := '^C';
    if options.units = imperial then grade := '^F';
    x := column * 10;

    Str(curDate.day, getLine);
    PutCString(getLine, VRAM + 0 * 40 + x, 3, 5);
    getLine[0] := #0;
    MergeStr(getLine, monthNames[curDate.month - 1]);
    PutString(getLine, VRAM + 8 * 40 + x, 1);

    // icon
    GetJsonKeyValue('icon', tmp);
    DrawIcon(GetIconPtr(tmp), VRAM + 17 * 40 + x);
    
    scrWidth := 80;
    getLine[0] := #0;
    MergeStr(getLine, dowNames[curDate.dow]);
    PutString(getLine, VRAM + 40 * 41 + x + 4, 1);
    scrWidth := 40;

    savmsc := VRAM + (41 * 40) + (9 * 80);

    o := 2; // left margin

    GetJsonKeyValue('night', getLine);
    if Length(getLine)>5 then setLength(getLine, 5); 
    MergeStr(getLine, grade);
    Gotoxy(x + o,1);
    Write(getLine);

    GetJsonKeyValue('day', getLine);
    if Length(getLine)>5 then setLength(getLine, 5); 
    MergeStr(getLine, grade);
    Gotoxy(x + o,2);
    Write(getLine);

    GetJsonKeyValue('pressure', getLine);
    tmp := 'hPa';
    if options.units = imperial then begin
        tmp := '"Hg';
        ConvertHPA2INHG(getLine);
    end;
    Gotoxy(x + o,4);
    Write(getLine, tmp);
    
    GetJsonKeyValue('wind_deg', getLine);
    Gotoxy(x + o,5);
    Write('Wind: ');
    Write(char(GetDirIndex(StrToInt(getLine))));
    
    GetJsonKeyValue('wind_speed', getLine);
    Gotoxy(x + o,6);
    Write(getLine);
    WriteSpeedUnit;
    
    GetJsonKeyValue('pop', tmp);
    prob := Trunc(StrToFloat(tmp) * 100);
    
    GetJsonKeyValue('snow', tmp);
    GetJsonKeyValue('rain', getLine);
    Gotoxy(x + o,7);
    if prob > 0 then begin
        if Length(tmp) > 0 then begin // snow
            Write(#$B' ',prob,'%');
            Gotoxy(x + o,8);
            Write(tmp,'mm');
        end 
        else if Length(getLine) > 0 then begin // rain
            Write(#$A' ',prob,'%');
            Gotoxy(x + o,8);
            Write(getLine,'mm');
        end;
    end;
end;

procedure ShowForecastPage(pageNum:byte);
var day, column:byte;
    pages: array[0..1] of byte = (PAGE_FORECAST0, PAGE_FORECAST1);
begin
    page := pages[pageNum];
    ScreenOff;
    InitGfx;
    InitScroll;

    color0 := colors[C_HEAD_NIGHT];
    color4 := colors[C_SKY_FORECAST];
    cityColor := 2;

    column := 0;
    day := (pageNum shl 2);

    repeat 
        if forecastPtrs[day]<>0 then begin
            jsonStart := forecastPtrs[day];
            ParseWeather;
            ShowDayofForecast(column);
        end;
        inc(day);
        inc(column);
    until column = 4;

    ShowMenu;
    ScreenOn;
end;

procedure ShowWelcomeMsg;
begin
    ScreenOff;
    ClearGfx;
    Pause;
    SDLSTL := DLIST2;
    nmien := $40; 
    savmsc := VRAMTXT;

    color1 := 10;
    color2 := $94;
    color4 := 0;

    // prepare logo charset
    move(pointer($e000), pointer(VRAM), $400);
    move(pointer(LOGO_CHARSET), pointer(VRAM + $200), $100);
    chbas := Hi(VRAM); 

    // draw logo
    move(logo[0*13],pointer(savmsc+40*1+2),13);
    move(logo[1*13],pointer(savmsc+40*2+2),13);
    move(logo[2*13],pointer(savmsc+40*3+2),13);
    move(logo[3*13],pointer(savmsc+40*4+2),13);
    
    Gotoxy(17,3);
    Write('Open Weather client');
    Gotoxy(17,4);
    Write('by bocianu@gmail.com');
    Gotoxy(2,6);
    Writeln;

    CursorOff;
    ScreenOn;
end;

procedure ShowMenuTime;
begin
    Gotoxy(24,9);
    Write('Time:    ');
    WriteTime(CurDate);
end;

procedure ForwardCurTime;
begin
    inc(curDate.minute);
    if (curDate.minute = 60) then begin
        curDate.minute := 0;
        Inc(curDate.hour);
        if curDate.hour = 24 then begin
            curDate.hour := 0;
            // refresh 15 sec after mindnight
            refreshCounter := refreshFrames - (framesPerMinute shr 2);
        end;
    end;
end;

procedure Animate;
var tz:shortInt;
begin
    if clockCount < framesPerMinute  then begin
        inc(clockCount);
        if clockCount = framesPerMinute then begin
            ForwardCurTime;
            if timeShown then ShowMenuTime;
            clockCount := 0;
        end;
    end;

    // menu
    if menuDelay>0 then begin
        dec(menuDelay);
        if menuDelay = 0 then menuColor := color4;
    end;
    
    if statusDelay < STATUS_TIME then Inc(statusDelay);
    if statusDelay = STATUS_TIME then begin 
        Inc(statusDelay);

        if page = PAGE_WEATHER then begin
            FillByte(pointer(VRAM + (41 * 40) + (9 * 80) + (8 * 40)),40,0);
            Gotoxy(2,9);
            Write('Time Zone: GMT');
            tz := timezone div 3600;
            if tz <> 0 then begin
                if tz > 0 then Write('+');
                Write(tz);
            end;
            ShowMenuTime;
            menuColor := cityColor;
            timeShown := true;
        end else begin
            FillByte(pointer(VRAM + (41 * 40) + (9 * 80) + (8 * 40)),40,0);
            MergeCityWithCC;
            Gotoxy(21-(Length(getLine) shr 1), 9);
            Writeln(getLine);
            menuColor := color4 - 2;
        end;
    end;
    
    //description
    if (weatherDescLen > 20) and (page = PAGE_WEATHER) then begin
        dec(descScroll);
        if descScroll = 0 then begin
            descScroll := SCROLL_SPEED;
            if descDir = 0 then begin 
                dec(descHSC);
                if descHSC <= 4 then begin
                    inc(descOffset);
                    MoveDescription;;
                    descHsc := 8;
                end;
                if descOffset = (weatherDescLen shl 1) - 40 then begin
                    descDir := 1;
                    descScroll := BOUNCE_DELAY;
                end;
            end else begin
                inc (descHSC);
                if descHSC >= 9 then  begin
                    dec(descOffset);
                    MoveDescription;;
                    descHSC := 5;
                end;
                if (descOffset = 0) and (descHSC = 8) then begin
                    descDir := 0;
                    descScroll := BOUNCE_DELAY;
                end;
            end;
            hscrol := descHsc;
        end;
    end;
end;

procedure ReloadWeather;
begin 
    GetWeather;
    ShowWeather;
end;

procedure ChangeLocation;
begin
    ShowWelcomeMsg;
    PromptLocation;
    SaveLocation;
    ReloadWeather;
end;

procedure WriteYesNo(yes:boolean);
begin
    if yes then Writeln('yes')
    else Writeln('no');
end;

procedure ShowCustomKeyRequest;
begin
    FillByte(pointer(VRAMTXT + 6*40), 17*40, 0);
    GotoXY(3,7);
    Writeln('Warning!                            '*);
    Writeln;
    Writeln('To change refresh interval, you need');
    Writeln('to provide your personal API key,');
    Writeln('for openweathermap.org service.');
    Writeln;
    Writeln('Just visit openweathermap.org,');
    Writeln('then '+'sign up'*+' with your email,');
    Writeln('and get your API key (APPID)');
    Writeln('in the confirmation message.');
    Writeln;
    Writeln('It is so '+'EASY'*' and '+'FREE'*'!!!');
    Writeln('I really recommend you do it.');     

    GotoXY(3,23);
    Write('Press '+'ESC'*' to return.');    
    ReadKey;
end;

procedure LoadTheme(var fname:string);
var fpath:string[16];
    f: file;
    len:word;
begin
    fpath:='D:';
    MergeStr(fpath, fname);
    Assign (f, fpath);
    Reset(f,1);
    BlockRead (f, theme,sizeOf(TTheme),len);
    close(f);
end;

procedure ApplyTheme;
var fpath: string[16];
    len: word;
    f: file;
    gfxmem: array [0..0] of byte absolute GFX;
    fontmem: array [0..0] of byte absolute FONT;
begin
    fpath:='D:';
    MergeStr(fpath, theme.gfx);
    Assign (f, fpath);
    Reset(f,1);
    BlockRead (f, gfxmem,3840 , len);
    close(f);
    fpath:='D:';
    MergeStr(fpath, theme.font);
    Assign (f, fpath);
    Reset(f,1);
    BlockRead (f, fontmem,1024 , len);
    close(f);
    Move(theme.colorsNTSC, colorsNTSC, sizeOf(colorsNTSC));
    Move(theme.colorsPAL, colorsPAL, sizeOf(colorsPAL));
end;

procedure SelectTheme;
var Info : TSearchRec;
    themeNames: array [0..(10*16)-1] of byte;
    themeId,id: byte;
    fname: string[16];
    c:char;
begin
    FillByte(pointer(VRAMTXT + 6*40), 17*40, 0);
    GotoXY(3,7);
    Writeln('Themes:                             '*);
    Writeln;
    themeId := 0;
        if FindFirst('D:*.THM', faAnyFile, Info) = 0 then begin
        repeat
            Move(@Info.Name[0], @themeNames[themeId shl 4], 16);
            Inc(themeId);
        until (FindNext(Info) <> 0) or (themeId = 10);
        FindClose(Info);
    end;
    for i:=0 to themeId-1 do begin
            move(@themeNames[i shl 4],@fname,16);
            LoadTheme(fname);
            Write(char(48+i+128),' ');
            Write(theme.name,' by ',theme.author);
            Writeln;
    end;
    c := readkey;
    id := byte(c) - 48;
    if (id >= 0) and (id < themeId) then begin
        Writeln;
        Writeln('Loading theme');
        move(@themeNames[id shl 4],@fname,16);
        options.theme := fname;
        LoadTheme(fname);
        ApplyTheme;
    end;
end;

procedure ShowOptions;
begin
    FillByte(pointer(VRAMTXT + 6*40), 17*40, 0);
    GotoXY(3,7);

    Writeln('Settings:                           '*);
    Writeln;
    
    Write('U'*'nits: ');
    ShowUnits;
    Writeln;

    Write('S'*'how Region: ');
    WriteYesNo(options.showRegion);
    Writeln;

    Write('D'*'etect Location: ');
    WriteYesNo(options.detectLocation);
    Writeln;
 
    Write('V'*'isual Theme: ');
    Writeln(options.theme);
    Writeln;
 
    Writeln('C'*'ustom weather API key:');
    if IsKeyCustom then Writeln(options.apiKeyOW)
    else Writeln('- not set -');
    Writeln;

    Write('R'*'efresh Interval: ');
    Write(options.refreshInterval);
    Write(' minute');
    if options.refreshInterval > 1 then write('s');

    GotoXY(22,9);
    Write('Decimal ','P'*'laces: ');
    Write(options.maxPrecision);

    GotoXY(3,23);
    Write('Press '+'ESC'*' to return to the weather');
end;

function TmpKeyValid: boolean;
begin
    result := false;
    ComposeQuery(getLine, CALL_CHECKKEY);
    HTTPGet(OW_api, getLine);
    if (FindKeyPos('current') <> 0) then result := true;
end;

procedure PromptKey;
begin
    Gotoxy(3, 18);
    Write('                                      ');
    CursorOn;
    Gotoxy(2, 18);
    Write(' ');
    tmp[0]:=#0;
    Readln(tmp);
    CursorOff;
    if Length(tmp) = 0 then begin
        options.apiKeyOW[0] := #0;
        ShowOptions;
        exit;
    end;
    if TmpKeyValid then begin
        Move(@tmp, @options.apiKeyOW, 33);
        options.apiKeyOW[0] := #32;
    end else begin
        Gotoxy(3, 18);
        Write('Invalid Key - try again.             ');
    end;
end;

procedure PromptPrecision;
var prec:shortInt;
begin
    Gotoxy(37, 9);
    Write(' ');
    CursorOn;
    Gotoxy(37, 9);
    Write(' ');
    tmp[0]:=#0;
    Readln(tmp);
    CursorOff;
    prec := StrToInt(tmp);
    if prec > 2 then prec := 2;
    if prec > -1 then options.maxPrecision := prec;    
end;


procedure PromptInterval;
var interval:cardinal;
begin
    Gotoxy(20, 20);
    Write('                    ');
    CursorOn;
    Gotoxy(20, 20);
    Write(' ');
    tmp[0]:=#0;
    Readln(tmp);
    CursorOff;
    interval := StrToInt(tmp);
    if interval > 255 then interval := 255;
    if interval > 0 then options.refreshInterval := interval;
end;


procedure PromptOptions;
var c:char;
begin
    c:=#0;
    repeat
        if keypressed then begin
            c := ReadKey;
            case c of
                'u','U': begin 
                    SwapUnits;
                end;
                's','S': begin 
                    options.showRegion := not options.showRegion;
                end;
                'd','D': begin 
                    options.detectLocation := not options.detectLocation;
                end;
                'v','V': begin 
                    SelectTheme;
                    //LoadTheme;
                end;        
                'c','C': begin 
                    PromptKey;
                end;
                'r','R': begin 
                    if IsKeyCustom then PromptInterval
                        else ShowCustomKeyRequest;
                end;                
                'p','P': begin 
                    PromptPrecision;
                end;                
            end;
			if c <> #27 then ShowOptions;
        end;
        pause;
    until c = #27;
    SaveOptions;
end;

procedure ChangeOptions;
begin
    ShowWelcomeMsg;
    ShowOptions;
    PromptOptions;
    ReloadWeather;
end;

procedure ChangeUnits;
begin
    SwapUnits;
    getLine := 'Changing Units';
    ShowHeader;
    ReloadWeather;
    SaveOptions;
end;

procedure UpdateWeather;
begin
    getLine := 'Reloading Weather';
    ShowHeader;
    ReloadWeather;
    refreshCounter := 0;
end;

procedure ShowForecast;
begin
    getLine := 'Checking Forecast';
    ShowHeader;
    GetForecast;
    ShowForecastPage(0);
end;

procedure ShowNextPage;
begin
    case page of
        0: ShowForecastPage(1);
        1: ShowForecastPage(0);
    end;
end;

procedure SetLocation;
var ll:byte;
begin
    ll := LoadLocation;
    if options.detectLocation or (ll <> 1) then begin
        Writeln('Connecting to ', IP_api);
        Writeln('Checking your ip and location');
        GetIPLocation;
        SaveLocation;
    end else SetLoadedLocation;
end;



// **********************************************************************
// *******************************************************************************  MAIN
// **********************************************************************

begin
    portb := $ff;
    hscrol := 8;
    ShowWelcomeMsg;
    InitOptions;
    Writeln('Loading theme');
    LoadTheme(options.theme);
    ApplyTheme;
    SetLocation;  
    ShowLocation;
    Writeln;
    Writeln('Press '+'START'*+' to show forecast');
    Writeln('Press '+'SELECT'*+' to select location');
    Writeln('Press '+'OPTION'*+' to change settings');

    Pause(50);

    Writeln;
    Writeln('Connecting to ', OW_api);
    Writeln('Checking weather for your location');
    ReloadWeather;
    
    repeat

        // main loop
        repeat  
            pause;
            Animate;
            if CRT_SelectPressed then ChangeLocation;
            if CRT_OptionPressed then ChangeOptions;
            if CRT_StartPressed then 
                if page = PAGE_WEATHER then ShowForecast
                    else ShowNextPage;
            atract := 1;
            inc(refreshCounter);
            if (refreshCounter = refreshFrames) and (page = PAGE_WEATHER) then UpdateWeather;
        until KeyPressed;

        // menu key reading
        k := readkey;
        if page = PAGE_WEATHER then         // weather page
            case k of
                'r', 'R': UpdateWeather;
                'u', 'U': ChangeUnits;
                'f', 'F': ShowForecast;
                else ShowMenu;
            end
        else                               // forecast page
            case k of
                'n', 'N': ShowNextPage;
                'b', 'B', #27: UpdateWeather            
                else ShowMenu;
            end;
        
    until (k = 'q') or (k = 'Q');

    TCP_DetachIRQ;
    TextMode(0);
end.
