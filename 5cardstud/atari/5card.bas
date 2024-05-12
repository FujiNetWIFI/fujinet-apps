'' 5 Card Stud Atari Client for #FujiNet
''
'' @author  Eric Carr
'' @email   eric dot carr at gmail dot com
'' @license gpl v. 3

' FujiNet AppKey settings. These should not be changed
AK_LOBBY_CREATOR_ID = 1     ' FUJINET Lobby
AK_LOBBY_APP_ID  = 1        ' Lobby Enabled Game
AK_LOBBY_KEY_USERNAME = 0   ' Lobby Username key
AK_LOBBY_KEY_SERVER = 1     ' 5 Card Stud Client registered as Lobby appkey 1

' 5 Card Stud client
AK_CREATOR_ID = $E41C       ' Eric Carr's creator id
AK_APP_ID = 1               ' 5 Card/Poker App ID
AK_KEY_SHOWHELP = 0         ' Shown help
AK_KEY_COLORTHEME = 1       ' Color theme

DATA NAppKeyBlock()=0,0,0

' Disable BASIC on XL/XE to make more memory available.
if dpeek(741)-$BC00<0
  ' Disable BASIC
  pause: poke $D301, peek($D301) ! 2: poke $3F8, 1
  ' Set memtop to 48K
  dpoke 741, $BC00
endif

' Read server endpoint stored from Lobby
serverEndpoint$=""
query$=""

@NReadAppKey AK_LOBBY_CREATOR_ID, AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, &serverEndpoint$

' Parse endpoint url into server and query
if serverEndpoint$<>""
  for i=1 to len(serverEndpoint$)
    if serverEndpoint$[i,1]="?"
      query$=serverEndpoint$[i]
      serverEndpoint$=serverEndpoint$[1,i-1]
      exit
    endif
  next
else
  ' Default to known server if not specified by lobby. Override for local testing
  serverEndpoint$="https://5card.carr-designs.com/"
  'serverEndpoint$="http://192.168.2.41:8080/"
  'query$="?table=dev7"
endif

' Silence the loud SIO noise
poke 65,0

' Game state variables (read from server)
Dim lastResult$, round, pot, activePlayer, prompt$, moveTime, viewing, validMoveCode$(4), validMove$(4)
dim player_name$(7), player_status(7), player_bet(7), player_move$(7), player_purse(7), player_hand$(7)

' Player's name
myName$ = "" 


' **************************************************
' ***************** IMPORTANT NOTE *****************

' ** These variables are dimensioned in a specific order to reduce wasted
' space when aligning certain arrays to 4K and 1K boundaries

' Receive buffer
dim responseBuffer(1023) BYTE

' Align screenBuffer at 4096 boundary (and charBuffer at 1024 boundary afterward)
i = (&responseBuffer+1023)/4096*4096 - &responseBuffer + 3071

' If space is available until the next 4096 boundary,
' initialize some player array strings to use this available space
j=0
while i > 768
  player_name$(j)="":player_hand$(j)="":player_move$(j)=""
  i=i-768
  inc j
wend

' Any remaining space is assigned to a filler array
if i>0 then dim filler(i) BYTE

' The screenbuffer holds the Playfield screen (40*26=1040), followed by an offscreen buffer of the same size.
' It only needs 2080 bytes.
' The charBuffer that is dimensioned immediately following the playfield needs to be 1024 byte aligned.
' This gives us 992 bytes (1024*3-2080) extra space to do something with.
' We use 768 of that to allocate another set of player strings
' 1024*3=3072 bytes, - 768 = 2304 bytes we are allocating below
dim screenBuffer(2303) BYTE

' Allocate 3 more strings, taking up 768 bytes 
player_name$(j)="":player_hand$(j)="":player_move$(j)=""

' Now charBuffer will be aligned to a 1K boundary
dim charBuffer(1023) BYTE


' *************** END IMPORTANT NOTE ***************
' **************************************************

' State related variables
dim validMoveCount, playerCount, currentCard, xOffset, requestedMove$, previousPot, playerJustMoved, prevPlayerCount

' Other varibles
DIM Screen,__print_inverse, move_color, __print_reverse, noAnim, cursorY, cursorX, errorCount
dim move_loc(7), move_bits(7) BYTE

' DLI Colors 
data background_color()B.=$0,0,0
data text_color()B.=$0E,$0E,0

' Color Themes: (Background & ard border)
' Themes: Green, Blue, Brown, Gray
DATA colorThemeMap()      =  $B4,$88,  $84,$08, $22,$28, $04,$08,' NTSC
DATA                      =  $A4,$78,  $74,$08, $12,$18, $04,$08 ' PAL 
colorTheme=-1

' Player hand and bet locations onscreen
DIM playerX(7), playerY(7), playerDir(7), playerBetX(7), playerBetY(7)

DATA playerXMaster() = 17,1, 1, 1, 15, 37,37, 37
DATA playerYMaster() = 20, 19, 11, 3, 2,3,11,19
DATA playerDirMaster() = 1,1,1,1,1,-1,-1,-1

DATA playerBetXMaster() = 1,10,10,10,3,-9,-9,-9
DATA playerBetYMaster() = -3, -2, 1,4,5,4,1,-2

' Simple hard coded arrangment of players around the table based on player count.
' These refer to index positions in the Master arrays above
' Downside is new players will cause existing player positions to move.
'
'                         2                3                4  
DATA playerCountIndex() = 0,4,0,0,0,0,0,0, 0,2,6,0,0,0,0,0, 0,2,4,6,0,0,0,0, 
'       5                6                 7                8
DATA  = 0,2,3,5,6,0,0,0, 0,2,3,4,5,6,0,0,  0,2,3,4,5,6,7,0, 0,1,2,3,4,5,6,7




PROC CycleColorTheme
  ' First time called? Load from app key
  if colorTheme = -1
    colorTheme = 0
    temp$=""
    @NReadAppKey AK_CREATOR_ID, AK_APP_ID, AK_KEY_COLORTHEME, &temp$
    if len(temp$)=1 then colorTheme = val(temp$)
  else
    ' Otheriwse, just cycle theme
    sound 0, 220,10,5
    pause 4
    sound 0, 200,10,5
    
    colorTheme = (colorTheme + 1) mod 4 
  endif

  ' Set new theme colors (NTSC/PAL + theme index)
  i = (PEEK(53268)=1)*2*4 + 2*colorTheme
  h=colorThemeMap(i)
  j=colorThemeMap(i+1)

  pause 2
  background_color(0)= colorThemeMap(i)
  background_color(1)= colorThemeMap(i)
  POKE 708, colorThemeMap(i+1)

  ' Store in app key to recall on next program start
  @NWriteAppKey AK_CREATOR_ID, AK_APP_ID, AK_KEY_COLORTHEME, &STR$(colorTheme)
  sound
ENDPROC

' ============================================================================
' (Utility Functions) Convert string to upper case, replace character in string
PROC ToUpper text
  for __i=text+1 to text + peek(text)
    if peek(__i) >$60 and peek(__i)<$7B then poke __i, peek(__i)-32
  next
ENDPROC

' ============================================================================
' Print #6 Replacement
' Since there is a custom screen location for 26 rows, print #6 will not work out of the box, 
' I use POKE routines to PRINT. These were written over time as needed, so not as organized or optimized as they could be.
' I wrote these before switching to a double buffered approach. A speed/size optimization could be done
' revert to PRINT #6, but would still need some trickery for the last 2 rows. 

PROC PrintUpper text
  temp$=$(text)
  @ToUpper &temp$
  @Print &temp$
ENDPROC

PROC PrintAt _col _row text
  @Pos _col, _row
  @Print text
ENDPROC

PROC POS _col _row
  __loc = screen + 40*_row +_col
ENDPROC

' Inverse is the analog to COLOR(128)
PROC PrintInv text
  __print_inverse=128
  @Print text
  __print_inverse=0
ENDPROC

' Reverse prints right aligned, starting at the current location
PROC PrintReverse
  __print_reverse = 1
