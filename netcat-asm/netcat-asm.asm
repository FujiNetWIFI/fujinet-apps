; 
; NETCAT - THE Assembly / SIO VERSION - for FujiNet
; by Norman Davie
;
; Assembled using WUSDN / MADS
;
;     
    ORG $6000

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
CIOV                	= $E456	; Y will contain status code after call


; SIO 
SIOV	=       $E459	; SIO VECTOR
EOL	=	$9B     ; EOL

DDEVIC	=	$300    ; DEVICE SERIAL BUS ID
DUNIT	=	$301    ; SERIAL UNIT NUMBER
DCOMND	=	$302    ; SIO COMMAND
DSTATS	=	$303    ; SIO DATA DIRECTION
DBUFLO	=	$304    ; BUFFER LOW ADDRESS
DBUFHI	=	$305    ; BUFFER HIGH ADDRESS
DTIMLO	=	$306    ; SIO TIMEOUT
DTIMHI	=	$307
DBYTLO	=	$308    ; BUFFER LENGTH
DBYTHI	=	$309
DAUX1	=	$30A    ; AUXILARY BYTE---PRINTER MODE
DAUX2	=	$30B    ; AUXILARY BYTE---NOT USED  

PACTL 	=	$D302	;
IRQ_ENABLE =    $01     ; PACTL IRQ Enable: If set, enables interrupts from peripheral A. 

VPRCED	=	$0202	; Serial (peripheral) proceed line vector, 
			; initialize to 59314 ($E7B2), which is merely a PLA, RTI instruction sequence. 
			; It is used when an IRQ interrupt occurs due to the serial I/O bus proceed 
			; line which is available for peripheral use. 

CH                  	= $2FC ; keyboard status buffer
LMARGIN			= $52
SOUNDR			= $41
BRKKEY			= $11 ; 0 = break key pressed

;
FUJI_DEVICE_ID		= $71

FUJI_TRANSLATE_CR_LF	= $03
FUJI_TRANSLATE_LF   	= $02
FUJI_TRANSLATE_CR   	= $01
FUJI_TRANSLATE_NONE 	= $00
;
FUJI_CLIENT         	= $00
FUJI_SERVER	    	= $01

FUJI_TIMEOUT		= $001F	; appoximately 30 seconds

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

DWRITE			= $80   ; sending   data to SIO device
DREAD			= $40   ; receiving data from the SIO device

DVSTAT			= $02EA
DVSTAT_BYTES_WAITING_LO = $02EA
DVSTAT_BYTES_WAITING_HI = $02EB
DVSTAT_PROTOCOL		= $02EC ; depends on the protocol
DVSTAT_EXTENDED_ERROR	= $02ED

;
; largest GET/POST length is 2048, but we'll 
; keep it to BUFSIZE characters
BUFSIZE     		= 256	; NOTE: Code assumes a multiple of 256 byte buffer size
;
; ACCEPTABLE COMBINATIONS
;
; SCREEN (E:)
; OWRITE            - Screen Output
; OUPDATE           - Keyboard Input / Screen Write
; OUPDATE + OAPPEND - Keyboard Input / Screen Read and Write
; NOTE:   E: BIT 0 equals one is a forced read (GET command).
;
; KEYBOARD (K:)
; OREAD
;
; PRINTER (P:)
; OWRITE
;
; RS-232 (R:)
; OWRITE            - Block write
; OWRITE  + OAPPEND - Concurrent write
; OUPDATE + OAPPEND - Concurrent read and write
;
; SCREEN (S:)
;                             Clear      Text      Read
;                             Screen     Window    Oper-
;                             after GR.  also      ation
;
; OWRITE                      yes        no        no
; OUPDATE                     yes        no        yes
; SCLEAR_GT + OWRITE          yes        yes       no
; SCLEAR_GR + OUPDATE         yes        yes       yes
; SKEEP_GT  + OWRITE          no         no        no
; SKEEP_GT  + OUPDATE         no         no        yes
; SCLEAR_T  + OWRITE          no         yes       no
; SCLEAR_T  + OUPDATE         no         yes       yes
;
;
; Set up screen and keyboard access
;
START:
	LDA #$00				; make the left margin column 0
	STA LMARGIN
		    
	JSR GET_CHANNEL				; find an available IOCB channel
	CPX #$80
    	BNE GOT_SCREEN_CHANNEL
    	JMP ERROR_HANDLER     			; If X == $80, no CIO available
;
GOT_SCREEN_CHANNEL:

; Remember the channel we're using (Currently in X)

    	STX SCREEN_KEYBOARD_CHANNEL  

; Open Available Channel to the Screen

    	LDA #CMD_OPEN           		; open the device
    	STA ICCOM,X  
           
    	LDA #<SCREEN_KEYBOARD_DEV		; address of the string of the device (E:)
    	STA ICBAL,X
    	LDA #>SCREEN_KEYBOARD_DEV  
    	STA ICBAH,X

    	LDA #OUPDATE            		; put chars to screen get chars from keyboard
    	STA ICAX1,X

    	LDA #$00				; not used, but good practice to store zero here
    	STA ICAX2,X
            	
    	STA ICBLH,X				; 0/0 End of string is indicated by $9B
    	STA ICBLL,X
    	JSR CIOV
	BPL DISPLAY_VERSION 			; Opened device

    	JMP ERROR_HANDLER			; Geesh, we couldn't even display an error message

; Display program name

DISPLAY_VERSION:
    	LDA #<PROG_NAME
    	LDY #>PROG_NAME
    	JSR PRINT_STRING
  	  	
; Ask for the URL 

ASK_QUESTION:
	LDX SCREEN_KEYBOARD_CHANNEL
    	LDA #CMD_PUT_REC       			; Prepare to send to screen
    	STA ICCOM,X
    	
    	LDA #<ASK_SITE         			; Get our string
    	STA ICBAL,X
    	LDA #>ASK_SITE
    	STA ICBAH,X

    	LDA #$00				; EOL ends our string
    	STA ICBLL,X
    	LDA #$FF
    	STA ICBLH,X

    	JSR CIOV               
    	BPL READ_URL            		; start processig the user input
	CMP #$80				; break key was pressed?
	BNE NOT_BREAK1b
	JMP DISCONNECT
NOT_BREAK1b:
    	LDA #<KEYBOARD_FAILURE			; something very strange happened
    	LDY #>KEYBOARD_FAILURE
    	JSR PRINT_STRING
    	JMP ERROR_HANDLER       		
;
READ_URL:

; Get the users response or if he just presses RETURN use the default

    	LDA #CMD_GET_REC       			; Get input from keyboard
    	STA ICCOM,X
    	
    	LDA #<URL_FUJI      			; Where to store the result
    	STA ICBAL,X
    	LDA #>URL_FUJI  	
    	STA ICBAH,X   
    	         
    	LDA #<BUFSIZE          			; Max buffer size
    	STA ICBLL,X
    	LDA #>BUFSIZE
    	STA ICBLH,X
    	
    	JSR CIOV
	BPL PROCESS_URL        			; all is well
	CMP #$80				; is the break key held down?
	BNE NOT_BREAK2
	JMP DISCONNECT
NOT_BREAK2:
    	JMP ERROR_HANDLER      			; An error occurred
;
PROCESS_URL:
    	CLC
    	LDA ICBLH,X 				; returned length of string
    	ADC ICBLL,X 
    	CMP #$01 				; Equals 1 if just pressed return (or unlikely 256 characters)
    	BNE ASK_TRANSLATION
    	
; Copy the default URL

    	LDY #$FF    
COPY_DEFAULT:					; if return was pressed, then copy the default url
    	INY					; which is a zero terminated string
    	LDA URL_DEFAULT,Y 
    	STA URL_FUJI,Y 
    	BNE COPY_DEFAULT
    	
	
; TRANSLATION

ASK_TRANSLATION:

; this string has multiple EOL, so we can't use our print string routine

   	LDX SCREEN_KEYBOARD_CHANNEL
   	LDA #<TRANSLATION
   	STA ICBAL,X
   	LDA #>TRANSLATION
    	STA ICBAH,X
    	LDA #CMD_PUT_CHARS   			; Prepare to send to screen
    	STA ICCOM,X
    	LDA #$00
    	STA ICBLH,X
    	LDA #<(END_TRANS-TRANSLATION)		; the number of characters to output
    	STA ICBLL,X
    	JSR CIOV
    	BPL READ_TRANSLATION           		; All is well
	CMP #$80
	BNE NOT_BREAK1
	JMP DISCONNECT
NOT_BREAK1:
    	LDA #<KEYBOARD_FAILURE
    	LDY #>KEYBOARD_FAILURE
    	JSR PRINT_STRING
    	JMP ERROR_HANDLER       		; Oh oh
