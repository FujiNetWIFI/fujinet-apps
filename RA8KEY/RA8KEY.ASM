;
; RA8KEY - Remote Atari (8-bit) KEYBOARD
;

; Shadow registers which have all the debouncing done for us

	ORG	$6000

COLOR_NO_CONNECTION		= 00	; BLACK
COLOR_FUJI_OPENED		= 40	; YELLOW
COLOR_AWAITING_CONNECTION	= 50	; RED
COLOR_ACCEPTING_CONNECTION	= 04	; GREY
COLOR_CONNECTED			= 80	; PURPLE

COLOR4				= $02C8

CONNECTION_CLOSED		= $FF
CONNECTION_OPENED		= $00
CONNECTION_PENDING		= $01
CONNECTION_ACCEPTED		= $02
	
STARTING_FUJI_DEVICE_ID		= $70	

FUJI_TRANSLATE_CR_LF		= $03
FUJI_TRANSLATE_LF   		= $02
FUJI_TRANSLATE_CR   		= $01
FUJI_TRANSLATE_NONE 		= $00


FUJI_TIMEOUT			= $001F	; appoximately 30 seconds

; SIO 
SIOV				= $E459	; SIO VECTOR
EOL				= $9B     ; EOL

DDEVIC				= $300    ; DEVICE SERIAL BUS ID
DUNIT				= $301    ; SERIAL UNIT NUMBER
DCOMND				= $302    ; SIO COMMAND
DSTATS				= $303    ; SIO DATA DIRECTION
DBUFLO				= $304    ; BUFFER LOW ADDRESS
DBUFHI				= $305    ; BUFFER HIGH ADDRESS
DTIMLO				= $306    ; SIO TIMEOUT
DTIMHI				= $307
DBYTLO				= $308    ; BUFFER LENGTH
DBYTHI				= $309
DAUX1				= $30A  ; AUXILARY BYTE---PRINTER MODE
DAUX2				= $30B  ; AUXILARY BYTE---NOT USED  

CH				= $02FC
SHFLOK				= $02BE
DOSINI				= $0C

ATRACT				= $4D

NOKEY				= $FF

IRQ_ENABLE 			= $01 	; PACTL IRQ Enable: If set, enables interrupts from peripheral A. 

SOUNDR				= $41
BRKKEY				= $11 	; 0 = break key pressed

LMARGIN				= $52


DWRITE				= $80   	; sending   data to SIO device
DREAD				= $40   	; receiving data from the SIO device

DVSTAT				= $02EA
DVSTAT_BYTES_WAITING_LO 	= $02EA
DVSTAT_BYTES_WAITING_HI 	= $02EB
DVSTAT_PROTOCOL			= $02EC 	; depends on the protocol
DVSTAT_EXTENDED_ERROR		= $02ED

FUJI_BUFSIZE     		= 256 	; Must ALWAYS be this size
CHAR_SIZE			= 1
FUJI_STATUSSIZE			= 4

JIFFY_TICK			= $14	; VBLANK INCRECEMENTS THIS LOCATION EVERY 1/60 SEC.

CHECK_FOR_DATA			= $00
DATA_CHECKED			= $01

RTCLOK				= $12
RTCLOK_JIFFY			= $14

;---------------------------------------
;
; largest GET/POST length is 2048, but we'll 
; keep it to BUFSIZE characters
BUFSIZE     			= 40*24+1	
CHAR_TIMEOUT			= $03	; 3 SECONDS
STATUS_TIMEOUT			= $03	; 3 SECONDS
SAVMSC				= $58	; The lowest address of the screen memory, 
					; corresponding to the upper left corner 
					; of the screen

;----------------------------------------
;----------------------------------------
; START
;----------------------------------------
;----------------------------------------
START:
		JSR OPEN_E_DEVICE

; we can't do anything if we can't print 
; try anyway

		LDA #<PROGRAM_NAME
		LDY #>PROGRAM_NAME
		JSR PRINT_STRING

; Before we relocate, make sure our
; Data block states we're disconnected

		
		LDA #CONNECTION_CLOSED
		STA CONNECT_STATE		; $FF=disconnected 0=Opened 1=Accepted

		LDA #COLOR_NO_CONNECTION
		STA COLOR4


; Get the unit number from the URL

GET_UNIT_NUMBER:
		LDY #1				; Unit number is the 2nd or 3rd character
		LDA URL_FUJI,Y
		CMP #':'			; if we found a : then it must be the 3rd
		BNE GET_NUM
		LDA #$01
		BNE STORE_UNIT_NUM
