;
; Startup code for cc65 (ATARI version)
;
; Contributing authors:
;       Mark Keates
;       Freddy Offenga
;       Christian Groessler
;       Stefan Haubenthal
;

        .export         __STARTUP__ : absolute = 1      ; Mark as startup
        .export         _exit, start, excexit, SP_save

        .import         initlib, donelib
        .import         callmain, zerobss
        .import         __RESERVED_MEMORY__
        .import         __MAIN_START__, __MAIN_SIZE__
        .import         __LOWCODE_RUN__, __LOWCODE_SIZE__
.ifdef __ATARIXL__
        .import         __STACKSIZE__
        .import         sram_init
        .import         scrdev
        .import         findfreeiocb
        .forceimport    sramprep                        ; force inclusion of the "shadow RAM preparation" load chunk
        .include        "save_area.inc"
.endif

        .include        "zeropage.inc"
        .include        "atari.inc"

; ------------------------------------------------------------------------

.segment        "STARTUP"

; Real entry point:

start:

; Clear the BSS data.

        jsr     zerobss

; Set up the stack.

        tsx
        stx     SP_save

; Report the memory usage.

        lda     APPMHI
        ldx     APPMHI+1
        sta     APPMHI_save             ; remember old APPMHI value
        stx     APPMHI_save+1

        sec
        lda     MEMTOP
        sbc     #<__RESERVED_MEMORY__
        sta     APPMHI                  ; initialize our APPMHI value
        sta     sp                      ; set up runtime stack part 1
        lda     MEMTOP+1
        sbc     #>__RESERVED_MEMORY__
        sta     APPMHI+1
        sta     sp+1                    ; set up runtime stack part 2

; Call the module constructors.

        jsr     initlib

; Push the command-line arguments; and, call main().

        jsr     callmain

; Call the module destructors. This is also the exit() entry.

_exit:  ldx     SP_save
        txs                     ; Restore stack pointer

; Restore the system stuff.

excexit:jsr     donelib         ; Run module destructors; 'excexit' is called from the exec routine

; Back to DOS.

        rts

; *** end of main startup code

; ------------------------------------------------------------------------

.bss

SP_save:        .res    1
APPMHI_save:	.res    1

; ------------------------------------------------------------------------

.segment "LOWCODE"       ; have at least one (empty) segment of LOWCODE, so that the next line works even if the program doesn't make use of this segment
.assert (__LOWCODE_RUN__ + __LOWCODE_SIZE__ <= $4000 || __LOWCODE_RUN__ > $7FFF || __LOWCODE_SIZE__ = 0), warning, "'lowcode area' reaches into $4000..$7FFF bank memory window"
; check for LOWBSS_SIZE = 0 not needed since the only file which uses LOWBSS (irq.s) also uses LOWCODE
; check for LOWCODE_RUN > $7FFF is mostly for cartridges, where this segment is loaded high (into cart ROM)
; there is a small chance that if the user loads the program really high, LOWCODE is above $7FFF, but LOWBSS is below -- no warning emitted in this case
