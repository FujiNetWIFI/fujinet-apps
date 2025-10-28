;; Include the custom character set for CoCo PMODE 3
;; It can be edited with any 2bpp editor
;; I used https://github.com/matosimi/atari-fontmaker/releases/tag/V1.6.17.4

        SECTION rodata
        EXPORT _charset
_charset
        INCLUDEBIN ../../support/coco/pmode3.fnt
        ENDSECTION