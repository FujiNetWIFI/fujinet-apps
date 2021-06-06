	.export _dli15b
        .import _rgb_table, _rgb_ctr
	

_dli15b:
	; Push registers onto stack
	pha
	txa
	pha
	tya
	pha

	ldy #0

dli15b_loop1:
	lda _rgb_table,y
	sta $D40A ; WSYNC
	sta $D01A ; COLBK

	lda _rgb_table+1,y
	sta $D016 ; COLPF0

	lda _rgb_table+2,y
	sta $D017 ; COLPF1

	lda _rgb_table+3,y
	sta $D018 ; COLPF2

	iny
	iny
	iny
	iny
	bne dli15b_loop1

dli15b_loop2:
	lda _rgb_table+256,y
	sta $D40A ; WSYNC
	sta $D01A ; COLBK

	lda _rgb_table+257,y
	sta $D016 ; COLPF0

	lda _rgb_table+258,y
	sta $D017 ; COLPF1

	lda _rgb_table+259,y
	sta $D018 ; COLPF2

	iny
	iny
	iny
	iny
	bne dli15b_loop2

dli15b_loop3:
	lda _rgb_table+512,y
	sta $D40A ; WSYNC
	sta $D01A ; COLBK

	lda _rgb_table+513,y
	sta $D016 ; COLPF0

	lda _rgb_table+514,y
	sta $D017 ; COLPF1

	lda _rgb_table+515,y
	sta $D018 ; COLPF2

	iny
	iny
	iny
	iny
	bne dli15b_loop3

	; Pull registers from stack, and return
	pla
	tay
	pla
	tax
	pla
        rti

