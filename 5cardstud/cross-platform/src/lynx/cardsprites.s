    .export _card_up_spr, _halfcard_down_spr, _small_chip_spr
    .export _heart_spr, _diamond_spr, _spade_spr, _club_spr


    .rodata

_card_up_spr:
    .incbin "./card_sprites/card-face-up.spr"
_halfcard_down_spr:
    .incbin "./card_sprites/half-card-face-down.spr"

_small_chip_spr:
    .incbin "./card_sprites/small-chip.spr"

_heart_spr:
    .incbin "./card_sprites/heart.spr"
_diamond_spr:
    .incbin "./card_sprites/diamond.spr"
_spade_spr:
    .incbin "./card_sprites/spade.spr"
_club_spr:
    .incbin "./card_sprites/club.spr"