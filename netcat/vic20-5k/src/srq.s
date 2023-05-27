;;; petsciiterm SRQ handler
	
	.export _ih
	.export _ihsetup
	.export _ihdone
	.import _trip

VIA2IFR 	= $912D
VIA2IER 	= $912E
CB1_ENABLE 	= $10
IRQH		= $0314	
	
_ihsetup:
	SEI			; disable interrupts
	LDA #<_ih		; low byte of ih()
	STA IRQH		; put in IRQ vector lo
	LDA #>_ih		; hi byte of ih()
	STA IRQH+1		; put in IRQ vector hi

	LDA VIA2IER		; Get IER
	ORA CB1_ENABLE		; Set CB1 Enable
	STA VIA2IER
	CLI			; enable interrupts
	RTS			; Bye.
	
_ih:	LDA VIA2IER		; Get IER
	ORA CB1_ENABLE		; Enable CB1
	STA VIA2IER		; Get it.
	LDA VIA2IFR		; Check CIA1 interrupt status register
	AND #CB1_ENABLE		; Did the SRQ pin get wiggled?
	BEQ _ihd		; no, jump to _ihd

	STA _trip		; yes, set _trip to $10

_ihd:	JMP $EABF		; jump to original ROM IRQ

_ihdone:
	SEI			; disable interrupts

	LDA #$BF		; lo byte of original KERNAL IRQ handler
	STA IRQH		; put in IRQ vector lo
	LDA #$EA		; ...
	STA IRQH+1		; ...

	CLI			; enable interrupts
	RTS			; Bye.
