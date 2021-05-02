	.export _dli15
        .import _rgb_table, _rgb_ctr

_dli15:
	; Push registers onto stack
	pha
	txa
	pha
	tya
	pha

	; Scanline counter
	lda #(192/3)
	sta $CB

	; 3 * rgb_ctr
	lda _rgb_ctr
	asl a
	adc _rgb_ctr
	sta $CC

dli15_loop:
	; Scanline 0, 3, 6, ...
	; Load colors rgb_table[3*rgb_ctr + 0,1,2]
	ldy $CC
	lda _rgb_table+0,y
	pha
	ldx _rgb_table+1,y
	lda _rgb_table+2,y
	tay
	pla
	; Set colors
        sta $D40A ; WSYNC
        sta $D016 ; COLPF0
        stx $D017 ; COLPF1
        sty $D018 ; COLPF2

	; Scanline 1, 4, 7, ...
	; Load colors rgb_table[3*rgb_ctr + 3,4,5]
	ldy $CC
	lda _rgb_table+3,y
	pha
	ldx _rgb_table+4,y
	lda _rgb_table+5,y
	tay
	pla
	; Set colors
        sta $D40A ; WSYNC
        sta $D016 ; COLPF0
        stx $D017 ; COLPF1
        sty $D018 ; COLPF2

	; Scanline 3, 6, 9, ...
	; Load colors rgb_table[3*rgb_ctr + 6,7,8]
	ldy $CC
	lda _rgb_table+6,y
	pha
	ldx _rgb_table+7,y
	lda _rgb_table+8,y
	tay
	pla
	; Set colors
        sta $D40A ; WSYNC
        sta $D016 ; COLPF0
        stx $D017 ; COLPF1
        sty $D018 ; COLPF2

	; Next scanline
        dec $CB
        bne dli15_loop

	; Pull registers from stack, and return
	pla
	tay
	pla
	tax
	pla
        rti
