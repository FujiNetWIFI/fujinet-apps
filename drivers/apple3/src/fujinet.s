; This is a modified version of my smartport driver to match with the Fujinet
; The following devices are supported:
;   Unit  Name       Type
;   0     .PROFILE   Block  (to work with existing stuff that expects to be on a Profile)
;   1     .S2        Block
;   2     .S3        Block
;   3     .S3        Block
;   4     .NETWORK   Char
;   5     .PRINTER   Char
;   6     .CPM       Char
;   7     .RS232     Char -> mapped to Fujinet MODEM device
;   8     .FN_CLOCK  Char
;
; buffer is increased to 768, max size supported for smartport bus
;
; Todo: - search for the Char dev DIB Names and map to the correct SP Dev num
;         currently the mappingis hard coded
;       - optimise the buffer usage (stop the double buffering to speed it up)
;
; Add option to assemble just the character devices. This will work with either my
; soshdboot disk images, or would also work with stock standard SOS1.3
; - define symbol 'charonly' to remove the block devices
;   Note: char unit numbers stay the same, ie from 4 onwards.
;
;
; SmartPort driver for prodos compatible SmartPort cards
; This is based on the problock3 driver with some learnings aquired along the journey of
; developing that.
;
; The driver dynamically sets the slot when it receives an init request.
; Slots are searched from 4 to 1 for the rom signature indicating the card
; supports the Smartport interface. The first one it finds, it updates
; the DIB slot numbers to the found slot.
;
; The driver swaps in the screen holes and changes the ZP to $03($0300) to disable the A3 enhanced
; indirect addressing. This is incase the card firmware uses indirect addressing to fill the
; requested buffer. It then swaps them back and restores the ZP to $18($1800) before returning to SOS.
; It also disables VBL interrupts that can be used for font loading by the
; console before any screenhole changes to avoid font loading corruption.
;
; This version uses a 512 byte buffer for the firmware to store the block data, and then copies
; this to the requested address pointer after returning from the firmware
;

;            .TITLE "Apple /// Fujinet Smartport Driver"
            .PROC  Smartport

            .setcpu "6502"
            .reloc

DriverVersion    = $002B      ; Version number
DriverMfgr       = $524A      ; Driver Manufacturer - RJ
DriverType       = $E1        ; No formatter present for the time being
DriverSubtype    = $02        ;
DriverTypeN      = $66        ; Character dev, read/write
DriverSubtypeN   = $01        ;
DriverTypeP      = $66        ; Character dev, read/write
DriverSubtypeP   = $01        ;
DriverTypeC      = $63        ; Character dev, read/write
DriverSubtypeC   = $01        ;
DriverTypeM      = $63        ; Character dev, read/write
DriverSubtypeM   = $01        ;
ScanStart        = $04        ; Slot number to start scan from
AutoScan         = $FF        ; Auto scan slots

;
; SOS Equates
;
ExtPG       = $1401         ; Driver extended bank address offset
AllocSIR    = $1913         ; Allocate system internal resource
DeAlcSIR    = $1916         ; De-allocate system internal resource
QueEvent    = $191f         ; Queue an event
SELC800     = $1922         ; Enable Expansion Rom Space
SysErr      = $1928         ; Report error to system
ZPReg       = $FFD0         ; Zero Page register
EReg        = $FFDF         ; Environment register
BReg        = $FFEF         ; Bank register
E_IFR       = $FFED         ; VIA E Interrupt Flag Register
E_IER       = $FFEE         ; VIA E Interrupt Enable Register
CWrtOff     = $C0DA         ; Character(font) loading off

;
; SOS Zero page parameters
;
ReqCode     = $C0           ; Request code
SOS_Unit    = $C1           ; Unit number
SosBuf      = $C2           ; SOS buffer pointer
ReqCnt      = $C4           ; Requested byte count
CtlStat     = $C2           ; Control/status code
CSList      = $C3           ; Control/status list pointer
SosBlk      = $C6           ; Starting block number
QtyRead     = $C8           ; Bytes read return by D_READ


