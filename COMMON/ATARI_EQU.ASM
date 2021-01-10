.ifndef ATARI_EQUs

ATARI_EQUs = 1
; this is from cc65
SPARTADOS   			= 0
REALDOS     			= 1
BWDOS       			= 2
OSADOS      			= 3
XDOS        			= 4
ATARIDOS    			= 5
MYDOS       			= 6
NODOS       			= 255


;  https://www.atariarchives.org/mapping/memorymap.php 
LINZBS				= $00
CASINI				= $02		; Cassette initialization vector
RAMLO				= $04
TRAMSZ				= $06
TSTDAT				= $07
WARMST				= $08		; Warmstart flag 0=in powerup 255=normal reset (don't wipe memory)
; BOOT is defined twice in mapping the atari
;BOOT				= $09		; Boot flag success indicator 255=lockup on reset, 1=boot disk successful 2=cassette boot successful
DOSVEC				= $0A		; Start fector for disk software
DOSVEC_LO			= $0A
DOSVEC_HI			= $0B
DOSINI				= $0C
DOSINI_LO			= $0C
DOSINI_HI			= $0D
APPMHI				= $0E
POKMSK				= $10		; Pokey interrupts
BRKKEY				= $11		; Zero means BREAK key is pressed
RTCLOK				= $12
RTCLOK_JIFFY			= $14
BUFADR				= $15		; Temporary pointer to the current disk buffer
ICCOMT				= $17		; Command for CIO vector
DSKFMS				= $18		; Disk file manager pointer
DSKUTL				= $1A		; Disk utilities pointer
PTIMOT				= $1C		; Printer timeout
PBPNT				= $1D		; Print buffer pointer
PBUFSZ				= $1E		; Prin buffer size of printer record for current mode
PTEMP				= $1F		; temporary register used by the printer handler
ICHIDZ  			= $20    	; ID
ICDNOZ  			= ICHIDZ+1  	; UNIT #
ICCOMZ  			= ICHIDZ+2  	; COMMAND
ICSTAZ  			= ICHIDZ+3  	; STATUS
ICBALZ  			= ICHIDZ+4  	; BUF ADR LOW
ICBAHZ  			= ICHIDZ+5  	; BUF ADR HIGH
ICPTLZ  			= ICHIDZ+6  	; PUT ADDR L
ICPTHZ  			= ICHIDZ+7  	; PUT ADDR H
ICBLLZ  			= ICHIDZ+8  	; BUF LEN LOW
ICBLHZ  			= ICHIDZ+9  	; BUF LEN HIGH
ICAX1Z  			= ICHIDZ+10 	; AUX 1
ICAX2Z  			= ICHIDZ+11 	; AUX 2
ICAX3Z  			= ICHIDZ+12 	; AUX 3
ICAX4Z  			= ICHIDZ+13 	; AUX 4
ICAX5Z  			= ICHIDZ+14 	; AUX 5
ICAX6Z  			= ICHIDZ+15 	; AUX 6
STATUS				= $30
CHKSUM				= $31
BUFRLO				= $32
BUFRHI				= $33
BFENLO				= $34
BFENHI				= $35
CRETRY				= $36
DRETRY				= $37
BUFRLF				= $38		; Data buffer full flag(255 == full)
RECVDN				= $39
XMTDON				= $3A
CHKSNT				= $3B
NOCKSM				= $3C
BPTR				= $3D		; Cassette buffer pointer
FTYPE				= $3E
FEOF				= $3F
FREQ				= $40
SOUNDR				= $41
CRITIC				= $42
FMZSPG				= $43
ZBUFP				= $43
ZDRVA				= $45
ZSBA				= $47
;Errno is defined twice in mapping the atari
;ERRNO				= $49
CKEY				= $4A
CASSBT				= $4B
DSTAT				= $4C		; Display status and keyboard register
ATRACT				= $4D
DRKMSK				= $4E
COLRSH				= $4F
;TEMP				= $50		; defined twice with different values
TMPCHR				= $50
HOLD1				= $51
LMARGIN				= $52		; Column of left margin of text (GR.0 or text window only)
RMARGIN				= $53		; Right margin of text screen
ROWCRS				= $54		; current graphics or text screen cursor row
COLCRS				= $55		; current graphics or text screen cursor column
DINDEX				= $57		; current screen mode
SAVMSC				= $58		; The lowest address of the screen memory, 
						; corresponding to the upper left corner 
						; of the screen
OLDROW				= $5A		; Previous graphics cursor row
OLDCOL				= $5B		; Previous graphics cursor column
OLDCHR				= $5D		; character under cursor
OLDADR				= $5E		; memory location of current susor location
NEWROW				= $60
NEWCOL				= $61
LOGCOL				= $63
ADRESS				= $64
MLTTMP				= $66
SAVADR				= $68
FRMADR				= $68
RAMTOP				= $6A		; RAM size - PEEK(106) * 256 will tell you where the Atari thinks the last usable address --byte-- of RAM is
BUFCNT				= $6B
BUFSTR				= $6C
BITMSK				= $6E
SHFAMT				= $6F
ROWAC				= $70
COLAC				= $72
ENDPT				= $74
DELTAR				= $76
DELTAC				= $77
ROWINC				= $79
COLINC				= $7A
SWPFLG				= $7B
HOLDCH				= $7C
INSDAT				= $7D
COUNTR				= $7E
LOMEM				= $80
LOMEM_LO			= $80
LOMEM_HI			= $81
VNTP				= $82
VNTD				= $84
VVTP				= $86
STMTAB				= $88
STMCUR				= $8A
STARP				= $8C
RUNSTK				= $8E
; MEMTOP is defined twice in mapping the atari, so I've commented out both
;MEMTOP				= $90		; Pointer to the top of BASIC memory

; ($92 to $CA) are reserved for use by the 8K BASIC ROM.
; ($B0 to $CF) are reserved by the Assembler Editor cartridge for the user's page zero use

STOPLN				= $BA
ERRSAV				= $C3
PTABW				= $C9

; UNUSED $CB-$CF, $D0-$D3

FR0				= $D4		; Floating point register zero
FRE				= $DA
FR1				= $E0		; Floating point register one
FR2				= $E6
FRX				= $EC
EEXP				= $ED
NSIGN				= $EE
ESIGN				= $F0
DIGRT				= $F1
CIX				= $F2
INBUFF				= $F3		; Input ASCII text buffer pointer
ZTEMP1				= $F5
ZTEMP4				= $F7
ZTEMP3				= $F9
RADFLG				= $FB
DEGFLG				= $FB
FLPTR				= $FC
FPTR2				= $FE

VDSLST				= $0200
VPRCED				= $0202		; Serial proceed line vector
VINTER				= $0204		; Serial interrupt vector
VBREAK				= $0206
VKEYBD				= $0208		; POKEY keyboard interrupt vector
VSERIN				= $020A
VSEROR				= $020C
VSEROC				= $020E
VTIMR1				= $0210
VTIMR2				= $0212
VTIMR4				= $0214
VIMIRQ				= $0216
CDTMV1				= $0218
CDTMV2				= $021A
CDTMV3				= $021C
CDTMV4				= $021E
CDTMV5				= $0220
VVBLKI				= $0222
VVBLKD				= $0224
CDTMA1				= $0226
CDTMA2				= $0228
CDTMF3				= $022A
SRTIMR				= $022B
CDTMF4				= $022C
INTEMP				= $022D
CDTMF5				= $022E
SDMCTL				= $022F
SDLSTL				= $0230
SSKCTL				= $0232
SPARE				= $0233
LPENH				= $0234	
LPENV				= $0235
BRKKY				= $0236
CDEVIC				= $023A
CCOMND				= $023B
CAUX1				= $023C
CAUX2				= $023D
;TEMP				= $023E		; defined twice with different values
ERRFLG				= $023F
DFLAGS				= $0240
DBSECT				= $0241
BOOTAD				= $0242
COLDST				= $0244		; Coldstart flag. Zero is normal, if zero, then pressing RESET will not result in reboot.
DSKTIM				= $0246
LINBUF				= $0247
GPRIOR				= $026F
PADDL0				= $0270
PADDL1				= $0271
PADDL2				= $0272
PADDL3				= $0273
PADDL4				= $0274
PADDL5				= $0275
PADDL6				= $0276
PADDL7				= $0277
STICK0				= $0278
STICK1				= $0279
STICK2				= $027A
STICK4				= $027B
PTRING0				= $027C
PTRING1				= $027D
PTRING2				= $027E
PTRING3				= $027F
PTRING4				= $0280
PTRING5				= $0281
PTRING6				= $0282
PTRING7				= $0283
STRIG0				= $0284
STRIG1				= $0285
STRIG2				= $0286
STRIG3				= $0287
CSTAT				= $0288
WMODE				= $0289
BLIM				= $028A
TXTROW				= $0290
TXTCOL				= $0291
TINDEX				= $0293
TXTMSC				= $0294
TXTOLD				= $0296
TMPX1				= $029C
HOLD3				= $029D
SUBTMP				= $029E
HOLD2				= $029F
DMASK				= $02A0
TMPLBT				= $02A1
ESCFLG				= $02A2
TABMAP				= $02A3
LOGMAP				= $02B2
INVFLAG				= $02B6
FILFLG				= $02B7
TMPROW				= $02B8
TMPCOL				= $02B9
SCRFLG				= $02BB
HOLD4				= $02BC
HOLD5				= $02BD
SHFLOK				= $02BE
BOTSCR				= $02BF
PCOLR0				= $02C0
PCOLR1				= $02C1
PCOLR2				= $02C2
PCOLR3				= $02C3
COLOR0				= $02C4
COLOR1				= $02C5
COLOR2				= $02C6
COLOR3				= $02C7
COLOR4				= $02C8
GLBABS				= $02E0
RUNAD				= $02E0		; Used by DOS
INITAD				= $02E2
RAMSIZ				= $02E4		; RAM size; high byte only
;MEMTOP				= $02E5
MEMLO				= $02E7
DVSTAT				= $02EA
DVSTAT_BYTES_WAITING_LO 	= $02EA
DVSTAT_BYTES_WAITING_HI 	= $02EB
DVSTAT_PROTOCOL			= $02EC 	; depends on the protocol
DVSTAT_EXTENDED_ERROR		= $02ED

;     Four device status registers used by the I/O status operation as
;     follows:
;
;     746 ($2EA) is the device error status and the command status
;     byte. If the operation is a disk I/O, then the status returned is that
;     of the 1771 controller chip in your Atari disk drive. Bits set to one
;     return the following error codes:
;
;     Bit  Decimal  Error
;     0       1     An invalid command frame was received (error).
;     1       2     An invalid data frame was received.
;     2       4     An output operation was unsuccessful.
;     3       8     The disk is write-protected.
;     4      16     The system is inactive (on standby).
;     7      32     The peripheral controller is "intelligent" (has its
;                   own microprocessor: the disk drive). All Atari
;                   devices are intelligent except the cassette
;                   recorder, so BIT 7 will normally be one when a
;                   device is attached.
;
;     747 ($2EB) is the device status byte. For the disk, it holds the
;     value of the status register of the drive controller. For the 850
;     Interface, it holds the status for DSR,CTS,CRX and RCV when
;     concurrent I/O is not active (see the 850 Interface Manual). It also
;     contains the AUX2 byte value from the previous operation (see
;     the IOCB description at 832 to 959; $340 to $3AF).
;     748 ($2EC) is the maximum device time-out value in seconds. A
;     value of 60 here represents 64 seconds. This value is passed back
;     to location 582 ($246) after every disk status request. Initialized to
;     31.
;     749 ($2ED) is used for number of bytes in output buffer. See 850
;     Manual, p. 43.
;     When concurrent I/O is active, the STATUS command returns
;     the number of characters in the input buffer to locations 747 and
;     748, and the number of characters in the output buffer to location
;     749.

CBAUDL				= $02EE
CBAUDH				= $02EF
CRSINH				= $02F0		; Cursor inhibit flag. 0=Cursor on
KEYDEL				= $02F1	
CH1				= $02F2
CHACT				= $02F3
CHBAS				= $02F4
CHAR				= $02FA
ATACHR				= $02FB
CH				= $02FC
FILEDAT				= $02FD
DSPFLG				= $02FE
SSFLAG				= $02FF

DDEVIC  			= $0300     ; DEVICE #
DUNIT   			= DDEVIC+1   ; UNIT #
DCOMND  			= DDEVIC+2   ; COMMAND
DSTATS  			= DDEVIC+3   ; STATUS/DIR
DBUFLO   			= DDEVIC+4   ; BUF ADR L
DBUFHI   			= DDEVIC+5   ; BUF ADR H
DTIMLO  			= DDEVIC+6   ; TIMEOUT (S)
DUNUSE   			= DDEVIC+7   ; NOT USED
DBYTLO   			= DDEVIC+8   ; BUF LEN L
DBYTHI   			= DDEVIC+9   ; BUF LEN H
DAUX1   			= DDEVIC+10  ; AUX BYTE L
DAUX2   			= DDEVIC+11  ; AUX BYTE H

TIMER1				= $030C
ADDCOR				= $030E
CASFLG				= $030F
TIMER2				= $0310
TEMP1				= $0312
TEMP2				= $0314
TEMP3				= $0315
SAVIO				= $0316
STACKP				= $0318
TSTAT				= $0319
HATABS				= $031A
			

ICHID   			= $0340     	;   Set by OS. Handler Identifier. If not 
                    				;   in use, the value is 255 ($FF), which 
        			            	;   is also the initialization value.
ICDNO   			= ICHID+1   	;   Set by OS. Device number (eg: D1: D2:).
ICCOM   			= ICHID+2   	;   Set by User. Command
ICSTA   			= ICHID+3   	;   Set by OS. May or may not be the same 
                    				;   value as CMD_STATUS returned
ICBAL   			= ICHID+4   	;   Set by User. Buffer address (low byte)
ICBAH   			= ICHID+5   	;   Set by User. buffer address (high byte)
ICPTL   			= ICHID+6   	;   Used by BASIC. Address of put byte routine. 
ICPTH   			= ICHID+7   	;   Used by BASIC. Address of put byte routine. 
ICBLL   			= ICHID+8   	;   buffer length (low byte) in put/get operations
ICBLH   			= ICHID+9   	;   buffer length (high byte)
ICAX1   			= ICHID+10  	;   auxiliary information.  Used by Open cmd 
			                    	;   for READ/WRITE/UPDATE
			                    	;   Bit  7   6   5   4   3   2   1   0
			                    	;   Use  ....unused....  W   R   D   A
			                    	;   W equals write, R equals read, 
			                    	;   D equals directory, A equals append.
ICAX2   			= ICHID+11  	;   Auxiliary byte two
ICAX3   			= ICHID+12  	;   Auxiliary bytes three
ICAX4   			= ICHID+13  	;   Auxiliary bytes four
ICAX5   			= ICHID+14  	;   Auxiliary bytes five
ICAX6   			= ICHID+15  	;   Auxiliary bytes six

IOCB0				= $0340
IOCB1				= $0350
IOCB2				= $0360
IOCB3				= $0370
IOCB4				= $0380
IOCB5				= $0390
IOCB6				= $03A0
IOCB7				= $03B0

PRNBUF				= $03C0
CASBUF				= $03FD
LBPR1				= $057E
LBPR2				= $057F
LBUFF				= $0580
PLYARG				= $05E0
FPSCR				= $05E6
FPSCR1				= $05EC

SABYTE				= $0709
DRVBYT				= $070A
BSIO				= $076C
BSIOR				= $0772
DFMSDH				= $07CB
DINT				= $07E0
DFMOPN				= $08AB
DFMPUT				= $0900
WTBUR				= $0A1F
DFMGET				= $0ABF
DFMCLS				= $0B15
DFMDDC				= $0BA7
XRENAME				= $0BD9
XDELETE				= $0C32
XLOCK				= $0C7C
XUNLOCK				= $0C7C
XPOINT				= $0CBA
XNOTE				= $0D03
XFORMAT				= $0D18
LISTDIR				= $0DAD
FNDCODE				= $0E9E
FDSCHAR				= $0F0A
SFDIR				= $0F21
WRTNXS				= $0F94
RDNXTS				= $100F
RDDIR				= $106E
RDVTOC				= $108B
FRESECT				= $10C5
GETSECTOR			= $1106
SETUP				= $1164
WRTDOS				= $120A
; Errorno is defined twice in mapping the atari
;ERRNO				= $12B5
FCB				= $1381
FILDIR				= $1401
ENDFMS				= $1501
DOS				= $1540
DUPFLG				= $159D
OPT				= $159E
LOADFLG				= $159F
SFLOAD				= $15A4
USRDOS				= $1700
MEMLDD				= $170B
INISAV				= $179C
MEMFLG				= $179E
CLMJMP				= $1912
LMTR				= $1920
LDMEM				= $1939
INITIO				= $1979
; ISRSIR is defined twice in mapping the atari
;ISRSIR				= $1A23
LINE				= $1DA4
DBUF				= $1DF4
DMENU				= $1F0F

DOSOS				= $2075
DIRLST				= $2139
DELFIL				= $21C9
RENFIL				= $2637
FMTDSK				= $2680
STCAR				= $26EE
BRUN				= $274C
WBOOT				= $27D9
TESTVER2			= $28F3
LDFIL				= $291A
LKFILE				= $2970
ULFILE				= $2970
DDMG				= $29C2
DFFM				= $2D08
SAVFIL				= $2F2E

PORTA				= $D300
PORTB				= $D301
PACTL				= $D302
PBCTL				= $D303


DMACTL				= $D400
CHACTL				= $D401
DLISTL				= $D402
DLISTH				= $D403
HSCROL				= $D404
VSCROL				= $D405
PMBASE				= $D407
CHBASE				= $D409
WSYNC				= $D40A
VCOUNT				= $D40B
PENH				= $D40B
PENV				= $D40D
NMIEN				= $D40E
NMIRES				= $D40F


EDITRV				= $E400	; Screen Editor E: entry point table
SCRENV				= $E410 ; S:
KEYBDV				= $E420 ; K:
PRINTV				= $E430 ; P:
CASETV				= $E440 ; C:
DISKIV				= $E450 
DISKINV				= $E453
CIOV				= $E456
SIOV				= $E459
SETVBV				= $E45C
SYSVBV				= $E45F
XITVBV				= $E462
SIOINV				= $E465
SENDEV				= $E468
INTINV				= $E46B
CIOINV				= $E46E
BLKBDV				= $E471
WARMSV				= $E474
COLDSV				= $E477
RBLOKV				= $E47A
CSOPIV				= $E47D
VCTABL				= $E480
CIOORG				= $E4A6
CIOINT				= $E4A6
CIREAD				= $E569
INTORG				= $E6D5
PIRQ				= $E6F3
PNMI				= $E7B4
SYSVBL				= $E7D1
SETVBL				= $E912
SIOORG				= $E944
SEND				= $EA6B
ISRODN				= $EA90
ISRTD				= $EAD1
; ISRSIR is defined twice in mapping the atari
;ISRSIR				= $EB11
CASENT				= $EB84
COMPUT				= $ECA7
DSKORG				= $EDEA
PRNORG				= $EE78
CASORG				= $EF41
MONORG				= $F0E3
RESET				= $F11B
HARDI				= $F281
OSRAM				= $F294
; BOOT is defined twice in mapping the atari
;BOOT				= $F2CF
DOBOOT				= $F2ED
DOPEN				= $F37E
KBDORG				= $F3E4
EGETCH				= $F63E
EOUTCH				= $F6A4
KGETC2				= $F6DD
KGETCH				= $F6E2
SCROLL				= $FBAC
DRAW				= $FCFC
PIRQQ				= $FFBE
CHKSUN				= $FFF8


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
CMD_OPEN       			= $03
CMD_GET_REC    			= $05    ; reads data from the device 
;                   			   until a carriage-return
CMD_GET_CHARS  			= $07    ; get bytes
CMD_PUT_REC    			= $09    ; print until EOL
CMD_PUT_CHARS  			= $0B    ; put bytes 
CMD_CLOSE      			= $0C
CMD_STATUS     			= $0D

SUCCES   			= $01	; SUCCESSFUL OPERATION
BRKABT   			= $80	; (128) BREAK KEY ABORT
PRVOPN   			= $81	; (129) IOCB ALREADY OPEN
NONDEV   			= $82 	; (130) NON-EX DEVICE
WRONLY   			= $83 	; (131) IOCB OPENED FOR WRITE ONLY
NVALID   			= $84	; (132) INVALID COMMAND
NOTOPN   			= $85 	; (133) DEVICE OR FILE NOT OPEN
BADIOC   			= $86   ; (134) INVALID IOCB NUMBER
RDONLY   			= $87   ; (135) IOCB OPENED FOR READ ONLY
EOFERR   			= $88   ; (136) END OF FILE
TRNRCD   			= $89   ; (137) TRUNCATED RECORD
TIMOUT   			= $8A   ; (138) DEVICE TIMEOUT
DNACK    			= $8B   ; (139) DEVICE DOES NOT ACK COMMAND
FRMERR  	 		= $8C   ; (140) SERIAL BUS FRAMING ERROR
CRSROR   			= $8D   ; (141) CURSOR OUT OF RANGE
OVRRUN   			= $8E   ; (142) SERIAL BUS DATA OVERRUN
CHKERR   			= $8F   ; (143) SERIAL BUS CHECKSUM ERROR
DERROR   			= $90   ; (144) DEVICE ERROR (OPERATION INCOMPLETE)
BADMOD   			= $91   ; (145) BAD SCREEN MODE NUMBER
FNCNOT   			= $92   ; (146) FUNCTION NOT IN HANDLER
SCRMEM   			= $93   ; (147) INSUFFICIENT MEMORY FOR SCREEN MODE
;
; OPTIONS FOR OPEN ICAX1
OREAD  	 			= $04	; Read Only
OWRITE   			= $08	; Write Only
OUPDATE  			= $0C	; Read and write
ODIR     			= $02	; Directory
ORDIR    			= $06	; Read Directory
OAPPEND  			= $01	; Append
OWAPPEND 			= $09	; Write Append

DWRITE				= $80   	; sending   data to SIO device
DREAD				= $40   	; receiving data from the SIO device

EOL				= $9B		; ATASCII end of line

.endif
