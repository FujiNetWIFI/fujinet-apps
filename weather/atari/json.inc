const
    JSON_OPEN = #123;
    JSON_CLOSE = #125;

var
    utfMap192: array [0..0] of byte absolute UTFTABLE192;
    jsonRoot, jsonStart, jsonEnd: word;
    
type KeyString = string[20];   

// ***************************************************** JSON PARSER ROUTINES

function GetJsonRoot:word;
var ptr:word;
begin
    result := $ffff;
    ptr := 0;
    repeat
        if responseBuffer[ptr] = 13 then
            if responseBuffer[ptr+1] = 10 then
                if responseBuffer[ptr+2] = 13 then
                    if responseBuffer[ptr+3] = 10 then exit(word(ptr+4));
        inc(ptr);
    until ptr = jsonEnd;
end;

function FindKeyPos(skey:KeyString):word;
var inQuotes,keyParsing,gotKey,keyMatch,expectKey:boolean;
    keyLen:byte;
    level:byte;
    c: char;
    jptr: word;
begin
    result := 0;
    inQuotes := false;
    expectKey := true;
    keyParsing := false;
    gotKey := false;
    keyMatch := false;
    level := $ff;
    jptr := jsonStart;
    repeat
        c := char(responseBuffer[jptr]);
        if not inQuotes then begin     // out of Quotes

            case c of
                '"' :begin
                        if not gotKey and expectKey then begin
                            keyParsing := true;
                            keyLen := 0;
                            keyMatch := true;
                        end;
                        inQuotes := true;
                    end;
                
                ',' :begin
                        gotKey := false;
                        expectKey := true;
                    end;

                JSON_OPEN :begin
                        gotKey := false;
                        expectKey := true;
                        inc(level);
                    end;

                JSON_CLOSE :begin
                        gotKey := false;
                        expectKey := true;
                        dec(level);
                        if level = $ff then exit;
                    end;
                
                ':' :begin
                        if gotKey then begin
                            result := jptr + 1;
                            expectKey := false;
                            exit;
                        end;
                    end;
            end;
        
        end else begin                 // in Quotes

            if c = '"' then begin               // ending quote
                if keyParsing then begin
                    gotKey := keyMatch and (keyLen = Length(skey));
                end;
                keyParsing := false;
                inQuotes := false;
                expectKey := false;
            end else begin                      // other letter
                if keyParsing then begin
                    Inc(keyLen);
                    if (keyLen > Length(skey)) or (skey[keyLen] <> c) then keyMatch := false;
                end;
            end;
        
        end;
        
        Inc(jptr);
    until jptr >= jsonEnd;

end;

function GetElementsCount: byte;
var inQuotes,inObject, hasContent:boolean;
    alevel,olevel:byte;
    jptr:word;
    c: char;
begin
    alevel := $ff;
    olevel := $ff;
    result := 0;
    inQuotes := false;
    inObject := false;
    hasContent := false;
    jptr := jsonStart;

    repeat
        c := char(responseBuffer[jptr]);
        if not inQuotes then begin     // out of Quotes

            case c of
                '"':begin
                        inQuotes := true;
                        hasContent := true;
                    end;
                JSON_OPEN:begin
                        inc(olevel);
                        inObject := true;
                        hasContent := true;
                    end;
                JSON_CLOSE:begin
                        dec(olevel);
                        if olevel = $ff then inObject := false;
                    end;
                
                ',':begin
                        if (alevel = 0) and (not inObject) then begin
                            inc(result);
                        end;
                    end;

                '[':begin
                        if not inObject then begin
                            inc(alevel);
                            if alevel = 0 then hasContent := false
                                else hasContent := true;
                        end;
                    end;

                ']':begin
                        if not inObject then begin
                            dec(alevel);
                            if alevel = $ff then begin
                                if hasContent then inc(result);
                                exit(result);
                            end;
                        end;
                    end;
            end;
        end else                            // not inQuotes
            if c = '"' then begin           // ending quote
                inQuotes := false;
            end;
        
        Inc(jptr);
    until (jptr >= jsonEnd);
end;

function FindIndex(i:byte):word;
var inQuotes,inObject,found:boolean;
    element:byte;
    jptr: word;
    alevel,olevel:byte;
    c: char;
