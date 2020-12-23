; Driver protection
;
; Make generic by Norman Davie
; Adapted from code by Thomas Cherryhomes
;

; Pointers used by relocator.

RELOCATE_TABLE		=	$A2
RELOCATE_TABLE_LO	=	$A2
RELOCATE_TABLE_HI	= 	$A3

SRC_ADDR		=	$A4
SRC_ADDR_LO		=	$A4
SRC_ADDR_HI		=	$A5

FROM:			=	$A6
FROM_LO:		=	$A6
FROM_HI:		=	$A7

TO:			=	$A8
TO_LO			=	$A8
TO_HI			=	$A9

MOD_ADDR		=	$AA
MOD_ADDR_LO		=	$AA
MOD_ADDR_HI		=	$AB

       ; INTERRUPT VECTORS
       ; AND OTHER PAGE 2 VARS

VPRCED  		=     	$0202   ; PROCEED VCTR
MEMLO   		=     	$02E7   ; MEM LO

SETVBV			= 	$E45C
XITVBV			=	$E462

       ; HARDWARE REGISTERS

PACTL   		= 	$D302	; PIA CTRL A

;DEBUG			= 	$AD

; A = Low byte of relocation table
; Y = high byte of relocation table


;;; RESET HANDLER ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;RESET:    
;    JSR    $FFFF        ; Modified for original DOSINI
;    LDA    #$FF        ; Driver end LO
;    STA     MEMLO
;    LDA    #$FF        ; Driver end HI
;    STA    MEMLO+1
;    JSR     INIT            ; call any inits.
;    JSR    CLALL
;    RTS    
;;; END RESET HANDLER ;;;;;;;;;;;;;;;;;;;;;;;;;;;;

RELOCATE_TO_MEMLO:

	STA RELOCATE_TABLE_LO
	STY RELOCATE_TABLE_HI


.ifdef DEBUG
	LDA #$00
	STA DEBUG
.endif
	LDA MEMLO
	STA TO_LO
	LDA MEMLO+1
	STA TO_HI

; First entry in relocation table
; is the SOURCE ADDRESS of the block

	LDY #$00
	
	LDA (RELOCATE_TABLE),Y
	STA SRC_ADDR_LO
	STA FROM_LO
	INY
	LDA (RELOCATE_TABLE),Y
	STA SRC_ADDR_HI
	STA FROM_HI

; Second entry in relocation table
; is the size of the block

	INY
	LDA (RELOCATE_TABLE),Y
	STA RELOCATE_SIZE_LO
	STA COPY_SIZE_LO
	INY
	LDA (RELOCATE_TABLE),Y
	STA RELOCATE_SIZE_HI
	STA COPY_SIZE_HI
	
.ifdef DEBUG
; It may appear this is unnecessary since we're
; dong it twice, but actually we need the data
; present in order to use these routines
; while we're moving

	LDA MEMLO
	STA TO_LO
	LDA MEMLO+1
	STA TO_HI

	TYA
	PHA	
	JSR MOVE_TO_TARGET
	PLA
	TAY
	
	LDA MEMLO
	STA TO_LO
	LDA MEMLO+1
	STA TO_HI
.endif
	
		
; Third entry is the number of relocation entries

	INY
	LDA (RELOCATE_TABLE),Y
	TAX
		
; Figure out how much we need to modify
; the addresses by	

	LDA SRC_ADDR_LO
	SEC
	SBC TO_LO
	STA DIFF_LO
	
	LDA SRC_ADDR_HI
	SBC TO_HI
	STA DIFF_HI
	
; For easier calculations of the more than
; 128 entries, we're going to reset the Y
; to zero and work from the start of the table entries.
; That way we just need to update the high
; order byte by one when we go back to zero

	INY
	STY START_OF_TABLE

	LDA RELOCATE_TABLE_LO
	CLC
	ADC START_OF_TABLE
	STA RELOCATE_TABLE_LO
	
	LDA RELOCATE_TABLE_HI
	ADC #$00
	STA RELOCATE_TABLE_HI
	
	LDY #$00


MODIFY_TABLE:

; add one byte to the address 
; (we were currently pointing at the instruction)

	CLC			; add one to the relocation table address
	LDA (RELOCATE_TABLE),Y
	ADC #$01
	STA (RELOCATE_TABLE),Y

	INY
	LDA (RELOCATE_TABLE),Y
	ADC #$00
	STA (RELOCATE_TABLE),Y

	DEY			; back to the original table address
	
; now the table is pointing at the address
	
MODIFY_CODE:	
	