;
READ_TRANSLATION:
; read the keyboard
    	LDA #CMD_GET_REC       			; Get input from keyboard
    	STA ICCOM,X
    	
    	LDA #<TEMP_BUFFER      			; Where to store the result
    	STA ICBAL,X
    	LDA #>TEMP_BUFFER  	
    	STA ICBAH,X   
    	         
    	LDA #<BUFSIZE          			; Max buffer size
    	STA ICBLL,X
    	LDA #>BUFSIZE
    	STA ICBLH,X
    	
    	JSR CIOV
	BPL PROCESS_TRANS       			; all is well
	CMP #$80				; is the break key held down?
	BNE NOT_BREAK2b
	JMP DISCONNECT
NOT_BREAK2b:
    	JMP ERROR_HANDLER      			; An error occurred
;
PROCESS_TRANS:
    	LDA TEMP_BUFFER
    	CMP #$9B 				; Equals 1 if just pressed return (or unlikely 256 characters)
    	BNE USER_TRANSLATION
; Copy the default URL
    	LDA #FUJI_TRANSLATE_CR_LF
    	BNE STORE_TRANS
USER_TRANSLATION:    	
    	SEC
    	SBC '0'
STORE_TRANS:    	
    	STA TRANS

ASK_ECHO:
	LDA #<ECHO_INPUT
	LDY #>ECHO_INPUT
	JSR PRINT_STRING

READ_ECHO:
; read the keyboard
    	LDA #CMD_GET_REC       			; Get input from keyboard
    	STA ICCOM,X
    	
    	LDA #<TEMP_BUFFER      			; Where to store the result
    	STA ICBAL,X
    	LDA #>TEMP_BUFFER  	
    	STA ICBAH,X   
    	         
    	LDA #<BUFSIZE          			; Max buffer size
    	STA ICBLL,X
    	LDA #>BUFSIZE
    	STA ICBLH,X
    	
    	JSR CIOV
	BPL PROCESS_ECHO       			; all is well
	CMP #$80				; is the break key held down?
	BNE NOT_BREAK2c
	JMP DISCONNECT
NOT_BREAK2c:
    	JMP ERROR_HANDLER      			; An error occurred
;
PROCESS_ECHO:
	LDA TEMP_BUFFER
	CMP #$9B				; Did the user just press RETURN?
    	BEQ NO_ECHO				; use default
	CMP #'Y'				; How about Yes
	BNE NO_ECHO
    	LDA #$01 				; Equals 1 if just pressed return (or unlikely 256 characters)
    	BNE STORE_ECHO
NO_ECHO:
    	LDA #$00				; Store No echo
STORE_ECHO:
	STA ECHO
	
;
;
; Set up screen and keyboard access
;
OPEN_KEYBOARD:
    	JSR GET_CHANNEL				; we get a keyboard channel so we don't get echo
    	CPX #$80
    	BNE FOUND_KEYBOARD_CHANNEL
    	JMP ERROR_HANDLER  			; If X == $80, no CIO available
;
FOUND_KEYBOARD_CHANNEL:
; Remember the channel we're using (Stored in X)
    	STX KEYBOARD_CHANNEL  
    	
; Open Available Channel to the Keyboard
    	LDA #CMD_OPEN           		; open the device
    	STA ICCOM,X        
    	     
    	LDA #<KEYBOARD_DEV      		; address of the string of the device (E)
    	STA ICBAL,X
    	LDA #>KEYBOARD_DEV  
    	STA ICBAH,X
    	
    	LDA #OREAD              		; get chars from keyboard
    	STA ICAX1,X
    	
    	LDA #$00
    	STA ICAX2,X
    	
    	STA ICBLH,X				; Not needed for open
    	STA ICBLL,X
    	
    	JSR CIOV
    	BPL OPEN_KEYBOARD_OK    		; Opened device successfully
    	TYA
    	PHA
    	LDA #<KEYBOARD_FAILURE
    	LDY #>KEYBOARD_FAILURE
    	JSR PRINT_STRING
    	PLA  	

    	JMP ERROR_HANDLER
    	
OPEN_KEYBOARD_OK:


GET_UNIT_NUMBER:
	LDY #1					; Unit number is the 2nd or 3rd character
	LDA URL_FUJI,Y
	CMP #':'				; if we found a : then it must be the 3rd
	BNE GET_NUM
	LDA #$01
	BNE UNIT_NUM
GET_NUM:
	SEC
	SBC #'0'				; we should do some sanity check here, but nah
UNIT_NUM:
	STA UNIT				; store it as a raw number

SET_INTERRUPTS:

