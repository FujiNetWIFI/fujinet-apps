; relocate 6502 code
;
; By Norman Davie
; 
; Within your code you need the following tables
; in this order
;
;   RELOCATE_CODE_START	
;     <code>
;   RELOCATE_CODE_END
;     <data>
;     reloc000 .WORD <address>
;   RELOCATE_DATA_END
;
;RELOCATION_TABLE:
;			.WORD	reloc000
; icl "commn routines with relocation code table"
;
;	.WORD 	0 ; end of table
;			.DS 	255
;END_RELOCATION_TABLE:

; SUBROUTINES IN THIS FILE
;
;	RELOCATE_TO_MEMLO
;	RELOCATE_TO_TGT_ADDR
;	MOVE_TO_TARGET
;	ADJUST_MEMLO

; Pointers used by relocator.


RELOCATE_TABLE		=	$A2
RELOCATE_TABLE_LO	=	$A2
RELOCATE_TABLE_HI	= 	$A3

SRC_ADDR		=	$A4
SRC_ADDR_LO		=	$A4
SRC_ADDR_HI		=	$A5

TGT_ADDR		=	$A6
TGT_ADDR_LO		=	$A6
TGT_ADDR_HI		=	$A7

MOD_ADDR		=	$A8
MOD_ADDR_LO		=	$A8
MOD_ADDR_HI		=	$A9

INSTRUCT_SIZE		=	$AA




;====================================
; RELOCATE_TO_MEMLO
;   Move the code and data to MEMLO
; RELOCATE_TO_TGT_ADDRESS
;   Move the code and data to the address
;   stored in TGT_ADDRESS
;
; REGISTERS AFFECTED
;   ALL
;====================================
RELOCATE_TO_MEMLO:

	LDA MEMLO
	STA TGT_ADDR_LO
	LDA MEMLO+1
	STA TGT_ADDR_HI
	
RELOCATE_TO_TGT_ADDR:
	
; START ADDRESS of the CODE block

	LDA #<RELOCATE_CODE_START
	STA SRC_ADDR_LO
	LDA #>RELOCATE_CODE_START
	STA SRC_ADDR_HI

; Size of just the code block

	LDA #<(RELOCATE_CODE_END-RELOCATE_CODE_START)
	STA SRC_SIZE_LO
	STA CODE_SIZE_LO
	LDA #>(RELOCATE_CODE_END-RELOCATE_CODE_START)
	STA SRC_SIZE_HI
	STA CODE_SIZE_HI
			
; Figure out how much we need to adjust
; all addresses by	

	LDA SRC_ADDR_LO
	SEC
	SBC TGT_ADDR_LO
	STA DIFF_LO
	
	LDA SRC_ADDR_HI
	SBC TGT_ADDR_HI
	STA DIFF_HI
	
	LDA #<RELOCATION_TABLE
	STA RELOCATE_TABLE_LO
	LDA #>RELOCATION_TABLE
	STA RELOCATE_TABLE_HI

; The user may have manually added entries
; to the relocation block, so find the end
; of the entries

NEXT_RELOC:
	LDY #$00		
	LDA (RELOCATE_TABLE),Y
	INY
	ORA (RELOCATE_TABLE),Y
	BEQ CHECK_INSTRUCTION
	CLC
	LDA RELOCATE_TABLE_LO
	ADC #$02
	STA RELOCATE_TABLE_LO
	LDA RELOCATE_TABLE_HI
	ADC #$00
	STA RELOCATE_TABLE_HI

	CLC
	BCC NEXT_RELOC
	
CHECK_INSTRUCTION:
	
	LDY #$00
	LDA (SRC_ADDR),Y	; Get the instruction
	TAX
	LDA INSTRUCTION_SIZE,X	; find out how many bytes the instruction takes
	STA INSTRUCT_SIZE	; Keep this size so we can move to the next instruction
	BNE CHECK_ABSOLUTE
	JMP ILLEGAL_INSTRUCTION ; can't relocate

