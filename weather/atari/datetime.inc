type TdateTime = record
    year:word;
    month:byte;
    day:byte;
    hour:byte;
    minute:byte;
    second:byte;
    dow:byte;
end;

procedure UnixToDate(ux: cardinal; var date:TDateTime);
var second, minute, hour, day, month, year, dow, dim: cardinal;
    leap: boolean;
    daysInYear: word;
    daysInMonth: array [0..11] of byte = (31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
    fin:boolean;
begin
    fin := false;
    second := ux;
    minute := ux div 60;
    second := second - minute * 60;
    hour   := minute div 60;
    minute := minute - hour * 60;
    day    := hour div 24;
    hour   := hour - day * 24;
    
    year   := 1970;
    dow    := 4;
    
    repeat;
        leap := (year mod 4 = 0) and ((year mod 100 <> 0) or (year mod 400 = 0));
        daysInYear := 365;
        if leap then inc(daysInYear);
        if day >= daysInYear then begin
            inc(dow);
            if leap then inc(dow);
            day := day - daysInYear;
            if dow >= 7 then dow := dow -7;
            inc(year);
        end else begin
            date.day := day;
            dow := dow + day;
            dow := dow mod 7;
            month := 0;
            repeat
                dim := daysInMonth[month];
                if (month = 1) and leap then inc(dim);
                if day >= dim then day := day - dim else fin := true;;
                inc(month);
            until fin or (month = 12);
            fin := true;
        end;
    until fin;
    date.second := second;
    date.minute := minute;
    date.hour := hour;
    date.day := day + 1;
    date.month := month; 
    date.year := year;
    date.dow := dow;
end;

function hour24to12(hour:byte):byte;
begin
    result := hour;
    if hour > 12 then result := hour - 12;
end;
