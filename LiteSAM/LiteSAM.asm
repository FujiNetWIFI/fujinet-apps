;-------------------------------------------------------------------
;	REDIRECT SCREENOUTPUT TO SOMETHING ELSE FIRST.
;	(c) Mr.Atari FOR FUJINET 2020
;	USES THE STANDARD ATARI-LABELS
;	DRIVER/CODE IN CASSETTE-BUFFER, DO NOT USE IOCB-4 OR C:
;-------------------------------------------------------------------

	.INCLUDE LABELS.ASM

;-------------------------------------------------------------------
;	EXECUTABLE HEADER
;-------------------------------------------------------------------
	.BYTE	$FF,$FF
;-------------------------------------------------------------------
;PART1, SAM-DRIVER (FUJINET) INTO CASSETTE-BUFFER, DO NOT USE C:
;-------------------------------------------------------------------
	.WORD	CODE1S
	.WORD	CODE1E-1
;-------------------------------------------------------------------
	*=	$400
CODE1S
;-------------------------------------------------------------------
;	ABUSES IOCB #4 / IOCB #0 PUT-BYTE VECTOR / E:TABLE
;-------------------------------------------------------------------

	.BYTE 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0	;E: TABLE, SETUP HERE

;-------------------------------------------------------------------
;	PUT-BYTE ROUTINE, VOICE FIRST, THEN SCREEN
;-------------------------------------------------------------------

	PHA			;SAVE CHARACTER
	CMP	#$9B		;OEL?
	BEQ	POUT
	CMP	#$20		;SPACE?
	BNE	NOEOL
	LDA	#EOL		;CONVERT TO EOL
	BNE	POUT
NOEOL	CMP	#$30		;1
	BCC	NOASC
	CMP	#$3A		;9+1
	BCC	POUT		;NUMBER
	AND	#%11011111	;LOWER CASE -> UPPER CASE
	CMP	#$41		;A
	BCC	NOASC
	CMP	#$5B		;Z+1	
	BCS	NOASC
POUT	LDX	#$40		;ABUSE IOCB #4
	JSR	$0000		;SEND TO P4: / RTS

NOASC	PLA			;RESTORE
	LDX	#$00		;IOCB #0
EOUT	JMP	$0000		;SEND TO E: / Y=STATUS / RTS

;-------------------------------------------------------------------
;	RESET-CODE
;-------------------------------------------------------------------

INIT	JSR	$0000		;PREVIOUS DOSINI

	LDA	#$0F		;PUT-BYTE VECTOR
	STA	$346		;IN IOCB-0
	LDA	#$04
	STA	$347
	STA	$322		;PUT E: IN HATABS
	STA	$381		;ABUSE IOCB #4, DEVICE "4"
	LDA	#$00
	STA	$321
	STA	$380		;ABUSE IOCB #4 LINKED TO P:
;	LDA	#$01		;FOR TESTING SET #$01 AND WATCH PRINTER OUTPUT
;	STA	$381		;ABUSE IOCB #4, DEVICE "4"

NOINIT	RTS

;-------------------------------------------------------------------
CODE1E
;-------------------------------------------------------------------


;-------------------------------------------------------------------
;PART2, INSTALLER
;-------------------------------------------------------------------
	.WORD	CODE2S
	.WORD	CODE2E-1
;-------------------------------------------------------------------
	*=	$4000
CODE2S
;-------------------------------------------------------------------
INSTAL1	LDA	#8		;OPEN TYPE (WRITE)
	STA	ICAX1+$40
	LDA	#DEVP&$FF
	STA	ICBAL+$40
	LDA	#DEVP/256
	STA	ICBAH+$40
	LDA	#3		;OPEN
	STA	ICCOM+$40	;COMMAND

	LDX	#$40		;GENERAL ENTRY ON IOCB #4
	JSR	CIOV		;DO IO/RTS, SETUP IOCB #4 FOR PRINTER USAGE
	BMI	NOFUJI

	LDA	DOSINI		;COPY DOSINI TO CODE
	STA	INIT+1
	LDA	DOSINI+1
	STA	INIT+2

	LDA	#INIT&$FF	;SETUP NEW DOSINI
	STA	DOSINI
	LDA	#INIT/256
	STA	DOSINI+1

	LDX	$E406		;E: PUT-BYTE LOW
	LDY	$E407		;PUT-BYTE HIGH
	INX			;CONVERT TO JSR
	BNE	NPAGE		;BOUNDARY
	INY