CHECK_ABSOLUTE:	
	CMP #$03		; If it's 3 bytes long, then we have an absolute address
	BEQ ABSOLUTE_INSTRUCT	; and we need to see if the address is within our block
	
	BNE MOVE_TO_NEXT_INSTRUCTION	

ABSOLUTE_INSTRUCT:
	
; Store the address into our table even if we're
; we may not need it.

	LDY #$00
	LDA SRC_ADDR_LO	
	STA (RELOCATE_TABLE), Y
	INY
	LDA SRC_ADDR_HI	
	STA (RELOCATE_TABLE), Y 

; We're actually pointing at the instruction
; increase the address by one

	LDY #$00
	CLC				; move the the address 
	LDA (RELOCATE_TABLE),Y		; in the instruction
	ADC #$01
	STA (RELOCATE_TABLE),Y
	INY
	LDA (RELOCATE_TABLE),Y
	ADC #$00
	STA (RELOCATE_TABLE),Y	

; How we have the address location
; we need to grab the address at that location

	LDY #$00		
	LDA (RELOCATE_TABLE),Y		; The lo address in the table
	STA MOD_ADDR_LO

	INY
	LDA (RELOCATE_TABLE),Y		; the hi address in the table
	STA MOD_ADDR_HI

; Treat the number in the addresses as unsigned	

; IF ADDRESS_HI > END_OF_BLOCK THEN MOVE_TO_NEXT_INSTRUCTION

	LDY #$01
	LDA (MOD_ADDR),Y		; get the high byte of the address
	CMP #>RELOCATE_DATA_END		; is the address before our block?
	BEQ LAST_BYTES			; is the high byte equal to our block?  Check if too high
	BCS MOVE_TO_NEXT_INSTRUCTION	; We're too high, skip it.

; IF ADDRESS_HI < STARTING_OF_BLOCK THEN MOVE_TO_NEXT_INSTRUCTION

	CMP #>RELOCATE_CODE_START	
	BCC MOVE_TO_NEXT_INSTRUCTION	; is the address less than our starint address?

	BCS STORE_ADDRESS		; we're within range, so just store address

LAST_BYTES:

; We're on the same page as the final block, so we need to check the low portion of address

	LDY #$00
	LDA (MOD_ADDR),Y		; checking the low byte
	CMP #<RELOCATE_DATA_END		; is the address after our block
	BCS MOVE_TO_NEXT_INSTRUCTION 	

; we're between our SOURCE range, so we want to store
; the address of the instruction in the table

STORE_ADDRESS:
	LDY #$00
	LDA MOD_ADDR_LO			; store the address of
	STA (RELOCATE_TABLE),Y		; the address
	INY
	LDA MOD_ADDR_HI
	STA (RELOCATE_TABLE),Y

; move to the next entry in the table

	CLC
	LDA RELOCATE_TABLE_LO	
	ADC #$02
	STA RELOCATE_TABLE_LO
	LDA RELOCATE_TABLE_HI
	ADC #$00
	STA RELOCATE_TABLE_HI
				
MOVE_TO_NEXT_INSTRUCTION:

; move to the next instruction
; this updates SRC_ADDR by the
; instruction size

	CLC			
	LDA SRC_ADDR_LO		 
	ADC INSTRUCT_SIZE
	STA SRC_ADDR_LO
	LDA SRC_ADDR_HI
	ADC #$00
	STA SRC_ADDR_HI
	
; substract the instruction size from
; our size counter.  If we've reached
; zero, we are done

	SEC
	LDA SRC_SIZE_LO
	SBC INSTRUCT_SIZE
	STA SRC_SIZE_LO
	LDA SRC_SIZE_HI
	SBC #$00
	STA SRC_SIZE_HI

