.include "c64.inc"
	
	;; Bitmap location to COLOR_RAM transform, courtesy of
	;; McMartin in #c64friends, thanks!

.import _colorpt, _x_coord, _y_coord
.export _color_transform
	
.code
	
.proc _color_transform	
        lda     #$00
        sta     _colorpt+1
        lda     _y_coord
        and     #$F8
        sta     _colorpt
        asl     _colorpt
        rol     _colorpt+1
        asl     _colorpt
        rol     _colorpt+1
        ;; colorpt is now (_y_coord >> 3) << 5.
        ;; accumulator is now (_y_coord >> 3) << 3.
        clc
        adc     _colorpt
        sta     _colorpt
        lda     #$00
        adc     _colorpt+1
        sta     _colorpt+1
        ;; colorpt is now (_y_coord >> 3) * (2^5 + 2^3) = (_y_coord >> 3) * 40
        lda     _x_coord+1
        lsr
        lda     _x_coord
        ror
        lsr
        lsr
        adc     _colorpt
        sta     _colorpt
        lda     #$00
        adc     _colorpt+1
        sta     _colorpt+1
        ;; Now add COLORBASE, $D000
        ;; clc
        ;; lda     _colorpt+1
        ;; adc     #$d0
        ;; sta     _colorpt+1
	rts
.endproc
