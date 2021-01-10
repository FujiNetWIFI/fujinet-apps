




SCLEAR_GT		= $10	; Clear Graphics and Text
SKEEP_GT 		= $20	; Keep Graphics and Text
SCLEAR_T 		= $30	; Clear Text


TEMP_ADDR		= $CC

WAIT_BUFSIZE		= 80



;DEBUG_OUTPUT		= 1	; define this if you want the output to be 
				; displayed, but can't use it DURING a VBLANK


	
	
WAIT_FOR_VBLANK:
		PHA
WAIT_VBLANK:
		LDA VCOUNT
		BNE WAIT_VBLANK
		PLA
		RTS
		


;====================================
; GET_CHANNEL
;  Look for an unused channel
; RETURNS
;   X - Available channel * 16
;   if X == $80, no channel found
; OTHER REGISTERS AFFECTED
;   ALL
;====================================
GET_CHANNEL:
		LDY #$00
CHECK_CHANNEL:
   		TYA      		; Transfer Y to the Accumulator
	   	CLC
		ROL   		; Multiply it by 16
		ROL
		ROL
		ROL
		TAX      		; Transfer A to the X register
		LDA ICHID,X
		CMP #$FF 		; if the Channel ID is FF it's available
		BEQ AVAILABLE?
		INY
		CMP #$07
		BNE CHECK_CHANNEL
		LDX #$80		; $80 indicates no channel available
AVAILABLE?:
		RTS

;
;====================================
; REVERSE_GET_CHANNEL
;  Look for an unused channel
; RETURNS
;   X - Available channel * 16
;   if X == $80, no channel found
; OTHER REGISTERS AFFECTED
;   ALL
;====================================
REVERSE_GET_CHANNEL:
		LDY #$06		; BASIC	uses #7
CHECK_CHANNEL2:
		TYA      		; Transfer Y to the Accumulator
		CLC
		ROL   			; Multiply it by 16
		ROL
		ROL
		ROL
		TAX      		; Transfer A to the X register
		LDA ICHID,X
		CMP #$FF 		; if the Channel ID is FF it's available
		BEQ AVAILABLE2?
		DEY
		BNE CHECK_CHANNEL2
		LDX #$80		; $80 indicates no channel available
AVAILABLE2?:
		RTS

;====================================
; CLOSE_CIO
;   Close channel if open
;   X = channel to close
;     will be $80 if never opened
; RETURNS
;   Nothing
; OTHER REGISTERS AFFECTED
;   ALL
;====================================
CLOSE_CIO:
	    	CPX #$7F
    		BPL ALREADY_CLOSED?
; Close the channel
	    	LDA #CMD_CLOSE
    		STA ICCOM,X
    		JSR CIOV
ALREADY_CLOSED?:
	    	RTS
	    	
; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
; can't use these routines while being relocated
; these labels are here so you can check and not
; use them during the relocation process
; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

START_OF_RELOCATED_CIO:
   	
;
;====================================
; OPEN_E_DEVICE
;  X Contains channel to use * 16
; RETURNS
;   Y Contains error status
; OTHER REGISTERS AFFECTED
;   ALL
;====================================
OPEN_E_DEVICE:

		JSR REVERSE_GET_CHANNEL			; find an available IOCB channel
		CPX #$80
	    	BEQ SCREEN_ERROR? 			; If X == $80, no CIO available   			

GOT_SCREEN_CHANNEL:

; Remember the channel we're using (Currently in X)

		STX SCREEN_KEYBOARD_CHANNEL  

; Open Available Channel to the Screen

	    	LDA #CMD_OPEN           		; open the device
    		STA ICCOM,X  
           
	  	LDA RELOC_SCREEN_KEYBOARD_DEV		; address of the string of the device (E:)
    		STA ICBAL,X
		LDA RELOC_SCREEN_KEYBOARD_DEV+1
	    	STA ICBAH,X

    		LDA #OUPDATE            		; put chars to screen get chars from keyboard
    		STA ICAX1,X

	    	LDA #$00				; not used, but good practice to store zero here
    		STA ICAX2,X
            	
	    	STA ICBLH,X				; 0/0 End of string is indicated by $9B
    		STA ICBLL,X
    		JSR CIOV
SCREEN_ERROR?:
		RTS

;====================================
; CLOSE_E_DEVICE
;   Close E Device if open
; RETURNS
;   Nothing
; OTHER REGISTERS AFFECTED
;   ALL
;====================================
CLOSE_E_DEVICE:
		LDX SCREEN_KEYBOARD_CHANNEL
		JSR CLOSE_CIO
		RTS
	