ENDPROC

PROC PrintVal text
  @Print &str$(text)
ENDPROC

' Prints text, followed by space up the specified character length.
' Useful for printing "100" in one moment, then "1  " next, without building the string manually
PROC PrintValSpace text __len
  @PrintSpace &str$(text), __len
ENDPROC

' Prints space for the rest of this line - used for bottom status bar
PROC PrintSpaceRest
  __charsLeft = 40 - (__loc-screen-40*_row)
  if  __charsLeft>0 then mset __loc, __charsLeft, 0
ENDPROC

PROC PrintSpace text __len
  if __print_reverse 
    temp$=""
    while peek(text)+len(temp$)<__len: temp$ =+" ": wend
    temp$ =+ $(text)    
  else 
    temp$ = $(text)
    while len(temp$)<__len: temp$ =+" ": wend
  endif

  @print &temp$
ENDPROC

' ============================================================================
' Core Printing routine. Converts from ATASCII to INTERNAL, handling inverted, alphanumeric and a few other supported characters
PROC Print text
  if __print_reverse then __loc = __loc - peek(text)+1
  ' Go through each character and convert from ATASCII to INTERNAL, then poke to memory to draw it
  FOR __i=text+1 to text+peek(text)
    _code = peek(__i)
    
    if _code<32
      _code= _code + 64
    elif _code< 95
      _code= _code - 32
      if _code = 12 : _code=116 ' Handle comma
      elif _code = 13 : _code=125  ' Handle hyphen
      elif _code = 14 : _code=115  ' Handle period
      elif _code = 15 : _code=124  ' Handle /
      endif
    elif _code<128
    elif _code<160
      _code= _code + 64
    elif _code<223
      _code= _code - 32
    endif

    poke __loc,_code+__print_inverse
    inc __loc
  next

  ' Reset print reverse
  __print_reverse = 0
  
ENDPROC

' Print a byte directly (INTERNAL, not ATASCII)
PROC PrintByte _byte
  poke __loc,_byte
  inc __loc
ENDPROC


' ============================================================================
' (N AppKey Helpers) Call NRead/WriteAppKey to read or write app key

PROC __NOpenAppKey __N_creator __N_app __N_key __N_mode
  dpoke &NAppKeyBlock, __N_creator
  poke &NAppKeyBlock + 2, __N_app
  poke &NAppKeyBlock + 3, __N_key
  poke &NAppKeyBlock + 4, __N_mode
  SIO $70, 1, $DC, $80, &NAppKeyBlock, $09, 6, 0,0
ENDPROC

PROC NWriteAppKey __N_creator __N_app __N_key __N_string
  @__NOpenAppKey __N_creator, __N_app, __N_key, 1
  SIO $70, 1, $DE, $80, __N_string+1, $09, 64, peek(__N_string), 0
ENDPROC

PROC NReadAppKey __N_creator __N_app __N_key __N_string
  @__NOpenAppKey __N_creator, __N_app, __N_key, 0
  SIO $70, 1, $DD, $40, __N_string, $01, 66,0, 0
  MOVE __N_string+2, __N_string+1,64
  ' /\ MOVE - The first two bytes are the LO/HI length of the result. Since only the
  ' first byte is meaningful (length<=64), and since FastBasic string
  ' length is one byte, we just shift the entire string left 1 byte to
  ' overwrite the unused HI byte and instantly make it a string!
ENDPROC

' ============================================================================
' (N Helper) Gets the entire response from the specified unit into the provided buffer index for NInput to read from.
' WARNING! No check is made for buffer length. A more complete implimentation would handle that.
PROC NInputInit __NI_unit __NI_index
  __NI_bufferEnd = __NI_index + DPEEK($02EA)
  NGET __NI_unit, __NI_index, __NI_bufferEnd - __NI_index
ENDPROC

' ============================================================================
' (N Helper) Reads a line of text into the specified string - Similar to Atari BASIC: INPUT #N, MyString$
PROC NInput __NI_stringPointer

  ' Start the indexStop at the current index position
  __NI_indexStop = __NI_index
  
  ' Seek the end of this line (or buffer)
  while peek(__NI_indexStop) <> $9B and __NI_indexStop < __NI_bufferEnd
    inc __NI_indexStop
  wend

  ' Calculate the length of this result
  __NI_resultLen = __NI_indexStop - __NI_index
  
  ' Update the length in the output string 
  poke __NI_stringPointer, __NI_resultLen

  ' If we successfully read a value, copy from the buffer to the string that was passed in and increment the index
  if __NI_indexStop < __NI_bufferEnd
    move __NI_index, __NI_stringPointer+1, __NI_resultLen

    ' Move the buffer index for the next input
    __NI_index = __NI_indexStop + 1
  endif
ENDPROC

' ============================================================================
' Handles connection errors - silently retries first, then eventually stops, requiring a key press to proceed
PROC SetError text
  inc errorCount

  ' Expect occasional failure. Retry silently the first time
  if errorCount=1 then exit

  if errorCount<7
    @PrintAt 0,25, &"CONNECTING TO SERVER z"
    @PrintSpaceRest
    exit
  endif

  temp$=$(text)
  @PrintAt 0,25, &"ERROR "
  @printval PEEK(749)
  @print &": "
  @Print &temp$
  @PrintSpaceRest
  
  GET K

  ' After key press, set errorCount back to 3 to display connecting message
  errorCount=3

ENDPROC


' ============================================================================
' Call the server api endpoint
PROC ApiCall apiPath

  ' Set up URL
  temp$ = "N:"
  temp$ =+ serverEndpoint$
  temp$ =+ $(apiPath)
  temp$ =+ query$
  temp$ =+ ""$9B

  stateIsTables = $(apiPath) = "tables"

  ' Open connection
  pause
  NOPEN 8, 12, 0, temp$
  
  ' If not successful, then exit.
  IF SErr()<>1 
    @SetError &"COULD NOT CONNECT TO SERVER":EXIT
  ENDIF

  ' Change channel mode to JSON (1)
  pause
  SIO $71, 8, $FC, $00, 0, $1F, 0, 12, 1

  ' Ask FujiNet to parse JSON
  pause
  SIO $71, 8, $50, $00, 0, $1f, 0, 12, 0

  ' If successfully parsed JSON, update state
  IF SErr()=1 
    @UpdateState
  ELSE
    @SetError &"COULD NOT PARSE JSON":EXIT
  ENDIF

  ' Close connection
  pause
  NCLOSE 8
ENDPROC


' ============================================================================
' Calls the server, picking the appropriate path
Proc CallServer
  if len(requestedMove$)>0 
    path$ = "move/"
    path$ =+ requestedMove$
    requestedMove$=""
  else
    path$ = "state"
  endif

  ' Call the server (updates state on every response)
  @ApiCall &path$
endproc

Proc LeaveServer
  @ApiCall &"leave"
endproc

' ============================================================================
' Updates the state from the current Api call
PROC UpdateState
  
 ' Query Json response
 pause
 SIO $71, 8, $51, $80, &"N:"$9B+1, $1f, 256, 12, 0

 ' Check for query success
 pause
 NSTATUS 8
 IF PEEK($02ED) > 128
  @SetError &"COULD NOT CONNECT TO SERVER"
  EXIT
 ENDIF

