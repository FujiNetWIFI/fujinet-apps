
; CIO
ICHID   = $0340     ;   Set by OS. Handler Identifier. If not 
                    ;   in use, the value is 255 ($FF), which 
                    ;   is also the initialization value.
ICDNO   = ICHID+1   ;   Set by OS. Device number (eg: D1: D2:).
ICCOM   = ICHID+2   ;   Set by User. Command
ICSTA   = ICHID+3   ;   Set by OS. May or may not be the same 
                    ;   value as CMD_STATUS returned
ICBAL   = ICHID+4   ;   Set by User. Buffer address (low byte)
ICBAH   = ICHID+5   ;   Set by User. buffer address (high byte)
ICPTL   = ICHID+6   ;   Used by BASIC. Address of put byte routine. 
ICPTH   = ICHID+7   ;   Used by BASIC. Address of put byte routine. 
ICBLL   = ICHID+8   ;   buffer length (low byte) in put/get operations
ICBLH   = ICHID+9   ;   buffer length (high byte)
ICAX1   = ICHID+10  ;   auxiliary information.  Used by Open cmd 
                    ;   for READ/WRITE/UPDATE
                    ;   Bit  7   6   5   4   3   2   1   0
                    ;   Use  ....unused....  W   R   D   A
                    ;   W equals write, R equals read, 
                    ;   D equals directory, A equals append.
ICAX2   = ICHID+11  ;   Auxiliary byte two
ICAX3   = ICHID+12  ;   Auxiliary bytes three
ICAX4   = ICHID+13  ;   Auxiliary bytes four
ICAX5   = ICHID+14  ;   Auxiliary bytes five
ICAX6   = ICHID+15  ;   Auxiliary bytes six
;
; BASIC CIO assignments
;      Channel   0    Permanently assigned to the screen editor
;                6    Used for graphics commands
;                7    Used for the Cassette, disk and printer
;
; Only use these channels if you have BASIC running, 
;  otherwise they're all available
;IOCB1   = $10       ; Channel 1
;IOCB2   = $20       ; Channel 2
;IOCB3   = $30       ; Channel 3
;IOCB4   = $40       ; Channel 4
;IOCB5   = $50       ; Channel 5
;

;
; OPTIONS FOR ICCOM / XIO
;
; "record" of data, that is, data terminated by an ATASCII EOL ($9B) such as text.
CMD_OPEN       		= $03
CMD_GET_REC    		= $05    ; reads data from the device 
;                   		   until a carriage-return
CMD_GET_CHARS  		= $07    ; get bytes
CMD_PUT_REC    		= $09    ; print until EOL
CMD_PUT_CHARS  		= $0B    ; put bytes 
CMD_CLOSE      		= $0C
CMD_STATUS     		= $0D

; STATUS CODES
; OS STATUS CODES

SUCCES   		= $01	; SUCCESSFUL OPERATION
BRKABT   		= $80	; (128) BREAK KEY ABORT
PRVOPN   		= $81	; (129) IOCB ALREADY OPEN
NONDEV   		= $82 	; (130) NON-EX DEVICE
WRONLY   		= $83 	; (131) IOCB OPENED FOR WRITE ONLY
NVALID   		= $84	; (132) INVALID COMMAND
NOTOPN   		= $85 	; (133) DEVICE OR FILE NOT OPEN
BADIOC   		= $86   ; (134) INVALID IOCB NUMBER
RDONLY   		= $87   ; (135) IOCB OPENED FOR READ ONLY
EOFERR   		= $88   ; (136) END OF FILE
TRNRCD   		= $89   ; (137) TRUNCATED RECORD
TIMOUT   		= $8A   ; (138) DEVICE TIMEOUT
DNACK    		= $8B   ; (139) DEVICE DOES NOT ACK COMMAND
FRMERR   		= $8C   ; (140) SERIAL BUS FRAMING ERROR
CRSROR   		= $8D   ; (141) CURSOR OUT OF RANGE
OVRRUN   		= $8E   ; (142) SERIAL BUS DATA OVERRUN
CHKERR   		= $8F   ; (143) SERIAL BUS CHECKSUM ERROR
DERROR   		= $90   ; (144) DEVICE ERROR (OPERATION INCOMPLETE)
BADMOD   		= $91   ; (145) BAD SCREEN MODE NUMBER
FNCNOT   		= $92   ; (146) FUNCTION NOT IN HANDLER
SCRMEM   		= $93   ; (147) INSUFFICIENT MEMORY FOR SCREEN MODE
;
; OPTIONS FOR OPEN ICAX1
OREAD  	 		= $04	; Read Only
OWRITE   		= $08	; Write Only
OUPDATE  		= $0C	; Read and write
ODIR     		= $02	; Directory
ORDIR    		= $06	; Read Directory
OAPPEND  		= $01	; Append
OWAPPEND 		= $09	; Write Append