GET_NUM:
		SEC
		SBC #'0'			; we should do some sanity check here, but nah
STORE_UNIT_NUM:
		STA UNIT			; store it as a raw number	

; Match the device with the unit number

SET_DDEVICE:
		LDA #STARTING_FUJI_DEVICE_ID
		CLC
		ADC UNIT
		STA LDDEVIC

; We're doing this here because their is no point
; in setting interrupts if there is no fuji device

OPEN_FUJI1:		
		LDA #<FUJI_DETECT
		LDY #>FUJI_DETECT
		JSR PRINT_STRING_NO_EOL
	
		; Open the Fuji Device
		LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT			; UNIT #
		STA DUNIT

		LDA #'O'			; Open
		STA DCOMND

		LDA #DWRITE			; sending filespec to SIO
		STA DSTATS

		LDA #<URL_FUJI			; filespec
		STA DBUFLO
		LDA #>URL_FUJI
		STA DBUFHI

		LDA #<FUJI_TIMEOUT		; Approximate the number of seconds to wait
		STA DTIMLO
		LDA #>FUJI_TIMEOUT
		STA DTIMHI

		LDA #<FUJI_BUFSIZE		; the size of the buffer containing the URL
		STA DBYTLO
		LDA #>FUJI_BUFSIZE
		STA DBYTHI

		LDA #OUPDATE			; Read and Write 
		STA DAUX1

		LDA #FUJI_TRANSLATE_NONE	; don't translate cr and lf
		STA DAUX2

		JSR SIOV
		TYA
		CMP #SUCCES
		BEQ OPEN_FUJI_OK1        	; Opened device successfully

; OPEN FAILURE

    		PHA
	    	LDA #<FUJI_OPEN_ERROR
	    	LDY #>FUJI_OPEN_ERROR
	    	JSR PRINT_STRING
	    	PLA
	    	CMP #TIMOUT
	    	BNE NOT_TIMEOUT4c
	
; Timeout	
	    	LDA #<DEVICE_TIMEOUT_FAILURE	; we timed out
	    	LDY #>DEVICE_TIMEOUT_FAILURE
	    	JSR PRINT_STRING
       	
NOT_TIMEOUT4c:  
  	
; Some other error - print error code

		JSR PRINT_HEX			; print the error code
    		JMP STOP

OPEN_FUJI_OK1:

		LDA #COLOR_FUJI_OPENED
		STA COLOR4
		
		LDA #<SUCCESS
		LDY #>SUCCESS
		JSR PRINT_STRING
		
		LDA #$00			; at this point, no more sound
		STA SOUNDR 			; SILENCE
		LDA #CONNECTION_OPENED
		STA CONNECT_STATE		; 0 = opened

; insure interrupts are off before changing vector

; Let the user know where it will be
		
		
		LDA #<RELOCATING
		LDY #>RELOCATING
		JSR PRINT_STRING_NO_EOL
		LDA MEMLO+1
		JSR PRINT_HEX_NO_EOL
		LDA MEMLO
		JSR PRINT_HEX
		
		LDA #<RELOCATION_TABLE
		LDY #>RELOCATION_TABLE
		JSR RELOCATE_TO_MEMLO
		
;*****************************************************
; -- FROM THIS POINT ON MAKE SURE YOU'RE USING
; -- THE RELOCATED ADDRESSES FOR LOADS AND STORES
;*****************************************************

	
		; MEMLO has been changed

SET_INTERRUPTS:
		LDA #<VPRCED_INTERRUPT_SETTING
		LDY #>VPRCED_INTERRUPT_SETTING
		JSR PRINT_STRING_NO_EOL

;------------------------------------
		SEI
		LDA PACTL			; IRQ Enable: If set, enables interrupts from peripheral A. 
relocate000	STA OLD_INTERRUPT_STATE
		AND #~IRQ_ENABLE 		; disable the interrupt
		STA PACTL
		CLI
		
		LDA VPRCED			; save the old interrupt vector, but jump to it after
relocate001	STA OLD_INTERRUPT_HANDLER	; we process it
		LDA VPRCED+1
relocate002	STA OLD_INTERRUPT_HANDLER+1

		SEI
relocate003	LDA RELOC_FUJI_INTERRUPT_HANDLER		; put our interrupt handler in place
		STA VPRCED
relocate004	LDA RELOC_FUJI_INTERRUPT_HANDLER+1
		STA VPRCED+1
		CLI