' Initialize reading the api response
 @NInputInit 8, &responseBuffer

 ' Load state by looping through result and extracting each string at each EOL character
 isKey=1:inArray=0:playerCount=0:validMoveCount=0
 line$=""
 parent$=""
 do
  ' Get the next line of text from the api response
  @NInput &line$

  ' The response is mostly alternating lines of key and value, with the exception of arrays,
  ' which are handled as a special case further below.  
  if isKey

    ' An empty key means we reached the end of the response
    if len(line$) = 0 then exit

    key$= line$
    
    ' Special case - "players" and "validMoves" keys are arrays of key/value pairs
    if key$="pl" or key$="vm" or key$="NULL"

      ' If the key is a NULL object, we effectively break out of the array by setting parent to empty
      if key$="NULL" then key$=""

      parent$=key$

      ' Reset isKey since the next line will be a key
      isKey = 0
    endif
  else
    value$ = line$
    if stateIsTables
      ' Handle /tables call here. String space is at a premium, so reuse 
      ' the player_name and player_hand strings for the server details
      if key$="t" : player_name$(playerCount) = value$
      elif key$="n" : player_hand$(playerCount) = value$: @ToUpper &player_hand$(playerCount)
      elif key$="p"
        player_move$(playerCount) = value$
      elif key$="m"
        player_move$(playerCount) =+ " / "
        player_move$(playerCount) =+ value$
        inc playerCount 
      endif
    ' Valid Move properties
    elif parent$="vm" 
      if key$="m"
        @ToUpper &value$ 
         validMoveCode$(validMoveCount) = value$
      elif key$="n" 
        @ToUpper &value$ 
        validMove$(validMoveCount) = value$
        inc validMoveCount
      else :parent$="": endif
    ' Player properties
    elif parent$="pl"
      if key$="n" 
        @ToUpper &value$
        if len(value$)>8 then value$=value$[1,8]
        player_name$(playerCount) = value$
      elif key$="s"       : player_status(playerCount) = val(value$)
      elif key$="b"           : player_bet(playerCount) = val(value$)
      elif key$="m"          : player_move$(playerCount) = value$
      elif key$="p"         : player_purse(playerCount) = val(value$)
      elif key$="h"          : player_hand$(playerCount) = value$: inc playerCount 
      else
        parent$=""
      endif
    ' State level properties
    elif key$="l" : lastResult$ = value$
    elif key$="r" : round = val(value$)
    elif key$="p" : pot = val(value$) 
    elif key$="a" : activePlayer = val(value$)
    elif key$="p" : prompt$ = value$
    elif key$="v" : viewing = val(value$)
    elif key$="m" : 
      moveTime = val(value$)
      timer ' Reset timer when we get an updated movetime
    endif
  endif

  ' Toggle if we are reading a key or a value
  isKey = not isKey
 loop

 ' A successful connection has been made. Reset error count.
 errorCount = 0
ENDPROC

' ============================================================================
' Draws the specified card (e.g. "3C" for 3 of clubs) on the screen
' It looks at the existing playfield to render this card overlapping
' an existing card in a few supported cases.

PROC DrawCard text _col _row
  loc = screen + 40* _row +_col
  topBottomOffset=0
  
  ' Get character index based on value of card
  val = peek(text+1)

  if  val=$3F : topBottomOffset = 4: val=$3C ' Special case - back of card
  elif val=$54 : val = 10         ' 10
  elif val<$40 : val = val - $30  ' 2 to 9
  elif val=$4A : val = 12         ' Jack
  elif val=$51 : val = 13         ' Queen
  elif val=$4B : val = 14         ' King
  elif val=$41 : val = 15         ' Ace
  endif

   ' Get suit character index
   suit = peek(text+2)
   if   suit=$48 : suit = $C0: val=val+128 ' Hearts
   elif suit=$44 : suit = $C2: val=val+128 ' Diamonds
   elif suit=$43 : suit = $44 ' Clubs
   elif suit=$53 : suit = $46 ' Spade
   endif

  ' Check if a card exists underneath to the right. This will adjust some chars being drawn
  rightUnder =  peek(loc+42)>0

  if rightUnder 
    topBottomOffset=-4
    ' Draw left top and bottom corners
    poke loc-1, 75
    poke loc+159, 76
    suit=suit+32
  endif

  ' Draw Top of card
  dpoke loc, 256*($52+topBottomOffset )+ ($51+topBottomOffset)
  

  if val<>$3C

    ' Draw Value of card followed by blank space or second "10" character if  value is 10 (or 128+10 for red suits)
    ' If there is a card underneath to the right, adjust char
    isTen = val=10 or val=138
    val = val+(1+val*isTen + rightUnder*(61-9*isTen))*256 
    
    dpoke loc+40, val

    ' Draw Middle white
    dpoke loc+80, $0101 + (rightUnder*$3D*256)

    ' Draw Suit of card
    dpoke loc+120, suit*257+256
  else ' Back of card
    dpoke loc+40,$BC3C
    dpoke loc+80,$3CBC
    dpoke loc+120,$BC3C
  endif

  ' Draw Edges of card
  for locSide=loc+39 to loc+39+80 step 40
    
    ' Left edge 
    val = peek(locSide)
      
    ' Check if overlaying on top of existing card
    if   val=$3C or val=$BC : inc val ' Card back
    elif val=1  : val= 62 ' White space
    elif (val>64 and val<72) or (val>191 and val<201) : val=val+32: poke locSide-1, peek(locSide-1)+32 ' Suit
    elif val=11 or val=139 : val=val+52 ' 10 
    elif rightUnder : val=72
    else : val=73 ' Default left edge
    endif

    poke locSide, val
    
    ' Right edge - draw only if empty
    if not rightUnder then poke locSide+3, 74
    
  next locSide

  ' Draw Bottom of card
  dpoke loc+40*4, 256*($54+topBottomOffset )+ ($53+topBottomOffset)
ENDPROC

PROC DrawBorder
  @DrawCard &"AS",1,0: @DrawCard &"AH",37,0
  @DrawCard &"AD",1,20: @DrawCard &"AC",37,20
  
  @POS 4,1:FOR I=1 TO 8:@Print &""$9C$99$1A$1B:NEXT
  @POS 4,22:FOR I=1 TO 8:@Print &""$9C$99$1A$1B:NEXT
ENDPROC

' Sets DirAdr to -1,0,1 for left, nothing, or right
' Sets TriggerAdr to if trigger/space/enter pressed, else 0
Proc GetCommonInput __dirAdr __triggerAdr __keyPress

  ' First check keyboard
  __valD = 0
  __valT = 0
  __val = 0
  if key()
     get __val

     ' Cycle color if C is pressed (works on any screen so handle here)
     if __val=67 then @CycleColorTheme
  endif
  dpoke __keyPress, __val

  ' Set trigger pressed
  __valT = __val=32 or __val=$9B or not strig(0)
  
  ' Set directional based on arrow keys
  __valD = (__val=31 or __val=42 or __val=61 or __val=29)-(__val=28 or __val=30 or __val=43 or __val=45)

  ' If directional key not pressed, check joystick 
  if not  __valD
    __val = stick(0)
    
    ' Get left or right direction. Use "AND 4" and "AND 8" to so diagnal movement works as well for left/right
    __valD = (not __val&8) - (not __val&4)
  endif
  
  ' Update the value addresses passed in
  dpoke __dirAdr, __valD
  dpoke __triggerAdr, __valT

  
endproc