SCLEAR_GT		= $10	; Clear Graphics and Text
SKEEP_GT 		= $20	; Keep Graphics and Text
SCLEAR_T 		= $30	; Clear Text

CIOV                	= $E456	; Y will contain status code after call

TEMP_ADDR		= $CC

WAIT_BUFSIZE		= 80

VCOUNT			= $D40B

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
		BEQ AVAILABLE
		INY
		CMP #$07
		BNE CHECK_CHANNEL
		LDX #$80		; $80 indicates no channel available
AVAILABLE:
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
		LDY #$07
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
		BEQ AVAILABLE2
		DEY
		BNE CHECK_CHANNEL2
		LDX #$80		; $80 indicates no channel available
AVAILABLE2:
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
    		BPL ALREADY_CLOSED
; Close the channel
	    	LDA #CMD_CLOSE
    		STA ICCOM,X
    		JSR CIOV
ALREADY_CLOSED:
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

cio_relocate1	JSR REVERSE_GET_CHANNEL			; find an available IOCB channel
		CPX #$80
	    	BEQ SCREEN_ERROR 			; If X == $80, no CIO available   			

GOT_SCREEN_CHANNEL:

; Remember the channel we're using (Currently in X)

cio_relocate2	STX SCREEN_KEYBOARD_CHANNEL  

; Open Available Channel to the Screen

	    	LDA #CMD_OPEN           		; open the device
    		STA ICCOM,X  
           
cio_relocate3  	LDA SCREEN_KEYBOARD_DEV_ADDR		; address of the string of the device (E:)
    		STA ICBAL,X
cio_relocate4	LDA SCREEN_KEYBOARD_DEV_ADDR+1
	    	STA ICBAH,X

    		LDA #OUPDATE            		; put chars to screen get chars from keyboard
    		STA ICAX1,X

	    	LDA #$00				; not used, but good practice to store zero here
    		STA ICAX2,X
            	
	    	STA ICBLH,X				; 0/0 End of string is indicated by $9B
    		STA ICBLL,X
    		JSR CIOV
SCREEN_ERROR:
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
cio_relocate10	LDA SCREEN_KEYBOARD_CHANNEL
cio_relocate11	JSR CLOSE_CIO
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
cio_relocate30	LDX SCREEN_KEYBOARD_CHANNEL
    		CPX #$80
    		BMI CONTINUE
cio_relocate31	JSR OPEN_E_DEVICE 
CONTINUE:
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
cio_relocate40	LDX SCREEN_KEYBOARD_CHANNEL
	 	STA TEMP_ADDR		; Zero page, no need to relocate
    		TYA
		STA TEMP_ADDR+1

PRINT_CHAR:
	    	LDY #$00
	  	LDA (TEMP_ADDR),Y
    		BEQ STOP_PRINT_CHAR	; NULL TERMINATED
		CMP #$9B
		BEQ STOP_PRINT_CHAR
    	
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
    	
STOP_PRINT_CHAR:

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
cio_relocate50	STA HEX_ENDING

		PLA
		LSR 
	   	LSR 
	   	LSR 
	   	LSR     	
	   	CLC
	   	CMP #$0A
	   	BPL A_F3
	   	ADC #'0'
	   	CLC
		BCC STORE_TOP_NIBBLE1B
A_F3:
		ADC #'A'-$0B
STORE_TOP_NIBBLE1B:
cio_relocate51	STA HEX_BUFFER
;   
	   	PLA		; Get back A
	   	AND #$0F
	   	CLC
	   	CMP #$0A
	   	BPL A_F4
	   	ADC #'0'
	   	CLC
		BCC STORE_BOT_NIBBLE1B
A_F4:
		ADC #'A'-$0B
STORE_BOT_NIBBLE1B:
cio_relocate52	STA HEX_BUFFER+1 
	   	TXA
	   	PHA		; Save X

		TYA		; Save Y
		PHA

cio_relocate53 	LDX SCREEN_KEYBOARD_CHANNEL
cio_relocate54 	LDA HEX_BUFFER_ADDR
	   	STA ICBAL,X