;------------------------------------
		
		LDA #DATA_CHECKED			
relocate005	STA FUJI_ACTIVITY		; before we start interrupts, let's tell it we have no data
		
		SEI
		LDA PACTL			; IRQ Enable 
		ORA #IRQ_ENABLE 			
		STA PACTL
		CLI
		
		LDA #<SUCCESS
		LDY #>SUCCESS
		JSR PRINT_STRING

		LDA #<VBLANK_INTERRUPT_SETTING
		LDY #>VBLANK_INTERRUPT_SETTING
		JSR PRINT_STRING_NO_EOL
			
		LDA #$00			; VBLANK not process
relocate006	STA VBLANK_PROCESSING
		
		LDA #$07 			; DEFERRED
	
relocate007	LDX RELOC_VBLANK_ADDR+1		; HIGH BYTE OF USER ROUTINE
relocate008	LDY RELOC_VBLANK_ADDR		; LOW BYTE
		JSR SETVBV
	
		LDA #<SUCCESS
		LDY #>SUCCESS
		JSR PRINT_STRING

	JMP SKIP
	
		LDA #<DOSINI_VECTOR_SETTING
		LDY #>DOSINI_VECTOR_SETTING
		JSR PRINT_STRING_NO_EOL
		

		
;------------------------------------	
		SEI
		LDA DOSINI+1
relocate009	STA OLD_DOSINI+1
		LDA DOSINI
relocate010	STA OLD_DOSINI

relocate011	LDA RELOC_RESET+1
		STA DOSINI+1
relocate012	LDA RELOC_RESET
		STA DOSINI
		CLI
;------------------------------------
		
		LDA #<SUCCESS
		LDY #>SUCCESS
		JSR PRINT_STRING
SKIP:
		LDA #<SETUP_COMPLETE
		LDY #>SETUP_COMPLETE
		JSR PRINT_STRING				

		LDA #<COLOR_INFO
		LDY #>COLOR_INFO
		JSR PRINT_STRING


STOP:

;		Don't close E device, we'll be using it in interrupt
;		JSR CLOSE_E_DEVICE



		RTS

	

;-----------------------------------------------------
;-----------------------------------------------------
;-----------------------------------------------------
;-----------------------------------------------------
; into the vertical blank
; will need relocatable code
;-----------------------------------------------------
;-----------------------------------------------------
;-----------------------------------------------------
;-----------------------------------------------------

VBLANK:	

relocate100	LDA VBLANK_PROCESSING		; Set non-zero when we're in VBLANK
		BEQ START_PROCESSING		; No previous processing going on
relocate101	JMP STILL_PROCESSING		; just leave
		
START_PROCESSING:
		LDA #$01
relocate102	STA VBLANK_PROCESSING		; we're processing!
		
relocate103	LDA CONNECT_STATE		; $FF=disconnected 0=Opened 1=Accepted
		
		CMP #CONNECTION_CLOSED		; FF Is connection close?
		BEQ OPEN_FUJI			; not opened so open

		CMP #CONNECTION_OPENED		; 00 Is connection opened?
		BEQ FUJI_AWAITING_CONNECTION	; check to see if we can accept a connection

		CMP #CONNECTION_PENDING 	; 01 Waiting for a connection
		BEQ ACCEPT

		CMP #CONNECTION_ACCEPTED	; 02 Has the connection been accepted?
		BEQ THEN_WORK	 		; yes, get to work!

relocate104	JMP LEAVE_INTERRUPT 		; Unknown state

relocate105
THEN_WORK:	JMP WORK			; opened and accepted so work
ACCEPT:
relocate106	JMP FUJI_ACCEPT_CONNECTION
;---------------------------------------------------------------
;---------------------------------------------------------------
; OPEN as a server waiting for connections
;---------------------------------------------------------------
;---------------------------------------------------------------
OPEN_FUJI:		
		; Open the Fuji Device

relocate200	LDA LDDEVIC			; Device
		STA DDEVIC

relocate201	LDA UNIT			; UNIT #
		STA DUNIT

		LDA #'O'			; Open
		STA DCOMND

		LDA #DWRITE			; sending filespec to SIO
		STA DSTATS

relocate202	LDA RELOC_URL_FUJI			; filespec
		STA DBUFLO