;====================================
; PRINT_STRING
;   Displays message on screen
;   A = low byte of address
;   Y = high byte of address
;   assumes succefull open of screen
; RETURNS
;   NOTHING
; OTHER REGISTERS AFFECTED
;   ALL
;====================================	
PRINT_STRING:
		LDX SCREEN_KEYBOARD_CHANNEL
    		CPX #$80
    		BMI CONTINUE?
		JSR OPEN_E_DEVICE 
CONTINUE?:
	    	STA ICBAL,X
    		TYA
    		STA ICBAH,X
	    	LDA #CMD_PUT_REC   	; Prepare to send to screen
    		STA ICCOM,X
    		LDA #$00
	    	STA ICBLL,X
    		LDA #$FF
    		STA ICBLH,X
	    	JSR CIOV
	    	
.ifdef DEBUG_OUTPUT
		JSR WAIT_FOR_VBLANK
.endif	    	
    		RTS

;====================================
; PRINT_STRING_NO_EOL
;   Displays message on screen
;   A = low byte of address
;   Y = high byte of address
;   assumes succefull open of screen
; RETURNS
;   NOTHING
; OTHER REGISTERS AFFECTED
;   ALL
;====================================	
PRINT_STRING_NO_EOL:
		LDX SCREEN_KEYBOARD_CHANNEL
	 	STA TEMP_ADDR		; Zero page, no need to relocate
    		TYA
		STA TEMP_ADDR+1

PRINT_CHAR:
	    	LDY #$00
	  	LDA (TEMP_ADDR),Y
    		BEQ STOP_PRINT_CHAR?	; NULL TERMINATED
		CMP #$9B
		BEQ STOP_PRINT_CHAR?
    	
	    	LDA TEMP_ADDR
    		STA ICBAL,X
    		LDA TEMP_ADDR+1
	    	STA ICBAH,X    	
    		LDA #CMD_PUT_CHARS   	; Prepare to send to screen
    		STA ICCOM,X
	    	LDA #$01
	    	STA ICBLL,X
    		LDA #$00
    		STA ICBLH,X
	    	JSR CIOV

; MOVE TO THE NEXT CHARACTER 
	    	CLC
    		LDA TEMP_ADDR
    		ADC #$01
	    	STA TEMP_ADDR
    		LDA TEMP_ADDR+1
    		ADC #$00
	    	STA TEMP_ADDR+1
    		CLC
    		BCC PRINT_CHAR
    	
STOP_PRINT_CHAR?:

.ifdef DEBUG_OUTPUT
		JSR WAIT_FOR_VBLANK
.endif	    	

	    	RTS
;
;====================================
; PRINT_HEX_NO_EOL
;   Displays the A register on screen
;   assumes succefull open of screen
; RETURNS
;   NOTHING
; REGISTERS AFFECTED
;   NONE
;====================================
PRINT_HEX_NO_EOL:
	   	PHA		; Save A once
	   	PHA          	; Save A again
		PHA		; Third times the charm

		LDA #$00 	; EOL
		STA HEX_ENDING

		PLA
		LSR 
	   	LSR 
	   	LSR 
	   	LSR     	
	   	CLC
	   	CMP #$0A
	   	BPL A_F3?
	   	ADC #'0'
	   	CLC
		BCC STORE_TOP_NIBBLE1B
A_F3?:
		ADC #'A'-$0B
STORE_TOP_NIBBLE1B:
		STA HEX_BUFFER
;   
	   	PLA		; Get back A
	   	AND #$0F
	   	CLC
	   	CMP #$0A
	   	BPL A_F4?
	   	ADC #'0'
	   	CLC
		BCC STORE_BOT_NIBBLE1B
A_F4?:
		ADC #'A'-$0B
STORE_BOT_NIBBLE1B:
		STA HEX_BUFFER+1 
	   	TXA
	   	PHA		; Save X

		TYA		; Save Y
		PHA

	 	LDX SCREEN_KEYBOARD_CHANNEL
	 	LDA RELOC_HEX_BUFFER
	   	STA ICBAL,X
	 	LDA RELOC_HEX_BUFFER+1
	    	STA ICBAH,X
	    	LDA #CMD_PUT_CHARS   	; Prepare to send to screen
    		STA ICCOM,X
	    	LDA #$00
    		STA ICBLH,X
	    	LDA #$02
    		STA ICBLL,X
    		JSR CIOV

		PLA
		TAY		; Restore Y

	   	PLA
	   	TAX		; Restore X

	   	PLA		; Restore A

.ifdef DEBUG_OUTPUT
		JSR WAIT_FOR_VBLANK
.endif	    	

	   	RTS  