; if both SRC_SIZE and SRC_SIZE+1 are zero
; we're done
	
	LDA SRC_SIZE_LO
	ORA SRC_SIZE_HI
	BEQ TERMINATE_TABLE
	JMP CHECK_INSTRUCTION
	
; TERMINATE THE TABLE
; by adding a NULL to the end of the table

TERMINATE_TABLE:
	LDA #$00
	TAY
	STA (RELOCATE_TABLE),Y
	INY
	STA (RELOCATE_TABLE),Y

;
; At this point we have a completed relocation table!
; So much better than doing it by hand!!
;
	
; BEGIN AT THE START OF RELOCATION TABLE AGAIN

	LDA #<RELOCATION_TABLE
	STA RELOCATE_TABLE_LO
	LDA #>RELOCATION_TABLE
	STA RELOCATE_TABLE_HI
	
; now we are at the first entry in the table

UPDATE_ADDRESSES:	
	
; the table contains all the addresses we need to
; update the source addresses.	

; read the address we're to modify
	LDY #$00
		
	LDA (RELOCATE_TABLE),Y		; The lo address in the table
	STA MOD_ADDR_LO

	INY
	LDA (RELOCATE_TABLE),Y		; the hi address in the table
	STA MOD_ADDR_HI

	LDA MOD_ADDR_LO
	ORA MOD_ADDR_HI
	BEQ TABLE_EXHAUSTED		; if the entry is NULL, we're done

; load the address and subtract the difference
; save the new address back
	
	DEY 				; y back to zero

	LDA (MOD_ADDR),Y; 
	SEC
	SBC DIFF_LO
	STA (MOD_ADDR),Y
		
	INY
	LDA (MOD_ADDR),Y
	SBC DIFF_HI
	STA (MOD_ADDR),Y

; move to the next entry in the table

	CLC
	LDA RELOCATE_TABLE_LO	
	ADC #$02
	STA RELOCATE_TABLE_LO
	LDA RELOCATE_TABLE_HI
	ADC #$00
	STA RELOCATE_TABLE_HI

; JMP always		
	CLC
	BCC UPDATE_ADDRESSES		
			
TABLE_EXHAUSTED:

; Now we can move the entire block of
; modified code and data to the appropriate
; memory location

	LDA #<RELOCATE_CODE_START
	STA SRC_ADDR_LO
	LDA #>RELOCATE_CODE_START
	STA SRC_ADDR_HI
	
	LDA #<(RELOCATE_DATA_END-RELOCATE_CODE_START)	; THIS SIZE INCLUDES THE DATA
	STA SRC_SIZE_LO
	LDA #>(RELOCATE_DATA_END-RELOCATE_CODE_START)
	STA SRC_SIZE_HI
	
	JSR MOVE_TO_TARGET

ILLEGAL_INSTRUCTION:
	RTS

;====================================
; MOVE_TO_TARGET
;   Generic routine for moving memory
; SRC_ADDR - address to take data from
; TGT_ADDR - where you want to put data
; SRC_SIZE - size in bytes of the block
;
; REGISTERS AFFECTED
;   ALL
;====================================
;
; Generic routine for moving memory
; The size of the block is in
; COPY_SIZE
; The Source is in SRC_ADDR
; The Target is in TGT_ADDR
	
MOVE_TO_TARGET:	
 
	LDY #0
	LDX SRC_SIZE_HI
	BEQ MOVE2
MOVE1	LDA (SRC_ADDR),Y ; move a page at a time
	STA (TGT_ADDR),Y
	INY
	BNE MOVE1
	INC SRC_ADDR+1
	INC TGT_ADDR+1
	DEX
	BNE MOVE1
MOVE2	LDX SRC_SIZE_LO
	BEQ MOVE_COMPLETED
MOVE3	LDA (SRC_ADDR),Y ; move the remaining bytes
	STA (TGT_ADDR),Y
	INY
	DEX
	BNE MOVE3

MOVE_COMPLETED
	RTS

