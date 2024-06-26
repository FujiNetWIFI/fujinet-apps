program buildThemes;
uses atari, sysutils, crt;

{$i-}

const 
{$i const.inc}
{$i types.inc}


var theme: TTheme;
    fname: string[16];
    f:file;                                         // sdw  sun  cld  skyn skyd skyf hean head 
    defaultPAL:     array [0..PALLETE_MAX] of byte = ( $96, $ec, $0f, $80, $8a, $84, $98, $94);
    defaultNTSC:    array [0..PALLETE_MAX] of byte = ( $a6, $2c, $0f, $90, $9a, $94, $a8, $a4);
    
    realPAL:        array [0..PALLETE_MAX] of byte = ( $7a, $7c, $0f, $70, $78, $76, $7a, $72);
    realNTSC:       array [0..PALLETE_MAX] of byte = ( $8a, $8c, $0f, $82, $88, $86, $88, $82);
    
    simplePAL:      array [0..PALLETE_MAX] of byte = ( $8a, $ec, $0f, $80, $88, $84, $88, $82);
    simpleNTSC:     array [0..PALLETE_MAX] of byte = ( $9a, $2c, $0f, $92, $98, $94, $98, $92);
    
    Info : TSearchRec;

begin

    fillbyte(@theme, sizeOf(TTheme), 0);
    theme.name := 'Default';
    theme.author := 'bocianu';
    theme.gfx := 'WEATHER.GFX';
    theme.font := 'WEATHER.FNT';
    move(defaultNTSC, theme.colorsNTSC, sizeOf(defaultNTSC));
    move(defaultPAL, theme.colorsPAL, sizeOf(defaultNTSC));
    fname := 'D:DEFAULT.THM';
    Assign (f,fname);
    Rewrite (F,1);
    BlockWrite (f,theme,sizeOf(TTheme));
    Close (f);
    Writeln(fname,' ',ioResult);

    fillbyte(@theme, sizeOf(TTheme), 0);
    theme.name := 'Simple';
    theme.author := 'PG';
    theme.gfx := 'SIMPLE.GFX';
    theme.font := 'WEATHER.FNT';
    move(simpleNTSC, theme.colorsNTSC, sizeOf(defaultNTSC));
    move(simplePAL, theme.colorsPAL, sizeOf(defaultNTSC));
    fname := 'D:SIMPLE.THM';
    Assign (f,fname);
    Rewrite (F,1);
    BlockWrite (f,theme,sizeOf(TTheme));
    Close (f);
    Writeln(fname,' ',ioResult);

    fillbyte(@theme, sizeOf(TTheme), 0);
    theme.name := 'Realistic';
    theme.author := 'PG';
    theme.gfx := 'REAL.GFX';
    theme.font := 'WEATHER.FNT';
    move(realNTSC, theme.colorsNTSC, sizeOf(defaultNTSC));
    move(realPAL, theme.colorsPAL, sizeOf(defaultNTSC));
    fname := 'D:REAL.THM';
    
    Assign (f,fname);
    Rewrite (F,1);
    BlockWrite (f,theme,sizeOf(TTheme));
    Close (f);
    Writeln(fname,' ',ioResult);
    
    readkey;

end.