; insure interrupts are off before changing vector

	LDA PACTL				; IRQ Enable: If set, enables interrupts from peripheral A. 
	STA OLD_INTERRUPT_STATE
	AND #~IRQ_ENABLE 			; disable the interrupt
	STA PACTL

	LDA VPRCED				; save the old interrupt vector, but jump to it after
	STA OLD_INTERRUPT_HANDLER		; we process it
	LDA VPRCED+1
	STA OLD_INTERRUPT_HANDLER+1
	
	LDA #<INTERRUPT_HANDLER			; put our interrupt handler in place
	STA VPRCED
	LDA #>INTERRUPT_HANDLER
	STA VPRCED+1
	
	LDA #$00			
	STA FUJI_DATA_WAITING			; before we start interrupts, let's tell it we have no data
	
	LDA PACTL				; IRQ Enable 
	ORA #IRQ_ENABLE 			
	STA PACTL

;---------------------------------------------------------------
;---------------------------------------------------------------
; OPEN
;---------------------------------------------------------------
;---------------------------------------------------------------
OPEN_FUJI:

	LDA #<CONNECTING			; show the user where we're connecting to
	LDY #>CONNECTING
	JSR PRINT_STRING
	
	LDA #<URL_FUJI				
	LDY #>URL_FUJI
	JSR PRINT_STRING
	
	
	LDA #FUJI_DEVICE_ID			; Device
	STA DDEVIC
	
	LDA UNIT
	STA DUNIT
	
	LDA #'O'				; Open
	STA DCOMND
	
	LDA #DWRITE				; sending filespec to SIO
	STA DSTATS
	
	LDA #<URL_FUJI				; filespec
	STA DBUFLO
	LDA #>URL_FUJI
	STA DBUFHI
	
	LDA #<FUJI_TIMEOUT			; Approximate the number of seconds to wait
	STA DTIMLO
	LDA #>FUJI_TIMEOUT
	STA DTIMHI
	
	LDA #<BUFSIZE				; the size of the buffer containing the URL
	STA DBYTLO
	LDA #>BUFSIZE
	STA DBYTHI
	
	LDA #OUPDATE				; Read and Write 
	STA DAUX1
	
	LDA #FUJI_TRANSLATE_CR_LF		; translate cr and lf
	STA DAUX2

	JSR SIOV

	LDA DSTATS				; find out if we were successful
	CMP #SUCCES
	BEQ OPEN_FUJI_OK        		; Opened device successfully
; OPEN FAILURE
    	TYA
    	PHA
    	LDA #<FUJI_ERROR
    	LDY #>FUJI_ERROR
    	JSR PRINT_STRING
    	PLA
    	PHA
    	CMP #TIMOUT
    	BNE NOT_TIMEOUT4
    	LDA #<DEVICE_TIMEOUT_FAILURE		; we timed out
    	LDY #>DEVICE_TIMEOUT_FAILURE
    	JSR PRINT_STRING
NOT_TIMEOUT4:    	
    	PLA  	    	
	JSR PRINT_HEX				; print the error code
    	JMP ERROR_HANDLER

OPEN_FUJI_OK:
	LDA #$00				; at this point, no more sound
	STA SOUNDR 				; SILENCE


; MAIN EVENT!
;
CHECK_KEYBOARD:
	LDA BRKKEY				; Doing it this way works
	BNE LOOK_KEYS				; on all versions of roms
	JMP DISCONNECT
	
LOOK_KEYS:	
    	LDA CH					; did the user press a key?
    	CMP #$FF
    	BNE GET_KEY
    	JMP FUJI_STATUS
GET_KEY:

; Get the key but don't display it
    	LDX KEYBOARD_CHANNEL
    	LDA #CMD_GET_CHARS       		; Get character from keyboard
    	STA ICCOM,X
    	
    	LDA #<TEMP_BUFFER       		; Where to store the character
    	STA ICBAL,X
    	LDA #>TEMP_BUFFER
    	STA ICBAH,X   
    	         
    	LDA #$01           	     		; Just one character
    	STA ICBLL,X
    	LDA #$00
    	STA ICBLH,X
    	
    	JSR CIOV
    	BPL SEND_KEY            		; all is well
	CMP #$80
	BNE NOT_BREAK3
	JMP DISCONNECT
NOT_BREAK3:
	LDA #<KEYBOARD_FAILURE			; what the???
	LDY #>KEYBOARD_FAILURE
	JSR PRINT_STRING
    	JMP DISCONNECT          		; assume disconnected

	LDA ECHO
	BEQ SEND_KEY

ECHO_KEYS:

; Print the data
    	LDX SCREEN_KEYBOARD_CHANNEL
    	LDA #CMD_PUT_CHARS        		; Prepare to send to screen
    	STA ICCOM,X
    	LDA #<TEMP_BUFFER       		; Where to store the result
    	STA ICBAL,X
    	LDA #>TEMP_BUFFER
    	STA ICBAH,X
    	LDA #$01  	    			; How many characters to send
    	STA ICBLL,X
    	LDA #$00
    	STA ICBLH,X
    	JSR CIOV               
    	BPL KEY_PRINTED    			; All is well
	CMP #$80
	BNE NOT_BREAK4
	JMP DISCONNECT
NOT_BREAK4:

	TYA
    	PHA
    	LDA #<SCREEN_PUT_FAILURE
    	LDY #>SCREEN_PUT_FAILURE
    	JSR PRINT_STRING
    	PLA  	
    	JMP DISCONNECT      			; Assume disconnect
;
KEY_PRINTED:
    	JMP SEND_KEY
;

;    

;---------------------------------------------------------------
;---------------------------------------------------------------
; WRITE
;---------------------------------------------------------------
;---------------------------------------------------------------
SEND_KEY:
; Send the key to the outside world
  	LDA #FUJI_DEVICE_ID			; Device
	STA DDEVIC
	
	LDA UNIT
	STA DUNIT
	
	LDA #'W'
	STA DCOMND
	
	LDA #DWRITE
	STA DSTATS
	
	LDA #<TEMP_BUFFER
	STA DBUFLO
	LDA #>TEMP_BUFFER
	STA DBUFHI
	
	LDA #<FUJI_TIMEOUT
	STA DTIMLO
	LDA #>FUJI_TIMEOUT
	STA DTIMHI

	LDA #$01 				; one character to send
	STA DBYTLO				; need to send both buffer size and the daux 
	STA DAUX1
	
	LDA #$00
	STA DBYTHI
	STA DAUX2
	
	JSR SIOV

	LDA DSTATS
	CMP #SUCCES
	BEQ FUJI_STATUS				; all is well
    	TYA
    	PHA
    	LDA #<PUT_FUJI_FAILURE
    	LDY #>PUT_FUJI_FAILURE
    	JSR PRINT_STRING
    	PLA
    	PHA
    	CMP #TIMOUT
    	BNE NOT_TIMEOUT3
    	LDA #<DEVICE_TIMEOUT_FAILURE
    	LDY #>DEVICE_TIMEOUT_FAILURE
    	JSR PRINT_STRING
NOT_TIMEOUT3:      	
    	PLA  	
    	JSR PRINT_HEX  

	JMP DISCONNECT          		; assume disconnect

;---------------------------------------------------------------
;---------------------------------------------------------------
; STATUS
;---------------------------------------------------------------
;---------------------------------------------------------------
FUJI_STATUS: 
  	LDA #FUJI_DEVICE_ID			; Device
	STA DDEVIC
	
	LDA UNIT
	STA DUNIT
	
	LDA #'S'
	STA DCOMND
	
	LDA #DREAD				; SIO is going to send us data
	STA DSTATS
	
	LDA #<DVSTAT				; fuji status will return length of buffer here
	STA DBUFLO
	LDA #>DVSTAT
	STA DBUFHI
	
	LDA #<FUJI_TIMEOUT
	STA DTIMLO
	LDA #>FUJI_TIMEOUT
	STA DTIMHI
	
	LDA #$04 				; four bytes
	STA DBYTLO

	LDA #$00
	STA DBYTHI
	
	STA DAUX1
	STA DAUX2
	
	JSR SIOV
	
CHECK_FOR_FUJI_DATA:

    	LDA FUJI_DATA_WAITING			; set by interrupt handler
    	BNE GET_FUJI_BYTES_WAITING		; something is coming in!
    	JMP CHECK_KEYBOARD			; if zero, no data

GET_FUJI_BYTES_WAITING:
;
; data has shown up!
;
	CLC					; check and see if we have data
	LDA DVSTAT_BYTES_WAITING_LO
	ADC DVSTAT_BYTES_WAITING_HI
	BNE FUJI_DATA_INCOMING			; yes
	JMP CHECK_KEYBOARD			; nope

FUJI_DATA_INCOMING:	

	LDA DVSTAT_BYTES_WAITING_LO		; find out how much data we have
	STA DATA_SIZE_LO
	
	LDA DVSTAT_BYTES_WAITING_HI
	STA DATA_SIZE_HI	

	CMP #>BUFSIZE				; is it more data than our buffer?
	BMI BUFFER_SIZE_OK
	
