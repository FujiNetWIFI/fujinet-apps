;
; RA8KEY - Remote Atari (8-bit) 
;

; Shadow registers which have all the debouncing done for us

	ORG	$6000
	
	icl "..\COMMON\ATARI_EQU.ASM"
	icl "..\COMMON\FUJI_EQU.ASM"

DONT_RELOC	=	0
DONT_OPEN	=	0
SHOW_DOS	=	0

COLOR_NO_CONNECTION		= 00	; BLACK
COLOR_FUJI_OPENED		= 40	; YELLOW
COLOR_AWAITING_CONNECTION	= 50	; RED
COLOR_ACCEPTING_CONNECTION	= 04	; GREY
COLOR_CONNECTED			= 80	; PURPLE

CONNECTION_CLOSED		= $FF
CONNECTION_OPENED		= $00
CONNECTION_PENDING		= $01
CONNECTION_ACCEPTED		= $02

DELAY_HUGE			= 60
DELAY_LONG			= 30	; 0.5 SECONDS
DELAY_SHORT			= 6	; 0.1 SECONDS
DELAY_TINY			= 3
DELAY_NONE			= 1	; 0 SECONDS
SCREEN_CHECK_DELAY		= 30	; 2 SECONDS
	
FUJI_TIMEOUT			= $001F	; appoximately 30 seconds

NOKEY				= $FF

IRQ_ENABLE 			= $01 	; PACTL IRQ Enable: If set, enables interrupts from peripheral A. 

FUJI_BUFSIZE     		= 256 	; Must ALWAYS be this size
CHAR_SIZE			= 1
FUJI_STATUSSIZE			= 4

CHECK_FOR_DATA			= $00
DATA_CHECKED			= $01

;---------------------------------------
;
; largest GET/POST length is 2048, but we'll 
; keep it to BUFSIZE characters
BUFSIZE     			= 40*24+1	
CHAR_TIMEOUT			= $03	; 3 SECONDS
STATUS_TIMEOUT			= $03	; 3 SECONDS

;----------------------------------------
;----------------------------------------
; START
;----------------------------------------
;----------------------------------------
START:
		
		JSR OPEN_E_DEVICE

		JSR dosdetect

; we can't do anything if we can't print 
; try anyway

		LDA #<PROGRAM_NAME
		LDY #>PROGRAM_NAME
		JSR PRINT_STRING

.if SHOW_DOS = 1
		LDA __dos_type
		CMP #7
		BMI inrange
		LDA #6	
inrange:		
		ASL
		TAY
		LDA DOSSTR,Y	; Low byte
		PHA
		INY
		LDA DOSSTR,Y	; high byte
		TAY
		PLA
		JSR PRINT_STRING
.endif
		

; Before we relocate, make sure our
; Data block states we're disconnected
	
		LDA #CONNECTION_CLOSED
		STA CONNECT_STATE		; $FF=disconnected 0=Opened 1=Accepted

		LDA COLOR4
		STA OLD_COLOR4
		
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

.if DONT_OPEN = 1
 		JMP OPEN_FUJI_OK1
.endif
	
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

		LDA MEMLO		; If we have to unload,
		STA OLD_MEMLO		; restore MEMLO to
		LDA MEMLO+1		; it's original value
		STA OLD_MEMLO+1

.if DONT_RELOC = 0		
		JSR RELOCATE_TO_MEMLO
		JSR ADJUST_MEMLO
.endif
		
;*****************************************************
; -- FROM THIS POINT ON MAKE SURE YOU'RE USING
; -- THE RELOCATED ADDRESSES FOR LOADS AND STORES
;*****************************************************

	
		; MEMLO has been changed

SET_INTERRUPTS:

;------------------------------------
		SEI
		LDA PACTL			; IRQ Enable: If set, enables interrupts from peripheral A. 
relocate000	STA OLD_INTERRUPT_STATE
		AND #~IRQ_ENABLE 		; disable the interrupt
		STA PACTL
		CLI
		
		LDA VPRCED			; save the old interrupt vector, but jump to it after
relocate001	STA OLD_VPRCED			; we process it
		LDA VPRCED+1
relocate002	STA OLD_VPRCED+1

		SEI
relocate003	LDA RELOC_FUJI_INTERRUPT_HANDLER ; put our interrupt handler in place
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
relocate006	JSR PRINT_STRING
		
		LDA #$00			; VBLANK not process
