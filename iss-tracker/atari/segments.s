	.setcpu		"6502"

	.export		_scr_mem
.segment	"SCRMEM"
_scr_mem:

	.export		_pmg_mem
.segment	"PMGMEM"
_pmg_mem:

	.export		_map_data
.segment	"MAP"
_map_data:
	.incbin "img/map.gr7"
	.code

