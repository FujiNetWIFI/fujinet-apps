	.setcpu		"6502"

	.export		_dlist_mem, _rgb_table
.segment	"DLISTMEM"
_dlist_mem:	.res 3*1024
_rgb_table:	.res 256

	.export		_scr_mem
.segment	"SCRMEM"
		.res 16
_scr_mem:
