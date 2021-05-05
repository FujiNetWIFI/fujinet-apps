	.setcpu		"6502"

	.export		_rgb_table
.segment	"RGBTABLEMEM"
_rgb_table:	.res 256

	.export		_scr_mem
.segment	"SCRMEM"
_scr_mem:

	.export		_txt_mem
.segment	"TXTMEM"
_txt_mem:
