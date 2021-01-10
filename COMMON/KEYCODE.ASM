

DISPLAY_SPIN:
		PHA
		TXA
		PHA
		TYA
		PHA
		LDY #0
		LDA (SAVMSC),Y
		TAX
		INX
		TXA
		STA (SAVMSC),Y
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
		JSR ATASCII_TO_SCREENCODE
		LDY #1
		STA (SAVMSC),Y
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
		STA (SAVMSC),Y
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
		STY TEMP_CONVERT
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
		STA (SAVMSC),Y

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
		STA (SAVMSC),Y
		
		LDY TEMP_CONVERT
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
		STY TEMP_CONVERT
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
		STA (SAVMSC),Y

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
		STA (SAVMSC),Y

		LDY TEMP_CONVERT
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
		STY TEMP_CONVERT
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
		STA (SAVMSC),Y

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
		STA (SAVMSC),Y

		LDY TEMP_CONVERT
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
		STY TEMP_CONVERT
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
		LDY TEMP_CONVERT
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
		LDA KEYCODES,X	
		LDY CTRL_SHIFT,X
		RTS

