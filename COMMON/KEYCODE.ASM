?SAVMSC	= $58

DISPLAY_SPIN:
		PHA
		TXA
		PHA
		TYA
		PHA
		LDY #0
		LDA (?SAVMSC),Y
		TAX
		INX
		TXA
		STA (?SAVMSC),Y
		PLA
		TAY
		PLA
		TAX
		PLA
		RTS
		
;====================================
; DISPLAY_CLUE
;  Send the equivalent code to the
; upper left corder of the screen
; INPUT
;	A = ATASCII
; RETURNS
;	A = SCREEN CODE
; OTHER REGISTERS AFFECTED
;   NONE
;====================================
DISPLAY_CLUE:
		PHA
		TYA
kc_relocate00	JSR ATASCII_TO_SCREENCODE
		LDY #1
		STA (?SAVMSC),Y
		PLA
		RTS

;====================================
; ATASCII_TO_SCREENCODE_CHANNEL
;  Convert the ATASCII character to what 
;  can be stored on the screen
; INPUT
;	A = ATASCII
; RETURNS
;	A = SCREEN CODE
; OTHER REGISTERS AFFECTED
;   NONE
;====================================
DISPLAY_CLUE_NO_TRANSLATE:
		PHA
		LDY #1
		STA (?SAVMSC),Y
		PLA
		RTS

;====================================
; SCREEN_HEX
;  display hex code on fixed locatio on screen
; INPUT
;	A = ATASCII
; RETURNS
;	Nothing
; OTHER REGISTERS AFFECTED
;   Y 
;====================================
SCREEN_HEX:
kc_relocate01	STY TEMP_CONVERT
	   	PHA		; Save A once
	   	PHA          	; Save A again

		LSR 
	   	LSR 
	   	LSR 
	   	LSR     	
	   	CLC
	   	CMP #$0A
	   	BPL K_F3A
	   	ADC #16
	   	CLC
		BCC K_TOP_NIBBLE1A
K_F3A:
		ADC #22
K_TOP_NIBBLE1A:
		LDY #40
		STA (?SAVMSC),Y

;   
	   	PLA		; Get back A
	   	AND #$0F
	   	CLC
	   	CMP #$0A
	   	BPL K_F4A
	   	ADC #16
	   	CLC
		BCC K_BOT_NIBBLE1A
K_F4A:
		ADC #22
K_BOT_NIBBLE1A:

		INY
		STA (?SAVMSC),Y
		
kc_relocate02	LDY TEMP_CONVERT
	   	PLA		; Restore A
		RTS
		

;====================================
; SCREEN_HEX2
;  display hex code on fixed locatio on screen
; INPUT
;	A = ATASCII
; RETURNS
;	Nothing
; OTHER REGISTERS AFFECTED
;   Y 
;====================================
SCREEN_HEX2:
kc_relocate03	STY TEMP_CONVERT
	   	PHA		; Save A once
	   	PHA          	; Save A again

		LSR 
	   	LSR 
	   	LSR 
	   	LSR     	
	   	CLC
	   	CMP #$0A
	   	BPL K_F3B
	   	ADC #16
	   	CLC
		BCC K_TOP_NIBBLE1B
K_F3B:
		ADC #22
K_TOP_NIBBLE1B:
		LDY #80
		STA (?SAVMSC),Y

;   
	   	PLA		; Get back A
	   	AND #$0F
	   	CLC
	   	CMP #$0A
	   	BPL K_F4B
	   	ADC #16
	   	CLC
		BCC K_BOT_NIBBLE1B
K_F4B:
		ADC #22
K_BOT_NIBBLE1B:
		INY
		STA (?SAVMSC),Y

kc_relocate04	LDY TEMP_CONVERT
	   	PLA		; Restore A

		RTS


;====================================
; SCREEN_HEX3
;  display hex code on fixed locatio on screen
; INPUT
;	A = ATASCII
; RETURNS
;	Nothing
; OTHER REGISTERS AFFECTED
;   Y 
;====================================
SCREEN_HEX3:
kc_relocate05	STY TEMP_CONVERT
	   	PHA		; Save A once
	   	PHA          	; Save A again
		

		LSR 
	   	LSR 
	   	LSR 
	   	LSR     	
	   	CLC
	   	CMP #$0A
	   	BPL K_F3C
	   	ADC #16
	   	CLC
		BCC K_TOP_NIBBLE1C
K_F3C:
		ADC #22
K_TOP_NIBBLE1C:
		LDY #120
		STA (?SAVMSC),Y

;   
	   	PLA		; Get back A
	   	AND #$0F
	   	CLC
	   	CMP #$0A
	   	BPL K_F4C
	   	ADC #16
	   	CLC
		BCC K_BOT_NIBBLE1C
K_F4C:
		ADC #22
K_BOT_NIBBLE1C:
		INY
		STA (?SAVMSC),Y

kc_relocate06	LDY TEMP_CONVERT
	   	PLA		; Restore A

		RTS
		
		
;====================================
; ATASCII_TO_SCREENCODE_CHANNEL
;  Convert the ATASCII character to what 
;  can be stored on the screen
; INPUT
;	A = ATASCII
; RETURNS
;	A = SCREEN CODE
; OTHER REGISTERS AFFECTED
;   NONE
;====================================
; ' ' through '_' 
; 32  through 95  - substract 32
;
; 0   through 31  - add 64

