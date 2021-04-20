	.export _dli9, _dli9_load_arg
        .import _rgb_table

_dli9:
        pha
_dli9_load:
        lda _rgb_table
        inc _dli9_load+1
        sta $D40A ; WSYNC
        sta $D01A ; COLOR4
        pla
        rti

_dli9_load_arg = _dli9_load + 1
