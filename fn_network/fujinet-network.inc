; ASM version of fujinet-network.h
;

FN_ERR_OK               := $00      ; No error
FN_ERR_IO_ERROR         := $01      ; The device returned an IO Error
FN_ERR_BAD_CMD          := $02      ; Function called with bad arguments
FN_ERR_OFFLINE          := $03      ; The device is offline
FN_ERR_WARNING          := $04      ; Device specific non-fatal warning issued

FN_ERR_UNKNOWN          := $ff      ; wasn't handled by library. should inspect the raw code. candidate for adding to library