cio_relocate55 	LDA HEX_BUFFER_ADDR+1
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
	   	BPL A_F5
	   	ADC #'0'
	   	CLC
	 	BCC STORE_BOT_NIBBLE1C
A_F5:
		ADC #'A'-$0B
STORE_BOT_NIBBLE1C:
cio_relocate60	STA HEX_BUFFER 

	   	TXA
	   	PHA		; Save X
	
		TYA		; Save Y
		PHA
	
		LDA #$9B
cio_relocate61	STA HEX_BUFFER+1
cio_relocate62	LDA HEX_BUFFER_ADDR
cio_relocate63	LDY HEX_BUFFER_ADDR+1
		
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
	   	BPL A_F1
	   	ADC #'0'
	   	CLC
	 	BCC STORE_TOP_NIBBLE
A_F1:
		ADC #'A'-$0B
STORE_TOP_NIBBLE:
cio_relocate71	STA HEX_BUFFER
	   
	   	PLA		; Get back A
	   	AND #$0F
	   	CLC
	   	CMP #$0A
	   	BPL A_F2
	   	ADC #'0'
	   	CLC
		BCC STORE_BOT_NIBBLE
A_F2:
		ADC #'A'-$0B
STORE_BOT_NIBBLE:
cio_relocate72	STA HEX_BUFFER+1 
; $9B is already in HEX_BUFFER+2
	   	TXA
	   	PHA		; Save X
	   	TYA
	   	PHA		; Save Y

cio_relocate73 	LDA HEX_BUFFER_ADDR
cio_relocate74 	LDY HEX_BUFFER_ADDR+1
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

cio_relocate80 	LDX SCREEN_KEYBOARD_CHANNEL
	    	CPX #$7F
	    	BPL WFE_DONE
cio_relocate81 	LDA WAITING_ADDR
cio_relocate82	LDY WAITING_ADDR+1
cio_relocate83 	JSR PRINT_STRING
    
; wait for a key
cio_relocate84 	LDX SCREEN_KEYBOARD_CHANNEL
	    	LDA #CMD_GET_REC     	  	; Get input from keyboard
	    	STA ICCOM,X
cio_relocate85 	LDA WAIT_BUFFER_ADDR      	; Where to store the result
	    	STA ICBAL,X
cio_relocate86 	LDA WAIT_BUFFER_ADDR+1
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
WFE_DONE:

.ifdef DEBUG_OUTPUT
		JSR WAIT_FOR_VBLANK
.endif	    	

	    	RTS

CIO_JMP_TABLE:

cio_relocate100			.BYTE	$4C
SCREEN_KEYBOARD_DEV_ADDR	.WORD	SCREEN_KEYBOARD_DEV

cio_relocate101			.BYTE 	$4C
WAITING_ADDR			.WORD 	WAITING

cio_relocate102			.BYTE	$4C
WAIT_BUFFER_ADDR		.WORD	WAIT_BUFFER

cio_relocate103			.BYTE	$4C
HEX_BUFFER_ADDR			.WORD	HEX_BUFFER

   	
SCREEN_KEYBOARD_CHANNEL 	.BYTE 	$FF   	
SCREEN_KEYBOARD_DEV: 		.BYTE 	'E:',$9B
HEX_BUFFER:			.BYTE 	$00,$00
HEX_ENDING:			.BYTE 	$9B
EOL_ENDING			.BYTE 	$9B
WAITING				.BYTE 	'press RETURN to continue',$9B
WAIT_BUFFER			.DS	WAIT_BUFSIZE

END_OF_RELOCATED_CIO:

;cio_relocate	.WORD	cio_relocate1,cio_relocate2,cio_relocate3,cio_relocate4
;		.WORD	cio_relocate10,cio_relocate11
;		.WORD	cio_relocate30,cio_relocate31
;		.WORD	cio_relocate40	
;		.WORD	cio_relocate50,cio_relocate51,cio_relocate52,cio_relocate53,cio_relocate54,cio_relocate55		
;		.WORD	cio_relocate60,cio_relocate61,cio_relocate62,cio_relocate63		
;		.WORD	cio_relocate70,cio_relocate71,cio_relocate72,cio_relocate73,cio_relocate74		
;		.WORD	cio_relocate80,cio_relocate81,cio_relocate82,cio_relocate83,cio_relocate84,cio_relocate85,cio_relocate86
;		.WORD	cio_relocate100,cio_relocate101,cio_relocate102,cio_relocate103

										