            .export     _dg,_dhclr,_pen,_dot_at,_hue,_bkgnd,_brush,_line_to
            .import     popa,popax
            .include    "apple2enh.inc"
            .setcpu     "6502"
;*******************************************************************************
;* Double Hi-res Subs           
;*******************************************************************************
;* Version 1.4 7/25/83          
;* Allen Watson III             
;*******************************************************************************
;* This code is derived from the listing in Apple Orchard Jan 1984 pp26
;* https://archive.org/details/Apple-Orchard-v5n1-1984-Jan/page/n29
;*  
;* Changes to the original listing:
;* 1) Syntax changes from Apple DOS Toolkit EdAsm to ca65
;* 2) Applesoft ampersand support has been commented-out
;* 3) Pixels are now ORed during Mode Write and XORed during Mode Overwrite
;*    to support the PLATO terminal protocol. Original always XORed.
;*
;* Michael Sternberg (@16kRAM) 2019-01-17
;*******************************************************************************
;*
;* Soft-switch Equates
;*
;SET80COL    := $C001            ; Write for dubl store
;RAMRDOFF    := $C002            ; 48K RAM read off
;RAMWTOFF    := $C004            ; 48K RAM write off
;W80OFF      := $C00C            ; Write for single display
;W80COL      := $C00D            ; Write for dubl display
;VBL         := $C019            ; Vertical Blanking
;READPAGE    := $C01C            ; $80 = aux page
;TXTCLR      := $C050            ; Grafix on
;MIXCLR      := $C052            ; Disable 4 lines of text
;MIXSET      := $C053            ; Mixed text with grafix
;LOWSCR      := $C054            ; Page 2 for main
;HISCR       := $C055            ; Page 2
AUXPAGE     := $C055            ; Page2 for aux.
;LORES       := $C056            ; Hi-res off
;HIRES       := $C057            ; Hi-res on
;AN3         := $C05E            ; Enables dubl hi-res hdw.
;AN3OFF      := $C05F            ; For lo-res display

;*
;* Variable equates
;*
AUXFLAG     := $19              ; (7)=main
HCOLOR1     := $1C              ; Running color byte
HNDX        := $1E              ; Horizontal offset
HBASL       := $26              ; 
HBASH       := $27              ; 
X0L         := $E0              ; X-coord save
X0H         := $E1              ; after DHPSN, DHPLT
Y0          := $E2              ; Y-coord save
HCOLOR      := $E4              ; Color byte on entry
HPAG        := $E6              ; Hi-res page ($20 normal)
HDFLAG      := $EB              ; (7)=2 colors, (6)=EOR
HDIR        := $EC              ; (7)=down, (6)=left
HWIDE       := $ED              ; (7)=use 4-bit mask
HMASK       := $EE              ; Hi-res color byte
HMASKE      := $EF              ; Exten. byte of HMASK
COUNTH      := $F9              ; Temp: counter hi byte
DL          := $FA              ; Temp: diff
DH          := $FB              ; 
DXL         := $FC              ; Temp: delta-X
DXH         := $FD              ; 
DYL         := $FE              ; Temp: delta-Y
DYH         := $FF              ; 

;*
;*
;* Applesoft equates
;*
TXTPTR      := $B8              ; Applesoft cmd pointer
AMPRHK      := $3F5             ; Ampersand hook
ADDISP      := $D998            ; Fixup line pointer
FRMEVL      := $DD67            ; Formula evaluator
SYNCHR      := $DEC0            ; Fixup line pointer
PRTERR      := $DEC9            ; Print syntax-error message
GETBYT      := $E6F8            ; Return parm in X-reg
GETADR      := $E752            ; Get 2-byte value in $50,$51
ILLQTY      := $F6E6            ; Print illegal-quantity message

;********************************
;*
;* Program starts at $6000
;*
;            .org $6000

;*
;* Set Amperand hook address
;*
;            LDA #>PARSR
;            STA AMPRHK+1
;            LDA #<PARSR
;            STA AMPRHK+2
;            RTS

;********************************
;*
;* Parameter subroutine
;*