ATASCII_TO_SCREENCODE:
kc_relocate07	STY TEMP_CONVERT
		TAY				; KEEP TO SEE IF WE NEED INVERSE
		AND #$7F
		CMP #96
		BPL SC_INVERSE
		CMP #31
		BPL SC_CHECK2
		CLC
		ADC #64
		CLC
		BCC SC_INVERSE
SC_CHECK2
		SEC
		SBC #32
SC_INVERSE:
		CPY #$00
		BNE SC_DONE
		ORA #$80
SC_DONE:
kc_relocate08	LDY TEMP_CONVERT
		RTS
	
;====================================
; ASCII_TO_ATASCII
;  Convert the ASCII character to 
;  the ATASCII equivalent
; INPUT
;	A = ASCII
; RETURNS
;	A = ATASCII
; OTHER REGISTERS AFFECTED
;   Y
;====================================

ASCII_TO_ATASCII:
		TAY			; NEED THIS TO DETERMINE IF THE INVERSE
		AND #$7F		; REMOVE THE HIGH BIT
		CMP #31			; ARE WE 32 OR GREATER?
		BPL AS_CHECK2
		CLC
		ADC #64			; ADD 64 TO ASCII CODE
		CLC
		BCC AS_INVERT
AS_CHECK2:
		CMP #95			; ARE WE 96 OR GREATER?
		BPL AS_INVERT
		SEC
		SBC #32			; SUBSTRACT 32

AS_INVERT:
		CPY #$00
		BEQ STOP_CONVERT
		ORA #$80		; SET THE HIGH BIT FOR INVERSE
STOP_CONVERT:
		RTS			; A NOW CONTAINS ATASCII


;====================================
; ATASCII_TO_KEYCODE
;  Convert the ATASCII character to 
;  the internal code which can be
;  "pushed" into memory CH
; INPUT
;	X = ATASCII
; RETURNS
;	A = KEYCODE
;       Y = CTRL / SHIFT STATE
; OTHER REGISTERS AFFECTED
;   None
;====================================
ATASCII_TO_KEYCODE:
kc_relocate09	LDA KEYCODES,X	
		LDY CTRL_SHIFT,X
		RTS

TEMP_CONVERT	.BYTE 0
	         //   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	         //   0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15  // START OF LINE
KEYCODES	.BYTE 0,   191, 149, 146, 186, 170, 184, 189, 185, 141, 0,   133, 128, 165, 163, 136 //   0 - $00
		.BYTE 138, 175, 0,   190, 173, 139, 144, 174, 150, 171, 151, 28,  142, 143, 134, 135 //  16 - $10
		.BYTE 97,  95,  94,  90,  88,  93,  91,  115, 112, 114, 7,   6,   32,  14,  34,  38  //  32 - $20
		.BYTE 50,  31,  30,  26,  24,  29,  27,  51,  53,  48,  66,  2,   54,  15,  55,  102 //  48 - $30
		.BYTE 117, 127, 85,  82,  122, 106, 120, 125, 121, 77,  65,  69,  0,   101, 99,  72  //  64 - $40
		.BYTE 74,  111, 104, 126, 109, 75,  80,  110, 86,  107, 87,  96,  70,  98,  71,  78  //  80 - $50
		.BYTE 162, 63,  21,  18,  58,  42,  56,  61,  57,  13,  1,   5,   0,   37,  35,  8   //  96 - $60
		.BYTE 10,  47,  40,  62,  45,  11,  16,  46,  22,  43,  23,  130, 79,  118, 52,  44  // 112 - $70
		.BYTE 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   // 128 - $80
		.BYTE 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   76,  116, 119, 172, 108 // 144 - $90
		.BYTE 0,   0,   0,   0,   12,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   // 160 - $A0
		.BYTE 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   // 176 - $B0
		.BYTE 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   // 192 - $C0
		.BYTE 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   // 208 - $D0
		.BYTE 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0   // 224 - $E0
		.BYTE 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   158, 180, 0   // 240 - $F0

// 0 = LOWERCASE,  $40 = UPPERCASE, $80 = CTRL
	         //   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	         //   0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
CTRL_SHIFT	.BYTE $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80  //   0 - $00
		.BYTE $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80, $80  //  16 - $10
		.BYTE $80, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  //  32 - $20
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  //  48 - $30
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  //  64 - $40
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  //  80 - $50
		.BYTE $40, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00  //  96 - $60
		.BYTE $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $40, $40, $40, $40, $40  // 112 - $70
		.BYTE $00, $00, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  // 128 - $80
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  // 144 - $90
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  // 160 - $A0
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  // 176 - $B0
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  // 192 - $C0
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  // 208 - $D0
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  // 224 - $E0
		.BYTE $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40, $40  // 240 - $F0


	
;kc_relocate	.WORD	kc_relocate00, kc_relocate01, kc_relocate02, kc_relocate03, kc_relocate04, kc_relocate05, kc_relocate06, kc_relocate07, kc_relocate08, kc_relocate09