' ============================================================================
' Show initial screen to capture player's name
PROC WelcomeScreen

  @DrawBorder
 
  N=13
  @POS N,3: @Print &"      i"
  @POS n,4: @Print &"     ljl"
  
  @POS n,5: @Print &"FUJInjpjhNET"
  @POS n,6: @Print &"     lljh"
  @POS n,7: @Print &"      mll"
  @POS n+4,9:@Print &"5 CARD STUD"
  @ShowScreen
  pause 1

  ' Read player's name from app key
  @NReadAppKey AK_LOBBY_CREATOR_ID, AK_LOBBY_APP_ID, AK_LOBBY_KEY_USERNAME, &myName$
  @ToUpper(&myName$)
 
  ' Ask player for name if it is not yet populated
  if len(myName$)=0
    @POS 13,15:@Print &"ENTER YOUR NAME"

    @POS 14,17:@Print &";@@@@@@@@@@<"
    @POS 14,18:@Print &"?          "$BF
    @POS 14,19:@Print &"=":@PrintInv &"@@@@@@@@@@>"

    cursor = $76
    frame = 0

    @POS 16,18:@Print &MyName$:@PrintByte cursor
    
    ' Input box to capture player name and show blinking cursor
    ' Ensure at least 1 character name  
    do
      if key()
        get k
        if k=$9B and len(myName$)>0 then exit
        if k>96 then k=k-32
        if k=94 and len(myName$)>0
          myName$=myName$[1,len(myName$)-1]
          @POS 16,18::@Print &myName$:@PrintByte $76:@PrintByte 0
        endif
        
        if (k=32 or (k>=65 and k<=90) or (k>=48 and k<58)) and len(myName$)<8
          myName$=+chr$(k)
          @POS 16,18:@Print &MyName$:@PrintByte $76
        endif 
      endif

      pause
      inc frame
      if frame=40
        frame=0
        cursor = $76 + (cursor=$76)
        @POS 16+len(myName$),18:@PrintByte cursor
      endif
    loop


    ' Name has been captured. Save to app key and show welcome text
    @NWriteAppKey AK_LOBBY_CREATOR_ID, AK_LOBBY_APP_ID, AK_LOBBY_KEY_USERNAME, &myName$

  endif

  for n=15 to 19
  @POS 13,n:@Print &"                  "
  next
  @POS 16-len(myname$)/2,15:@Print &"WELCOME ":@Print &myname$
  @ShowScreen

  ' If the very first run, show the help screen
  temp$=""
  @NReadAppKey AK_CREATOR_ID, AK_APP_ID, AK_KEY_SHOWHELP, &temp$
  if len(temp$)=0
    temp$="1"
    @NWriteAppKey AK_CREATOR_ID, AK_APP_ID, AK_KEY_SHOWHELP, &temp$
    pause 60
    @ViewHowToPlay
  endif

ENDPROC

PROC QuitGame
  ' Enable FujiNet Config to take over D1:
  SIO $70, 1, $D9, $00, 0, $09, 0, 1,0

  ' Reboot via assembly: JMP $E477     
  i=usr(&""$4C$77$E4+1)
ENDPROC

PROC ViewHowToPlay
  ' This COULD retrieve from the server. Hard coded for now.
  @EnableDoubleBuffer
  @ClearStatusBar:@HidePlayerSecretCardMask':@EnableDoubleBuffer
  @ResetScreen:@DrawBorder
  @DrawBuffer
  y=4
  @PrintAt 8,3, &"HOW TO PLAY 5 CARD STUD"
  @DrawBuffer
                      ' __________________________________
  inc y:@PrintAt 3,y, &"PLAYERS ARE DEALT 5 CARDS OVER THE"
  inc y:@PrintAt 3,y, &"COURSE OF 4 ROUNDS. ON EACH ROUND"
  inc y:@PrintAt 3,y, &"PLAYERS BET, CALL, AND RE-RAISE."
  @DrawBuffer
  INC Y
  inc y:@PrintAt 18,y, &"MOVES"
  INC Y
  inc y:@PrintAt 3,y, &"FOLD   x QUIT THE HAND":inc y
  inc y:@PrintAt 3,y, &"CHECK  x FREE PASS":inc y
  inc y:@PrintAt 3,y, &"BET OR x INCREASE BET. OTHERS MUST"
  inc y:@PrintAt 3,y, &"RAISE    CALL TO STAY IN THE HAND":inc y
  inc y:@PrintAt 3,y, &"CALL   x MATCH THE CURRENT BET AND"
  inc y:@PrintAt 3,y, &"         STAY IN THE HAND" 
  
  @DrawBuffer
  @PrintAt 7,25, &"PRESS ANY KEY TO CONTINUE"

  @DrawBufferEnd
  get k
  @ResetScreen
  
ENDPROC


PROC SelectTable

  ' Reset some variables
  prevRound = 99
  prevPlayerCount = 0
  wasViewing = -1
  waitCount=-1

  if query$=""
    
    index=0
  
    ' Display screen to select table
    while query$="" 
      
      @DisableDoubleBuffer
      @ClearStatusBar
      @PrintAt 0,25, &"REFRESHING TABLE LIST z"

      @EnableDoubleBuffer
      @ResetScreen
      
      @DrawBorder
      @PrintAt 9,4, &"CHOOSE A TABLE TO JOIN"
      @PrintAt 4,7, &"TABLE"
      @PrintAt 29,7, &"PLAYERS"
      @PrintAt 4,8, &"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"

      playerCount = 0
      @ApiCall &"tables"
      
      if playerCount > 0 
        for i=0 to playerCount-1 
          @PrintAt 4,9+i*2, &player_hand$(i)
          @PrintReverse
          @PrintAt 35,9+i*2, &player_move$(i)
          if player_move$(i)[1,1]<>"0" 
            @pos 29,9+i*2:
            @printByte $7b
          endif
        next i
      else
        @PrintAt 5,12, &"SORRY, NO TABLES ARE AVAILABLE"
      endif
      @PrintAt 0,25, &"":@PrintSpaceRest
      @PrintAt 0,25, &"Rx REFRESH   Hx HELP  Cx COLOR   Qx QUIT"
      
      ' Clear out any queued key presses
      @ClearKeyQueue
      prevInputDir=0:inputDir=0:inputTrigger=0:j=20:chipColor=0

      if playerCount > 0 then @PrintAt 3, 9+index*2, &"o"
      
      @DrawBuffer

      repeat
        @GetCommonInput &inputDir, &inputTrigger, &k

        if playerCount>0 
          dec j
          if not j
            j=5+chipColor*15
            chipColor = not chipColor
            @PrintAt 3, 9+index*2, &"o"$EF[chipColor+1,1]
            @DrawBuffer
          endif
  
          ' Update Index Position
          @PrintAt 3, 9+index*2, &" "
          index = index + inputDir
          if index <0 : index= playerCount-1 :elif index>= playerCount: index=0 : endif
          @PrintAt 3, 9+index*2, &"o"$EF[chipColor+1,1]
          @DrawBuffer
        endif
        
        ' Q - quit
        if k=81
          @QuitGame
        elif k=82 
          ' R - refresh list
          exit
        elif k=72 ' H - how to play
          @ViewHowToPlay
          exit
        endif

      until inputTrigger and playerCount > 0

      sound
      ' If a table was selected, exit out of the loop and load the game!
      if inputTrigger 
        query$="?table="
        query$ =+ player_name$(index)
        
        temp$= serverEndpoint$
        temp$ =+ query$

        ' Update server app key in case of reboot 
        @NWriteAppKey AK_LOBBY_CREATOR_ID, AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, &temp$

        @ResetScreen
        @DrawBorder
        @PrintAt 20-len(player_hand$(index))/2, 15, &player_hand$(index)

        @DrawBufferEnd
        exit
      endif
      
    wend
  endif
  
  @POS 10,18:@Print &"CONNECTING TO SERVER"
  @ProgressAnim 18,20

  query$ =+ "&player="
  for i=&myName$+1 to &myName$+peek(&myName$)
    k = peek(i)
    if (k>=65 and k<=90) or (k>=48 and k<58)
      query$ =+ chr$(k)
    elif k = 32
     query$ =+ "+"
    endif
  NEXT

  
ENDPROC

PROC CheckIfSpectatorStatusChanged
  if viewing = wasViewing then exit
  wasViewing = viewing
   
  if viewing
    @PrintAt 0,25, &"TABLE FULL: WATCHING AS A SPECTATOR"
    @PrintSpaceRest
    for i=0 to 2
       sound 1,255,10,8:pause 2:sound:pause 3
    next i
    pause 80
  elif player_status(0)=0 or (round=1 and player_status(0)=1 and activePlayer <>0)
    ' Display intro text if player is joining the table on 
    ' the opening round or sitting down to wait for the next round.
    ' Otherwise, they are re-joining due to connection error, so we do not delay

    @PrintAt 0,25, &"YOU SIT DOWN AT THE TABLE"
    @PrintSpaceRest
    for vol=8 to 0 step -1: sound 0,80,10,vol:pause :next
    for vol=8 to 0 step -1: sound 0,100,10,vol:pause :next
    for vol=8 to 0 step -1: sound 0,65,10,vol:pause 1:next
    sound
    pause 50  
   
  endif
  
ENDPROC

