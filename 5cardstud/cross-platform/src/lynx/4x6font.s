	.export _font4x6_0x20
	.export _font4x6_0x21, _font4x6_0x22, _font4x6_0x23, _font4x6_0x24, _font4x6_0x25, _font4x6_0x26, _font4x6_0x27, _font4x6_0x28 
	.export _font4x6_0x29, _font4x6_0x2A, _font4x6_0x26, _font4x6_0x2B, _font4x6_0x2C, _font4x6_0x2D, _font4x6_0x2E, _font4x6_0x2F 
	.export _font4x6_0x30, _font4x6_0x31, _font4x6_0x32, _font4x6_0x33, _font4x6_0x34, _font4x6_0x35, _font4x6_0x36, _font4x6_0x37
	.export _font4x6_0x38, _font4x6_0x39, _font4x6_0x3A, _font4x6_0x3B, _font4x6_0x3C, _font4x6_0x3D, _font4x6_0x3E, _font4x6_0x3F
	.export _font4x6_0x40, _font4x6_0x41, _font4x6_0x61, _font4x6_0x42, _font4x6_0x62, _font4x6_0x43, _font4x6_0x63, _font4x6_0x44
	.export _font4x6_0x64, _font4x6_0x45, _font4x6_0x65, _font4x6_0x46, _font4x6_0x66, _font4x6_0x47, _font4x6_0x67, _font4x6_0x48
	.export	_font4x6_0x68, _font4x6_0x49, _font4x6_0x69, _font4x6_0x4A, _font4x6_0x6A, _font4x6_0x4B, _font4x6_0x6B, _font4x6_0x4C
	.export _font4x6_0x6C, _font4x6_0x4D, _font4x6_0x6D, _font4x6_0x4E, _font4x6_0x6E, _font4x6_0x4F, _font4x6_0x6F, _font4x6_0x50
	.export	_font4x6_0x70, _font4x6_0x51, _font4x6_0x71, _font4x6_0x52, _font4x6_0x72, _font4x6_0x53, _font4x6_0x73, _font4x6_0x54
	.export	_font4x6_0x74, _font4x6_0x55, _font4x6_0x75, _font4x6_0x56, _font4x6_0x76, _font4x6_0x57, _font4x6_0x77, _font4x6_0x58
	.export	_font4x6_0x78, _font4x6_0x59, _font4x6_0x79, _font4x6_0x5A, _font4x6_0x7A, _font4x6_0x5B, _font4x6_0x5C, _font4x6_0x5D
	.export _font4x6_0x5E, _font4x6_0x5F, _font4x6_0x60, _font4x6_0x7B, _font4x6_0x7C, _font4x6_0x7D;, _font4x6_0x7E


	.rodata	

; ASCII codes 0x21 - 0x2F
_font4x6_0x20:
	.incbin "./4x6/space.spr"
_font4x6_0x21:
	.incbin "./4x6/exclam.spr"
_font4x6_0x22:
	.incbin "./4x6/quotation.spr"
_font4x6_0x23:
	.incbin "./4x6/hash.spr"
_font4x6_0x24:
	.incbin "./4x6/dollar.spr"
_font4x6_0x25:
	.incbin "./4x6/percent.spr"
_font4x6_0x26:
	.incbin "./4x6/plus.spr"		; ampersand is same as plus sign
_font4x6_0x27:
	.incbin "./4x6/apostr.spr"
_font4x6_0x28:
	.incbin "./4x6/left-rbrack.spr"
_font4x6_0x29:
	.incbin "./4x6/right-rbrack.spr"
_font4x6_0x2A:
	.incbin "./4x6/asterisk.spr"
_font4x6_0x2B:					
	.incbin "./4x6/plus.spr"
_font4x6_0x2C:
	.incbin "./4x6/comma.spr"
_font4x6_0x2D:
	.incbin "./4x6/hypen.spr"
_font4x6_0x2E:
	.incbin "./4x6/period.spr"
_font4x6_0x2F:
	.incbin "./4x6/forwardslash.spr"

; ASCII codes 0x30 - 0x39
_font4x6_0x30:
	.incbin "./4x6/0.spr"
_font4x6_0x31:
	.incbin "./4x6/1.spr"
_font4x6_0x32:
	.incbin "./4x6/2.spr"
_font4x6_0x33:
	.incbin "./4x6/3.spr"
_font4x6_0x34:
	.incbin "./4x6/4.spr"
_font4x6_0x35:
	.incbin "./4x6/5.spr"