relocate007	STA VBLANK_PROCESSING
		LDA #DELAY_NONE
relocate008	LDA DELAY_COUNTER
		
		LDA VVBLKD
relocate009	STA OLD_VBLANK
		LDA VVBLKD+1
relocate010	STA OLD_VBLANK+1
		
		LDA #$07 			; DEFERRED	
relocate011	LDX RELOC_VBLANK+1		; HIGH BYTE OF USER ROUTINE
relocate012	LDY RELOC_VBLANK		; LOW BYTE
		JSR SETVBV
	
;------------------------------------	
		SEI
		LDA DOSINI
relocate013	STA OLD_DOSINI
		LDA DOSINI+1
relocate014	STA OLD_DOSINI+1

relocate015	LDA RELOC_RESET
		STA DOSINI
relocate016	LDA RELOC_RESET+1
		STA DOSINI+1
		CLI
;------------------------------------
		
		LDA #<SETUP_COMPLETE
		LDY #>SETUP_COMPLETE
relocate017	JSR PRINT_STRING				

		LDA #<COLOR_INFO
		LDY #>COLOR_INFO
relocate018	JSR PRINT_STRING

		JSR dosdetect
relocate019	LDA __dos_type
		CMP #ATARIDOS
		BNE STOP

		LDA #<WILL_UNLOAD
		LDY #>WILL_UNLOAD
relocate020	JSR PRINT_STRING
		
relocate021	JSR SET_UNLOADONDOS

STOP:		
;		Don't close E device, we'll be using it in interrupt
;		JSR CLOSE_E_DEVICE

		LDA #$00			; at this point, no more sound
		STA SOUNDR 			; SILENCE

		RTS


RELOCATE_CODE_START:

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
		DEC DELAY_COUNTER	
		BNE DONT_PROCESS
		
		LDA VBLANK_PROCESSING		; Set non-zero when we're in VBLANK
		BEQ START_PROCESSING		; No previous processing going on
DONT_PROCESS
		JMP STILL_PROCESSING		; just leave
		
START_PROCESSING:
		LDA #$01
		STA VBLANK_PROCESSING		; we're processing!
		
		LDA CONNECT_STATE		; $FF=disconnected 0=Opened 1=Accepted
		
		CMP #CONNECTION_CLOSED		; FF Is connection close?
		BEQ OPEN_FUJI			; not opened so open

		CMP #CONNECTION_OPENED		; 00 Is connection opened?
		BEQ FUJI_AWAITING_CONNECTION	; check to see if we can accept a connection

		CMP #CONNECTION_PENDING 	; 01 Waiting for a connection
		BEQ ACCEPT

		CMP #CONNECTION_ACCEPTED	; 02 Has the connection been accepted?
		BEQ THEN_WORK	 		; yes, get to work!

		JMP LEAVE_INTERRUPT 		; Unknown state

THEN_WORK:	
		JMP WORK			; opened and accepted so work
ACCEPT:
		JMP FUJI_ACCEPT_CONNECTION
;---------------------------------------------------------------
;---------------------------------------------------------------
; OPEN as a server waiting for connections
;---------------------------------------------------------------
;---------------------------------------------------------------
OPEN_FUJI:		
		; Open the Fuji Device

		LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT			; UNIT #
		STA DUNIT

		LDA #'O'			; Open
		STA DCOMND

		LDA #DWRITE			; sending filespec to SIO
		STA DSTATS

		LDA RELOC_URL_FUJI			; filespec
		STA DBUFLO
		LDA RELOC_URL_FUJI+1
		STA DBUFHI

		LDA #<FUJI_TIMEOUT		; Approximate the number of seconds to wait
		STA DTIMLO
		LDA #>FUJI_TIMEOUT

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

		LDA #DELAY_LONG
		STA DELAY_COUNTER
; OPEN FAILURE
; JUST LEAVE
    		CLC
    		BCC LEAVE_INTERRUPT2

OPEN_FUJI_OK:
		LDA #COLOR_FUJI_OPENED
		STA COLOR4

		LDA #CONNECTION_OPENED
		STA CONNECT_STATE		; 0 = opened

LEAVE_INTERRUPT2
		JMP LEAVE_INTERRUPT

;---------------------------------------------------------------
;---------------------------------------------------------------
; WAITING for connections
;---------------------------------------------------------------
;---------------------------------------------------------------

FUJI_AWAITING_CONNECTION:
	