; if the amount of data coming is
; greater than our buffer, clamp it

	LDA #<BUFSIZE
	STA DATA_SIZE_LO
	LDA #>BUFSIZE
	STA DATA_SIZE_HI

	
BUFFER_SIZE_OK:	


;---------------------------------------------------------------
;---------------------------------------------------------------
; READ
;---------------------------------------------------------------
;---------------------------------------------------------------
GET_FUJI_DATA:

  	LDA #FUJI_DEVICE_ID			; Device
	STA DDEVIC
	
	LDA UNIT
	STA DUNIT
	
	LDA #'R'
	STA DCOMND

	LDA #DREAD
	STA DSTATS
	
	LDA #<TEMP_BUFFER
	STA DBUFLO
	LDA #>TEMP_BUFFER
	STA DBUFHI
	
	LDA #<FUJI_TIMEOUT 
	STA DTIMLO
	LDA #>FUJI_TIMEOUT
	STA DTIMHI
	
	LDA DATA_SIZE_HI
	STA DBYTHI
	STA DAUX2

	LDA DATA_SIZE_LO
	STA DBYTLO
	STA DAUX1

	JSR SIOV

	LDA DSTATS	
	CMP #SUCCES
    	BEQ DATA_HANDLED          		; all is well
	TYA
    	PHA
    	LDA #<FUJI_DATA_FAILURE
    	LDY #>FUJI_DATA_FAILURE
    	JSR PRINT_STRING
    	PLA
    	PHA
    	CMP #TIMOUT
    	BNE NOT_TIMEOUT2
    	LDA #<DEVICE_TIMEOUT_FAILURE
    	LDY #>DEVICE_TIMEOUT_FAILURE
    	JSR PRINT_STRING
NOT_TIMEOUT2:      	
    	PLA  	
    	JMP DISCONNECT          	

DATA_HANDLED:
	LDA #$00
	STA FUJI_DATA_WAITING
	
; re-enable interrupts
	LDA PACTL				; IRQ Enable: If set, enables interrupts from peripheral A. 
	ORA #IRQ_ENABLE 			; If clear, interrupts are disabled.
	STA PACTL
;
PRINT_INCOMING:
; Print the data
    	LDX SCREEN_KEYBOARD_CHANNEL
    	LDA #CMD_PUT_CHARS        		; Prepare to send to screen
    	STA ICCOM,X
    	LDA #<TEMP_BUFFER       		; Where to store the result
    	STA ICBAL,X
    	LDA #>TEMP_BUFFER
    	STA ICBAH,X
    	LDA DATA_SIZE_LO  	    		; How many characters to send
    	STA ICBLL,X
    	LDA DATA_SIZE_HI
    	STA ICBLH,X
    	JSR CIOV               
    	BPL DATA_PRINTED    			; All is well
	CMP #$80
	BNE NOT_BREAK5
	JMP DISCONNECT
NOT_BREAK5:

	TYA
    	PHA
    	LDA #<SCREEN_PUT_FAILURE
    	LDY #>SCREEN_PUT_FAILURE
    	JSR PRINT_STRING
    	PLA  	
	JMP DISCONNECT      			; Assume disconnect
;
DATA_PRINTED:
    	JMP CHECK_KEYBOARD
;
DISCONNECT:
    	LDA #<DISCONNECTED
    	LDY #>DISCONNECTED
    	JSR PRINT_STRING
;
ERROR_HANDLER:

; Wait for RETURN  key from screen *IF* we have the screen and keyboard channel

    	JSR WAIT_FOR_RETURN
;    
CLOSE_ALL:
   
; Close all the channels, but 
; don't close channels that were never opened

    	LDX SCREEN_KEYBOARD_CHANNEL
    	JSR CLOSE_CHANNEL

    	LDX KEYBOARD_CHANNEL
    	JSR CLOSE_CHANNEL
    	
;
;---------------------------------------------------------------
;---------------------------------------------------------------
; restore the old interrupt state
;---------------------------------------------------------------
;---------------------------------------------------------------
	LDA PACTL	 		; IRQ Enable: If set, enables interrupts from peripheral A. 
	AND #~IRQ_ENABLE 		; If clear, interrupts are disabled.
	STA PACTL
	
	LDA OLD_INTERRUPT_HANDLER+1	; Put back the old vector
	STA VPRCED+1
	LDA OLD_INTERRUPT_HANDLER
	STA VPRCED
	
	LDA OLD_INTERRUPT_STATE		; if interrupts were enabled before we started
	AND #IRQ_ENABLE			; restore interrupts
	ORA PACTL
	STA PACTL
	
	
