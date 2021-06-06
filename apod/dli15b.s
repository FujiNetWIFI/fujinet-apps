	.export _dli15b
        .import _rgb_table, _rgb_ctr

_dli15b:
	; Push registers onto stack
	pha
	txa
	pha
	tya
	pha

	; Set up moving pointer within RGB table
	lda <_rgb_table
	sta $CB
	lda #0 ; rgb_table[] lives on a page boundary ($xx00)
	sta $CC

	ldx #192

dli15b_loop:
	ldy #0

	; Fetch and store the 4 colors 
	lda ($CB),y
	sta $D40A ; WSYNC
	sta $D01A ; COLBK
	iny

	lda ($CB),y
	sta $D016 ; COLPF0
	iny

	lda ($CB),y
	sta $D017 ; COLPF1
	iny

	lda ($CB),y
	sta $D018 ; COLPF2
	iny

	; Inc. the pointer by 4, for the next scanline
	lda $CC
	adc #4
	sta $CC

	; Roll over the LSB to MSB when we need to
	bcc dli15b_next
	inc $CB

dli15b_next:

	dex
	bne dli15b_loop

	; Pull registers from stack, and return
	pla
	tay
	pla
	tax
	pla
        rti