; Check and see if we can accept a connection
; but only every 60 jiffies (once per second)
		
		LDA #COLOR_AWAITING_CONNECTION
		STA COLOR4

		LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT
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

		LDA #DELAY_LONG
		STA DELAY_COUNTER
; ACCEPT RETURNED FAILURE

		CLC
		BCC LEAVE_INTERRUPT2

CHECK_FOR_CONNECTION:

		LDA DVSTAT_PROTOCOL
		CMP #$00
		BNE CHANGE_STATE
		
		LDA #DELAY_TINY
		STA DELAY_COUNTER
		CLC
		BCC LEAVE_INTERRUPT2		; No connection!
CHANGE_STATE:		
		LDA #CONNECTION_PENDING
		STA CONNECT_STATE		; change the state machine

		LDA #DELAY_NONE
		STA DELAY_COUNTER

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
	
; Check and see if screen has changed
; but only every 60 jiffies (once per second)
		
		LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT			; UNIT #
		STA DUNIT

		LDA #'A'			; Accept
		STA DCOMND

		LDA #0
		STA DSTATS
		STA DBUFLO
		STA DBUFHI
		STA DTIMLO
		STA DBYTLO
		STA DBYTHI
		STA DAUX1
		STA DAUX2

		JSR SIOV
		
		TYA
		CMP #SUCCES
		BEQ ACCEPT_FUJI_OK
		
		LDA #DELAY_LONG
		STA DELAY_COUNTER
		CLC
		BCC LEAVE_INTERRUPT3		

; assume connection accepted

;---------------------------------------------------------------
;---------------------------------------------------------------
; CONNECTION ESTABLISHED
;---------------------------------------------------------------
;---------------------------------------------------------------
ACCEPT_FUJI_OK:	
		LDA #CONNECTION_ACCEPTED
		STA CONNECT_STATE
			
		LDA #COLOR_CONNECTED
		STA COLOR4	

		LDA #1
		STA CHANGE_COUNTER
		
		LDA #DELAY_NONE
		STA DELAY_COUNTER
LEAVE_INTERRUPT3:
		JMP LEAVE_INTERRUPT

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

		DEC CHANGE_COUNTER
		BPL GET_FUJI_STATUS	; If it's not time, then check for incoming data
	
; Check and see if screen has changed
; but only every 60 jiffies (once per second)
		
		LDA #SCREEN_CHECK_DELAY		; wait a second before checking again
		STA CHANGE_COUNTER

; send screen if it's changed once per second

		JMP TRANSMIT_SCREEN 	
		
;---------------------------------------------------------------
;---------------------------------------------------------------
; STATUS - once per second need to insure we still have a connection
;---------------------------------------------------------------
;---------------------------------------------------------------
GET_FUJI_STATUS: 

		LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT
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
		JMP CLOSE_CONNECTION		; We were connected, but now we're not

; FUJI_ACTIVITY is set my the PROCEED interrupt

CHECK_FOR_FUJI_ACTIVITY:

		LDA DVSTAT_PROTOCOL		; 0=Disconnected 1=Connected
		CMP #00				; Have we been disconnected?
		BEQ CLOSE_CONNECTION2		; then close the connection
		
		LDA DVSTAT_BYTES_WAITING_HI	; Save the high byte
		STA DATA_SIZE_HI		; even though it's not currently used by FujiNet

		LDA DVSTAT_BYTES_WAITING_LO	; Apparently only the low byte is used on FujiNet
		STA DATA_SIZE_LO		; Save this, another call to SIO could cause this to be lost
		CMP #$00			; No data, just leave
		BNE CHECK_ACTIVITY

NO_DATA:		
		LDA #DELAY_TINY
		STA DELAY_COUNTER
		CLC
		BCC LEAVE_INTERRUPT4

CHECK_ACTIVITY:		
	    	LDA FUJI_ACTIVITY		; set by interrupt handler
  		BEQ POSSIBLE_FUJI_DATA		; something may have coming in!				
		BNE NO_DATA
		
LEAVE_INTERRUPT4:
		JMP LEAVE_INTERRUPT

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
					
		LDA DATA_SIZE_LO	 	; check and see if we have data
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

		LDA DATA_SIZE_LO
		CMP #$00
		BNE BEGIN_GET
		
		LDA #DELAY_TINY
		STA DELAY_COUNTER
		CLC
		BCC LEAVE_INTERRUPT4	; no data!
		