PROC CheckIfPlayerCountChanged

  if playerCount <> prevPlayerCount
    
    ' Handle if player joins mid game
    if playerCount>1 and prevPlayerCount > 0 
      @ClearStatusBar
      @pos 0,25
      
      if playerCount < prevPlayerCount
        @Print &"A PLAYER LEFT THE TABLE"
        
        for j=8 to 2 step -2
          sound 1,255-j*j,10,j:pause 2:sound:pause 8
        next
      ELSE
        lastResult$="A NEW PLAYER JOINS THE TABLE"
        @Print &lastResult$
      ENDIF
    
      if playerCount > prevPlayerCount
        for j = 1 to 30 step 5
          sound j,20,0,1+j/5:pause 2
          for vol=j/5 to 0 step -1
            sound j,20,0,vol:pause
          next
          pause 3
        next
      endif

      sound
    
      pause 40
      if round > 1 then noAnim = 1
    endif

    prevPlayerCount = playerCount
    
    ' Don't shuffle player locations until
    ' multple players exist
    if playerCount<2 then exit

    i=0
    for j=(playerCount-2)*8 to (playerCount-2)*8+7
      n=playerCountIndex(j)
      playerX(i) = playerXMaster(n)
      playerY(i) = playerYMaster(n)
      playerDir(i) = playerDirMaster(n)
      playerBetX(i) = playerBetXMaster(n)
      playerBetY(i) = playerBetYMaster(n)
      inc i
    next

    
  endif
endproc

PROC ResetStateIfNewGame
  if round >= prevRound then exit
 
  if prevRound<> 99
    @SetStatusBarHeight 1
    @ClearStatusBar   
  endif

  xOffset=0
  currentCard=0
  cursorY=246
  cursorX=128
  prevPot=0
  
  ' If the round is already past 1, we are joining a game in progress. Skip animation this update
  if round>1 then noAnim=1

  @HidePlayerSecretCardMask
ENDPROC

PROC RenderPot
  ' Pot border
  @POS 17,12:@PrintInv &";@@@@<"
  @POS 17,13:@PrintInv &"?o   ?"
  @POS 17,14:@PrintInv &"=@@@@>"

  ' Pot total
  @POS 20-1*(pot>99),13:@PrintVal pot

ENDPROC

PROC RenderBets
  if round <1 or round >4 then exit

  for i=0 to playerCount-1
    ' Print bet
    if player_bet(i)>0 
      x= playerX(i)+playerBetX(i)
      if playerDir(i)<0 then x=x-len(str$(player_bet(i)))
      @POS x, playerY(i)+playerBetY(i)+1
      @PrintByte $EF
      @PrintVal player_bet(i)
    endif
    
    ' Print Move
    @POS playerX(i)+playerBetX(i), playerY(i)+playerBetY(i)
    if playerDir(i)<0 then @PrintReverse    
    @PrintSpace &player_move$(i), 5
  next
ENDPROC

PROC RenderActivePlayer
  if activePlayer=0 and not viewing
     @printat playerX(0)-5, playerY(0)+3, &"xYOU" 
  elif activePlayer>=0
    dir = playerDir(activePlayer)
    wid = len(player_name$(activePlayer))
    x = playerX(activePlayer)-dir+(dir<0)
    y = playery(activePlayer)-1

    @Printat x, y, &"xy"[1+(dir<0),1]
    @MoveHighlightToLocation 52+4*(x-(wid+1)*(dir<0)), 28+8*y, wid, 0

  endif
ENDPROC

PROC RenderGameStatus
  if activePlayer = 0 then exit
  'round=5
  'activePlayer=-1
  'lastResult$="some result winner, queens and sixes"
  @pos 0,25
  if activePlayer>0
    @Print &"WAITING ON "
    @PrintUpper &player_name$(activePlayer)

  elif activePlayer< 0 and (round = 5 or round = 0)
   ' If everyone but the player folded, don't indicate their card was flipped
    ii=1
    if round=5 and player_status(0)=1
      ii=0
      for i=1 to playerCount-1
        if player_status(i)=1 then ii=1
      next
    endif

    if ii=1 then @HidePlayerSecretCardMask
    

    ' End of (or in between) games
     if round=5 and prevRound <> round 
      sound 1,200,10,8:pause 2
      sound 1,170,10,8:pause 2
    endif
    
    ' If game over message is >40 chars, split result into two rows, rendering the first result here
    if round=5 and len(lastResult$)>40 
      @SetStatusBarHeight 0
      text = &lastResult$
      FOR i=text+1 to text+peek(text)
        ' Split on comma (,)
        if peek(i) = $2C 
          @pos 0,24
          i=i-text
          @PrintUpper &lastResult$[1,i]
          @PrintSpaceRest
          lastResult$=lastResult$[i+2]
          @pos 0,25
          exit
        endif
      NEXT
    endif

    @PrintUpper &lastResult$
    
    if round=5 and prevRound <> round 
      sound 1,150,10,8:pause 2
      sound 1,140,10,8:pause 3
      sound 1,135,10,8:pause 2
      sound 1,132,10,8:pause 3
      sound
    endif
  endif

  @PrintSpaceRest
  if round < 5
    
    if round = 0 
      ' Waiting for a player to join
      waitCount = (waitCount + 1) mod 8
      @pos 30, 25
      @PrintInv &"pop  o|po p o|po  po|p o po|p  opo|p o po|po  po|po p o"[7*waitCount+1,6]
    else
      ' Waiting on a player to make a move
      if moveTime>0
        @Pos 38,25
        @PrintReverse
        @PrintVal moveTime
        @pos 39, 25:@PrintByte $7A
      endif
    endif
  endif
 
ENDPROC


PROC RenderChipsToPotOnRoundEnd
  if round <= prevRound or pot =0 then exit
  
  @ClearStatusBar

  ' Hide Moves
   for i=0 to playerCount-1
    @POS playerX(i)+playerBetX(i), playerY(i)+playerBetY(i)
    if playerDir(i)<0 then @PrintReverse    
    @Print &"     "
  next

  ' Don't render bets to pot if the pot hasn't changed
  if prevPot = pot then exit
  prevPot = pot

  @DrawBuffer
  pause 20

  ' Move bets to pot

  potNote=250

  for i=0 to playerCount-1  
    pause 1
    @POS playerX(i)+playerBetX(i), playerY(i)+playerBetY(i)+1
    if playerDir(i)<0 then @PrintReverse
    @print &"     "
    sound 1,potNote,10,6
    @DrawBuffer
    sound 
    potNote = potNote - 5
  next

  ' Update Pot
  @RenderPot
  @DrawBuffer
  pause 45
  

ENDPROC

' ============================================================================
' Refreshes the entire table screen from the state.
' There is a bit of checking of gamestate here to know when to draw
' new cards, show the player moves and current bet total. 
PROC UpdateScreen
  @CheckIfSpectatorStatusChanged
  @CheckIfPlayerCountChanged
  @EnableDoubleBuffer

  @RenderChipsToPotOnRoundEnd

  @ResetScreen
  @ResetStateIfNewGame

  @RenderPot
  if playerCount>1
    @RenderNamePurse
    @RenderBets  
    @RenderCards
  endif
  @RenderGameStatus
  
  @DrawBufferEnd

  @RenderActivePlayer
  @ShowScreen
  
  prevRound=round
ENDPROC

PROC RenderNamePurse

  for i=0 to playerCount-1
    
    ' Print name, left or right justified based on direction
    x = playerX(i)
    if playerDir(i)<0 then x=x+1

    if i>0 or viewing
      @POS x, playerY(i)-1
      if playerDir(i)<0 then @PrintReverse
      @Print &player_name$(i)

      if playerDir(i)>0 then @POS x-1, playerY(i)-1
      if activePlayer<>i
        @PrintByte 0
      endif
    else
      @printat x-5, playerY(i)+3, &" YOU"
    endif

    ' Print purse (chip count)
    if i=0 then x=x-2
    y = playerY(i)-2+(i=0)*3
    if playerDir(i)<0 or i=0 then x=x-len(str$(player_purse(i)))
    @POS x,y
    @PrintByte $EF
    @PrintVal player_purse(i)

 
  next
