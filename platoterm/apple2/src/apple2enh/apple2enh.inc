
;-----------------------------------------------------------------------------
; Zero page stuff

WNDLFT  :=      $20     ; Text window left
WNDWDTH :=      $21     ; Text window width
WNDTOP  :=      $22     ; Text window top
WNDBTM  :=      $23     ; Text window bottom+1
CH      :=      $24     ; Cursor horizontal position
CV      :=      $25     ; Cursor vertical position
BASL    :=      $28     ; Text base address low
BASH    :=      $29     ; Text base address high
INVFLG  :=      $32     ; Normal/inverse(/flash)
PROMPT  :=      $33     ; Used by GETLN
RNDL    :=      $4E     ; Random counter low
RNDH    :=      $4F     ; Random counter high
HIMEM   :=      $73     ; Highest available memory address+1

;-----------------------------------------------------------------------------
; Vectors

DOSWARM :=      $03D0   ; DOS warmstart vector
BRKVec  :=      $03F0   ; Break vector
SOFTEV  :=      $03F2   ; Vector for warm start
PWREDUP :=      $03F4   ; This must be = EOR #$A5 of SOFTEV+1

;-----------------------------------------------------------------------------
; Hardware

; Keyboard input
KBD     :=      $C000   ; Read keyboard
KBDSTRB :=      $C010   ; Clear keyboard strobe

; 80 column video switches
CLR80COL:=      $C000   ; Disable 80 column store
SET80COL:=      $C001   ; Enable 80 column store
RAMRDOFF:=      $C002   ; 48K RAM read off
RAMWTOFF:=      $C004   ; 48K RAM write off
W80OFF  :=      $C00C   ; Write for single display
W80COL  :=      $C00D   ; Write for double display
RD80COL :=      $C018   ; >127 if 80 column store enabled
VBL     :=      $C019   ; Vertical Blanking
READPAGE:=      $C01C   ; $80 = aux page
RD80VID :=      $C01F   ; >127 if 80 column video enabled

; Character set switches
CLRALTCHAR :=   $C00E   ; Normal Apple II char set
SETALTCHAR :=   $C00F   ; Norm/inv LC, no flash
ALTCHARSET :=   $C01E   ; >127 if alt charset switched in

; Language card switches
RDLCBNK2:=      $C011   ; >127 if LC bank 2 in use
RDLCRAM :=      $C012   ; >127 if LC is read enabled
ROMIN   :=      $C081   ; Swap in D000-FFFF ROM
LCBANK2 :=      $C083   ; Swap in LC bank 2
LCBANK1 :=      $C08B   ; Swap in LC bank 1

; Video mode switches
TXTCLR  :=      $C050   ; Display graphics
TXTSET  :=      $C051   ; Display text
MIXCLR  :=      $C052   ; Disable 4 lines of text
MIXSET  :=      $C053   ; Enable 4 lines of text
LOWSCR  :=      $C054   ; Page 1
HISCR   :=      $C055   ; Page 2
LORES   :=      $C056   ; Lores graphics
HIRES   :=      $C057   ; Hires graphics
AN3     :=      $C05E   ; Annunciator enables DHGR
AN3OFF  :=      $C05F   ; For lo-res display

; Game controller
BUTN0   :=      $C061   ; Open-Apple Key
BUTN1   :=      $C062   ; Closed-Apple Key

; DHGR Stuff
;HBASL   :=      $26     ; 
;HBASH   :=      $27     ; 
;HCOLOR1 :=      $1C     ; Running color byte
;AUXFLAG :=      $19     ; (7)=main
;HNDX    :=      $1E     ; Horizontal offset

;X0L     :=      $E0     ; X-coord save
;X0H     :=      $E1     ; after DHPSN, DHPLT
;Y0      :=      $E2     ; Y-coord save

;HCOLOR  :=      $E4     ; Color byte on entry
;HPAG    :=      $E6     ; Hi-res page ($20 normal)
;HDFLAG  :=      $EB     ; (7)=2 colors, (6)=EOR
;HWIDE   :=      $ED     ; (7)=use 4-bit mask
;HMASK   :=      $EE     ; Hi-res color byte
;HMASKE  :=      $EF     ; Exten. byte of HMASK