BEGIN_GET
		LDA #DELAY_NONE
		STA DELAY_COUNTER

		LDA #<CHAR_SIZE		; only get 1 character
		STA DATA_SIZE_LO
		LDA #>CHAR_SIZE
		STA DATA_SIZE_HI

;---------------------------------------------------------------
;---------------------------------------------------------------
; READ the data into our buffer
;---------------------------------------------------------------
;---------------------------------------------------------------
GET_FUJI_DATA:
	  	LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT
		STA DUNIT

		LDA #'R'
		STA DCOMND

		LDA #DREAD
		STA DSTATS

		LDA RELOC_TEMP_BUFFER
		STA DBUFLO
		LDA RELOC_TEMP_BUFFER+1
		STA DBUFHI

		LDA #<CHAR_TIMEOUT 
		STA DTIMLO

		LDA DATA_SIZE_HI
		STA DBYTHI
		STA DAUX2

		LDA DATA_SIZE_LO
		STA DBYTLO
		STA DAUX1

		JSR SIOV
		TYA
		CMP #SUCCES
	    	BEQ DATA_HANDLED          	; all is well

		LDA #DELAY_NONE
		STA DELAY_COUNTER

		CLC
		BCC LEAVE_INTERRUPT4
  	

;---------------------------------------------------------------
;---------------------------------------------------------------
; we get the data, now we need to inject it into our keyboard
;---------------------------------------------------------------
;---------------------------------------------------------------
DATA_HANDLED:
		LDA #DATA_CHECKED
		STA FUJI_ACTIVITY

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
		LDX TEMP_BUFFER
		JSR ATASCII_TO_KEYCODE
		STY SHFLOK			; set the shift/control state
		STA CH				; set the key

		LDA #$00			; stop attract mode
		STA ATRACT
    	
    		LDA #06
		STA CHANGE_COUNTER		; force screen update 

		LDA #DELAY_TINY
		STA DELAY_COUNTER

		CLC
		BCC LEAVE_INTERRUPT

;---------------------------------------------------------------
;---------------------------------------------------------------
; we're a long time in our interrupt.
; more work: if the checksum has changed
; transmit the whole screen
;---------------------------------------------------------------
;---------------------------------------------------------------

TRANSMIT_SCREEN:
		JSR TRANSMIT_IF_CHANGED_GRAPHICS0
		
		LDA #DELAY_LONG
		STA DELAY_COUNTER

		CLC		
		BCC LEAVE_INTERRUPT


;---------------------------------------------------------------
;---------------------------------------------------------------
; We lost our connection to the client
;---------------------------------------------------------------
;---------------------------------------------------------------
   	
CLOSE_CONNECTION:

		LDA #DELAY_LONG
		STA DELAY_COUNTER
		
		LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT			; UNIT #
		STA DUNIT

		LDA #'C'			; Accept
		STA DCOMND

		LDA #0
		STA DSTATS
		STA DBUFLO
		STA DBUFHI

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
		STA CONNECT_STATE	
 
		LDA #COLOR_NO_CONNECTION
		STA COLOR4

LEAVE_INTERRUPT:	
		LDA #$00
		STA VBLANK_PROCESSING
		
STILL_PROCESSING:
; EXIT DEFERRED VERTICAL BLANK INTERRUPT ROUTINE FOLLOWS:
	
		JMP XITVBV
		
;------------------------
;------------------------
; RESET HANDLER 
;------------------------
;------------------------
RESET_HANDLER: 
		JSR JMP_OLD_DOSINI         	; call original DOSINI

		LDA #01
		STA CHANGE_COUNTER

		LDA #DELAY_NONE
		STA DELAY_COUNTER
		
		SEI
		LDA PACTL			; IRQ Enable: If set, enables interrupts from peripheral A. 
		STA OLD_INTERRUPT_STATE
		AND #~IRQ_ENABLE 		; disable the interrupt
		STA PACTL
		CLI
		
		LDA VPRCED			; save the old interrupt vector, but jump to it after
		STA OLD_VPRCED			; we process it
		LDA VPRCED+1
		STA OLD_VPRCED+1

		SEI
		LDA RELOC_FUJI_INTERRUPT_HANDLER; put our interrupt handler in place
		STA VPRCED
		LDA RELOC_FUJI_INTERRUPT_HANDLER+1
		STA VPRCED+1
		CLI
