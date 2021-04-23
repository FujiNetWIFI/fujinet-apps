	.export _dli256, _dli256_load_arg
        .import _rgb_table

_dli256:
        pha
_dli256_load:
        lda _rgb_table
        inc _dli256_load+1
        sta $D40A ; WSYNC
        sta $D01A ; COLOR4
        pla
        rti

_dli256_load_arg = _dli256_load + 1
