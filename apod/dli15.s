	.export _dli15
        .import _rgb_table, _rgb_ctr

_dli15:
	; Push registers onto stack
	pha
	txa
	pha

_dli15_load:
	lda _rgb_ctr
	adc _rgb_ctr
	adc _rgb_ctr
	tax

	; Scanline 1 (e.g., Reds)
        sta $D40A ; WSYNC

        lda _rgb_table+0,x
        sta $D016 ; COLPF0
	lda _rgb_table+1,x
        sta $D017 ; COLPF1
	lda _rgb_table+2,x
        sta $D018 ; COLPF2

	; Scanline 2 (e.g., Greens)
        sta $D40A ; WSYNC
	lda _rgb_table+3,x
        sta $D016 ; COLPF0
	lda _rgb_table+4,x
        sta $D017 ; COLPF1
	lda _rgb_table+5,x
        sta $D018 ; COLPF2

	; Scanline 3 (e.g., Blues)
        sta $D40A ; WSYNC
	lda _rgb_table+6,x
        sta $D016 ; COLPF0
	lda _rgb_table+7,x
        sta $D017 ; COLPF1
	lda _rgb_table+8,x
        sta $D018 ; COLPF2

	; Scanline 4 (e.g., Reds again)
        sta $D40A ; WSYNC
	lda _rgb_table+0,x
        sta $D016 ; COLPF0
	lda _rgb_table+1,x
        sta $D017 ; COLPF1
	lda _rgb_table+2,x
        sta $D018 ; COLPF2

	; Scanline 5 (...and so forth)
        sta $D40A ; WSYNC
	lda _rgb_table+3,x
        sta $D016 ; COLPF0
	lda _rgb_table+4,x
        sta $D017 ; COLPF1
	lda _rgb_table+5,x
        sta $D018 ; COLPF2

	; Scanline 6
        sta $D40A ; WSYNC
	lda _rgb_table+6,x
        sta $D016 ; COLPF0
	lda _rgb_table+7,x
        sta $D017 ; COLPF1
	lda _rgb_table+8,x
        sta $D018 ; COLPF2

        ; The DLI is invoked every 9th scanline

	; Pull registers from stack, and return
	pla
	tax
	pla
        rti