;------------------------------------
		
		LDA #DATA_CHECKED			
		STA FUJI_ACTIVITY		; before we start interrupts, let's tell it we have no data
		
		SEI
		LDA PACTL			; IRQ Enable 
		ORA #IRQ_ENABLE 			
		STA PACTL
		CLI

		LDA #$00			; at this point, no more sound
		STA SOUNDR 			; SILENCE
			
		LDA #$00			; VBLANK not process
		STA VBLANK_PROCESSING
		
		LDA #$07 			; DEFERRED
		LDX RELOC_VBLANK+1		; HIGH BYTE OF USER ROUTINE
		LDY RELOC_VBLANK		; LOW BYTE
		JSR SETVBV
					 		
    		RTS    
   
;------------------------
;------------------------
;;; END RESET HANDLER ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;------------------------
;------------------------

; Some DOS's don't respect MEMLO, so we 
; have to unload all our stuff prior
; to calling DOS or we'll crash the machine

UNLOADONDOS:

		LDA RELOC_UNLOADING
		LDY RELOC_UNLOADING+1
		JSR PRINT_STRING

		LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT			; UNIT #
		STA DUNIT

		LDA #'C'			; Accept
		STA DCOMND

		LDA #0
		STA DSTATS
		STA DBUFLO
		STA DBUFHI

		STA DAUX1
		STA DAUX2
		LDA #$0F
		STA DTIMLO

		LDA #<FUJI_BUFSIZE
		STA DBYTLO
		LDA #>FUJI_BUFSIZE
		STA DBYTHI
		JSR SIOV

		JSR CLOSE_E_DEVICE

		LDA OLD_COLOR4
		STA COLOR4
		
		LDA #$01			; at this point, sound
		STA SOUNDR 			

		LDA #$07 			; DEFERRED
		LDX OLD_VBLANK+1		; HIGH BYTE OF USER ROUTINE
		LDY OLD_VBLANK			; LOW BYTE
		JSR SETVBV
		
		JSR WAIT_FOR_VBLANK		; insure vblank has changed
			
		LDA OLD_DOSVEC			; remove our unload on dos
		STA DOSVEC
		LDA OLD_DOSVEC+1
		STA DOSVEC+1
		
		LDA MEMLO			; reset memory IF it hasn't 
		CMP RELOC_VBLANK_END		; been modified by someone else
		BNE CALL_DOS
		LDA MEMLO+1
		CMP RELOC_VBLANK_END+1
		BNE CALL_DOS
		
		LDA OLD_MEMLO
		STA MEMLO
		LDA OLD_MEMLO+1
		STA MEMLO+1
		
CALL_DOS:				
		JSR JMP_OLD_DOSVEC		; call DOS
		RTS
		
SET_UNLOADONDOS:
		LDA DOSVEC
		STA OLD_DOSVEC
		LDA DOSVEC+1
		STA OLD_DOSVEC+1
		
		LDA RELOC_UNLOAD
		STA DOSVEC
		LDA RELOC_UNLOAD+1
		STA DOSVEC+1
		RTS			
;------------------------
;------------------------ 
; FUJI INTERRUPT HANDLER  
;------------------------
;------------------------   		
FUJI_INTERRUPT_HANDLER:
		
		LDA #CHECK_FOR_DATA
		STA FUJI_ACTIVITY	
		.BYTE $4C 	; JMP
OLD_VPRCED:	.WORD $FFFF	; to the old interrupt handler
;------------------------
;------------------------ 




	icl "..\COMMON\KEYCODE.ASM"	
	icl "..\COMMON\CIO-Routines.asm"
	icl "ScreenRoutines.asm"

RELOCATE_CODE_END:

	icl "..\COMMON\KEYCODE_DATA.ASM"
	icl "..\COMMON\CIO-Routines_DATA.ASM"
	icl "..\COMMON\dosdetect_data.asm"
	icl "ScreenRoutines_DATA.ASM"

OLD_COLOR4			.BYTE $00
	
OLD_INTERRUPT_STATE		.BYTE $00	

FUJI_ACTIVITY			.BYTE $00

DATA_SIZE_LO:			.BYTE 0
DATA_SIZE_HI:			.BYTE 0

UNIT				.BYTE 3
LDDEVIC				.BYTE 3

TEMP_BUFFER:			.DS 	FUJI_BUFSIZE

VBLANK_PROCESSING		.BYTE   0