relocate203	LDA RELOC_URL_FUJI+1
		STA DBUFHI

		LDA #<FUJI_TIMEOUT		; Approximate the number of seconds to wait
		STA DTIMLO
		LDA #>FUJI_TIMEOUT
		STA DTIMHI

		LDA #<FUJI_BUFSIZE		; the size of the buffer containing the URL
		STA DBYTLO
		LDA #>FUJI_BUFSIZE
		STA DBYTHI

		LDA #OUPDATE			; Read and Write 
		STA DAUX1

		LDA #FUJI_TRANSLATE_NONE	; no translation of cr and lf
		STA DAUX2

		JSR SIOV
		TYA
		CMP #SUCCES
		BEQ OPEN_FUJI_OK        	; Opened device successfully

; OPEN FAILURE
; JUST LEAVE
    		CLC
    		BCC LEAVE_INTERRUPT2

OPEN_FUJI_OK:
		LDA #COLOR_FUJI_OPENED
		STA COLOR4

		LDA #$00			; at this point, no more sound
		STA SOUNDR 			; SILENCE
		LDA #CONNECTION_OPENED
relocate204	STA CONNECT_STATE		; 0 = opened

LEAVE_INTERRUPT2
relocate205	JMP LEAVE_INTERRUPT

;---------------------------------------------------------------
;---------------------------------------------------------------
; WAITING for connections
;---------------------------------------------------------------
;---------------------------------------------------------------

FUJI_AWAITING_CONNECTION:

relocate600	DEC CHANGE_COUNTER
		BPL LEAVE_INTERRUPT2
	
; Check and see if we can accept a connection
; but only every 60 jiffies (once per second)
		
;		LDA #60
relocate601	STA CHANGE_COUNTER

		LDA #COLOR_AWAITING_CONNECTION
		STA COLOR4

relocate602	LDA LDDEVIC			; Device
		STA DDEVIC

relocate603	LDA UNIT
		STA DUNIT

		LDA #'S'
		STA DCOMND

		LDA #DREAD			; SIO is going to send us data
		STA DSTATS

		LDA #<DVSTAT			; fuji status will return length of buffer here
		STA DBUFLO
		LDA #>DVSTAT
		STA DBUFHI

		LDA #<STATUS_TIMEOUT
		STA DTIMLO
		LDA #>STATUS_TIMEOUT
		STA DTIMHI

		LDA #<FUJI_STATUSSIZE		; four bytes
		STA DBYTLO
		LDA #>FUJI_STATUSSIZE
		STA DBYTHI

		STA DAUX1
		STA DAUX2

		JSR SIOV
		TYA
		CMP #SUCCES			; connection open?
		BEQ CHECK_FOR_CONNECTION

; ACCEPT RETURNED FAILURE

		CLC
		BCC LEAVE_INTERRUPT2

CHECK_FOR_CONNECTION:

		LDA DVSTAT_PROTOCOL
		CMP #$00
		BEQ LEAVE_INTERRUPT2		; No connection!
		

		LDA #CONNECTION_PENDING
relocate604	STA CONNECT_STATE		; change the state machine

		CLC
		BCC LEAVE_INTERRUPT2		
;---------------------------------------------------------------
;---------------------------------------------------------------
; ACCEPT connections
;---------------------------------------------------------------
;---------------------------------------------------------------
FUJI_ACCEPT_CONNECTION:

		LDA #COLOR_ACCEPTING_CONNECTION
		STA COLOR4
				
relocate500	LDA LDDEVIC			; Device
		STA DDEVIC

relocate501	LDA UNIT			; UNIT #
		STA DUNIT

		LDA #'A'			; Accept
		STA DCOMND

		LDA #0
		STA DSTATS
		STA DBUFLO
		STA DBUFHI
		STA DTIMLO
		STA DTIMHI
		STA DBYTLO
		STA DBYTHI
		STA DAUX1
		STA DAUX2

		JSR SIOV
		
		TYA
		CMP #SUCCES
		BNE LEAVE_INTERRUPT3		

; assume connection accepted

;---------------------------------------------------------------
;---------------------------------------------------------------
; CONNECTION ESTABLISHED
;---------------------------------------------------------------
;---------------------------------------------------------------
ACCEPT_FUJI_OK:	

		LDA #CONNECTION_ACCEPTED
relocate400	STA CONNECT_STATE
			
		LDA #COLOR_CONNECTED
		STA COLOR4	

		LDA #2
relocate401	STA CHANGE_COUNTER
LEAVE_INTERRUPT3:
relocate402	JMP LEAVE_INTERRUPT