ENDPROC

PROC RenderCards  
  if round <1 then exit
  
  ' Draw next cards with sound/delay as if the dealer is dealing out one by one
  
  cardIndex = 0
  xOffset = 0
  finalFlip = prevRound<round and round=5
  
  while cardIndex <= round
    doAnim = (round<5 and not noAnim and cardIndex >= currentCard)' or (round=5 and prevRound<round and cardIndex=4)
    if doAnim 
      doAnim = 1
    endif
    if doAnim then pause 10
    inc cardIndex
    j=cardIndex*2-1
    if doAnim then pause 10
    for ii=1 to playerCount
      i = ii mod playerCount
      if len(player_hand$(i))>j 
        fullFirst = i=0 and (not Viewing or finalFlip)
        if doAnim then sound 1,0,0,1
        hand$=player_hand$(i)[j,2]
        if finalFlip and j=1 and i>0 then hand$="??"
        @DrawCard &hand$, playerX(i)+((not fullFirst)*xOffset+(fullFirst)*(J-1))*playerDir(i), playerY(i)

        if doAnim
          @DrawBuffer
          for vol=2 to 0 step -1
            pause :sound 1,0,0,vol
          next
          sound
        endif
    
        if doAnim and cardIndex >1 then pause 5
      endif
    next
    inc xOffset
    if xOffset>1 or (round=5 and not finalFlip) then inc xOffset
  wend
  
  ' Create or hide player's hidden card mask
  ' End of game reveal or player folded
  
  if round=5 or player_status(0) <> 1
    
    ' If everyone else folded, don't flip the winner's card
    ii=1
    if round=5 and player_status(0)=1
      ii=0
      for i=1 to playerCount-1
        if player_status(i)=1 then ii=1
      next
    endif

    if ii=1 then @HidePlayerSecretCardMask
  elif currentCard = 0 and round<5 
    @CreatePlayerSecretCardMask
  endif
  
  ' If round 5, flip all remaining hands
  if finalFlip
    @DrawBuffer
    for i=1 to playerCount-1
      if player_status(i)=1

        ' Reveal full hand on-screen  
        sound 1,0,0,1:vol=4:xOffset=0
        x = screen+playerY(i)*40+playerX(i)-1-7*(playerDir(i)<0)
        for j=0 to 199 step 40:mset x+j,11,0:next

        for j=1 to len(player_hand$(i)) step 2
          @DrawCard &player_hand$(i)[j,2], playerX(i)+((i>0)*(xOffset)+(i=0)*(J-1))*playerDir(i), playerY(i)
          inc xOffset:inc xOffset
          sound 1,0,0,vol: if vol>0 then dec vol
        next
        @DrawBuffer
        sound 
        pause 40
      endif
    next
  endif

  currentCard = cardIndex
  noAnim=0
ENDPROC

PROC ClearCursor
  mset pm.2+cursorY,2,0
ENDPROC

PROC MoveHighlightToLocation __x __y __len __lineStyle
  mset pm.2,256,0
  bit=128:total=0
  for j=1 to __len
    total = total + bit
    bit=bit/2
  next 

  x=__x-20
  y=__y
  cursorX = __x
  cursorY = __y
  ny=y
  nx=x
  repeat

    if x<>cursorX 
      delta = (cursorX-x)/4
      if delta=0 then delta = sgn(cursorX-x)
      nx=x+delta
    endif

    pause
    mset pm.2+ny,2+__lineStyle,total
    PMHPOS 2,nx
    y=ny:x=nx
  until x=cursorX and y=cursorY
endproc

PROC WaitOnPlayerMove
  
  if viewing or activePlayer <> 0 then exit

  ' Clear the bottom row  
  mset screen+40*25,40,0

  text_color(2) = $0

  ' Draw the moves and store the locations and player bits
  
  @POS 1,25
  x=0

  for i=0 to validMoveCount-1
    move_loc(i) = x
    @Print &validMove$(i)
    @Print &"  "
    x = x + len(validMove$(i))+2

    bit=128:total=0
    for j=1 to len(validMove$(i))
      total = total + bit
      bit=bit/2
    next 
    move_bits(i)=total    
  next 

  ' Setup move player line indicator
  move = validMoveCount>1
  x = 52+4*move_loc(move)
  @MoveHighlightToLocation x, 232, len(validMove$(move)), 1
  x=cursorX

 ' Fade in moves and play ding-ding sound
  sound 0,80,10,5:vol=8
  for color = move_color-12 to move_color 
    text_color(2) = color
    pause
    vol=vol-1
    if vol=0  then vol=8
    sound 0,80,10,vol
  next

  ' Fade out ding-ding sound
  while vol>=0
    sound 0,80,10,vol:pause 2:dec vol
  wend

  ' Turn off keystroke sound
  POKE 731,255

  ' Clear out any queued key presses
  @ClearKeyQueue

  ' Determine jiffies per second (60 NTSC, 50 for PAL)
  jifsPerSec = 60-(PEEK(53268)=1)*10
  maxJifs = jifsPerSec*moveTime
  jifCount = 0
  
  prevInputDir=0:inputDir=0:inputTrigger=0
  repeat

    ' Update time countdown timer once per second
    inc jifCount    
    if jifCount >= jifsPerSec
      jifCount=1
      moveTime = (maxJifs-time)/jifsPerSec
      if moveTime > 0
        sound 0, 200,8,3
        pause
        @Pos 38,25
        @PrintReverse
        @PrintValSpace moveTime, 2
        @pos 39, 25:@PrintByte $7A
        sound 
      endif
    endif

    pause
    if x<>cursorX 
      x=x+(cursorX-x)/2
      if abs(cursorX-x)<=1 : x=cursorX : sound : endif
      PMHPOS 2,x
      sound
    endif

    ' Read input
    @GetCommonInput &inputDir, &inputTrigger, &k
   
    if k=27 
      @ResetScreenBuffered
      @AskToLeave
      if query$=""
        @SelectTable
      else
        @UpdateScreen
      endif
      k=27
      exit
    endif
    ' If moved input left or right
    if prevInputDir = 0 and inputDir
      
      move = move + inputDir

      ' Check if in bounds of move counts
      if move <0 or move >= validMoveCount 
        ' At edge. Show bump animation
        move = move - inputDir
        PMHPOS 2,x+inputDir
        sound 1,255,10,8:pause 2:sound
        PMHPOS 2,x
      else
        ' Can move
        sound 1,100,10,8
        mset pm.2+cursorY,3,move_bits(move)
        cursorX = 52+4*move_loc(move)
      endif
    endif

    ' Store inputDir location so we know whenever the player changes direction
    prevInputDir = inputDir
  until moveTime<1 or inputTrigger
  
  ' Exit early if Esc was hit
  if k=27 then exit

  ' Clear the other moves 
  sound 1,100,10,8
  pause
  mset screen+40*25+1, move_loc(move),0
  mset screen+40*25+1+ move_loc(move)+len(validMove$(move))+1,40,0
  requestedMove$ = validMoveCode$(move)
  text_color(2) = move_color
  
  sound 1,80,10,8:pause 2:
  sound 1,70,10,8:pause 2:sound
  
  ' Clear move highlight
  PMHPOS 2,0

  ' Turn on keystroke sound
  POKE 731,0
  
ENDPROC

Proc HidePlayerSecretCardMask
  PMHPOS 1,0
endproc

Proc ShowPlayerSecretCardMask
  PMHPOS 1,116
endproc

