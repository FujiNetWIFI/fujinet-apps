	.export _dli256
        .import _rgb_ctr, _apac_scanline

_dli256:
        pha
	lda #64
        sta $D40A ; WSYNC
        sta $D01B ; PRIOR
	lda #192
        sta $D40A ; WSYNC
        sta $D01B ; PRIOR
        pla
        rti