;---------------------------------------------------------------
;---------------------------------------------------------------
;---------------------------------------------------------------
; WHERE THE MAGIC IS *******************************************
;---------------------------------------------------------------
;---------------------------------------------------------------
;---------------------------------------------------------------
;---------------------------------------------------------------
WORK:
		LDA #COLOR_CONNECTED
		STA COLOR4	

; Our work involves checking the fuji status
; If no fuji activity, then just leave the interrupt
; if there is, then check and see if that activity
; brought us data 

; don't bother getting a 'key' from FujiNet if the last
; one hasn't been processed
		LDA CH	
		CMP #NOKEY		; FF means all characters processed
		BNE LEAVE_INTERRUPT3	; If we don't have FF then OS hasn!t processed it yet

relocate900	DEC CHANGE_COUNTER
		BPL GET_FUJI_STATUS	; If it's not time, then check for incoming data
	
; Check and see if screen has changed
; but only every 60 jiffies (once per second)
		
		LDA #60			; wait a second before checking again
relocate901	STA CHANGE_COUNTER

; send screen if it's changed once per second

relocate902	JMP TRANSMIT_SCREEN 	; can't transmit yet -- soon
		
;---------------------------------------------------------------
;---------------------------------------------------------------
; STATUS - once per second need to insure we still have a connection
;---------------------------------------------------------------
;---------------------------------------------------------------
GET_FUJI_STATUS: 

relocate910	LDA LDDEVIC			; Device
		STA DDEVIC

relocate911	LDA UNIT
		STA DUNIT

		LDA #'S'
		STA DCOMND

		LDA #DREAD			; SIO is going to send us data
		STA DSTATS

		LDA #<DVSTAT			; fuji status will return length of buffer here
		STA DBUFLO
		LDA #>DVSTAT
		STA DBUFHI

		LDA #<STATUS_TIMEOUT
		STA DTIMLO
		LDA #>STATUS_TIMEOUT
		STA DTIMHI

		LDA #<FUJI_STATUSSIZE		; four bytes
		STA DBYTLO
		LDA #>FUJI_STATUSSIZE
		STA DBYTHI

		STA DAUX1
		STA DAUX2

		JSR SIOV
		TYA
		CMP #SUCCES			; connection open?
		BEQ CHECK_FOR_FUJI_ACTIVITY		

CLOSE_CONNECTION2:
relocate912	JMP CLOSE_CONNECTION		; We were connected, but now we're not

; FUJI_ACTIVITY is set my the PROCEED interrupt

CHECK_FOR_FUJI_ACTIVITY:

		LDA DVSTAT_PROTOCOL		; 0=Disconnected 1=Connected
		CMP #00				; Have we been disconnected?
		BEQ CLOSE_CONNECTION2		; then close the connection
		
		LDA DVSTAT_BYTES_WAITING_HI	; Save the high byte
relocate913	STA DATA_SIZE_HI		; even though it's not currently used by FujiNet

		LDA DVSTAT_BYTES_WAITING_LO	; Apparently only the low byte is used on FujiNet
relocate914	STA DATA_SIZE_LO		; Save this, another call to SIO could cause this to be lost
		CMP #$00			; No data, just leave
		BEQ LEAVE_INTERRUPT4
		
relocate915    	LDA FUJI_ACTIVITY		; set by interrupt handler
  		BEQ POSSIBLE_FUJI_DATA		; something may have coming in!				

; Nothing to do
LEAVE_INTERRUPT4:
relocate916	JMP LEAVE_INTERRUPT

;---------------------------------------------------------------
;---------------------------------------------------------------
;---------------------------------------------------------------
; See if data has ACTUALLY shown up
;---------------------------------------------------------------
;---------------------------------------------------------------
;---------------------------------------------------------------

POSSIBLE_FUJI_DATA:

; Fuji activity happened
; Check and see if there is data
					
relocate700	LDA DATA_SIZE_LO	 	; check and see if we have data
		BNE FUJI_DATA_INCOMING		; yes
		CLC
		BCC LEAVE_INTERRUPT4

;---------------------------------------------------------------
;---------------------------------------------------------------
;---------------------------------------------------------------
;Data available
;---------------------------------------------------------------
;---------------------------------------------------------------
;---------------------------------------------------------------
FUJI_DATA_INCOMING:	
			
; To reduce the amount of time we're in 
; VBLANK, just do one char at a time
; otherwise we'd need to check to see
; if the character has been processed
; before injecting the next character
; all during the VBLANK

