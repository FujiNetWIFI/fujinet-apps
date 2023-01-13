	;; Call SIO

	.export _siov
	.export _rtclr
	.export _cold_start
	
_siov:	JSR $E459
	RTS
	
_rtclr:	LDA #$00
	STA $12
	STA $13
	STA $14
	RTS
	
_cold_start:
	JMP $E477
