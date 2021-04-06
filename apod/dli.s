	.export _dli, _dli_load_arg
        .import _rgb_table

_dli:
        pha
_dli_load:
        lda _rgb_table
        inc _dli_load+1
        sta $D40A
        sta $D01A
        pla
        rti

_dli_load_arg = _dli_load + 1