relocate800	LDA DATA_SIZE_LO
		CMP #$00
		BEQ LEAVE_INTERRUPT4	; no data!


		LDA #<CHAR_SIZE		; only get 1 character
relocate801	STA DATA_SIZE_LO
		LDA #>CHAR_SIZE
relocate802	STA DATA_SIZE_HI


;---------------------------------------------------------------
;---------------------------------------------------------------
; READ the data into our buffer
;---------------------------------------------------------------
;---------------------------------------------------------------
GET_FUJI_DATA:
	
relocate950  	LDA LDDEVIC			; Device
		STA DDEVIC

relocate951	LDA UNIT
		STA DUNIT

		LDA #'R'
		STA DCOMND

		LDA #DREAD
		STA DSTATS

relocate952	LDA RELOC_TEMP_BUFFER
		STA DBUFLO
relocate953	LDA RELOC_TEMP_BUFFER+1
		STA DBUFHI

		LDA #<CHAR_TIMEOUT 
		STA DTIMLO
		LDA #>CHAR_TIMEOUT
		STA DTIMHI

relocate954	LDA DATA_SIZE_HI
		STA DBYTHI
		STA DAUX2

relocate955	LDA DATA_SIZE_LO
		STA DBYTLO
		STA DAUX1

		JSR SIOV
		TYA
		CMP #SUCCES
	    	BEQ DATA_HANDLED          	; all is well

		CLC
		BCC LEAVE_INTERRUPT4
  	

;---------------------------------------------------------------
;---------------------------------------------------------------
; we get the data, now we need to inject it into our keyboard
;---------------------------------------------------------------
;---------------------------------------------------------------
DATA_HANDLED:
	
		LDA #DATA_CHECKED
relocate956	STA FUJI_ACTIVITY

; re-enable interrupts
		SEI
		LDA PACTL			; IRQ Enable: If set, enables interrupts from peripheral A. 
		ORA #IRQ_ENABLE 		; If clear, interrupts are disabled.
		STA PACTL
		CLI
;---------------------------------------------------------------
;---------------------------------------------------------------
; we're a long time in our interrupt.
; more work, what to do with the data we received
;---------------------------------------------------------------
;---------------------------------------------------------------

INJECT_KEY:
	
relocate970	LDX TEMP_BUFFER
relocate971	JSR ATASCII_TO_KEYCODE
		STY SHFLOK			; set the shift/control state
		STA CH				; set the key

		LDA #$00			; stop attract mode
		STA ATRACT
    	
    		LDA #$02
relocate972	STA CHANGE_COUNTER		; force screen update 
		CLC
		BCC LEAVE_INTERRUPT

;---------------------------------------------------------------
;---------------------------------------------------------------
; we're a long time in our interrupt.
; more work: if the checksum has change
; transmit the whole screen
;---------------------------------------------------------------
;---------------------------------------------------------------

TRANSMIT_SCREEN:
		CLC		
		BCC LEAVE_INTERRUPT


;---------------------------------------------------------------
;---------------------------------------------------------------
; We lost our connection to the client
;---------------------------------------------------------------
;---------------------------------------------------------------
   	
CLOSE_CONNECTION:

relocate1000	LDA LDDEVIC			; Device
		STA DDEVIC

relocate1001	LDA UNIT			; UNIT #
		STA DUNIT

		LDA #'C'			; Accept
		STA DCOMND

		LDA #0
		STA DSTATS
		STA DBUFLO
		STA DBUFHI

		STA DTIMHI

		STA DAUX1
		STA DAUX2
		LDA #$0F
		STA DTIMLO

		LDA #<FUJI_BUFSIZE
		STA DBYTLO
		LDA #>FUJI_BUFSIZE
		STA DBYTHI
		JSR SIOV

		LDA #CONNECTION_CLOSED
relocate1002	STA CONNECT_STATE	
 
		LDA #COLOR_NO_CONNECTION
		STA COLOR4

LEAVE_INTERRUPT:	
		LDA #$00
relocate1003	STA VBLANK_PROCESSING
		
STILL_PROCESSING:
; EXIT DEFERRED VERTICAL BLANK INTERRUPT ROUTINE FOLLOWS:
	
		JMP XITVBV
		

;------------------------
;------------------------
; RESET HANDLER 
;------------------------
;------------------------
RESET:    
relocate1200	JSR JMP_OLD_DOSINI         ; call original DOSINI

   		SEI
relocate1201	LDA RELOC_VBLANK_END       ; Driver end LO
    		STA MEMLO