NPAGE	STX	EOUT+1
	STY	EOUT+2

	LDX	$E436		;P: PUT-BYTE LOW
	LDY	$E437		;PUT-BYTE HIGH
	INX			;CONVERT TO JSR
	BNE	NPAGE2		;BOUNDARY
	INY
NPAGE2	STX	POUT+3
	STY	POUT+4

	LDX	#$0F		;COPY E:TABLE TO DRIVER
COPTAB	LDA	$E400,X
	STA	$400,X
	DEX
	BPL	COPTAB

	LDA	#$0F		;INSTALL PATCHED E:
	STA	$406		;IN TABLE
	LDA	#$04
	STA	$407

	JSR	INIT+3		;INSTALL IT

	LDX	#WMES2&$FF	;SHOW, SAY IT, EXIT
	LDY	#WMES2/256
	JSR	SCROUT

;	RETURN TO CALLER

EXIT	LDA	COLDST		;PREPAIR TO EXIT
	BMI	CEXIT		;WE ARE BOOTING

;	NOT BOOTING, USER MEMORY USED

	LDX	#$FF		;PREPAIR 'COLD-RESET'
	TXS			;RESET STACK-POINTER
	INX			;MEMORY-CONTENT CHANGED
	STX	WARMST		;CHANGE OS-FLAG TO COLD

	LDA	CART		;CHECK FOR CARTRIDGE, MUST BE ZERO.
	BNE	EXITR		;NOP

	LDA	CARTFG		;DOES IT WANT TO RUN?
	AND	#$04
	BEQ	EXITR		;NO

;	BACK TO (SOFT) CARTRIDGE

	JSR	CARINI		;RE-INIT CARTRIDGE
	JMP	($BFFA)		;JUMP TO IT

CARINI	JMP	($BFFE)		;INIT CARTRIDGE

;	BACK TO LOADED SOFTWARE

EXITR	JMP	(DOSVEC)	;USE VECTOR....

CEXIT	LDY	#1		;SUCCESS
	RTS

;-------------------------------------------------------------------

;	NO DEVICE

NOFUJI	LDX	#WMES1&$FF	;SORRY, NO DEVICE FOUND
	LDY	#WMES1/256
	JSR	SCROUT
	BPL	EXIT

;	MESSAGES

SCROUT	LDA	#255		;LENGTH
	STX	ICBAL		;LO
	STY	ICBAH		;HI
	STA	ICBLL		;LENGTH
	STY	ICBLH		;HI
	LDX	#SEIOCB		;SCREEN
	LDA	#PUTREC		;SEND
	STA	ICCOM		;COMMAND
	JMP	CIOV		;DO IT / RTS

WMES1	.BYTE	"LiteSAM - No FujiNet found",$9B
WMES2	.BYTE	"LiteSAM - Installed",$9B
DEVP	.BYTE	"P4:",EOL	;FOR TESTING, USE P1, AND WATCH PRINTER OUTPUT
;-------------------------------------------------------------------
CODE2E
;-------------------------------------------------------------------


;-------------------------------------------------------------------
;PART3,	RUN-ADDRESS
;-------------------------------------------------------------------
	.WORD	CODE3S
	.WORD	CODE3E-1
;-------------------------------------------------------------------
	*=	$2E0
CODE3S
;-------------------------------------------------------------------
	.WORD	INSTAL1
;-------------------------------------------------------------------
CODE3E

	.END