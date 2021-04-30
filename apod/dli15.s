	.export _dli15, _dli15_load_arg
        .import _rgb_table, _rgb_ctr

_dli15:
	; Push registers onto stack
	pha

_dli15_load:
        lda _rgb_table
        inc _dli15_load+1

	; Scanline 0
        sta $D40A ; WSYNC
        sta $D016 ; COLPF0
	lda _rgb_table+1
        sta $D017 ; COLPF1
	lda _rgb_table+2
        sta $D018 ; COLPF2

	; Scanline 1
        sta $D40A ; WSYNC
	lda _rgb_table+3
        sta $D016 ; COLPF0
	lda _rgb_table+4
        sta $D017 ; COLPF1
	lda _rgb_table+5
        sta $D018 ; COLPF2

	; Scanline 2
        sta $D40A ; WSYNC
	lda _rgb_table+6
        sta $D016 ; COLPF0
	lda _rgb_table+7
        sta $D017 ; COLPF1
	lda _rgb_table+8
        sta $D018 ; COLPF2

	; Scanline 3
        sta $D40A ; WSYNC
	lda _rgb_table+9
        sta $D016 ; COLPF0
	lda _rgb_table+10
        sta $D017 ; COLPF1
	lda _rgb_table+11
        sta $D018 ; COLPF2

	; Scanline 4
        sta $D40A ; WSYNC
	lda _rgb_table+12
        sta $D016 ; COLPF0
	lda _rgb_table+13
        sta $D017 ; COLPF1
	lda _rgb_table+14
        sta $D018 ; COLPF2

	; Scanline 5
        sta $D40A ; WSYNC
	lda _rgb_table+15
        sta $D016 ; COLPF0
	lda _rgb_table+16
        sta $D017 ; COLPF1
	lda _rgb_table+17
        sta $D018 ; COLPF2

	; Pull registers from stack, and return
	pla
        rti

_dli15_load_arg = _dli15_load + 1

