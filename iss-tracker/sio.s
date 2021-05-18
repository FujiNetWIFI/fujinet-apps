	;; Call SIO

	.export _siov

_siov:	JSR $E459		; Call SIOV
	LDA $0303		; get/return DVSTAT
	RTS			; Bye
	