;
;====================================
; PRINT_HEX_NIBBLE_NO_EOL
;   Displays the A register on screen
;   assumes succefull open of screen
; RETURNS
;   NOTHING
; REGISTERS AFFECTED
;   NONE
;====================================
PRINT_HEX_NIBBLE_NO_EOL:
	   	PHA		; Save A once
	   	AND #$0F
	   	CLC
	   	CMP #$0A
	   	BPL A_F5?
	   	ADC #'0'
	   	CLC
	 	BCC STORE_BOT_NIBBLE1C?
A_F5?:
		ADC #'A'-$0B
STORE_BOT_NIBBLE1C?:
		STA HEX_BUFFER 

	   	TXA
	   	PHA		; Save X
	
		TYA		; Save Y
		PHA
	
		LDA #$9B
		STA HEX_BUFFER+1
		LDA RELOC_HEX_BUFFER
		LDY RELOC_HEX_BUFFER+1
		
		JSR PRINT_STRING_NO_EOL
	
		PLA
		TAY		; Restore Y
		
	   	PLA
	   	TAX		; Restore X

	   	PLA		; Restore A

.ifdef DEBUG_OUTPUT
		JSR WAIT_FOR_VBLANK
.endif	    	


	   	RTS  

;====================================
; PRINT_HEX
;   Displays the A register on screen
;   assumes succefull open of screen
; RETURNS
;   NOTHING
; REGISTERS AFFECTED
;   NONE
;====================================
PRINT_HEX:

	   	PHA		; Save A once
	   	PHA          	; Save A again
		PHA		; Third times the charm
		LDA #$9B 	; EOL
cio_relocate70	STA HEX_ENDING
		PLA
		LSR 
	   	LSR 
	   	LSR 
	   	LSR     	
	   	CLC
	   	CMP #$0A
	   	BPL A_F1?
	   	ADC #'0'
	   	CLC
	 	BCC STORE_TOP_NIBBLE
A_F1?:
		ADC #'A'-$0B
STORE_TOP_NIBBLE:
cio_relocate71	STA HEX_BUFFER
	   
	   	PLA		; Get back A
	   	AND #$0F
	   	CLC
	   	CMP #$0A
	   	BPL A_F2?
	   	ADC #'0'
	   	CLC
		BCC STORE_BOT_NIBBLE?
A_F2?:
		ADC #'A'-$0B
STORE_BOT_NIBBLE?:
		STA HEX_BUFFER+1 
; $9B is already in HEX_BUFFER+2
	   	TXA
	   	PHA		; Save X
	   	TYA
	   	PHA		; Save Y

	 	LDA RELOC_HEX_BUFFER
	 	LDY RELOC_HEX_BUFFER+1
	   	JSR PRINT_STRING
	   	PLA
	   	TAY		; Restore Y
	   	PLA
	   	TAX		; Restore X
	   	PLA		; Restore A

.ifdef DEBUG_OUTPUT
		JSR WAIT_FOR_VBLANK
.endif	    	

	   	RTS  

	 
;====================================
; WAIT_FOR_RETURN
;   If screen_keyboard_channel is open
;   then waits for enter key
; RETURNS
;   Nothing
; OTHER REGISTERS AFFECTED
;   None
;====================================    	
WAIT_FOR_RETURN:    	
; Wait for enter key from screen *IF* we have the screen and keyboard channel
		PHA ; SAVE REGISTERS
		TXA
		PHA
		TYA
		PHA

	 	LDX SCREEN_KEYBOARD_CHANNEL
	    	CPX #$7F
	    	BPL WFE_DONE?
	 	LDA RELOC_WAITING
		LDY RELOC_WAITING+1
	 	JSR PRINT_STRING
    
; wait for a key
	 	LDX SCREEN_KEYBOARD_CHANNEL
	    	LDA #CMD_GET_REC     	  	; Get input from keyboard
	    	STA ICCOM,X
	 	LDA RELOC_WAIT_BUFFER      	; Where to store the result
	    	STA ICBAL,X
	 	LDA RELOC_WAIT_BUFFER+1
	    	STA ICBAH,X            
	    	LDA #<WAIT_BUFSIZE           	; Max buffer size
	    	STA ICBLL,X
	    	LDA #>WAIT_BUFSIZE
	    	STA ICBLH,X
	    	JSR CIOV
	    	
	    	PLA ; RESTORE REGISTERS
	    	TAY
	    	PLA
	    	TAX
	    	PLA
WFE_DONE?:

.ifdef DEBUG_OUTPUT
		JSR WAIT_FOR_VBLANK
.endif	    	

	    	RTS
									