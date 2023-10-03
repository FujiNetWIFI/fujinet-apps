	;; Call SIO

	.export _rtclr
	.export _cold_start
	
_rtclr:	LDA #$00
	STA $12
	STA $13
	STA $14
	RTS
	
_cold_start:
	JMP $E477