;---------------------------------------------------------------
;---------------------------------------------------------------
; CLOSE
;---------------------------------------------------------------
;---------------------------------------------------------------
;
  	LDA #FUJI_DEVICE_ID		; Device
	STA DDEVIC
	
	LDA UNIT
	STA DUNIT
	
	LDA #'C'
	STA DCOMND
	
	LDA #$00
	STA DSTATS
	STA DBUFLO
	STA DBUFHI
	
	LDA #<FUJI_TIMEOUT
	STA DTIMLO
	LDA #>FUJI_TIMEOUT
	STA DTIMHI
	
	LDA #$00
	STA DBYTHI
	LDA #$01
	STA DBYTLO
	
	LDA #$00
	STA DAUX1
	STA DAUX2

	JSR SIOV
	
	
	LDA #$01			; Let the noise return
	STA SOUNDR 			; NOISE

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
    	BPL WFE_DONE
    	LDA #<WAITING
    	LDY #>WAITING
    	JSR PRINT_STRING
    
; wait for a key
    	LDX SCREEN_KEYBOARD_CHANNEL
    	LDA #CMD_GET_REC       	; Get input from keyboard
    	STA ICCOM,X
    	LDA #<WAIT_BUFFER       	; Where to store the result
    	STA ICBAL,X
    	LDA #>WAIT_BUFFER
    	STA ICBAH,X            
    	LDA #<BUFSIZE           		; Max buffer size
    	STA ICBLL,X
    	LDA #>BUFSIZE
    	STA ICBLH,X
    	JSR CIOV
    	
    	PLA ; RESTORE REGISTERS
    	TAY
    	PLA
    	TAX
    	PLA
WFE_DONE:
    	RTS
    	

DUMP_SIO:
	LDY #00
DUMP:
	LDA DDEVIC,Y
	JSR PRINT_HEX_NO_EOL
	INY
	CPY #17
	BNE DUMP
	LDA #<EOL_ENDING
	LDY #>EOL_ENDING
	JSR PRINT_STRING
	RTS
	  	    	    	
;====================================
; CLOSE_CHANNEL
;   Close channel if open
;   X = channel to close
;     will be $80 if never opened
; RETURNS
;   Nothing
; OTHER REGISTERS AFFECTED
;   ALL
;====================================
CLOSE_CHANNEL:
    	CPX #$7F
    	BPL ALREADY_CLOSED
; Close the channel
    	LDA #CMD_CLOSE
    	STA ICCOM,X
    	JSR CIOV
ALREADY_CLOSED:
    	RTS
;
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
; PRINT_STRING
;   Displays message on screen
;   A = low byte of address
;   Y = high byte of address
;   assumes succefull open of screen
; RETURNS
;   NOTHING
; OTHER REGISTERS AFFECTED
;   NONE
;====================================
PRINT_STRING:
    	LDX SCREEN_KEYBOARD_CHANNEL
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
    	RTS
;
;====================================
; STRING_LENGTH
;   Calculate string length ended by $9B
; RETURNS
;   Y = Length
; OTHER REGISTERS AFFECTED
;   A
;====================================
STRING_LENGTH:
    	LDY #$FF
TEST:
    	INY
    	LDA URL_FUJI,Y
    	CMP #$9B
    	BNE TEST
    	INY	; length is not zero based
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
	LDA #$20 	; SPACE
	STA HEX_ENDING
	PLA
   	LSR 
   	LSR 
   	LSR 
   	LSR     	
   	CLC
   	CMP #$0A
   	BPL A_F3
   	ADC #'0'
   	JMP STORE_TOP_NIBBLE1
A_F3:
   	ADC #'A'-$0B
STORE_TOP_NIBBLE1:
   	STA HEX_BUFFER
;   
   	PLA		; Get back A
   	AND #$0F
   	CLC
   	CMP #$0A
   	BPL A_F4
   	ADC #'0'
   	JMP STORE_BOT_NIBBLE1
A_F4:
   	ADC #'A'-$0B
STORE_BOT_NIBBLE1:
   	STA HEX_BUFFER+1 