' ===================================
' Update "player 1" to be a mask of the player's hidden card, so it can be
' displayed as darker to indicate it is hidden from other players
PROC CreatePlayerSecretCardMask
  
  ' If viewing a game, there is nothing to mask
  if viewing then exit

  ' Move the mask offscreen when creating
  @HidePlayerSecretCardMask

  ' Left characters
  i = screen+40*playerY(0)+playerX(0)+40
  for j= 0 to 23
    if j mod 8 = 0 
      val = peek(i) mod 128:  src = &charBuffer+val*8: i = i + 40
    endif
    v = peek(src+j mod 8)
    poke pm.1+187+j,  (v&$C0<$C0)*$80 + (v&$30<$30)*$40 + (v&$0C<$0C)*$20 + (v&$03<$03)*$10
  next

  ' Right characters
  i = screen+40*playerY(0)+playerX(0)+41
  for j= 0 to 23
    if j mod 8 = 0 
      val = peek(i) mod 128:  src = &charBuffer+val*8: i = i + 40
    endif
    v = peek(src+j mod 8)
    poke pm.1+187+j, peek(pm.1+187+j) + (v&$C0<$C0)*$08 + (v&$30<$30)*$04 + (v&$0C<$0C)*$02
  next

  poke pm.1+186,$7e:poke pm.1+211,$7e

  @ShowPlayerSecretCardMask
ENDPROC

proc AskToLeave
  ' Turn on keystroke sound
  POKE 731,0

  @ClearStatusBar
  @HidePlayerSecretCardMask
  
  @EnableDoubleBuffer
  
  x=8:Y=5

  INC Y:@PrintAt x,y, &";@@@@@@@@@@@@@@@@@@@@@<"
  INC Y:@PrintAt x,y, &"?"$19:@PrintAt x+22,y, &"?"
  INC Y:@PrintAt x,y, &"?":@PrintAt x+22,y, &"?"
  INC Y:@PrintAt x,y, &"?    Q: QUIT TABLE    ?"
  INC Y:@PrintAt x,y, &"?":@PrintAt x+22,y, &"?"
  INC Y:@PrintAt x,y, &"?    H: HOW TO PLAY   ?"
  INC Y:@PrintAt x,y, &"?":@PrintAt x+22,y, &"?"
  INC Y:@PrintAt x,y, &"?    C: TABLE COLOR   ?"
  INC Y:@PrintAt x,y, &"?":@PrintAt x+22,y, &"?"
  INC Y:@PrintAt x,y, &"?  ESC: KEEP PLAYING  ?"
  INC Y:@PrintAt x,y, &"?":@PrintAt x+22,y, &"?"
  INC Y:@PrintAt x,y, &"?":@PrintAt x+21,y, &""$1c"?" 
  INC Y:@PrintAt x,y, &"=@@@@@@@@@@@@@@@@@@@@@>"
  
  @DrawBorder
  @DrawBufferEnd

  @ClearKeyQueue

  do
    @GetKeyPress
    
    if k=72 ' H - how to play
      @ViewHowToPlay
      exit
    elif k=76 or k=81   ' Q/L - leave table
      @EnableDoubleBuffer
      @ResetScreen
      @DrawBorder
      @DrawBufferEnd

      ' Clear server app key in case of reboot 
      @NWriteAppKey AK_LOBBY_CREATOR_ID, AK_LOBBY_APP_ID, AK_LOBBY_KEY_SERVER, &""

      ' Inform server player is leaving
      @PrintAt 15,11, &"PLEASE WAIT"
      @LeaveServer
      @ProgressAnim 18,13
      
      query$=""
     ' if k=81 then @QuitGame
      exit
    endif

    ' Esc - cancel and play
    if k=27 then exit
  loop

  '@ResetScreenBuffered
endproc

proc ProgressAnim x y
  @Pos x,y
  pause 15:@Print &"o "
  pause 15:@Print &"p "
  pause 15:@Print &"o"
endproc

' ==========================================================
' DATA - Character Fonts
data font() byte = 0,0,0,0,0,0,0,0,
data byte = 170,170,170,170,170,170,170,170,
data byte = 170,174,187,187,171,174,186,191,
data byte = 170,174,187,171,174,171,187,174,
data byte = 170,171,187,187,191,171,171,171,
data byte = 170,191,186,190,171,171,171,190,
data byte = 170,175,186,190,187,187,187,174,
data byte = 170,191,187,171,174,174,174,174,
data byte = 170,174,187,187,174,187,187,174,
data byte = 170,174,187,187,187,175,171,190,
data byte = 170,186,187,187,187,187,187,186,
data byte = 170,234,186,186,186,186,186,234,
data byte = 170,175,171,171,171,187,187,174,
data byte = 170,174,187,187,187,187,174,171,
data byte = 170,187,187,190,190,187,187,187,
data byte = 170,174,187,187,191,187,187,187,
data byte = 0,32,136,136,136,136,136,32,
data byte = 0,32,160,32,32,32,32,168,
data byte = 0,32,136,136,8,32,128,168,
data byte = 0,32,136,8,32,8,136,32,
data byte = 0,8,136,136,168,8,8,8,
data byte = 0,168,128,160,8,8,8,160,
data byte = 0,40,128,160,136,136,136,32,
data byte = 0,168,136,8,32,32,32,32,
data byte = 0,32,136,136,32,136,136,32,
data byte = 0,32,136,136,136,40,8,160,
data byte = 0,0,32,32,0,0,32,32,
data byte = 0,0,0,0,0,3,15,12,
data byte = 0,0,0,0,192,240,60,12,
data byte = 12,12,12,15,3,0,0,0,
data byte = 12,12,12,60,240,192,0,0,
data byte = 12,12,12,12,12,12,12,12,
data byte = 0,0,0,0,255,255,0,0,
data byte = 0,32,168,136,136,168,136,136,
data byte = 0,160,136,136,160,136,136,160,
data byte = 0,40,128,128,128,128,128,40,
data byte = 0,160,136,136,136,136,136,160,
data byte = 0,168,136,128,160,128,128,168,
data byte = 0,168,136,128,160,128,128,128,
data byte = 0,40,136,128,152,136,136,40,
data byte = 0,136,136,136,168,136,136,136,
data byte = 0,168,32,32,32,32,32,168,
data byte = 0,40,8,8,8,8,136,32,
data byte = 0,136,136,160,160,136,136,136,
data byte = 0,128,128,128,128,128,128,168,
data byte = 0,136,168,168,136,136,136,136,
data byte = 0,136,136,168,168,168,136,136,
data byte = 0,32,136,136,136,136,136,32,
data byte = 0,160,136,136,136,160,128,128,
data byte = 0,32,136,136,136,136,32,8,
data byte = 0,160,136,136,136,160,136,136,
data byte = 0,40,136,128,32,8,136,160,
data byte = 0,168,32,32,32,32,32,32,
data byte = 0,136,136,136,136,136,136,168,
data byte = 0,136,136,136,136,168,32,32,
data byte = 0,136,136,136,136,168,168,136,
data byte = 0,136,136,32,32,136,136,136,
data byte = 0,136,136,136,168,32,32,32,
data byte = 0,168,8,32,32,128,128,168,
data byte = 0,0,0,0,0,0,0,0,
data byte = 76,76,19,19,196,196,49,49,
data byte = 77,77,17,17,197,197,49,49,
data byte = 169,169,169,169,169,169,169,169,
data byte = 169,233,185,185,185,185,185,233,
data byte = 171,175,175,175,175,171,171,170,
data byte = 186,254,254,254,254,250,250,234,
data byte = 170,171,175,175,175,171,170,170,
data byte = 234,250,254,254,254,250,234,170,
data byte = 171,171,170,175,175,175,170,171,
data byte = 250,250,234,254,190,190,234,250,
data byte = 170,171,171,175,175,175,170,171,
data byte = 234,250,250,254,254,190,234,250,
data byte = 6,6,6,6,6,6,6,6,
data byte = 1,1,1,1,1,1,1,1,
data byte = 64,64,64,64,64,64,64,64,
data byte = 0,0,0,0,0,0,1,1,
data byte = 5,13,3,0,0,0,0,0,
data byte = 0,0,0,0,0,85,85,170,
data byte = 0,0,0,0,0,81,85,165,
data byte = 170,85,85,255,0,0,0,0,
data byte = 165,85,95,240,0,0,0,0,
data byte = 0,0,0,0,0,21,85,106,
data byte = 0,0,0,0,0,84,85,169,
data byte = 106,85,213,63,0,0,0,0,
data byte = 169,85,87,252,0,0,0,0,
data byte = 0,0,0,0,0,21,85,113,
data byte = 0,0,0,0,0,84,85,49,
data byte = 127,85,213,63,0,0,0,0,
data byte = 77,85,87,252,0,0,0,0,
data byte = 255,252,240,240,192,192,192,0,
data byte = 255,63,15,15,3,3,3,0,
data byte = 0,192,192,192,240,240,252,255,
data byte = 0,3,3,3,15,15,63,255,
data byte = 0,0,0,0,0,0,0,0,
data byte = 0,0,0,0,0,0,0,0,
data byte = 0,0,0,0,0,0,0,0,
data byte = 174,191,191,191,191,175,175,171,
data byte = 233,249,249,249,249,233,233,169,
data byte = 171,175,191,191,191,175,171,170,
data byte = 169,233,249,249,249,233,169,169,
data byte = 175,175,171,191,190,190,171,175,
data byte = 233,233,169,249,249,249,169,233,
data byte = 171,175,175,191,191,190,171,175,
data byte = 169,233,233,249,249,249,169,233,
data byte = 0,0,0,64,0,0,0,0,
data byte = 0,0,0,0,0,0,16,16,
data byte = 16,32,168,169,168,168,32,16,
data byte = 0,48,252,253,252,252,48,16,
data byte = 16,16,16,85,16,16,16,16,
data byte = 16,16,16,85,16,16,16,0,
data byte = 0,0,16,21,16,16,0,0,
data byte = 0,48,116,220,220,220,116,48,
data byte = 0,48,184,236,236,236,184,48,
data byte = 0,40,170,170,174,174,40,0,
data byte = 0,40,170,170,186,186,40,0,
data byte = 0,0,0,0,0,32,32,0,
data byte = 0,0,0,0,0,32,32,128,
data byte = 170,170,85,0,0,0,164,169,
data byte = 0,148,164,164,164,164,164,148,
data byte = 0,124,92,92,92,92,92,124,
data byte = 0,0,0,32,24,20,28,48,
data byte = 0,0,0,32,144,80,208,48,
data byte = 0,40,138,138,130,170,170,40,
data byte = 255,215,215,215,255,0,102,153,
data byte = 0,8,8,32,32,32,128,128,
data byte = 0,0,0,0,168,0,0,0,
data byte = 0,0,0,0,0,0,0,0,
data byte = 0,0,0,0,0,0,0,0