begin
    alevel := $ff;
    olevel := $ff;
    element := 0;
    found := false;
    inQuotes := false;
    inObject := false;
    jptr := jsonStart;

    repeat
        c := char(responseBuffer[jptr]);
        if not inQuotes then begin     // out of Quotes

            case c of
                '"':begin
                        inQuotes := true;
                    end;
                JSON_OPEN:begin
                        inc(olevel);
                        inObject := true;
                    end;
                JSON_CLOSE:begin
                        dec(olevel);
                        if olevel = $ff then inObject := false;
                    end;
                
                ',':begin
                        if (alevel = 0) and (not inObject) then begin
                            inc(element);
                            if element = i then found := true;
                        end;
                    end;

                '[':begin
                        if not inObject then begin
                            inc(alevel);
                            if (alevel = 0) and (element = i) then found := true;
                        end;
                    end;

                ']':begin
                        if not inObject then begin
                            dec(alevel);
                            if alevel = $ff then exit(0);
                        end;
                    end;
            end;
        end else                            // not inQuotes
            if c = '"' then begin           // ending quote
                inQuotes := false;
            end;
        
        Inc(jptr);
    until found or (jptr >= jsonEnd);
    result := 0;
    if found then result := jptr;
end;

procedure GetJsonKeyValue(skey:KeyString;var v:string);
var c: char;
    copychar, inQuotes: boolean;
    jptr: word;
begin
    v[0] := #0;
    inQuotes := false;
    jptr := FindKeyPos(skey);
    //writeln('key at: ', jsonPtr);
    if jptr <> 0 then begin
        repeat
            c := char(responseBuffer[jptr]);
            copychar := true;
            case c of 
				' ': begin
					if not inQuotes then copychar := false;
				end;
                '"':
                    begin
                        if not inQuotes then begin
                            inQuotes := true;
                            copychar := false;
                        end else begin
                            exit;
                        end;
                    end;
                ',', JSON_CLOSE, ']':
                    begin
                        if not inQuotes then exit;
                    end;
            end;
            if copychar then begin
                Inc(v[0]);
                v[Length(v)] := c;
                if Length(v) = 40 then exit;
            end;
            
            inc(jptr);
        until (jptr >= jsonEnd);
    end;
    //Writeln('Key not found: ', skey);
    v[0] := #0;
end;

function FollowKey(s:KeyString):word;
var jptr:word;
begin
    jptr := FindKeyPos(s);
    if jptr <> 0 then begin
        jsonStart := jptr;
    end;
    result := jptr;
end;

function FollowIndex(i:byte):word;
var jptr:word;
begin
    jptr := FindIndex(i);
    if jptr <> 0 then begin
        jsonStart := jptr;
    end;
    result := jptr;
end;

// ***************************************************** JSON HELPERS

procedure EscapeJson(var s:string);
var i: byte;
    c: char;
begin
    i := 0;
    while (i < Length(s)) do begin
        Inc(i);
        c := s[i];
        case c of  
            JSON_OPEN: s[i] := '<';
            JSON_CLOSE: s[i] := '>';
        end;
    end;
end;

procedure DumpJson;
var s:string[40];
    jptr, jlen, tomove:word;
begin
    jlen := jsonEnd - jsonStart;
    jptr := jsonStart;
    while (jlen>0) do begin
        tomove := jlen;
        if jlen>40 then tomove := 40;
        s[0] := char(tomove);
        Move(responseBuffer[jptr],s[1],tomove);
        jlen := jlen - tomove;
        jptr := jptr + tomove;
        EscapeJson(s);
        Write(s);
        if keypressed then begin
            readkey;
            break;
        end;
    end;
    Writeln;
end;

function Hex2Dec(c:char):byte;
begin
    result:=0;
    case c of
        '0'..'9': begin 
            exit(byte(byte(c)-48));
        end;
        'a'..'f': begin 
            exit(byte(byte(c)-87));
        end;
        'A'..'F': begin 
            exit(byte(byte(c)-55));
        end;
    end;
end;

procedure UtfNormalize(var s:string);
var readHead, writeHead :byte;
    utfLen:byte;
    hex:word;
begin
    utfLen := Length(s);
    readHead := 1;
    writeHead := 1;
    while readHead <= utfLen do begin
        if (s[readHead] = '\') and (s[readHead + 1] = 'u') then begin  // detect and decode \uXXXX codes
            hex := Hex2dec(s[readHead+2]) shl 12;
            hex := hex + Hex2dec(s[readHead + 3]) shl 8;
            hex := hex + Hex2dec(s[readHead + 4]) shl 4;
            hex := hex + Hex2dec(s[readHead + 5]);
            if (hex > 191) and (hex < 577) then s[writeHead] := char(utfMap192[hex - 192])
                else s[writeHead] := '_';
            inc(readHead,6);
            inc(writeHead);
        end 
        else 
            if byte(s[readHead]) > $c1 then begin                // detect and decode inline symbols
                hex := word(((byte(s[readHead]) - $c2) shl 6)) + word(s[readHead + 1]);
                if (hex > 191) and (hex < 577) then s[writeHead] := char(utfMap192[hex - 192])
                    else s[writeHead] := '_';
                inc(readHead, 2);
                inc(writeHead);
            end else begin
                s[writeHead] := s[readHead];
                inc(readHead);
                inc(writeHead);
            end;
    end;
    SetLength(s,writeHead - 1);
end;