DHFNS:      PHA                 ; Y0 __fastcall__ so last arg in A/X

            JSR popa            ; X0L
            TAX

            JSR popa            ; X0H
            TAY

            PLA                 ; Retrieve Y0
            RTS

;********************************
;*
;* Ampersand command parser
;*
;PARSR:      LDX #0
;            STX CMDNUM
;PARSR1:     LDY #0
;PARSR2:     LDA CMDTBL,X
;            BEQ CMDFND
;            CMP #$FF            ; End of table?
;            BEQ ERRMSG          ; No match -> err
;            CMP (TXTPTR),Y      ; Char. in line
;            BNE PARSR3          ; no match; -> next cmd
;            INY
;            INX
;            BNE PARSR2          ; always taken
;ERRMSG:     JMP PRTERR          ; Print syntax err
;
;PARSR3:     INX
;            LDA CMDTBL,X        ; Char from table
;            BNE PARSR3          ; (0=end this cmd.)
;            INX                 ; Start of next cmd.
;            INC CMDNUM
;            BNE PARSR1          ; always taken
;
;CMDFND:     JSR ADDISP          ; Fixup TXTPTR
;            ASL CMDNUM          ; dubl for 2-byte addr
;            LDX CMDNUM
;            LDA ADDTBL+1,X      ; (Hack for
;            PHA                 ; JSR to addr
;            LDA ADDTBL,X
;            PHA
;            RTS                 ; -> to addr on stack!

;********************************
;*
;* Local data for Parser
;*
;CMDNUM:     .res 1                ; local data

            ;MSB OFF

;CMDTBL:     EQU *
;CMDTBL:     .byte "DG"          ; uses DHINIT
;            .byte 0
;
;            .byte "GMODE"       ; uses DHINIT
;            .byte 0
;
;            .byte "HUE"         ; uses DHCOLR
;            .byte $D0           ; "="
;            .byte 0
;
;            .byte "DOT"         ; uses DHPLT
;            .byte $C5           ; "AT"
;            .byte 0
;
;            .byte "LINE"        ; uses DHLIN
;            .byte $C1           ; "TO"
;            .byte 0
;
;            .byte "PEN"         ; uses DHPEN
;            .byte 0
;
;            .byte "BRUSH"       ; uses DHBRUSH
;            .byte 0
;
;            .byte "BKGND"       ; uses DHBKG
;            .byte 0
;
;            .byte $FF           ; End of table
;
;ADDTBL:     .word DHINIT-1      ; & DG
;            .word DHINIT-1      ; & GMODE
;            .word DHCOLR-1      ; & HUE =
;            .word DHPLT-1       ; & DOT AT
;            .word DHLIN-1       ; & LINE TO
;            .word DHPEN-1       ; & PEN
;            .word DHBRUSH-1     ; & BRUSH
;            .word DHBKG-1       ; & BKGND

;*****************************
;*
;* Dupl Hi-res Initialization
;*

_dg:
DHINIT:     LDA #$20            ; Init $2000-$3FFF
            STA HPAG

            LDA HIRES           ; Hi-res on
            LDA MIXCLR          ; Graphix on
            LDA TXTCLR          ; No mixed
            LDA RAMRDOFF        ; Read main mem
            LDA RAMWTOFF        ; Write main mem
            STA SET80COL        ; Store in both
            STA W80COL          ; Dubl display
            LDA AN3             ; Enable hdw.
;*
;* fall thru into Clear...
;*
;*************************
;*
;* Dubl Hi-res Clear
;*
_dhclr:
DHCLR:      LDA #0
            BEQ DHBKG0          ; always taken

;*************************
;*
;* Dubl Hi-res Background
;*

_bkgnd:
DHBKG:      LDA HCOLOR
DHBKG0:     STA HCOLOR1
DHBKG1:     LDA HPAG            ; Index hi-res
            STA DH              ; page using
            LDY #0              ; DL, DH
            STY DL

            LDA HCOLOR1
            STA DXL             ; Set up color bytes
            LDX #1
DHBKG2:     ASL A
            ADC #0              ; (prop. carry bit)
            STA DXL,X           ; for fast filling
            INX
            CPX #4
            BCC DHBKG2

            LDA AUXPAGE         ; Aux mem first
DHBKG3:     LDA DYH             ; Color byte #4
            STA (DL),Y
            INY
            LDA DXH             ; Color byte #2
            STA (DL),Y
            INY
            BNE DHBKG3
            INC DH
            LDA DH
            AND #$1F            ; Test for done
            BNE DHBKG3

            LDA HPAG
            STA DH              ; Set up again,
            LDA LOWSCR        ; for main
DHBKG4:     LDA DXL             ; Color byte #1
            STA (DL),Y
            INY
            LDA DYL             ; Color byte #3
            STA (DL),Y
            INY
            BNE DHBKG4
            INC DH
            LDA DH
            AND #$1F            ; Test for done
            BNE DHBKG4

            RTS

;***********************
;*
;* Set color bytes
;*
_hue:
DHCOLR:     TAX                 ; __fastcall__ Get color number

            LDA HDFLAG
            AND #$BF            ; Clear EOR mark
            STA HDFLAG

            TXA
            BPL DHCOLR1
            LDA HDFLAG          ; Neg. color value =>
            ORA #$40            ; Set EOR mark
            STA HDFLAG
            TXA

DHCOLR1:    AND #$0F
            STA HCOLOR
            ASL A
            ASL A
            ASL A
            ASL A
            ORA HCOLOR          ; Both nybbles
            STA HCOLOR

;*
;* Spin color for column
;*
            LDA AUXFLAG
            ASL A
            LDA HNDX
            ROL A               ; Restore Y0/7 in A-reg
            JMP DHSPN

;**********************
;*
;* Set mask width
;*
;* DHPEN   -> 1-bit mask
;* DHBRUSH -> 4-bit mask
;*
_pen:
DHPEN:      LDA #$00            ; (7) off
            STA HWIDE
            RTS

_brush:
DHBRUSH:    LDA #$80            ; (7) on
            STA HWIDE
            RTS

;*********************
;*
;* Dubl Hi-res Position
;*
DHPSN:      STA Y0              ; At entry, Y in acc,
            STX X0L             ; X-lo in X-reg,
            STY X0H             ; and X-hi in Y-reg.

;*
;* Vertical base calc.
;*
            STA HBASH           ; Y coord. value
            AND #$C0
            STA HBASL
            LSR A
            LSR A
            ORA HBASL
            STA HBASL
            LDA HBASH
            ASL A
            ASL A
            ASL A
            ROL HBASH
            ASL A
            ROL HBASH
            ASL A
            ROR HBASL
            LDA HBASH
            AND #$1F
            ORA HPAG
            STA HBASH

;*
;* Horizontal position
;*
            TXA                 ; X-lo to acc.
            CPY #0              ; Test X-hi:
            BEQ DHPSN2
            CPY #1
            BEQ OLDHP
            LDY #$48            ; X-hi = 2
            ADC #0              ; (rem mod 7)
            BCC DHPSN0          ; always taken

OLDHP:      LDY #$23            ; X-hi = 1
            ADC #3              ; (rem mod 7)
            BCC DHPSN0
            LDY #$47
            ADC #3

DHPSN0:     SEC
DHPSN1:     INY                 ; Divide by 7
DHPSN2:     SBC #7
            BCS DHPSN1

            STY HNDX
            LSR HNDX            ; byte offset
            ROR AUXFLAG         ; neg = aux, pos = main

            TAX                 ; For mask select

;*
;* Shift color byte
;*
            TYA                 ; (still Y0/7)

            JSR DHSPN           ; Spin color for column
;*
;* Which mask table?
;*
            BIT HWIDE
            BMI DHPMK4          ; Use 4-bit mask
;
DHPMK1:     LDA MSKTB1-249,X
            STA HMASK
            LDA #0
            STA HMASKE

            RTS

DHPMK4:     LDA MSKTB4-249,X
            STA HMASK
            LDA MSKTB5-249,X
            STA HMASKE          ; Exten. byte

            RTS

;*
;* Spin color byte for column
;*
DHSPN:      CLC                 ; (added color shift to
            ADC #3              ; make 'em like lo-res)
            AND #$03
            TAY                 ; How many shifts, mod 3

            LDA HCOLOR
DHSPN1:     STA HCOLOR1
            DEY
            BMI DHSPN2
            ASL A
            ADC #0
            BCC DHSPN1          ; always taken
DHSPN2:     RTS

;*************************
;*
;* Dubl Hi-res Plot
;*
_dot_at:
DHPLT:      JSR DHFNS           ; Get parms in A,X,Y-regs

DHPLT0:     JSR DHPSN

            LDA HNDX            ; Test for out
            CMP #$28            ; of bounds.
            BCC DHPLT1

            RTS                 ; Out of bounds

DHPLT1:     BIT AUXFLAG         ; Aux or main?
            BMI DHPLTM
            STA AUXPAGE
            BPL DHPLT2
DHPLTM:     STA LOWSCR

DHPLT2:     LDY HNDX            ; Byte offset
            LDA HMASK
            BIT HDFLAG
            BVS DHXPL1          ; Go EOR it.

            LDA HCOLOR1
            EOR (HBASL),Y       ; 
            AND HMASK
DHXPL1:     EOR (HBASL),Y       ; Set and
            STA (HBASL),Y       ; store dot

            BIT HWIDE           ; if wide mask,
            BMI DHPLT3          ; go plot more

            RTS

DHPLT3:     BIT AUXFLAG         ; Aux or main?
            BMI DHPLTA          ; Set for other:
            STA LOWSCR
            BPL DHPLT4
DHPLTA:     STA AUXPAGE

            INY                 ; Next column pair
            CPY #$28            ; Past right edge?
            BCC DHPLT4
            LDY #0              ; wrap to left edge

DHPLT4:     LDA HMASKE
            BIT HDFLAG
            BVS DHXP2           ; Go EOR it.

            LDA HCOLOR1
            ASL A               ; Shift color for
            ADC #0              ; next column right
            EOR (HBASL),Y
            AND HMASKE          ; extension mask
DHXP2:      EOR (HBASL),Y       ; Set and
            STA (HBASL),Y       ; store dot

            RTS

;*************************
;*
;* Move up (new base address)
;*
UP:         CLC
            LDA HBASH
            BIT EQ1C
            BNE UP5             ; Not end bank
            ASL HBASL
            BCS UP3             ; Not top row
            BIT EQ3
            BEQ UP1
            ADC #$1F
            SEC
            BCS UP4

UP1:        ADC #$23
            STA HBASH           ; temp
            LDA HBASL
            ADC #$B0
            BCS UP2
            ADC #$F0
UP2:        STA HBASL
            LDA HBASH
            BCS UP4             ; always taken

UP3:        ADC #$1F
UP4:        ROR HBASL
UP5:        ADC #$FC
            STA HBASH
            RTS

;*************************
;*
;* Move down (new base address)
;*
DOWN:       CLC
DOWN0:      LDA HBASH
            ADC #4
            BIT EQ1C
            BNE DOWN4           ; Not end bank

            ASL HBASL
            BCC DOWN2           ; Not bottom row

            ADC #$E0
            CLC
            BIT EQ4
            BEQ DOWN3

            LDA HBASL
            ADC #$50
            EOR #$F0
            BEQ DOWN1
            EOR #$F0 
DOWN1:      STA HBASL
            LDA HPAG
            BCC DOWN3
DOWN2:      ADC #$E0
DOWN3:      ROR HBASL
DOWN4:      STA HBASH
            RTS

;*************************
;*
;* Move left (new mask)
;*
LEFT:       LDY HNDX

            BIT HWIDE           ; Which mask width?
            BPL LMSK1           ; 1-bit masks

            LDA HMASK           ; 4-bit masks
            BIT EQF0            ; Is it left-most?
            BEQ LEFT1           ; yes
            ASL A               ; Throw away sign bit,
            LSR HMASKE          ; shift both masks
            ROR A
            LSR A
LR1:        STA HMASK
            RTS

LMSK1:      LDA HMASK           ; 1-bit mask
            LSR A
            BNE LR1             ; Mask still OK

;*
;* Next column to left
;*
LEFT1:      BIT AUXFLAG
            BPL LEFT2           ; Aux: need new index
            LSR AUXFLAG         ; flag -> aux
            BPL LEFT4           ; always taken
LEFT2:      SEC
            ROR AUXFLAG         ; flag -> main

LEFT3:      DEY                 ; New index value
            BPL LEFT4
            LDY #$27            ; Wrap l. to r. edge
LEFT4:      LDA #$07            ; New masks for
            STA HMASKE          ; right edge
            LDA #$40
            STA HMASK
            STY HNDX

;*
;* Color shift for next column left
;*
CLSHFT:     LDA HCOLOR1
            LSR A               ; Shift color byte...
            BCC CLSHFT1
            ORA #$80            ; prop. carry bit
CLSHFT1:    STA HCOLOR1

            RTS

;*************************
;*
;* Move right (new mask)
;*
RIGHT:      LDY HNDX

            LDA HMASK
            ASL A
            ASL A               ; Shift past sign
            ROL HMASKE          ; for exten byte
            LSR A               ; undo extra shift
            BEQ RMSK
            STA HMASK           ; Shift is OK
            RTS

RMSK:       STA HMASKE          ; 0-> exten byte

            BIT HWIDE           ; Which mask width?
            BPL RMSK1           ; 1-bit masks
            LDA #$0F            ; New left mask
            BNE RIGHT1          ; always taken
RMSK1:      LDA #$01            ; New left mask

;*
;* Next column right
;*
RIGHT1:     BIT AUXFLAG
            BMI RIGHT2
            SEC
            ROR AUXFLAG         ; flag -> main
            BMI RIGHT4          ; always taken

RIGHT2:     LSR AUXFLAG         ; flag -> aux
RIGHT3:     INY                 ; Next column
            CPY #$28
            BCC RIGHT4
            LDY #0              ; Wrap r. to l. edge
RIGHT4:     STA HMASK
            STY HNDX

;*
;* Color shift for next column right
;*
CRSHFT:     LDA HCOLOR1
            ASL A
            ADC #0              ; Prop. carry bit
            STA HCOLOR1

            RTS

;**********************
;*
;* Dubl Hi-res Line
;*
;* NOTICE! Parms same as DHPLT:
;*         Y in A-reg
;*         XL in X-reg
;*         XH in Y-reg
;*
_line_to:
DHLIN:      JSR DHFNS           ; Get parms in A,X,Y-regs

DHLIN0:     STA DL              ; (temporarily)

            TXA                 ; (get X1L)
            SEC
            SBC X0L             ; Compute Delta X
            STA DXL
            TYA                  ; (get X1H)
            SBC X0H
            STA DXH
            STA HDIR            ; Save sign of DX
            BCS DHLIN1          ; (it's pos.)

            SEC
            LDA #0              ; Make DX abs.value
            SBC DXL
            STA DXL
            LDA #0
            SBC DXH
            STA DXH

DHLIN1:     LDA DL              ; (get Y1)
            SEC
            SBC Y0              ; Compute Delta Y
            STA DYL
            LDA #0
            STA DYH
            BCS DHLIN2          ; DY positive
            SEC
            SBC DYL             ; Make DY absolute value
            STA DYL
            CLC
DHLIN2:     ROR HDIR            ; (7)=down, (6)=left

            LDA DL
            STA Y0              ; Update Y0
            STX X0L
            STY X0H             ; ...and X0

;*
;* Select X or Y as main direction
;*
            LDA DXH
            BNE XMAIN
            LDA DXL
            BNE DHLIN3
            CMP DYL
            BNE DHLIN3

            RTS                 ; DX=0 and DY=0 -> NOP!

DHLIN3:     CMP DYL
            BCS XMAIN           ; DX > DY

;**********************
;*
;* Y is main direction
;*
YMAIN:      SEC
            LDA #0
            SBC DYL
            TAX                 ; Count lo is X-reg.
            LDA #0
            SBC #0
            STA COUNTH

            ASL DXL
            ROL DXH             ; DX = 2*DX (D1)

            SEC
            LDA DXL
            SBC DYL
            STA DL
            LDA DXH
            SBC #0
            STA DH              ; D = 2*DX - DY

            SEC
            LDA DL
            SBC DYL
            STA DYL
            LDA DH
            SBC #0
            STA DYH             ; DY = 2*DX - 2*DY (D2)

;*
;* Y Line Loop: do until COUNT -> 0
;*
YMOVE:      BIT HDIR            ; (7)=down
            BMI YDOWN

            JSR UP              ; Move plot point up
            BNE BUMPY           ; always taken

YDOWN:      JSR DOWN            ; Move plot point down

BUMPY:      BIT DH
            BPL BMPY2
            CLC
            LDA DL
            ADC DXL
            STA DL
            LDA DH
            ADC DXH
            STA DH
            JMP YDRAW

BMPY2:      CLC
            LDA DL
            ADC DYL
            STA DL
            LDA DH
            ADC DYH
            STA DH

            BIT HDIR            ; (6)=left
            BVC YRIGHT
YLEFT:      JSR LEFT            ; Move plot point left
            JMP YDRAW

YRIGHT:     JSR RIGHT           ; Move plot point right

YDRAW:      JSR DHPLT1          ; Plot next point

            INX
            BNE YMOVE
            INC COUNTH
            BNE YMOVE

            RTS                 ; -> EXIT

;*************************
;*
;*     X is main direction
;*
XMAIN:      SEC
            LDA #0
            SBC DXL
            TAX                 ; Count lo in X-reg.
            LDA #0
            SBC DXH
            STA COUNTH

            ASL DYL
            ROL DYH             ; DY = 2;*DY (D1)

            SEC
            LDA DYL
            SBC DXL
            STA DL
            LDA DYH
            SBC DXH
            STA DH              ; D = 2;*DY - DX

            SEC
            LDA DL
            SBC DXL
            STA DXL
            LDA DH
            SBC DXH
            STA DXH             ; DX = 2;*DY - 2*DX (D2)

;*
;* X Line Loop: do until COUNT -> 0
;*
XMOVE:      BIT HDIR            ; (6)=left
            BVC XRIGHT

            JSR LEFT            ; Move plot point left
            JMP BUMPX

XRIGHT:     JSR RIGHT           ; Move plot point right

BUMPX:      BIT DH
            BPL BMPX2
            CLC
            LDA DL
            ADC DYL
            STA DL
            LDA DH
            ADC DYH
            STA DH
            JMP XDRAW

BMPX2:      CLC
            LDA DL
            ADC DXL
            STA DL
            LDA DH
            ADC DXH
            STA DH

            BIT HDIR            ; (7)=down
            BMI XDOWN
XUP:        JSR UP              ; Move plot point up
            JMP XDRAW

XDOWN:      JSR DOWN            ; Move plot point down

XDRAW:      JSR DHPLT1

            INX
            BNE XMOVE
            INC COUNTH
            BNE XMOVE

            RTS                 ; -> EXIT

;************************
;*
;* Local data
;*
EQ3:        .byte $03
EQ4:        .byte $04
EQ1C:       .byte $1C
EQF0:       .byte $F0

;************************
;*
;* Bit mask tables
;*
;* One-bit masks
;*
MSKTB1:     .byte %00000001     ; Left-most bit
            .byte %00000010
            .byte %00000100
            .byte %00001000
            .byte %00010000
            .byte %00100000
            .byte %01000000     ; Right-most bit
;*
;* Four-bit masks
;*
MSKTB4:     .byte %00001111     ; Left-most 4-bits
            .byte %00011110
            .byte %00111100
            .byte %01111000
            .byte %01110000
            .byte %01100000
            .byte %01000000     ; Right-most bits

MSKTB5:     .byte %00000000     ; Exten bytes
            .byte %00000000
            .byte %00000000
            .byte %00000000
            .byte %00000001
            .byte %00000011
            .byte %00000111     ; Right-most bits
;*
;*
;****************************************
