;
; nmi_trampoline.s -- Catches Non-Maskable Interrupts when the C64 ROM and I/O are disabled.
;
; 2018-05-15, Greg King
;
; void install_nmi_trampoline (void);
;

        .export         _install_nmi_trampoline

        .include        "c64.inc"

nmi_vec :=      $FFFA

_install_nmi_trampoline:
        lda     #<nmi_trampoline
        ldx     #>nmi_trampoline
        sta     nmi_vec
        stx     nmi_vec+1
        rts

.segment        "LOWCODE"

nmi_trampoline:
        pha
        lda     $01             ; save RAM config.
        pha
        ora     #%00000110      ; enable Kernal and I/O
        sta     $01
        lda     #>ret           ; create NMI return address
        pha
        lda     #<ret
        pha
        php
        jmp     (NMIVec)

ret:    pla
        sta     $01
        pla
        rti
