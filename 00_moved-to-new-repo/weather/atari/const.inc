//
// ;*** define your project wide constants here
// 
// ;*** I like to keep memory locations at top of this file
//

LOGO_CHARSET = $8D00; // 256b
UTFTABLE192 = $8E00;  // 386b

FONT = $9000; // 1Kb
VRAM = $9400; 
VRAMTXT = VRAM + $400;
VRAM_SIZE = (41 * 40) + (9 * 80) + (9 * 40); // $AA0 b

GFX = $A000;    //  3840  $f00
DLIST = $AF00;  //  1 page both
DLIST2 = DLIST + $80;


JSON_BUFFER = $B000;  // 4Kb

DEFAULT_REFRESH = 120;

SCROLL_SPEED = 2;
BOUNCE_DELAY = 150;

CALL_WEATHER = 0;
CALL_CHECKCITY = 1;
CALL_FORECAST = 2;
CALL_CHECKKEY = 3;

PAGE_FORECAST0 = 0;
PAGE_FORECAST1 = 1;
PAGE_WEATHER = 2;

MENU_TIME = 150;
STATUS_TIME = 250;

C_SHADOW        = 0;
C_SUN           = 1;
C_CLOUD         = 2;
C_SKY_NIGHT     = 3;
C_SKY_DAY       = 4;
C_SKY_FORECAST  = 5;
C_HEAD_NIGHT    = 6;
C_HEAD_DAY      = 7;

PALLETE_MAX = 7;