' ============================================================================
' Call to show the screen, or occasionally to stop Atari attract/screensaver color mode from occuring
PROC ShowScreen
  poke 77,0:pause:poke 559,46+16
ENDPROC


' ============================================================================
' Call to clear the screen to an empty table
PROC ResetScreen
  mset screen,40*26,0
  
  ' Draw the four black corners of the screen
  poke screen, 89:poke screen+39,90
  poke screen+40*24, 91:poke screen+40*25-1,92
ENDPROC

' Reset the screen in a double buffered manner
PROC ResetScreenBuffered
  @EnableDoubleBuffer
  @ResetScreen
  @DrawBufferEnd
endproc

PROC DrawBuffer
  pause
  move &screenBuffer+1040,&screenBuffer, 1040
ENDPROC

Proc DrawBufferEnd
  @DrawBuffer
  @DisableDoubleBuffer
endproc

PROC EnableDoubleBuffer
  screen = &screenBuffer + 1040
ENDPROC

PROC DisableDoubleBuffer
  screen = &screenbuffer
ENDPROC

PROC ClearStatusBar
  PMHPOS 2,0
  mset &screenBuffer+1040+40*25, 40,0
  mset &screenBuffer+40*25, 40,0
ENDPROC

PROC ClearKeyQueue
  ' Clear out any queued key presses
  while key() : get k:wend
ENDPROC

' Fills K value in with keypress
' Sets K to "H" if "HELP" consol key pressed
PROC GetKeyPress
  if peek(732)=17 or key() 
    if peek(732)=17
      k = 72: poke 732,0
    else
      get K 
      ' Cycle color if C is pressed (works on any screen so handle here)
      if k=67 then @CycleColorTheme
    endif
  else
    K = 0
  endif
ENDPROC



' ============================================================================
' Init screen/graphics - leaves screen blank. ShowScreen must be called afer
PROC InitScreen

  ' ============= PLAYER MISSLE GRAPHICS =============

  ' Use player missle graphics as follows:
  ' Player 0 and 3 - Left and Right black bars to cover up the background to render a nice round table
  ' Player 1 - Darken player's secret card
  ' Player 2 - Move selection/active player indicator
  pmgraphics 1

  ' Keeping here in case I need it again - Set player missle priority: Players 0-1, playfield, players 2-3, background
   P.623,2

  ' Hide screen for faster startup
  poke 559,0
    
  ' Clear player data, then set sidebars
  mset pm.0,1024,0
  mset pm.0,255,255
  mset pm.3,255,255

  ' Make the sidebar and selection cursor 4x wide to block as much of the screen as possible, and the secret card 1x wide
  mset 53256,4,3: poke 53257, 0
  
  ' Set side par positions to left and right edge of screen
  PMHPOS 0,16:PMHPOS 3,208
  
  ' ============= COLORS =============

  ' Order: Players 0,1,2,3, Playfield 0,1,2,3, Background
  
  if PEEK(53268)=1 ' Check if we are running PAL on GTIA machines
    ' PAL colors
    move &""$00$0a$fa$00$78$0E$00$24$00+1, 704, 9
    move_color = $EE
  else
    ' NTSC colors
    move &""$00$0a$fa$00$88$0E$00$34$00+1, 704, 9
    move_color = $FE
  endif

  text_color(2) = move_color

  @CycleColorTheme  

  ' ============= PLAYFIELD =============
  @DisableDoubleBuffer
  
  ' Copy the custom character set data to the location
  move &font,&charBuffer,8*128

  ' Tell Atari to use the new location
  P.756,&charBuffer/256

  ' Custom Display List to give us 2 more rows (40x26) with DLI for coloring
  DL$ = ""$20$F0$44$00$00$04$04$04$04$04$04$04$04$04$04$04$04$04$04$04$04$04$04$04$04$04$04$84$84$20$04$41$00$00
  
  ' Copy the display list from the string to memory.
  displayList = &DL$+1
  dpoke displayList+len(DL$)-2,displayList

  ' Tell the display list the new location of the screen buffer
  dpoke displayList+3,&screenBuffer

  ' Use DLI to change the text/background colors of the bottom status rows
  DLISET dli_colors = background_color INTO $D01A, text_color INTO $D017
  DLI dli_colors

  ' Enable the new Display list
  dpoke 560, displayList

  ' Reset the screen
  @ResetScreen

  ' Disable accidental break key press
  poke 16,64:poke 53774,64

ENDPROC


PROC SetStatusBarHeight __height
 
  if __height = 1
    background_color(1)=background_color(0)
    text_color(1)=text_color(0)
    dpoke displayList+len(DL$)-6,$2084
    @DrawBuffer
  elif background_color(1)=background_color(0)
    mset &screenBuffer+40*24,80,0
    background_color(1)=0
    text_color(1)=text_color(2)
    dpoke displayList+len(DL$)-6,$8420
  endif
  
ENDPROC


@InitScreen
@WelcomeScreen
@SelectTable

' ===================================
' MAIN PROGRAM LOOP

do
  @CallServer
  if errorCount=0
    @UpdateScreen
    @WaitOnPlayerMove
  else
    pause 10
  endif

  if len(requestedMove$)=0 then pause 20

  @GetKeyPress
  if k
    if k=27 
      @ResetScreenBuffered
      @AskToLeave
      if query$=""
        @SelectTable
      else
        @ShowPlayerSecretCardMask
        @UpdateScreen
      endif
    elif k=72 ' H
      @ViewHowToPlay
    endif
  endif

loop
