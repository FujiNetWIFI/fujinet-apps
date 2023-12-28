        .export _do_gr0
        .export _debug

        .include "macros.inc"
        .include "atari.inc"

.proc _debug
        rts
.endproc

; void do_gr0();
;
; closes and reopens E: device.
; Requires compiler arg (or cfg setting) for RESERVED_MEMORY to be > 0
.proc _do_gr0

        ldx     #$00
        mva     #$0C,       {ICCOM, x}
        jsr     CIOV

        ldx     #$00
        mva     #$03,       {ICCOM, x}  ; open
        mva     #<dev_name, {ICBAL, x}
        mva     #>dev_name, {ICBAH, x}
        mva     #$0C,       {ICAX1, x}
        jmp     CIOV
.endproc

.data
dev_name:        .byte "E:", 0
