	.export _dli15, _dli15_load_arg
        .import _rgb_table

_dli15:
        pha
_dli15_load:
        lda _rgb_table
        inc _dli15_load+1
        sta $D40A ; WSYNC
        sta $D01A ; COLOR4
        pla
        rti

_dli15_load_arg = _dli15_load + 1