_font4x6_0x36:
	.incbin "./4x6/6.spr"
_font4x6_0x37:
	.incbin "./4x6/7.spr"
_font4x6_0x38:
	.incbin "./4x6/8.spr"
_font4x6_0x39:
	.incbin "./4x6/9.spr"

; ASCI codes 0x3A - 0x40
_font4x6_0x3A:
	.incbin "./4x6/colon.spr"
_font4x6_0x3B:
	.incbin "./4x6/semicolon.spr"
_font4x6_0x3C:
	.incbin "./4x6/left-abrack.spr"
_font4x6_0x3D:
	.incbin "./4x6/equal.spr"
_font4x6_0x3E:
	.incbin "./4x6/right-abrack.spr"
_font4x6_0x3F:
	.incbin "./4x6/question.spr"
_font4x6_0x40:
	.incbin "./4x6/at.spr"

; ASCII codes 0x41 - 0x5A (uppercase) 0x61 - 7a (lowercase)
_font4x6_0x41:
_font4x6_0x61:
	.incbin "./4x6/a.spr"
_font4x6_0x42:
_font4x6_0x62:
	.incbin "./4x6/b.spr"
_font4x6_0x43:
_font4x6_0x63:
	.incbin "./4x6/c.spr"
_font4x6_0x44:
_font4x6_0x64:
	.incbin "./4x6/d.spr"
_font4x6_0x45:
_font4x6_0x65:
	.incbin "./4x6/e.spr"
_font4x6_0x46:
_font4x6_0x66:
	.incbin "./4x6/f.spr"
_font4x6_0x47:
_font4x6_0x67:
	.incbin "./4x6/g.spr"
_font4x6_0x48:
_font4x6_0x68:
	.incbin "./4x6/h.spr"
_font4x6_0x49:
_font4x6_0x69:
	.incbin "./4x6/i.spr"
_font4x6_0x4A:
_font4x6_0x6A:
	.incbin "./4x6/j.spr"
_font4x6_0x4B:
_font4x6_0x6B:
	.incbin "./4x6/k.spr"
_font4x6_0x4C:
_font4x6_0x6C:
	.incbin "./4x6/l.spr"
_font4x6_0x4D:
_font4x6_0x6D:
	.incbin "./4x6/m.spr"
_font4x6_0x4E:
_font4x6_0x6E:
	.incbin "./4x6/n.spr"
_font4x6_0x4F:
_font4x6_0x6F:
	.incbin "./4x6/o.spr"
_font4x6_0x50:
_font4x6_0x70:
	.incbin "./4x6/p.spr"
_font4x6_0x51:
_font4x6_0x71:
	.incbin "./4x6/q.spr"
_font4x6_0x52:
_font4x6_0x72:
	.incbin "./4x6/r.spr"
_font4x6_0x53:
_font4x6_0x73:
	.incbin "./4x6/s.spr"
_font4x6_0x54:
_font4x6_0x74:
	.incbin "./4x6/t.spr"
_font4x6_0x55:
_font4x6_0x75:
	.incbin "./4x6/u.spr"
_font4x6_0x56:
_font4x6_0x76:
	.incbin "./4x6/v.spr"
_font4x6_0x57:
_font4x6_0x77:
	.incbin "./4x6/w.spr"
_font4x6_0x58:
_font4x6_0x78:
	.incbin "./4x6/x.spr"
_font4x6_0x59:
_font4x6_0x79:
	.incbin "./4x6/y.spr"
_font4x6_0x5A:
_font4x6_0x7A:
	.incbin "./4x6/z.spr"

; ASCII codes 0x5B - 0x60
_font4x6_0x5B:
	.incbin "./4x6/left-sbrack.spr"
_font4x6_0x5C:
	.incbin "./4x6/backslash.spr"
_font4x6_0x5D:
	.incbin "./4x6/right-sbrack.spr"
_font4x6_0x5E:
	.incbin "./4x6/carat.spr"
_font4x6_0x5F:
	.incbin "./4x6/underscore.spr"
_font4x6_0x60:
	.incbin "./4x6/backtick.spr"

; ASCII codes 0x7B - 0x7E
_font4x6_0x7B:
	.incbin "./4x6/left-cbrack.spr"
_font4x6_0x7C:
	.incbin "./4x6/bar.spr"
_font4x6_0x7D:
	.incbin "./4x6/right-cbrack.spr"
;_font4x6_0x7E:
;	.incbin "./4x6/tilde.spr"
	
	