; we've updated the table entry to point at the address 
; we are to modify, calculate the new destination address	


; read the address we're to modify

	LDA (RELOCATE_TABLE),Y		; The lo address in the table
	STA MOD_ADDR_LO

	INY
	LDA (RELOCATE_TABLE),Y		; the hi address in the table
	STA MOD_ADDR_HI

; Need the Y for later; save it	
	TYA
	PHA
	
.ifdef DEBUG	
		LDA DEBUG
		BEQ DEBUG_SKIP1
		
		TXA
		PHA
		TYA
		PHA
		JSR PRINT_HEX_NO_EOL
		LDA #<DEBUG_ARROW
		LDY #>DEBUG_ARROW
		JSR PRINT_STRING_NO_EOL

		PLA
		TAY
		
		LDA (RELOCATE_TABLE),Y		; the hi address in the table
		JSR PRINT_HEX_NO_EOL
		DEY 
		LDA (RELOCATE_TABLE),Y
		JSR PRINT_HEX_NO_EOL

		TYA
		PHA		
		LDA #<DEBUG_ARROW
		LDY #>DEBUG_ARROW
		JSR PRINT_STRING_NO_EOL
		PLA
		TAY
		
		PLA
		TAX
DEBUG_SKIP1
.endif
		
; load the address and subtract the difference
; save the new address back
	
	LDY #$00	
	LDA (MOD_ADDR),Y; 
	SEC
	SBC DIFF_LO
	STA (MOD_ADDR),Y
		
	INY
	LDA (MOD_ADDR),Y
	SBC DIFF_HI
	STA (MOD_ADDR),Y

.ifdef DEBUG
		LDA DEBUG
		BEQ DEBUG_SKIP2
		LDA (MOD_ADDR),Y
		JSR PRINT_HEX_NO_EOL
		DEY 
		LDA (MOD_ADDR),Y
		JSR PRINT_HEX
		INY
DEBUG_SKIP2		

;		JSR WAIT_FOR_RETURN
.endif

; get our y back
	PLA
	TAY

.ifdef DEBUG
	CPY #$B1
	BNE SKIP3
	LDA #$01
	STA DEBUG
SKIP3:
.endif
	DEX
	BEQ TABLE_EXHAUSTED

	INY	; next entry
	BNE MODIFY_TABLE		
;	We've wrapped back to zero
	LDY #$00
	INC RELOCATE_TABLE_HI	
	CLC
	BCC MODIFY_TABLE
		
TABLE_EXHAUSTED:

	LDA MEMLO
	STA TO_LO
	LDA MEMLO+1
	STA TO_HI
	
	JSR MOVE_TO_TARGET
	JSR ADJUST_MEMLO
	RTS
	
MOVE_TO_TARGET:	

	LDY #0
	LDX COPY_SIZE_HI
	BEQ MD2
MD1	LDA (FROM),Y ; move a page at a time
	STA (TO),Y
	INY
	BNE MD1
	INC FROM+1
	INC TO+1
	DEX
	BNE MD1
MD2	LDX COPY_SIZE_LO
	BEQ MOVE_COMPLETED
MD3	LDA (FROM),Y ; move the remaining bytes
	STA (TO),Y
	INY
	DEX
	BNE MD3

MOVE_COMPLETED
	RTS
	
ADJUST_MEMLO:

; 	ADJUST MEMLO TO PROTECT US
	
	SEI
	CLC
	LDA MEMLO
	ADC RELOCATE_SIZE_LO
	STA MEMLO
	LDA MEMLO+1
	ADC RELOCATE_SIZE_HI
	STA MEMLO+1
	CLI
	
	RTS
	
	
; Each entry is points to the instruction 
; (not the address) we need to relocate

	
RELOCATE_SIZE:
RELOCATE_SIZE_LO:	.BYTE $00
RELOCATE_SIZE_HI:	.BYTE $00

COPY_SIZE:
COPY_SIZE_LO:		.BYTE $00
COPY_SIZE_HI:		.BYTE $00

DIFF:
DIFF_LO:		.BYTE $00
DIFF_HI:		.BYTE $00

START_OF_TABLE		.BYTE $00
FIRST_TIME		.BYTE $01



DEBUG1:
DEBUG1_LO:		.BYTE $00
DEBUG1_HI		.BYTE $00

DEBUG2:
DEBUG2_LO:		.BYTE $00
DEBUG2_HI		.BYTE $00

DEBUG3:
DEBUG3_LO:		.BYTE $00
DEBUG3_HI:		.BYTE $00

Y:			.BYTE $00

DEBUG_ARROW:		.BYTE ' -> ',0		