;
; Parameter block specific to current SOS request
;
Num_Blks    = $D2           ; Number of blocks requested (we'll never ever have > 128 blocks)
Count       = $D3           ; 2 bytes lb,hb

;
; Extra zero page variables
;
ScreenBase  = $D5           ; 2 bytes lb,hb for save/restore screenholes
Pointer     = $D7           ; 2 byte pointer for signature check
SosBuf2     = $D9           ; 2 byte temp pointer for buffer transfers

;
; SOS System calls
;
SET_TIME    = $62           ; set time
D_STATUS    = $82           ; device status
GET_DEV_NUM = $84           ; Get device number

;
; SOS Error Codes
;
XDNFERR     = $10           ; Device not found
XBADDNUM    = $11           ; Invalid device number
XREQCODE    = $20           ; Invalid request code
XCTLCODE    = $21           ; Invalid control/status code
XCTLPARAM   = $22           ; Invalid control/status parameter
XNORESRC    = $25           ; Resource not available
XBADOP      = $26           ; Invalid operation
XIOERROR    = $27           ; I/O error
XNODRIVE    = $28           ; Drive not connected
XBYTECNT    = $2C           ; Byte count not a multiple of 512
XBLKNUM     = $2D           ; Block number to large
XDISKSW     = $2E           ; Disk switched
XDCMDERR    = $31           ; device command ABORTED error occurred
XCKDEVER    = $32           ; Check device readiness routine failed
XNORESET    = $33           ; Device reset failed
XNODEVIC    = $38           ; Device not connected

;
; Smartport command codes
;
SP_Status     = $00
SP_ReadBlock  = $01
SP_WriteBlock = $02
SP_Format     = $03
SP_Control    = $04
SP_Init       = $05
SP_Open       = $06
SP_Close      = $07
SP_Read       = $08
SP_Write      = $09


;
; Switch Macro
;
.MACRO      SWITCH index,bounds,adrs_table,noexec    ; See SOS Reference
.IFNBLANK   index                         ; If PARM1 is present,
            lda        index              ; load A with switch index
.ENDIF
.IFNBLANK   bounds                        ; If PARM2 is present,
            cmp        #bounds+1          ; perform bounds checking
            bcs        @110               ; on switch index
.ENDIF
            asl        A                  ; Multiply by 2 for table index
            tay
            lda        adrs_table+1,y     ; Get switch address from table
            pha                           ; and push onto Stack
            lda        adrs_table,y
            pha
.IFBLANK    noexec
            rts                           ; Exit to code
.ENDIF
@110:
.ENDMACRO

            .SEGMENT "TEXT"

;
; Comment Field of driver
;
            .word    $FFFF ; Signal that we have a comment
            .word    COMMENT_END - COMMENT
COMMENT:    .byte    "Apple /// Fujinet Smartport Driver"
COMMENT_END:

            .SEGMENT    "DATA"

.ifndef charonly
;------------------------------------
;
; Device identification Block (DIB) - #0
;
;------------------------------------

DIB_0:      .word    DIB_1            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $08              ; Name length byte
            .byte    ".PROFILE       "; Device name
            .byte    $80              ; Active, no page alignment
DIB0_Slot:  .byte    AutoScan         ; Slot number
            .byte    $00              ; Unit number
            .byte    DriverType       ; Type
            .byte    DriverSubtype    ; Subtype
            .byte    $00              ; Filler
DIB0_Blks:  .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB

; Page alignment begins here
;

;
; Device identification Block (DIB) - #1
;
DIB_1:      .word    DIB_2            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $03              ; Name length byte
            .byte    ".S2            "; Device name
            .byte    $80              ; Active
DIB1_Slot:  .byte    AutoScan         ; Slot number
            .byte    $01              ; Unit number
            .byte    DriverType       ; Type
            .byte    DriverSubtype    ; Subtype
            .byte    $00              ; Filler
DIB1_Blks:  .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Driver manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB
;
; Device identification Block (DIB) - #2
;
DIB_2:      .word    DIB_3            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $03              ; Name length byte
            .byte    ".S3            "; Device name
            .byte    $80              ; Active
DIB2_Slot:  .byte    AutoScan         ; Slot number
            .byte    $02              ; Unit number
            .byte    DriverType       ; Type
            .byte    DriverSubtype    ; Subtype
            .byte    $00              ; Filler
DIB2_Blks:  .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Driver manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB
;
; Device identification Block (DIB) - #3
;
DIB_3:      .word    DIB_4            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $03              ; Name length byte
            .byte    ".S4            "; Device name
            .byte    $80              ; Active
DIB3_Slot:  .byte    AutoScan         ; Slot number
            .byte    $03              ; Unit number
            .byte    DriverType       ; Type
            .byte    DriverSubtype    ; Subtype
            .byte    $00              ; Filler
DIB3_Blks:  .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Driver manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB
.endif

;
; Device identification Block (DIB) - #4
;
DIB_4:      .word    DIB_5            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $08              ; Name length byte
DIB4_Name:  .byte    ".NETWORK       "; Device name
            .byte    $80              ; Active
DIB4_Slot:  .byte    AutoScan         ; Slot number
            .byte    $04              ; Unit number
            .byte    DriverTypeN      ; Type
            .byte    DriverSubtypeN   ; Subtype
            .byte    $00              ; Filler
            .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Driver manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB
;
; Device identification Block (DIB) - #5
;
DIB_5:      .word    DIB_6            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $08              ; Name length byte
DIB5_Name:  .byte    ".PRINTER       "; Device name
            .byte    $80              ; Active
DIB5_Slot:  .byte    AutoScan         ; Slot number
            .byte    $05              ; Unit number
            .byte    DriverTypeP      ; Type
            .byte    DriverSubtypeP   ; Subtype
            .byte    $00              ; Filler
            .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Driver manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB
;
; Device identification Block (DIB) - #6
;
DIB_6:      .word    DIB_7            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $04              ; Name length byte
DIB6_Name:  .byte    ".CPM           "; Device name
            .byte    $80              ; Active
DIB6_Slot:  .byte    AutoScan         ; Slot number
            .byte    $06              ; Unit number
            .byte    DriverTypeC      ; Type
            .byte    DriverSubtypeC   ; Subtype
            .byte    $00              ; Filler
            .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Driver manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB

;
; Device identification Block (DIB) - #7
;
DIB_7:      .word    DIB_8            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $06              ; Name length byte
DIB7_Name:  .byte    ".RS232         "; Device name
            .byte    $80              ; Active
DIB7_Slot:  .byte    AutoScan         ; Slot number
            .byte    $07              ; Unit number
            .byte    DriverTypeM      ; Type
            .byte    DriverSubtypeM   ; Subtype
            .byte    $00              ; Filler
            .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Driver manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB

;
; Device identification Block (DIB) - #8
;
DIB_8:      .word    $0000            ; Link pointer
            .word    Entry            ; Entry pointer
            .byte    $09              ; Name length byte
DIB8_Name:  .byte    ".FN_CLOCK      "; Device name
            .byte    $80              ; Active
DIB8_Slot:  .byte    AutoScan         ; Slot number
            .byte    $08              ; Unit number
            .byte    DriverTypeM      ; Type
            .byte    DriverSubtypeM   ; Subtype
            .byte    $00              ; Filler
            .word    $0000            ; # Blocks in device
            .word    DriverMfgr       ; Driver manufacturer
            .word    DriverVersion    ; Driver version
            .word    $0000            ; DCB length followed by DCB


;------------------------------------
;
; Local storage locations
;
;------------------------------------

Buffer:     .res    768                 ; buffer for smartport interface in this bank
                                        ; max size for Char SP read/write command

PrtBuf:     .res    256                 ; printer output buffer
PrtPtr:     .byte   0                   ; printer buffer pointer
tempy:      .byte   0                   ; temp Y storage
temprq:     .byte   0                   ; temp ReqCnt MSB storage

LastOP:     .res    $09, $FF            ; Last operation for D_REPEAT calls
SIR_Addr:   .word    SIR_Tbl
SIR_Tbl:    .res    $05, $00
SIR_Len     =        *-SIR_Tbl
MaxUnits:   .byte    $09                    ; The maximum number of units

.ifndef charonly
DCB_Idx:    .byte    $00                    ; DCB 0's blocks
            .byte    DIB1_Blks-DIB0_Blks    ; DCB 1's blocks
            .byte    DIB2_Blks-DIB0_Blks    ; DCB 2's blocks
            .byte    DIB3_Blks-DIB0_Blks    ; DCB 3's blocks
.endif

CardIsOK:   .byte   $00                 ; Have we found an intelligent disk controller yet?

NumSPDevs:  .byte   $00                 ; Number of found SP devices

SPUnitMap:  .byte   $01                 ; block dev0 (mapping not used, assume they are always the first 4)
            .byte   $02                 ; block dev1 (mapping not used, assume they are always the first 4)
            .byte   $03                 ; block dev2 (mapping not used, assume they are always the first 4)
            .byte   $04                 ; block dev3 (mapping not used, assume they are always the first 4)
            .byte   $07                 ; Network device unit#
PrtUnit:    .byte   $08                 ; Printer device unit#
            .byte   $05                 ; CPM device unit#
            .byte   $09                 ; Modem device unit#
            .byte   $06                 ; FN_Clock device unit#

BytesReadL: .byte   $00                 ; temp storage for X & Y returned from SP call
BytesReadH: .byte   $00

TmpScrH:    .res    $10, $00            ; Storage for screenholes, slot0 & current slot
TmpZero:    .res    $30, $00            ; Storage to save $20-$4F zero page locations

Signature:  .byte   $FF, $20, $FF, $00  ; Disk card signature for SP
            .byte   $FF, $03, $FF, $00

; Smartport Status Call Parameter List
StatParam:  .byte   $03               ; #params
            .byte   $00               ; Unit
            .word   Buffer            ; Status list pointer for Result
StatCode:   .byte   $00               ; Status Code
StatNetSub: .byte   $00               ; Network device Sub Unit

; Smartport Control Call Parameter List
CtrlParam:  .byte   $03               ; #params
            .byte   $00               ; Unit
            .word   Buffer            ; Ctrl list pointer
CtrlCode:   .byte   $00               ; Control Code
CtrlNetSub: .byte   $00               ; Network device Sub Unit

; Smartport ReadBlock/WriteBlock Call Parameter List
RWParam:    .byte   $03               ; #params
            .byte   $00               ; Unit
            .word   Buffer            ; Data buffer pointer
SPBlk:      .byte   $00,$00,$00       ; Block Number (low byte,mid byte,high byte)

; Smartport Open/Close Call Parameter List
OpClParam:  .byte   $01               ; #params
            .byte   $00               ; Unit
OpClNetSub: .byte   $00               ; Network device Sub Unit


; Smartport Read/Write Call Parameter List
CRWParam:   .byte   $04               ; #params
            .byte   $00               ; Unit
            .word   Buffer            ; Data buffer pointer
SPReqCnt:   .word   $0000             ; Request count
CRWNetSub:  .byte   $00,$00,$00       ; Address pointer (low byte,mid byte,high byte)
                                      ; used as Network device Sub Unit
            

StatLen:    .byte   0                 ; Length for status call data

; Smartport Printer buffer Write Call Parameter List
PWParam:    .byte   $04               ; #params
            .byte   $00               ; Unit
            .word   PrtBuf            ; Printer buffer pointer
PSPReqCnt:  .word   $0000             ; Request count
            .byte   $00,$00,$00       ; Address pointer (low byte,mid byte,high byte)



; Hack for relocatable code supporting only 16bit values
StatParAdr:  .word StatParam
CtrlParAdr:  .word CtrlParam
RWParamAdr:  .word RWParam
OpClParAdr:  .word OpClParam
CRWParamAdr: .word CRWParam
PWParamAdr:  .word PWParam

;
; .RS232 Device control parameters
;  and .CPM (assume only one used at one time for now)
;

CNTL_PARAM:   .byte  15                 ; List length

BAUD:         .byte  00                 ; BAUD RATE
DFORMAT:      .byte  00                 ; Data format
                                        ; CTL - Hi nybble
                                        ; CMD - Lo nybble
CRDELAY:      .byte  00                 ; Carriage return delay
LFDELAY:      .byte  00                 ; Line feed delay
FFDELAY:      .byte  00                 ; Form feed delay
PROTOCOL:     .byte  00                 ; 00 - none
                                        ; 80 - XON/XOFF
                                        ; 40 - ENQ/ACK
CTLCHR1:      .byte  00                 ; Character to use as XOFF (or ENQ)
CTLCHR2:      .byte  00                 ; Character to use as XON (or ACK)
MAXBUF:       .byte  00                 ; Buffer level which triggers XOFF
                                        ;        (or RTS false)
MINBUF:       .byte  00                 ; Buffer level which triggers XON
                                        ;        (or RTS true)
CHARCNT:      .byte  00                 ; Character count for ENQ/ACK
HDW_HSHAKE:   .byte  00                 ; Hardware handshake
                                        ;        Bit 7 (1=enabled)
RD_IMMEDIATE: .byte  00                 ; Bit 7 (1=read immediate mode)
STAT_REG:     .byte  00                 ; Status reg - saved from last interrupt
STAT_LATCH:   .byte  00                 ; Latched status bits - cleared by reset
                                        ;       or status request-1
                                        ; Bit 0 (1=parity error)
                                        ; Bit 1 (1=framing error)
                                        ; Bit 2 (1=overrun)
                                        ; Bit 5 (1=DCD went false)
                                        ; Bit 6 (1=DSR went false)
                                        ; Bit 7 (1=input character lost)

CNTL_LN     =       *-CNTL_PARAM


;------------------------------------
;
; Driver request handlers
;
;------------------------------------

Entry:      jsr     Dispatch            ; Call the dispatcher
            ldx     SOS_Unit            ; Get drive number for this unit
            lda     ReqCode             ; Keep request around for D_REPEAT
            sta     LastOP,x            ; Keep track of last operation
            rts

;
; The Dispatcher.  Note that if we came in on a D_INIT call,
; we do a branch to Dispatch normally.
; Dispatch is called as a subroutine!
;
DoTable:    .word   DRead-1             ; 0 Read request
            .word   DWrite-1            ; 1 Write request
            .word   DStatus-1           ; 2 Status request
            .word   DControl-1          ; 3 Control request
            .word   BadReq-1            ; 4 Unused
            .word   BadReq-1            ; 5 Unused
            .word   DOpen-1             ; 6 Open - valid for character devices
            .word   DClose-1            ; 7 Close - valid for character devices
            .word   DInit-1             ; 8 Init request
            .word   DRepeat-1           ; 9 Repeat last read or write request
Dispatch:   SWITCH  ReqCode,9,DoTable   ; Serve the request

;
; Dispatch errors
;
BadReq:     lda     #XREQCODE           ; Bad request code!
            jsr     SysErr              ; Return to SOS with error in A
BadOp:      lda     #XBADOP             ; Invalid operation!
            jsr     SysErr              ; Return to SOS with error in A


;
; D_REPEAT - repeat the last D_READ or D_WRITE call
;
DRepeat:    ldx     SOS_Unit
            lda     LastOP,x            ; Recall the last thing we did
            cmp     #$02                ; Looking for operation < 2
            bcs     BadOp               ; Can only repeat a read or write
            sta     ReqCode
            jmp     Dispatch


;
; D_INIT call processing - called once each for all volumes.
; on first entry we search the slots from 4 to 1 for a smartport device
; and use the first one we find
;
DInit:      lda     CardIsOK            ; Check if we have checked for a card already
            beq     CheckSig            ; No, lets check
            jmp     FoundCard           ; Yes, skip signature check

CheckSig:   jsr     GoSlow              ; set 1MHz, Grappler plus does not like 2M
            lda     DIB4_Slot           ; Check configured slot for autoscan
            bpl     FixedSlot           ; No, use configured DIB1 slot
            lda     #ScanStart          ; else load starting scan slot
FixedSlot:  ora     #$C0                ; Form a $Cs00 address, where s = slot #
            sta     Pointer+1
            lda     #$00
            sta     Pointer
            sta     Pointer+ExtPG       ; zero out the xbyte

                                        ; We check all 4 sp sig bytes, starting from last
CheckNext:  ldy     #$05                ; <-- hack, just check the 3 sig bytes, then CFFA will work in slot1-4
@1:         lda     (Pointer),y         ;     this assumes the card detected can do smartport even though it
            cmp     Signature,Y         ;     does not indicate it
            beq     @m1
            jmp     NoMatch             ; No device if bytes don't match
@m1:        dey
            dey
            bpl     @1

                                        ; We found a SmartPort controller
            ldy     #$ff                ; $CxFF - check last byte for entry point
            lda     (Pointer),y
            sta     PDEntry+1           ; Set card prodos driver entry low byte
            clc
            adc     #3                  ; add 3 to find Smartport entry point
            sta     SPEntry+1           ; Set card smartport driver entry low byte
            lda     Pointer+1
            sta     PDEntry+2           ; Set card prodos driver entry high byte
            sta     SPEntry+2           ; Set card smartport driver entry high byte
            and     #$07
.ifndef charonly
            sta     DIB0_Slot           ; Set found slot for all DIBs
            sta     DIB1_Slot
            sta     DIB2_Slot
            sta     DIB3_Slot
.endif
            sta     DIB4_Slot
            sta     DIB5_Slot
            sta     DIB6_Slot
            sta     DIB7_Slot
            sta     DIB8_Slot
            ora     #$10                ; SIR = $10+slot#
            sta     SIR_Tbl
            sta     CardIsOK            ; Remember that we found a card
            lda     #SIR_Len
            ldx     SIR_Addr
            ldy     SIR_Addr+1

.ifndef charonly    ; don't claim the SIR for charonly so we don't double up with the soshdboot driver
            jsr     AllocSIR            ; Claim SIR
            bcc     @ok2
            jmp     NoDevice
.endif
                                        ; Lets check number of devices connected
                                        ; From P8 1.7 source:
                                        ;    the Smartportinterface does not set up
                                        ;    its device list until it receives a
                                        ;    Prodos Status call ; therefore, we have
                                        ;    made two calls : first a prodos status call
                                        ;    then a smartport status call
@ok2:       lda     DIB4_Slot           ; From Prodos style drive/slot
            asl                         ; 7 6 5 4 3 2 1 0
            asl                         ; D S S S 0 0 0 0
            asl
            asl
            sta     TmpZero+$23         ; Set slot for block driver call
            jsr     ProDriver           ; Do block driver status call
                                        ; Now setup for smartport status call
            lda     #SP_Status          ; Status cmd=0
            sta     CmdNum              ; Command for SP call
            sta     StatParam+1         ; Set unit 00 in Param list
            Sta     StatParam+4         ; Set status call type - 00
            lda     StatParAdr          ; Set Status parameter list pointer
            sta     CmdList
            lda     StatParAdr+1
            Sta     CmdList+1
            jsr     SmartPort           ; call smartport
            bcc     @11
            jmp     NoDevice
@11:
            lda     Buffer              ; First byte returned is the num devices
            beq     NoDevice            ; if zero, no devices attached
            sta     NumSPDevs
            jmp     FoundCard

; Todo, just hard mapped for now
;;; scan for the char dev names and map them to the SOS unit numbers
;;            lda     #3
;;            sta     StatParam+4         ; Set status call type - 03 (get DIB)
;;
;;NextDev:    inc     StatParam+1         ; next unit
;;            lda     StatParam+1
;;            cmp     NumSPDevs
;;            beq     @1
;;            bcs     FoundCard           ; all done
;;@1:
;;            jsr     SmartPort
;;            bcs     NoDevice
;;
;;            ldx     #0
;;NextChar:   lda     Buffer+5,x          ; start of name in dib
;;            cmp     DIB4_Name+1,x       ; is it NETWORK?
;;            bne     NextName
;;            inx
;;            cpx     #7
;;            bne     NextChar
;;            lda     StatParam+1         ; yes, save the unit#
;;            sta     SPUnitMap
;;            bne     NextDev             ; check next device (bra)
;;
;;NextName:   cmp     DIB5_Name+1,x       ; is it PRINTER?
;;            bne     NextName2
;;            inx
;;            cpx     #7
;;            bne     NextChar
;;            lda     StatParam+1         ; yes, save the unit#
;;            sta     SPUnitMap+1
;;            bne     NextDev             ; check next device (bra)
;;
;;NextName2:  cmp     DIB6_Name+1,x       ; is it CPM?
;;            bne     NextName3
;;            inx
;;            cpx     #3
;;            bne     NextChar
;;            lda     StatParam+1         ; yes, save the unit#
;;            sta     SPUnitMap+2
;;            bne     NextDev             ; check next device (bra)
;;
;;NextName3:  cmp     DIB7_DCB+1,x        ; is it MODEM? (RS232)
;;            bne     NextDev
;;            inx
;;            cpx     #5
;;            bne     NextChar
;;            lda     StatParam+1         ; yes, save the unit#
;;            sta     SPUnitMap+3
;;            bne     NextDev             ; check next device (bra)

NoMatch:    dec     Pointer+1           ; Try next slot
            lda     Pointer+1
            and     #$07
            beq     NoDevice            ; we did not find one :-(
            jmp     CheckNext           ; Check next slot

FoundCard:  jsr     CkUnit              ; Checks for unit below unit max
            jsr     CkCharDev           ; Check dev type
            bcs     notblkdev

.ifndef charonly
            jsr     UpdVolSize          ; Its ok, lets Get volume size and update DIB
.endif

notblkdev:  lda     SOS_Unit
            cmp     #8                  ; if its init for clock, trigger update time
			beq     SetupEvent
            clc
            rts

NoDevice:   lda     #XDNFERR            ; Device not found
            jsr     SysErr              ; Return to SOS with error in A

;
; Setup SOS time from Fujinet clock dev
;  We cannot do SOS calls from in here, se we set up an event
;  to trigger the update code
;

; queue event param list
EventPar:   .byte   $ff                 ; event priority (ff=highest)
            .byte   $10                 ; id byte
            .word   UpdateTime          ; event handler address
xbyte:      .byte   $00                 ; xbyte (this drivers bank)

ParamAdr:   .word   EventPar            ; to work around relocation

; setup event in sos event queue
; this will get triggered after sos has setup all of the drivers
SetupEvent: lda     BReg                ; setup xbyte for this bank
            sta     xbyte
            ldx     ParamAdr            ; load the Event param list address
            ldy     ParamAdr+1
            jsr     QueEvent            ; call sos and setup event
            clc                         ; no error, and return from init call
            rts
;
; event handler
;
UpdateTime: brk                         ; get .FN_CLOCK device number
            .byte   GET_DEV_NUM
            .word   GDNParam
            beq     @ok
            rts                         ; just return if we get an error

@ok:        lda     devnum
            sta     Sdevnum

            brk                         ; get time from Fujinet
            .byte   D_STATUS
            .word   DSTParam
            beq     @ok2
            rts                         ; same here

@ok2:       brk                         ; do SOS set_time call
            .byte   SET_TIME
            .word   STParam
            rts

; get device number param list
GDNParam:   .byte   2
            .word   ClkName
devnum:     .byte   0

ClkName:    .byte   9
            .byte   ".FN_CLOCK"

; device status param list
DSTParam:   .byte   3
Sdevnum:    .byte   0
            .byte   'S'                 ; Fujinet get SOS time
            .word   Buffer

; set time param list
STParam:    .byte   1
            .word   Buffer


;
; D_READ call processing
;
DRead:
            lda     CardIsOK            ; Did we previously find a card?
            bne     DReadGo
            jmp     NoDevice            ; If not... then bail
DReadGo:
            jsr     CkUnit              ; Checks for unit below unit max
            lda     #$00                ; Zero # bytes read
            sta     Count               ; Local count of bytes read
            sta     Count+1
            tay
            sta     (QtyRead),Y         ; Userland count of bytes read
            iny
            sta     (QtyRead),Y         ; Msb of userland bytes read

            jsr     CkCharDev           ; is it a char dev?
            bcs     CharRead            ; yes, do char write

.ifndef charonly
            jsr     CkCnt               ; Checks for validity, aborts if not
            lda     Num_Blks            ; Check for block count greater than zero
            beq     ReadExit
            jsr     FixUp               ; Correct for addressing anomalies

            lda     #SP_ReadBlock       ; Setup SP command code
            sta     CmdNum
            lda     RWParamAdr          ; Set Read/Write parameter list pointer
            sta     CmdList
            lda     RWParamAdr+1
            Sta     CmdList+1

            ldx     SOS_Unit            ; Set unit in Param list
            inx                         ; smartport units start from 1
            stx     RWParam+1
            lda     SosBlk              ; copy over blocknum
            sta     SPBlk
            lda     SosBlk+1
            sta     SPBlk+1

ReadOne:    jsr     SmartPort           ; card smartport firmware interface
            bcs     IO_Error

            ldy     SosBuf              ; setup pointer for second 256 bytes
            sty     SosBuf2             ;  quicker to do in one loop
            ldy     SosBuf+1
            iny
            sty     SosBuf2+1
            ldy     SosBuf+ExtPG
            sty     SosBuf2+ExtPG
            ldy     #0

@10:        lda     Buffer,Y            ; copy first 256 bytes from Buffer to SosBuf
            sta     (SosBuf),Y          ;
            lda     Buffer+256,Y        ; copy second 256 bytes from Buffer to SosBuf
            sta     (SosBuf2),Y         ;
            dey
            bne     @10

            inc     SPBlk
            bne     SkipReadMSBIncrement
            inc     SPBlk+1
SkipReadMSBIncrement:
            jsr     BumpSosBuf          ;   ...512 bytes in, and check the pointer
            dec     Num_Blks
            bne     ReadOne
            ldy     #0
            lda     Count               ; Local count of bytes read
            sta     (QtyRead),Y         ; Update # of bytes actually read
            lda     Count+1
            iny
            sta     (QtyRead),Y
            clc
ReadExit:   rts                         ; Exit read routines
.endif

IO_Error:   lda     #XIOERROR           ; I/O error
            jsr     SysErr              ; Return to SOS with error in A

; character device reads
CharRead:   jsr     CkChrCnt            ; Check request count is valid
            lda     #SP_Read            ; Setup SP command code
            sta     CmdNum
            lda     CRWParamAdr         ; Set Char Read/Write parameter list pointer
            sta     CmdList
            lda     CRWParamAdr+1
            Sta     CmdList+1
            ldx     SOS_Unit            ; Set unit in Param list
            lda     SPUnitMap,x
            sta     CRWParam+1

            lda     ReqCnt              ; copy over the request count
            sta     SPReqCnt
            lda     ReqCnt+1
            sta     SPReqCnt+1

ReadAgain:  jsr     SmartPort           ; go do the SP read
            bcs     IO_Error

            lda     #SOS_Unit           ; special case for .rs232 unit and .cpm
            cmp     #7
            beq     ChkImd
            cmp     #6
            bne     ReadCopy
ChkImd:     lda     BytesReadH
            bne     ReadCopy
            lda     BytesReadL
            bne     ReadCopy
            bit     RD_IMMEDIATE        ; is read immediate mode set?
            bpl     ReadAgain           ; no, try again until we get something

ReadCopy:   jsr     CpBuf_SosB          ; copy read data from our buffer to Sos Buffer

            ldy     #0                  ; Update the bytes actually read
            lda     BytesReadL
            sta     (QtyRead),Y
            lda     BytesReadH
            iny
            sta     (QtyRead),Y

            clc
            rts                            ; Exit char read


;
; D_WRITE call processing
;
DWrite:     lda     CardIsOK            ; Did we previously find a card?
            bne     DWriteGo
            jmp     NoDevice            ; If not... then bail

DWriteGo:
            jsr     CkUnit              ; Checks for unit below unit max

            jsr     CkCharDev           ; is it a char dev?
            bcs     CharWrite           ; yes, do char write

.ifndef charonly
            jsr     CkCnt               ; Checks for validity, aborts if not
            lda     Num_Blks            ; Check for block count greater than zero
            beq     WriteExit
            jsr     FixUp               ; Correct for addressing anomalies

            lda     #SP_WriteBlock      ; Setup SP command code
            sta     CmdNum
            lda     RWParamAdr          ; Set Read/Write parameter list pointer
            sta     CmdList
            lda     RWParamAdr+1
            Sta     CmdList+1

            ldx     SOS_Unit            ; Set unit in Param list
            inx                         ; smartport units start from 1
            stx     RWParam+1
            lda     SosBlk
            sta     SPBlk
            lda     SosBlk+1
            sta     SPBlk+1
WriteOne:
            ldy     SosBuf              ; setup pointer for second 256 bytes
            sty     SosBuf2             ;  quicker to do in one loop
            ldy     SosBuf+1
            iny
            sty     SosBuf2+1
            ldy     SosBuf+ExtPG
            sty     SosBuf2+ExtPG
            ldy     #0

@10:        lda     (SosBuf),Y          ; copy first 256 bytes from SosBuf to Buffer
            sta     Buffer,Y            ;
            lda     (SosBuf2),Y         ; copy second 256 bytes from SosBuf to Buffer
            sta     Buffer+256,Y        ;
            dey
            bne     @10

            jsr     SmartPort
            bcc     @ok1
            jmp     IO_Error

@ok1:       inc     SPBlk               ; Bump the block number
            bne     SkipWriteMSBIncrement
            inc     SPBlk+1
SkipWriteMSBIncrement:
            jsr     BumpSosBuf          ;   ...512 bytes in, and check the pointer
            dec     Num_Blks
            bne     WriteOne

WriteExit:  clc
            rts
.endif


; character device writes
CharWrite:  ldx     SOS_Unit            ; Get SP unit
            lda     SPUnitMap,x
            cmp     PrtUnit             ; check if we are writing to the printer
            bne     DW1                 ; no, continue
;            beq     DW1             ;uncomment this to disable the printer buffering

            ldy     ReqCnt+1
            sty     temprq

            ldy     #0                  ; yes, buffer it
NxtChar:    ldx     PrtPtr              ; current print buffer pointer
            lda     (SosBuf),Y          ; copy char to print buffer
            sta     PrtBuf,X

            cpx     #$fe                ; is print buffer full?
            bne     NotFull

            sty     tempy               ; yes, save y, then output the contents of the buffer
            inx
            jsr     PrtBufW             ; write the buffer to SP
            inc     PrtPtr              ; reset the buffer pointer (inc to $ff, next inc below incs to 0)
            ldy     tempy               ; restore y

NotFull:    inc     PrtPtr
            iny
            lda     temprq
            bne     Prtgt256            ; > 256 bytes
            cpy     ReqCnt              ; print request < 256 bytes
            bne     NxtChar
            clc
            rts

Prtgt256:   cpy     #0                  ; check for more than 256 chars processed
            bne     NxtChar
            dec     temprq              ; yes, dec msb, and inc sosbuf msb
            inc     SosBuf+1
            jmp     NxtChar


DW1:        lda     #SP_Write           ; Setup SP command code
            sta     CmdNum
            lda     CRWParamAdr         ; Set Char Read/Write parameter list pointer
            sta     CmdList
            lda     CRWParamAdr+1
            sta     CmdList+1
            ldx     SOS_Unit            ; Set unit in Param list
            lda     SPUnitMap,x
            sta     CRWParam+1

            lda     ReqCnt              ; copy over the byte count
            sta     SPReqCnt
            lda     ReqCnt+1
            sta     SPReqCnt+1

; copy over the write data from the SOS buf to ours
;  todo - need to optimise this better
            lda     ReqCnt+1
            beq     w100                ; <$100
            cmp     #1
            beq     w200                ; <$200

            ldy     #0                  ; <$300
w301:       lda     (SosBuf),Y
            sta     Buffer,Y
            iny
            bne     w301
            inc     SosBuf+1
w302:       lda     (SosBuf),Y
            sta     Buffer+256,Y
            iny
            bne     w302
            inc     SosBuf+1

            lda     #0                  ; break up and do $200 bytes as one SP write Packet
            sta     SPReqCnt
            jsr     SmartPort           ; go do the SP write
            bcc     wrok
            jmp     IO_Error

wrok:       ldy     #0                  ; then send the remaining bytes as another SP write packet
w303:       lda     (SosBuf),Y
            sta     Buffer,Y
            iny
            cpy     ReqCnt
            bcc     w303
            dec     SosBuf+1            ; restore sosbuf
            dec     SosBuf+1
            lda     ReqCnt
            sta     SPReqCnt
            lda     #0
            sta     SPReqCnt+1

            jmp     wcopied

w200:       ldy     #0                  ; <$200
w201:       lda     (SosBuf),Y
            sta     Buffer,Y
            iny
            bne     w201
            inc     SosBuf+1
w202:       lda     (SosBuf),Y
            sta     Buffer+256,Y
            iny
            cpy     ReqCnt
            bcc     w202
            dec     SosBuf+1
            bcs     wcopied

w100:       ldy     #0                  ; <$100
w101:       lda     (SosBuf),Y
            sta     Buffer,Y
            iny
            cpy     ReqCnt
            bne     w101


wcopied:    jsr     SmartPort           ; go do the SP write
            bcc     ExitW
            jmp     IO_Error

ExitW:      clc
            rts                         ; Exit char write


;
; D_STATUS call processing
;  $00 = Driver Status
;  $FE = Return preferred bitmap location ($FFFF)
;
; Smartport status bute
;  7 0=character device 1=block device
;  6 1=write allowed
;  5 1=read allowed
;  4 1=device online or disk in drive
;  3 0=format allowed
;  2 1=medium write protected
;  1 1=device currently interupting
;  0 1=device currently open(char devs only)
;

; SOS status byte
;  7 0=device not busy 1=device busy
;  6 not used
;  5 not used
;  4 not used
;  3 not used
;  2 not used
;  1 0=device not write-protected 1=device write-protected
;  0 not used


DStatus:    lda     CardIsOK            ; Did we previously find a card?
            bne     DStatusGo
            jmp     NoDevice            ; If not... then bail

DStatusGo:
            jsr     CkUnit              ; Checks for unit below unit max

            lda     CtlStat             ; Which status code to run?
            bne     @s10                ; Stat 0, we'll just return
            clc
            rts

@s10:       lda     SOS_Unit            ; check unit#
            cmp     #7                  ; .rs232?
            beq     ChkStat
            cmp     #6                  ; .cpm?
            bne     statcont

ChkStat:    lda     CtlStat             ; now check the status code
            cmp     #3                  ; stat code 3?
            beq     RSStat03
            cmp     #1
            bne     statcont

; These are borrowed directly from the RS232 driver
; rs232 status code 1 -> get device parameter block
            ldy     #0                  ;1 -- Retrieve device control
                                        ;  parameters (including RD_IMMEDIATE
                                        ;     STAT_REG and STAT_LATCH)
            lda     (CSList),Y
            cmp     CNTL_PARAM          ;Check for room in status list
            bcs     @s01                ;  >= OK

            lda     #XCTLPARAM          ;  < NG
            jsr     SysErr

@s01:       ldy     #CNTL_LN-1

@s05:       lda     CNTL_PARAM,Y
            sta     (CSList),Y
            dey
            bpl     @s05
            rts

; rs232 status code 3 -> get driver buffer info
; we'll attempt to get the bytes waiting from Fujinet
RSStat03:   lda     #SP_Status          ; Setup SP command code
            sta     CmdNum
            lda     StatParAdr          ; Set parameter list pointer
            sta     CmdList
            lda     StatParAdr+1
            Sta     CmdList+1

            ldx     SOS_Unit            ; Set unit in Param list
            lda     SPUnitMap,x
            sta     StatParam+1

            lda     #'S'                ; bytes waiting
            Sta     StatCode

            jsr     SmartPort           ; go do the SP call
            bcc     @ok2
            jmp     NoDevice

@ok2:       ldy     #0
            lda     #$FF                ; Output buffer size
            jsr     CntOut
            lda     #0                  ; Number of chars in output buffer
            jsr     CntOut
            lda     #$FF                ; Input buffer size
            jsr     CntOut
            lda     Buffer+1            ; >256 chars waiting?
            beq     @less256            ; no, report what we have
            lda     #$ff                ; else, send max bytes waiting
            bne     @grtt256            ; bra always
@less256:   lda     Buffer              ; Number of chars in input buffer
@grtt256:   jsr     CntOut
            rts

CntOut:     sta     (CSList),Y
            iny
            lda     #0                  ; high byte (0)
            sta     (CSList),Y
            iny
            rts

; send the rest through to Fujinet
statcont:   lda     #SP_Status          ; Setup SP command code
            sta     CmdNum
            lda     StatParAdr          ; Set parameter list pointer
            sta     CmdList
            lda     StatParAdr+1
            Sta     CmdList+1

            ldx     SOS_Unit            ; Set unit in Param list
            lda     SPUnitMap,x
            sta     StatParam+1

            lda     CtlStat             ; copy status command code
            Sta     StatCode

            jsr     SmartPort           ; go do the SP call
            bcc     @ok
            jmp     NoDevice
@ok:

; copy status data from our buffer to Sos Buffer
            lda     BytesReadH          ; use actual bytes read from SP
            beq     s100                ; <$100
            cmp     #1
            beq     s200                ; <$200

            ldy     #0                  ; <$300
s301:       lda     Buffer,Y
            sta     (CSList),Y
            iny
            bne     s301
s302:       lda     Buffer+256,Y
            sta     (CSList),Y
            iny
            bne     s302
s303:       lda     Buffer+512,Y
            sta     (CSList),Y
            iny
            cpy     BytesReadL
            bcc     s303
            bcs     StatDone

s200:       ldy     #0                  ; <$200
s201:       lda     Buffer,Y
            sta     (CSList),Y
            iny
            bne     s201
s202:       lda     Buffer+256,Y
            sta     (CSList),Y
            iny
            cpy     BytesReadL
            bcc     s202
            bcs     StatDone

s100:       lda     BytesReadL          ; check for zero bytes to copy
            beq     StatDone

            ldy     #0                  ; <$100
s101:       lda     Buffer,Y
            sta     (CSList),Y
            iny
            cpy     BytesReadL
            bcc     s101

StatDone:   clc
            rts

DSWhat:     lda     #XCTLCODE           ; Control/status code no good
            jsr     SysErr              ; Return to SOS with error in A


;
; D_CONTROL call processing
;  Just pass these through to smartport interface

DControl:   lda     CardIsOK            ; Did we previously find a card?
            bne     DControlGo
            jmp     NoDevice            ; If not... then bail

DControlGo:
            ;we just drop the set newline for now (ctl #2)
            lda     CtlStat             ; check control command code
            cmp     #2                  ; if its a set newline
            bne     @no
            rts                         ; we'll just ignore and return for now

@no:        lda     CtlStat
            cmp     #1                  ; ctl code 1?
            bne     @no2                ; pass through the rest
            lda     SOS_Unit            ; now lets check which unit its for
            cmp     #7                  ; .rs232 unit#?
            beq     @Ctl1
            cmp     #6                  ; .cpm unit#?
            bne     @no2

; Load device control parameters for .RS232 & .CPM devices
;  copied from real .rs232 driver to mirror the same behaviour
;
@Ctl1:      ldy     #0                  ; 1 -- Load device control parameters
;  except STAT_REG and STAT_LATCH
            lda     (CSList),Y
            cmp     CNTL_PARAM          ;Check length of control list
            beq     @c11                ;  = OK

            lda     #XCTLPARAM          ;   NG
            jsr     SysErr

@c11:       ldy     #CNTL_LN-3
@c15:       lda     (CSList),Y
            sta     CNTL_PARAM,Y
            dey
            bpl     @c15
            rts


@no2:       lda     #SP_Control         ; Setup SP command code
            sta     CmdNum
            lda     CtrlParAdr          ; Set parameter list pointer
            sta     CmdList
            lda     CtrlParAdr+1
            sta     CmdList+1

            ldx     SOS_Unit            ; Set unit in Param list
            lda     SPUnitMap,x
            sta     CtrlParam+1

            lda     CtlStat             ; copy control command code
            sta     CtrlCode            ; into param list

                                        ; copy the control list
            ldy     #0                  ; get the length of the control list
            lda     (CSList),y          ; SOS only uses one byte for length, SP uses two
            sta     Buffer              ; we'll just support 256 bytes for now
            lda     #0
            sta     Buffer+1

            ldx     Buffer              ; get length lsb
            beq     @Done               ; all done if len = 0
            iny                         ; else lets copy it over
@L1:        lda     (CSList),y
            sta     Buffer+1,y
            iny
            bne     @skip
            inc     CSList+1
@skip:      dex
            bne     @L1

@Done:      jsr     SmartPort           ; go do the SP call
            bcs     Ctrl_Err
            rts

Ctrl_Err:   lda     #XIOERROR           ; I/O error
            jsr     SysErr              ; Return to SOS with error in A


;
; D_OPEN call processing
;
DOpen:      lda     CardIsOK            ; Did we previously find a card?
            bne     DOpenGo
            jmp     NoDevice            ; If not... then bail

DOpenGo:    jsr     CkCharDev           ; Open only for char dev
            bcs     DOCont
            jmp     BadOp

DOCont:     lda     #SP_Open            ; Setup SP command code
            sta     CmdNum
            lda     OpClParAdr          ; Set parameter list pointer
            sta     CmdList
            lda     OpClParAdr+1
            sta     CmdList+1

            ldx     SOS_Unit            ; Set unit in Param list
            lda     SPUnitMap,x
            sta     OpClParam+1

            cmp     PrtUnit             ; check if we are opening the printer
            bne     DO1
            lda     #0                  ; yes, reset printer buffer pointer
            sta     PrtPtr

DO1:        jsr     SmartPort
            bcs     Open_Err
            rts

Open_Err:   jsr     SysErr              ; we'll just run with the returned SP error code for now


;
; D_CLOSE call processing
;
DClose:     lda     CardIsOK            ; Did we previously find a card?
            bne     DCloseGo
            jmp     NoDevice            ; If not... then bail

DCloseGo:   jsr     CkCharDev           ; Close only for char dev
            bcs     DCCont
            jmp     BadOp

DCCont:     ldx     SOS_Unit            ; Get SP unit
            lda     SPUnitMap,x
            cmp     PrtUnit             ; check if we are closing the printer
            bne     DC1

            ldx     PrtPtr              ; yes, check printer output buffer
            beq     DC1                 ; nothing in the output buffer, cont
            jsr     PrtBufW             ; else output the contents of the buffer

DC1:        lda     #SP_Close           ; Setup SP command code
            sta     CmdNum
            lda     OpClParAdr          ; Set parameter list pointer
            sta     CmdList
            lda     OpClParAdr+1
            sta     CmdList+1

            ldx     SOS_Unit            ; Set unit in Param list
            lda     SPUnitMap,x
            sta     OpClParam+1

;            sta     OpClParam+2  ;;;debug come out later

;            lda     EReg         ;;;debug come out later
;            sta     OpClParam+3  ;;;debug come out later

            jsr     SmartPort
            bcs     Close_Err
            rts

Close_Err:  jsr     SysErr              ; we'll just run with the returned SP error code for now



;------------------------------------
;
; Utility routines
;
;------------------------------------

;
; Write the printer buffer to FujiNet
;  input x = chars to print
;
PrtBufW:    stx     PSPReqCnt
            lda     #0
            sta     PSPReqCnt+1

            lda     #SP_Write           ; Setup SP command code
            sta     CmdNum
            lda     PWParamAdr          ; Set printer Write parameter list pointer
            sta     CmdList
            lda     PWParamAdr+1
            sta     CmdList+1
            ldx     SOS_Unit            ; Set unit in Param list
            lda     SPUnitMap,x
            sta     PWParam+1

            jsr     SmartPort           ; go do the SP write
            bcc     ExitPB
            jmp     IO_Error

ExitPB:     rts                         ; Exit print buffer write

;
; copy the Buffer data to the SOS buffer
;  BytesReadL & BytesReadH hold the count to be copied
;  todo - need to optimise this better, ok for now
;
CpBuf_SosB:
            lda     BytesReadH          ; use actual bytes read
            beq     r100                ; <$100
            cmp     #1
            beq     r200                ; <$200

            ldy     #0                  ; <$300
r301:       lda     Buffer,Y
            sta     (SosBuf),Y
            iny
            bne     r301
r302:       lda     Buffer+256,Y
            sta     (SosBuf),Y
            iny
            bne     r302
r303:       lda     Buffer+512,Y
            sta     (SosBuf),Y
            iny
            cpy     BytesReadL
            bcc     r303
            bcs     copied

r200:       ldy     #0                  ; <$200
r201:       lda     Buffer,Y
            sta     (SosBuf),Y
            iny
            bne     r201
r202:       lda     Buffer+256,Y
            sta     (SosBuf),Y
            iny
            cpy     BytesReadL
            bcc     r202
            bcs     copied

r100:       lda     BytesReadL          ; check for zero bytes to copy
            beq     copied

            ldy     #0                  ; <$100
r101:       lda     Buffer,Y
            sta     (SosBuf),Y
            iny
            cpy     BytesReadL
            bcc     r101

copied:     rts

.ifndef charonly
;
; Check ReqCnt for block devices to ensure it is a multiple of 512.
;
CkCnt:      lda     ReqCnt              ; Look at the lsb of bytes requested
            bne     BadCnt              ; No good!  lsb should be 00
            lda     ReqCnt+1            ; Look at the msb
            lsr     A                   ; Put bottom bit into carry, 0 into top
            sta     Num_Blks            ; Convert bytes to number of blks to xfer
            bcc     CvtBlk              ; Carry is set from LSR to mark error.
.endif

BadCnt:     lda     #XBYTECNT
            jsr     SysErr              ; Return to SOS with error in A

;
; Check ReqCnt for char devices, 767 max for SP bus.
;
CkChrCnt:   lda     ReqCnt+1            ; check msb for <3
            cmp     #3
            bcs     BadCnt              ; no good, error
            rts                         ; else, all good

.ifndef charonly
;
; Test for valid block number; abort on error
;
CvtBlk:
            ldx     SOS_Unit
            ldy     DCB_Idx,x
            sec
            lda     DIB0_Blks+1,y       ; Blocks on unit msb
            sbc     SosBlk+1            ; User requested block number msb
            bvs     BlkErr              ; Not enough blocks on device for request
            beq     @1                  ; Equal msb; check lsb.
            rts                         ; Greater msb; we're ok.
@1:         lda     DIB0_Blks,y         ; Blocks on unit lsb
            sbc     SosBlk              ; User requested block number lsb
            bvs     BlkErr              ; Not enough blocks on device for request
            rts                         ; Equal or greater msb; we're ok.
BlkErr:     lda     #XBLKNUM
            jsr     SysErr              ; Return to SOS with error in A

BumpSosBuf: inc     SosBuf+1            ; Increment SosBuf MSB
            inc     SosBuf+1
            ; fallthrough to FixUp

;
; Fix up the buffer pointer to correct for addressing
; anomalies.  We just need to do the initial checking
; for two cases:
; 00xx bank N -> 80xx bank N-1
; 20xx bank 8F if N was 0
; FDxx bank N -> 7Dxx bank N+1
; If pointer is adjusted, return with carry set
;
FixUp:      lda     SosBuf+1            ; Look at msb
            beq     @1                  ; That's one!
            cmp     #$FD                ; Is it the other one?
            bcs     @2                  ; Yep. fix it!
            rts                         ; Pointer unchanged, return carry clear.
@1:         lda     #$80                ; 00xx -> 80xx
            sta     SosBuf+1
            dec     SosBuf+ExtPG        ; Bank N -> band N-1
            lda     SosBuf+ExtPG        ; See if it was bank 0
            cmp     #$7F                ; (80) before the DEC.
            bne     @3                  ; Nope! all fixed.
            lda     #$20                ; If it was, change both
            sta     SosBuf+1            ; Msb of address and
            lda     #$8F
            sta     SosBuf+ExtPG        ; Bank number for bank 8F
            rts                         ; Return carry set
@2:         and     #$7F                ; Strip off high bit
            sta     SosBuf+1            ; FDxx ->7Dxx
            inc     SosBuf+ExtPG        ; Bank N -> bank N+1
@3:         rts                         ; Return carry set
.endif

CkUnit:     lda     SOS_Unit            ; Checks for unit below unit max
            cmp     MaxUnits
            bmi     UnitOk
NoUnit:     lda     XBADDNUM            ; Report no unit to SOS
            jsr     SysErr
UnitOk:     clc
            rts

;
; Check  we are using a char dev (unit > 3)
;  carry set = char dev
;  carry clr = block dev
;
CkCharDev:  lda     SOS_Unit
            cmp     #4
            rts

;
; Throttle back to 1 MHz
;
GoSlow:     pha
            php
            lda     EReg
            ora     #$80
            sta     EReg
            plp
            pla
            rts

;
; Throttle up to 2 MHz
;
GoFast:     pha
            php
            lda     EReg
            and     #$7F
            sta     EReg
            plp
            pla
            rts

.ifndef charonly
;
; Get Volume size for Unit and update DIB
;
UpdVolSize: lda     #SP_Status          ; set command for SP call
            sta     CmdNum
            lda     StatParAdr          ; Set Status parameter list pointer
            sta     CmdList
            lda     StatParAdr+1
            Sta     CmdList+1

            ldx     SOS_Unit            ; Set unit in Param list
            inx
            stx     StatParam+1         ; smartport units start from 1
            lda     #$00                ; Set status call type - 00
            Sta     StatParam+4

            jsr     SmartPort           ; call smartport
            bcs     @error

            lda     Buffer              ; First byte returned is the unit status
            bpl     @error              ; bit7=0 - is a char device, error
                                        ; only check that for now
            lda     Buffer+1            ; Next is the low byte of size in blocks
            ldx     SOS_Unit
            ldy     DCB_Idx,x
            sta     DIB0_Blks,y
            lda     Buffer+2            ; and then the mid byte of size
            sta     DIB0_Blks+1,y
            rts

@error:     jmp     NoDevice

.endif

;
; jsr to card prodos driver
; We update the address based on the slot and firmware CxFF byte
;
ProDriver:
            sei                         ; disable interrupts while we have the zeropage changed
            lda     #$03                ; Set Zeropage to 3 to disable A3 Enhanced Indirect addressing
            sta     ZPReg
            jsr     SaveMem             ; save and swap in card screen hole & zeropage
            jsr     GoSlow              ; down to 1MHz to ensure compatibility
PDEntry:    jsr     $0000               ; Update this address
            jsr     GoFast              ; back to 2MHz again
            jsr     RestMem             ; save and swap out card screen hole & zeropage
            ldx     #$18                ; Restore SOS Zero page (keep error in A)
            stx     ZPReg
            rts

; jsr to card SmartPort driver
; We update the address based on the slot and firmware CxFF byte + 3
SmartPort:
            sei                         ; disable interrupts while we have the zeropage changed
            lda     #$03                ; Set Zeropage to 3 to disable A3 Enhanced Indirect addressing
            sta     ZPReg
            jsr     SaveMem             ; save and swap in card screen hole & zeropage
            jsr     GoSlow              ; down to 1MHz to ensure compatibility
SPEntry:    jsr     $0000               ; Update this address
CmdNum:     .byte   $00
CmdList:    .word   $0000
            stx     BytesReadL          ; save byte count returned from SP
            sty     BytesReadH
            jsr     GoFast              ; back to 2MHz again
            jsr     RestMem             ; save and swap out card screen hole & zeropage
            ldx     #$18                ; Restore SOS Zero page (keep error in A)
            stx     ZPReg
            rts
;
; Save current screenholes and restore peripheral card values (current slot + slot0)
; Save zeropage $20-$4F & xbyte for pointer
;
SaveMem:    lda     E_IER
            and     #$18                ; See if either CB1 or CB2 interrupts are enabled
            beq     SkipVbl             ; No, they are not, skip the wait for vbl
                                        ; now wait for 2xVBL before we muck with the screenholes
            lda     #$18                ; Clear CB2 flag - VBL
            sta     E_IFR
VWait:      bit     E_IFR               ; Wait for vertical retrace
            beq     VWait
                                        ; wait for another one to ensure font data is loaded
            lda     #$18                ; Clear CB2 flag - VBL
            sta     E_IFR
VWait2:     bit     E_IFR               ; Wait for vertical retrace
            beq     VWait2

SkipVbl:    bit     CWrtOff             ; Then disable font loading

            jsr     SwapZero            ; Swap in the driver zeropage values
            jsr     SwapScrH            ; Swap in the screenhole data with ours
            rts

;
; Save Card screen holes and restore original values
; Restore zeropage $20-$4F & xbyte
;
RestMem:
            pha
            php                         ; save error indication in carry
            jsr     SwapScrH            ; Swap back the original screenhole data
            jsr     SwapZero            ; Swap back the original zeropage values
            plp                         ; restore error indication in carry
            pla
            rts
;
; Swap screenholes with driver values
; we do Slot 0 and current slot values
; This nice code is from Peter Ferrie
;
SwapScrH:
            lda     #$07                ; Init ZP $D0-$D1, X and A
            sta     ScreenBase+1
            lda     #$00
            sta     ScreenBase
            ldx     #$0F
            lda     #$F8
@loop:
            tay                         ; A holds screen page index at loop entry
            lda     (ScreenBase),Y
            pha                         ; Save current screen hole byte on stack.
            lda     TmpScrH,X
            sta     (ScreenBase),Y      ; Restore screen hole byte from array
            pla
            sta     TmpScrH,X           ; Copy saved screen hole byte into array.
            dex
            bmi     done                ; Exit when array is full (all 16 bytes are copied, X<0).
            txa                         ; TXA/LSR tests whether array index is odd or even
            lsr                         ; and sets carry accordingly (1 = odd).
            tya                         ; Bring screen index into A for manipulation
            eor     DIB4_Slot           ; Cycle page index between $x8 and $x8+n as long as N in 1..7
            bcc     @loop               ; Take branch every other loop, using array index odd/even
                                        ;  (carry still valid from TXA/LSR)
            eor     #$80                ; Cycle page index between $F8 and $78
            bpl     @loop               ; If flipping from $78->$F8 (now negative), continue
            dec     ScreenBase+1        ; Go to next page counting down ($07->$06, etc.)
            bne     @loop               ; Always -- equiv. to BRA or JMP.  Never reaches 0.
done:       rts

;
; Swap zeropage with driver values
; save all of $20 - $4F
;
SwapZero:   ldx     #$2f
@loop2:
            lda     $20,X
            pha                         ; Save current zeropage byte on stack.
            lda     TmpZero,X
            sta     $20,X               ; Restore zeropage byte from temp storage
            pla
            sta     TmpZero,X           ; Copy saved zeropage byte into temp storage.
            dex
            bpl     @loop2              ; Exit when temp storage is full
            rts


            .ENDPROC
            .END