;====================================
; ADJUST_MEMLO
;   Using the size in the relocation 
; table, adjust MEMLO accordingly
; NOTE:  this routine briefly disables
;        interrupts
;
; REGISTERS AFFECTED
;   Accumulator
;====================================

;
; 	ADJUST MEMLO TO PROTECT US
;	
ADJUST_MEMLO:

	LDA #<(RELOCATE_DATA_END-RELOCATE_CODE_START)	; THIS SIZE INCLUDES THE DATA
	STA SRC_SIZE_LO
	LDA #>(RELOCATE_DATA_END-RELOCATE_CODE_START)
	STA SRC_SIZE_HI
	
; Make sure another process
; doesn't modify our 
; MEMLO, while we are modifying it
	
	SEI
	CLC
	LDA MEMLO
	ADC SRC_SIZE_LO
	STA MEMLO
	LDA MEMLO+1
	ADC SRC_SIZE_HI
	STA MEMLO+1
	CLI
	
	RTS
	
		
; Each entry is points to the instruction 
; (not the address) we need to relocate

INSTRUCTION_SIZE	.BYTE $02 ; $00 BRK
			.BYTE $02 ; $01 ORA X,IND	
			.BYTE 0,0,0
			.BYTE $02 ; $05 ORA ZPG
			.BYTE $02 ; $06 ASL ZPG
			.BYTE $00
			.BYTE $01 ; $08 PHP
			.BYTE $02 ; $09 ORA #
			.BYTE $01 ; $0A ASL
			.BYTE 0,0
			.BYTE $03 ; $0D ORA ABS 
			.BYTE $03 ; $0E ASL ABS
			.BYTE $00

			.BYTE $02 ; $10 BPL
			.BYTE $02 ; $11 ORA IND,Y	
			.BYTE 0,0,0
			.BYTE $02 ; $15 ORA ZPG,X
			.BYTE $02 ; $16 AND ZPG
			.BYTE $00
			.BYTE $01 ; $18 CLC
			.BYTE $03 ; $19 ORA ABS,Y
			.BYTE 0  ; 
			.BYTE 0,0
			.BYTE $03 ; $1D ORA ABS,X
			.BYTE $03 ; $1E ASL ABS,X
			.BYTE $00

			.BYTE $03 ; $20 JSR ABS
			.BYTE $02 ; $21 AND X,IND	
			.BYTE 0,0
			.BYTE $02 ; $24 BIT ZPG
			.BYTE $02 ; $25 AND ZPG
			.BYTE $02 ; $26 ROL ZPG
			.BYTE $00
			.BYTE $01 ; $28 PLP
			.BYTE $02 ; $29 AND #
			.BYTE $01 ; $2A ROL 
			.BYTE 0
			.BYTE $03 ; $2C BIT ABS
			.BYTE $03 ; $2D ORA ABS
			.BYTE $03 ; $2E ROL ABS
			.BYTE $00

			.BYTE $02 ; $30 BMI REL
			.BYTE $02 ; $31 AND IND,Y	
			.BYTE 0,0,0
			.BYTE $02 ; $35 AND ZPG,X
			.BYTE $02 ; $36 ROL ZPG,X
			.BYTE $00
			.BYTE $01 ; $38 SEC
			.BYTE $03 ; $39 AND ABS,Y
			.BYTE 0  ; 
			.BYTE 0,0
			.BYTE $03 ; $3D AND ABS,X
			.BYTE $03 ; $3E ROL ABS,X
			.BYTE $00	
			
			.BYTE $01 ; $40 RTI 
			.BYTE $02 ; $41 EOR X,IND	
			.BYTE 0,0,0
			.BYTE $02 ; $45 EOR ZPG
			.BYTE $02 ; $46 LSR ZPG
			.BYTE $00
			.BYTE $01 ; $48 PHA
			.BYTE $02 ; $49 EOR #
			.BYTE $01 ; $4A LSR 
			.BYTE 0
			.BYTE $03 ; $4C JMP ABS
			.BYTE $03 ; $4D EOR ABS
			.BYTE $03 ; $4E LSR ABS
			.BYTE $00

			.BYTE $02 ; $50 BVC REL
			.BYTE $02 ; $51 EOR IND,Y	
			.BYTE 0,0,0
			.BYTE $02 ; $55 EOR ZPG,X
			.BYTE $02 ; $56 LSR ZPG,X
			.BYTE $00
			.BYTE $01 ; $58 CLI
			.BYTE $03 ; $59 EOR ABS,Y
			.BYTE 0  ; 
			.BYTE 0,0
			.BYTE $03 ; $5D EOR ABS,X
			.BYTE $03 ; $5E LSR ABS,X
			.BYTE $00
			
			.BYTE $01 ; $60 RTS 
			.BYTE $02 ; $61 ADC X,IND	
			.BYTE 0,0,0
			.BYTE $02 ; $65 ADC ZPG
			.BYTE $02 ; $66 ROR ZPG
			.BYTE $00
			.BYTE $01 ; $68 PLA
			.BYTE $02 ; $69 ADC #
			.BYTE $01 ; $6A ROR 
			.BYTE 0
			.BYTE $03 ; $6C JMP IND
			.BYTE $03 ; $6D ADC ABS
			.BYTE $03 ; $6E ROR ABS
			.BYTE $00

			.BYTE $02 ; $70 BVS REL
			.BYTE $02 ; $71 ADC IND,Y	
			.BYTE 0,0,0
			.BYTE $02 ; $75 ADC ZPG,X
			.BYTE $02 ; $76 ROR ZPG,X
			.BYTE $00
			.BYTE $01 ; $78 SEI
			.BYTE $03 ; $79 ADC ABS,Y
			.BYTE 0  ; 
			.BYTE 0,0
			.BYTE $03 ; $7D ADC ABS,X
			.BYTE $03 ; $7E ROR ABS,X
			.BYTE $00

			.BYTE 0 
			.BYTE $02 ; $81 STA X,IND	
			.BYTE 0,0
			.BYTE $02 ; $84 STY ZPG
			.BYTE $02 ; $85 STA ZPG
			.BYTE $02 ; $86 STX ZPG
			.BYTE $00
			.BYTE $01 ; $88 DEY
			.BYTE 9  
			.BYTE $01 ; $8A TXA 
			.BYTE 0
			.BYTE $03 ; $8C STY ABS
			.BYTE $03 ; $8D STA ABS
			.BYTE $03 ; $8E STX ABS
			.BYTE $00

			.BYTE $02 ; $90 BCC REL
			.BYTE $02 ; $91 STA IND,Y	
			.BYTE 0,0
			.BYTE $02 ; $94 STY ZPG,X
			.BYTE $02 ; $95 STA ZPG,X
			.BYTE $02 ; $96 STX ZPG,X
			.BYTE $00
			.BYTE $01 ; $98 TYA
			.BYTE $03 ; $99 STA ABS,Y
			.BYTE $01 ; $9A TXS
			.BYTE 0,0
			.BYTE $03 ; $9D STA ABS,X
			.BYTE 0
			.BYTE $00

			.BYTE $02 ; $A0 LDY #
			.BYTE $02 ; $A1 LDA X, IND	
			.BYTE $02 ; $A2 LDX #
			.BYTE 0
			.BYTE $02 ; $A4 LDY ZPG
			.BYTE $02 ; $A5 LDA ZPG
			.BYTE $02 ; $A6 LDX ZPG
			.BYTE $00
			.BYTE $01 ; $A8 TAY
			.BYTE $02 ; $A9 LDA #
			.BYTE $01 ; $AA TAX
			.BYTE 0
			.BYTE $03 ; $AC LDY ABS 
			.BYTE $03 ; $AD LDA ABS
			.BYTE $03 ; $AE LDX ABS
			.BYTE $00
			
			.BYTE $02 ; $B0 BCS REL
			.BYTE $02 ; $B1 LDA IND,Y	
			.BYTE 0,0
			.BYTE $02 ; $B4 LDY ZPG,X
			.BYTE $02 ; $B5 LDA ZPG,X
			.BYTE $02 ; $B6 LDX ZPG,Y
			.BYTE $00
			.BYTE $01 ; $B8 CLV
			.BYTE $03 ; $B9 LDA ABS,Y
			.BYTE $01 ; $BA TSX
			.BYTE 0
			.BYTE $03 ; $BC LDY ABS,X 
			.BYTE $03 ; $BD LDA ABS,X
			.BYTE $03 ; $BE LDX ABS,Y
			.BYTE $00

			.BYTE $02 ; $C0 CPY #
			.BYTE $02 ; $C1 CMP X,IND	
			.BYTE 0,0
			.BYTE $02 ; $C4 CPY ZPG
			.BYTE $02 ; $C5 CMP ZPG
			.BYTE $02 ; $C6 DEC ZPG
			.BYTE $00
			.BYTE $01 ; $C8 INY
			.BYTE $02 ; $C9 CMP #
			.BYTE $01 ; $CA DEX
			.BYTE 0
			.BYTE $03 ; $CC CPY ABS 
			.BYTE $03 ; $CD CMP ABS
			.BYTE $03 ; $CE DEC ABS
			.BYTE $00

			.BYTE $02 ; $D0 BNE REL
			.BYTE $02 ; $D1 CMP IND,Y	
			.BYTE 0,0,0
			.BYTE $02 ; $D5 CMP ZPG,X
			.BYTE $02 ; $D6 DEC ZPG,X
			.BYTE $00
			.BYTE $01 ; $D8 CLD
			.BYTE $03 ; $D9 CMP ABS,Y
			.BYTE 0,0,0
			.BYTE $03 ; $DD CMP ABS,X
			.BYTE $03 ; $DE DEC ABS,X
			.BYTE $00																								

			.BYTE $02 ; $E0 CPX #
			.BYTE $02 ; $E1 SBC X,IND	
			.BYTE 0,0
			.BYTE $02 ; $E4 CPX ZPG
			.BYTE $02 ; $E5 SBC ZPG
			.BYTE $02 ; $E6 INC ZPG
			.BYTE $00
			.BYTE $01 ; $E8 INX
			.BYTE $02 ; $E9 SBC #
			.BYTE $01 ; $EA NOP
			.BYTE 0
			.BYTE $03 ; $EC CPX ABS 
			.BYTE $03 ; $ED SBC ABS
			.BYTE $03 ; $EE INC ABS
			.BYTE $00
																								
			.BYTE $02 ; $F0 BEQ REL
			.BYTE $02 ; $F1 SBC IND,Y	
			.BYTE 0,0,0
			.BYTE $02 ; $F5 SBC ZPG,X
			.BYTE $02 ; $F6 INC ZPG,X
			.BYTE $00
			.BYTE $01 ; $F8 SED
			.BYTE $02 ; $F9 SBC #
			.BYTE 0,0,0
			.BYTE $03 ; $FD SBC ABS,X
			.BYTE $03 ; $FE INC ABS,X
			.BYTE $00

																							
; RC_SIZE
SRC_SIZE:
SRC_SIZE_LO:		.BYTE $00
SRC_SIZE_HI:		.BYTE $00

;SRC_ADDR_END_LO:	.BYTE $00
;SRC_ADDR_END_HI:	.BYTE $00

;SRC_DATA_END_LO:	.BYTE $00
;SRC_DATA_END_HI:	.BYTE $00

CODE_SIZE_LO		.BYTE $00
CODE_SIZE_HI		.BYTE $00

DIFF:
DIFF_LO:		.BYTE $00
DIFF_HI:		.BYTE $00