; $9B is already in HEX_BUFFER+2
   	TXA
   	PHA		; Save X

	TYA		; Save Y
	PHA

   	LDX SCREEN_KEYBOARD_CHANNEL
   	LDA #<HEX_BUFFER
   	STA ICBAL,X
   	LDA #>HEX_BUFFER
    	STA ICBAH,X
    	LDA #CMD_PUT_CHARS   	; Prepare to send to screen
    	STA ICCOM,X
    	LDA #$00
    	STA ICBLH,X
    	LDA #$03
    	STA ICBLL,X
    	JSR CIOV

	PLA
	TAY		; Restore Y
	
   	PLA
   	TAX		; Restore X

   	PLA		; Restore A
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
	STA HEX_ENDING
	PLA
	LSR 
   	LSR 
   	LSR 
   	LSR     	
   	CLC
   	CMP #$0A
   	BPL A_F1
   	ADC #'0'
   	JMP STORE_TOP_NIBBLE
A_F1:
   	ADC #'A'-$0B
STORE_TOP_NIBBLE:
   	STA HEX_BUFFER
;   
   	PLA		; Get back A
   	AND #$0F
   	CLC
   	CMP #$0A
   	BPL A_F2
   	ADC #'0'
   	JMP STORE_BOT_NIBBLE
A_F2:
   	ADC #'A'-$0B
STORE_BOT_NIBBLE:
   	STA HEX_BUFFER+1 
; $9B is already in HEX_BUFFER+2
   	TXA
   	PHA		; Save X
   	TYA
   	PHA		; Save Y
   	LDA #<HEX_BUFFER
   	LDY #>HEX_BUFFER
   	JSR PRINT_STRING
   	PLA
   	TAY		; Restore Y
   	PLA
   	TAX		; Restore X
   	PLA		; Restore A
   	RTS  


;------------------------
;------------------------   
;------------------------
;------------------------   		
INTERRUPT_HANDLER:
	LDA #$01
	STA FUJI_DATA_WAITING
        .BYTE $4C 	; JMP
OLD_INTERRUPT_HANDLER:	; to old vector
	.WORD 0
	
;=================DATA===============
;
ASK_SITE:		.byte 'Enter URL (ENTER for default): ',$9B
TRANSLATION:		.byte 'TRANSLATION',$9B
                        .byte '  0 = NONE, 1=CR, 2=LF, *3=CR/LF?',$9B
                        .byte '  *=Default',$9B
END_TRANS:
ECHO_INPUT:		.byte 'LOCAL ECHO? Y=YES, *NO? *Default', $9B  
 
CONNECTING:		.byte 'Connecting to: ',$9B

DISCONNECTED:		.byte 'Disconnected.',$9B

WAITING:		.byte 'Press RETURN.',$9B

FUJI_OPENED:		.byte 'Fuji successfully opened.',$9B
NO_CHANNEL_ERROR: 	.byte 'No CIO channel available',$9B
FUJI_ERROR:		.byte 'Could not open FujiNet Device.',$9B
STATUS_FAILURE:		.byte 'Status failure.', $9B
KEYBOARD_FAILURE 	.byte 'Keyboard failure', $9B
FUJI_DATA_FAILURE: 	.byte 'Fuji data failure.',$9B
SCREEN_PUT_FAILURE: 	.byte 'Screen Output failure.',$9B
PUT_FUJI_FAILURE:	.byte 'Put fuji failure.',$9B
DEVICE_TIMEOUT_FAILURE: .byte 'Device Timeout.',$9B

KEYBOARD_DEV:		.byte 'K:',$9B
SCREEN_KEYBOARD_DEV: 	.byte 'E:',$9B
FUJI_DEV:		.byte 'N:',$9B

KEYBOARD_CHANNEL: 	 .BYTE $80
SCREEN_KEYBOARD_CHANNEL: .BYTE $80

ECHO			.BYTE $01
TRANS			.BYTE $03
FUJI_DATA_WAITING	.BYTE $00
OLD_INTERRUPT_STATE	.BYTE $00

HEX_BUFFER:		.BYTE $00,$00
HEX_ENDING:		.BYTE $9B
EOL_ENDING		.BYTE $9B

PROG_NAME:		.byte 'NETCAT SIO/ASM Version 2020/11/13 5:00',$9B
URL_DEFAULT:		.byte 'N1:TCP://BBS.FOZZTEXX.NET/',$9B,00

DATA_SIZE_LO:		.BYTE 0
DATA_SIZE_HI:		.BYTE 0

UNIT			.BYTE 0

URL_FUJI:		.DS   BUFSIZE
			
TEMP_BUFFER:		.DS   BUFSIZE
WAIT_BUFFER:		.DS   BUFSIZE 


    run START
    