; You need to relocate addresses within the data block

relocate1500			
RELOC_VBLANK			.WORD	VBLANK

relocate1501			
RELOC_FUJI_INTERRUPT_HANDLER	.WORD	FUJI_INTERRUPT_HANDLER

relocate1502			
RELOC_TEMP_BUFFER		.WORD	TEMP_BUFFER

relocate1503			
RELOC_URL_FUJI			.WORD	URL_FUJI

relocate1504			
RELOC_RESET			.WORD   RESET_HANDLER

relocate1505			
RELOC_VBLANK_END		.WORD	VBLANK_END

relocate1506
RELOC_UNLOAD			.WORD	UNLOADONDOS

JMP_OLD_DOSINI			.BYTE	$4C
OLD_DOSINI			.WORD 	$FFFF

JMP_OLD_DOSVEC			.BYTE	$4C
OLD_DOSVEC			.WORD	$FFFF	
			
OLD_VBLANK			.WORD	$FFFF				

OLD_MEMLO			.WORD	$FFFF


CHANGE_COUNTER			.BYTE	1
DELAY_COUNTER			.BYTE	1


CONNECT_STATE			.BYTE	CONNECTION_CLOSED	
URL_FUJI:			.BYTE 	'N:TCP://:6502/',EOL

relocate1507
RELOC_UNLOADING			.WORD	UNLOADING
UNLOADING			.BYTE   'Unloading RA8...',EOL

relocate1508
RELOC_WILL_UNLOAD		.WORD 	WILL_UNLOAD
WILL_UNLOAD			.BYTE   'RA8 will unload when ''DOS'' command is used', EOL

VBLANK_END: // EVERYTHING ABOVE IS RELOCATED

RELOCATE_DATA_END:

// Only used during setup, doesn't need to be relocated.

	icl "..\COMMON\RELOCATE.ASM"
	icl "..\COMMON\dosdetect.asm"
	
FUJI_OPEN_ERROR			.BYTE 	'Could not open FujiNet Device.',EOL
DEVICE_TIMEOUT_FAILURE	 	.BYTE 	'Device Timeout.',EOL
COLOR_INFO			.BYTE 	'BLACK=NO FUJINET  YELLOW=OPENED,      GREY=WAITING, RED=ACCEPT PENDING      PURPLE=ACCEPTED',EOL

FUJI_DETECT			.BYTE	'Detecting FujiNet Device...',EOL
RELOCATING			.BYTE   'Relocating to: ',EOL
SUCCESS				.BYTE	'Success!',EOL
SETUP_COMPLETE			.BYTE 	'Setup Complete',EOL

DOSSTR				.WORD 	SPARTA		; 0
				.WORD	REAL		; 1
				.WORD	BW		; 2
				.WORD	OSA		; 3
				.WORD	X		; 4
				.WORD	ATARI		; 5
				.WORD	MY		; 6
				.WORD	NONEFOUND	; 7

SPARTA				.BYTE 	'This is SPARTA!',EOL
REAL				.BYTE	'RealDos',EOL
BW				.BYTE   'BW',EOL
OSA				.BYTE   'OSA', EOL
X				.BYTE   'XDOS', EOL
ATARI				.BYTE	'Atari dos',EOL
MY				.BYTE	'My dos', EOL
NONEFOUND			.BYTE 	'No DOS found', EOL	


RELOCATION_TABLE:
		.WORD 	relocate000+1,relocate001+1,relocate002+1,relocate003+1,relocate004+1,relocate005+1,relocate006+1,relocate007+1,/*relocate008+1,*/relocate009+1
		.WORD 	relocate010+1,relocate011+1,relocate012+1,relocate013+1,relocate014+1,relocate015+1,relocate016+1,relocate017+1,relocate018+1,relocate019+1
		.WORD   relocate020+1,relocate021+1/*,relocate022+1*/
		.WORD 	relocate1500,relocate1501,relocate1502,relocate1503,relocate1504,relocate1505,relocate1506,relocate1507;,relocate1508,relocate1509

	icl "..\COMMON\CIO-Routines_RELOC.ASM"

END_RELOCATION_TABLE	.WORD 	0 ; end of table
			.DS	1024

; This should generate an error if there are too many entries (greater than 255)

	
PROGRAM_NAME	.BYTE	'RA8 (Remote Atari) 2021/01/09 19:00',EOL


	run START