relocate1202	LDA RELOC_VBLANK_END+1     ; Driver end HI
    		STA MEMLO+1
    		
		LDA PACTL			; IRQ Enable: If set, enables interrupts from peripheral A. 
relocate1203	STA OLD_INTERRUPT_STATE
		AND #~IRQ_ENABLE 		; disable the interrupt
		STA PACTL
		
		LDA VPRCED			; save the old interrupt vector, but jump to it after
relocate1204	STA OLD_INTERRUPT_HANDLER	; we process it
		LDA VPRCED+1
relocate1205	STA OLD_INTERRUPT_HANDLER+1

relocate1206	LDA RELOC_FUJI_INTERRUPT_HANDLER		; put our interrupt handler in place
		STA VPRCED
relocate1207	LDA RELOC_FUJI_INTERRUPT_HANDLER+1
		STA VPRCED+1
		CLI
		 		
    		RTS    
;------------------------
;------------------------
;;; END RESET HANDLER ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;------------------------
;------------------------
	
;------------------------
;------------------------ 
; FUJI INTERRUPT HANDLER  
;------------------------
;------------------------   		
FUJI_INTERRUPT_HANDLER:
		
			LDA #CHECK_FOR_DATA
relocate1010		STA FUJI_ACTIVITY	
			.BYTE $4C 	; JMP
OLD_INTERRUPT_HANDLER:	.WORD 0		; to the old interrupt handler
;------------------------
;------------------------ 

	icl "..\COMMON\KEYCODE.ASM"
	icl "..\COMMON\CIO-Routines.asm"


OLD_INTERRUPT_STATE		.BYTE $00	

FUJI_ACTIVITY			.BYTE $00

DATA_SIZE_LO:			.BYTE 0
DATA_SIZE_HI:			.BYTE 0

UNIT				.BYTE 3
LDDEVIC				.BYTE 3

TEMP_BUFFER:			.DS 	FUJI_BUFSIZE

VBLANK_PROCESSING		.BYTE   0

JMP_TABLE:
relocate1500			.BYTE  	$4C
RELOC_VBLANK_ADDR		.WORD	VBLANK
relocate1501			.BYTE 	$4C
RELOC_FUJI_INTERRUPT_HANDLER	.WORD	FUJI_INTERRUPT_HANDLER
relocate1502			.BYTE	$4C
RELOC_TEMP_BUFFER		.WORD	TEMP_BUFFER
relocate1503			.BYTE	$4C
RELOC_URL_FUJI			.WORD	URL_FUJI
relocate1504			.BYTE   $4C
RELOC_RESET			.WORD   RESET
relocate1505			.BYTE   $4C
RELOC_VBLANK_END		.WORD	VBLANK_END

JMP_OLD_DOSINI
relocate1506			.BYTE   $4C
OLD_DOSINI			.WORD 	$00
				RTS
				


FUJI_OPEN_ERROR			.BYTE 	'Could not open FujiNet Device.',EOL
DEVICE_TIMEOUT_FAILURE	 	.BYTE 	'Device Timeout.',EOL


COLOR_INFO			.BYTE 	'BLACK=NO FUJINET  YELLOW=OPENED,      GREY=WAITING, RED=ACCEPT PENDING      PURPLE=ACCEPTED',EOL

CHANGE_COUNTER			.BYTE	1
	
CONNECT_STATE			.BYTE	CONNECTION_CLOSED	

URL_FUJI:			.BYTE 	'N:TCP://:6502/',EOL
				.DS 	256
DO_NOTHING	RTS

	
VBLANK_END: // EVERYTHING ABOVE IS RELOCATED


// Only used during setup
	icl "..\COMMON\DRIVER_PROTECTION.asm"


FUJI_DETECT			.BYTE	'Detecting FujiNet Device...',EOL
RELOCATING			.BYTE   'Relocating to: ',EOL
VBLANK_INTERRUPT_SETTING	.BYTE	'Setting VBLANK interrupt...', EOL
VPRCED_INTERRUPT_SETTING	.BYTE	'Setting VPRCED interrupt...', EOL
DOSINI_VECTOR_SETTING		.BYTE   'Setting DOSINI vector...', EOL
SUCCESS				.BYTE	'Success!',EOL
SETUP_COMPLETE			.BYTE 	'Setup Complete',EOL

RELOCATION_TABLE:
		.WORD 	VBLANK 		; location of handler
		.WORD 	VBLANK_END-VBLANK	; size of handler
RC_ENTRIES	.BYTE	[END_RELOCATION_TABLE - START_RELOCATION_TABLE] / 2 ; error if more than 255 relocations

