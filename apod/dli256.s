	.export _dli256
        .import _rgb_ctr, _apac_scanline, _apac_lum

_dli256:
        pha
	lda #64
        sta $D40A ; WSYNC
        sta $D01B ; PRIOR
        lda #0
        sta $D01A ; COLBK
	lda #192
        sta $D40A ; WSYNC
        sta $D01B ; PRIOR
        lda _apac_lum
        sta $D01A ; COLBK
        pla
        rti
