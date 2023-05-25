	.export _ih
	.export _ihsetup
	.import _trip

_ihsetup:
	SEI			; disable interrupts
	LDA #<_ih		; low byte of ih()
	STA $0314		; put in IRQ vector lo
	LDA #>_ih		; hi byte of ih()
	STA $0315		; put in IRQ vector hi
	CLI			; enable interrupts
	RTS			; Bye.
	
_ih:	LDA $DC0D		; Check CIA1 interrupt status register
	AND #$10		; Did the SRQ pin get wiggled?
	BEQ _ihd		; no, jump to _ihd
	STA _trip		; yes, set _trip to $10
_ihd:	JMP $EA31		; jump to original ROM IRQ handler

