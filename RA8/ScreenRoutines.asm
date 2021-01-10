
GRAPHICS0_SCREEN_SIZE	=	960
.IFNDEF MOD_ADDR

MOD_ADDR		=	$AB
MOD_ADDR_LO		=	$AB
MOD_ADDR_HI		=	$AC

.ENDIF

;====================================
; TRANSMIT_IF_CHANGED_GRAPHICS0
;   Calculates a checksum
; and compares it will the prevously
; calculated checksum.  If they're the
; same, no transmission takes place
; RETURNS
;   Nothing
; REGISTERS AFFECTED
;   ALL
;====================================
TRANSMIT_IF_CHANGED_GRAPHICS0:

		; parameters for calc_chksum
		LDX #>GRAPHICS0_SCREEN_SIZE
		STX CHK_SIZE_HI
		LDX #<GRAPHICS0_SCREEN_SIZE
		STX CHK_SIZE_LO
							
		JSR CALC_CHKSUM			; Calculate a checksum

		CMP LAST_CHKSUM			; Is it the same as the last one?
		BEQ GRAPHICS0_SAME		; don't transmit

		STA LAST_CHKSUM			; Keep track of the new checksum
				
		LDA LDDEVIC			; Device
		STA DDEVIC

		LDA UNIT
		STA DUNIT

		LDA #'W'
		STA DCOMND

		LDA #DWRITE
		STA DSTATS

		LDA SAVMSC
		STA DBUFLO
		LDA SAVMSC+1
		STA DBUFHI

		LDA #<FUJI_TIMEOUT 
		STA DTIMLO


		LDA #<GRAPHICS0_SCREEN_SIZE
		STA DBYTLO
		STA DAUX1

		LDA #>GRAPHICS0_SCREEN_SIZE
		STA DBYTHI
		STA DAUX2
		JSR SIOV
			
GRAPHICS0_SAME:			
		RTS
			
			
;====================================
; CALC_CHKSUM
;   Performs an exclusive or starting
; on the region stored in the address
; MOD_ADDR with a length of X*Y
; 
; RETURNS
;   A = Checksum
; REGISTERS AFFECTED
;   ALL
;====================================			
CALC_CHKSUM:
		LDA SAVMSC
		STA MOD_ADDR_LO
		LDA SAVMSC+1
		STA MOD_ADDR_HI
		
		LDY #$00			; START
		TYA
	; FOR DEBUGGING REMOVE FIRST CHARACTER ON SCREEN
	; LDA (MOD_ADDR),Y
		STA (MOD_ADDR),Y		; clear the checksum
		LDX CHK_SIZE_HI
		BEQ CALC_NEXT_PAGE
SAME_PAGE_CRC:
		EOR (MOD_ADDR),Y		; Use Y to go through each	
		INY				; pages
		BNE SAME_PAGE_CRC
		
		INC MOD_ADDR_HI
		DEX
		BNE SAME_PAGE_CRC
		
CALC_NEXT_PAGE:			
		LDX CHK_SIZE_LO
		BEQ COMPLETE
REMAINING:	EOR (MOD_ADDR),Y		
		INY
		DEX				; Use X to determine now many 
		BNE REMAINING			; Pages we've gone through
COMPLETE:
		RTS



						