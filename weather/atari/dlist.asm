  icl 'const.inc' 


display_list
  dta $70, $70, $70, $4d, a(VRAM), $0d, $0d, $0d, $0d, $0d
  dta $0d, $0d, $10, $0d, $0d, $0d, $0d, $0d, $0d, $0d
  dta $0d, $0d, $10, $90, $0d, $0d, $0d, $0d, $0d, $0d
  dta $0d, $0d, $0d, $0d, $0d, $0d, $0d, $0d, $0d, $0d
  dta $0d, $0d, $0d, $0d, $0d, $0d, $0d, $0d, $30
  dta $5d, a(VRAM+(41*40)), $5d, a(VRAM+(41*40)+$50), $5d, a(VRAM+(41*40)+$a0)
  dta $5d, a(VRAM+(41*40)+$f0), $5d, a(VRAM+(41*40)+$140), $5d, a(VRAM+(41*40)+$190)
  dta $5d, a(VRAM+(41*40)+$1e0), $5d, a(VRAM+(41*40)+$230), $5d, a(VRAM+(41*40)+$280)
  dta $01, a(DLWEATHER)
  dta <dlweather, <dlforecast
dlweather
  dta $f0, $42, a(VRAM+(41*40)+(9*80)), $f0, $02, $02, $02, $02, $02, $02, $02
  dta $80, $02, $41, a(DLIST)
dlforecast  
  dta $b0, $42, a(VRAM+(41*40)+(9*80)), $90, $02, $02
  dta $02, $20, $02, $02, $20, $02, $02, $b0, $02, $41
  dta a(DLIST)