START_RELOCATION_TABLE:
		.WORD	relocate000,relocate001,relocate002,relocate003,relocate004,relocate005,relocate006,relocate007,relocate008,relocate009
		.WORD   relocate010,relocate011,relocate012;,relocate013,relocate014,relocate015,relocate016,relocate017,relocate018,relocate019
		.WORD 	relocate100,relocate101,relocate102,relocate103,relocate104,relocate105,relocate106;,relocate107,relocate108,relocate109
		.WORD 	relocate200,relocate201,relocate202,relocate203,relocate204,relocate205;,relocate206,relocate207,relocate208,relocate209
		;.WORD	relocate210,relocate211,relocate212,relocate213,relocate214,relocate215,relocate216,relocate217,relocate218,relocate219
		;.WORD 	relocate300,relocate301,relocate302,relocate303,relocate304,relocate305,relocate306,relocate307,relocate308,relocate309
		.WORD 	relocate400,relocate401,relocate402;,relocate403,relocate404,relocate405,relocate406,relocate407,relocate408,relocate409
		.WORD 	relocate500,relocate501;,relocate502,relocate503,relocate504,relocate505,relocate506,relocate507,relocate508,relocate509
		.WORD 	relocate600,relocate601,relocate602,relocate603,relocate604;,relocate605,relocate606,relocate607,relocate608,relocate609
		.WORD 	relocate700;,relocate701,relocate702,relocate703,relocate704,relocate705,relocate706,relocate707,relocate708,relocate709
		.WORD 	relocate800,relocate801,relocate802;,relocate803,relocate804,relocate805,relocate806,relocate807,relocate808,relocate809
		.WORD 	relocate900,relocate901,relocate902;,relocate903,relocate904,relocate905,relocate906,relocate907,relocate908,relocate909
		.WORD   relocate910,relocate911,relocate912,relocate913,relocate914,relocate915,relocate916;,relocate917,relocate918,relocate919
		.WORD 	relocate950,relocate951,relocate952,relocate953,relocate954,relocate955,relocate956;,relocate957,relocate958,relocate959
		;.WORD 	relocate960,relocate961,relocate962,relocate963,relocate964,relocate965,relocate966,relocate967,relocate968,relocate969
		.WORD 	relocate970,relocate971,relocate972;,relocate973,relocate974,relocate975,relocate976,relocate977,relocate978,relocate979
		.WORD 	relocate1000,relocate1001,relocate1002,relocate1003;,relocate1004,relocate1005,relocate1006,relocate1007,relocate1008,relocate1009
		.WORD	relocate1010;,relocate1011,relocate1012,relocate1013,relocate1014,relocate1015,relocate1016,relocate1017,relocate1018,relocate1019
		.WORD	relocate1200,relocate1201,relocate1202,relocate1203,relocate1204,relocate1205,relocate1206,relocate1207;,relocate1208,relocate1209

		.WORD 	relocate1500,relocate1501,relocate1502,relocate1503,relocate1504,relocate1505,relocate1506;,relocate1507,relocate1508,relocate1509

cio_relocate	.WORD	cio_relocate1,cio_relocate2,cio_relocate3,cio_relocate4
		.WORD	cio_relocate10,cio_relocate11
		.WORD	cio_relocate30,cio_relocate31
		.WORD	cio_relocate40	
		.WORD	cio_relocate50,cio_relocate51,cio_relocate52,cio_relocate53,cio_relocate54,cio_relocate55		
		.WORD	cio_relocate60,cio_relocate61,cio_relocate62,cio_relocate63		
		.WORD	cio_relocate70,cio_relocate71,cio_relocate72,cio_relocate73,cio_relocate74		
		.WORD	cio_relocate80,cio_relocate81,cio_relocate82,cio_relocate83,cio_relocate84,cio_relocate85,cio_relocate86
		.WORD	cio_relocate100,cio_relocate101,cio_relocate102,cio_relocate103


kc_relocate	.WORD	kc_relocate00, kc_relocate01, kc_relocate02, kc_relocate03, kc_relocate04, kc_relocate05, kc_relocate06, kc_relocate07, kc_relocate08, kc_relocate09


END_RELOCATION_TABLE	.WORD 	0 ; end of table

; This should generate an error if there are too many entries (greater than 255)

	
PROGRAM_NAME	.BYTE	'RA8KEY (Keyboard) 2020/12/23 8:30',EOL


	run START
