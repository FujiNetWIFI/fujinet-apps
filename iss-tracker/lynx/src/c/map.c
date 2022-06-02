#include <lynx.h>
#include <tgi.h>

extern char img_map[];
extern SCB_REHV_PAL issSprite;

SCB_REHV_PAL mapSprite = {
  BPP_4 | TYPE_BACKGROUND, REHV | PACKED, 0x0,
  (char *)&issSprite, &img_map[0],
  0, 0,
  256, 256,
  {0xCA,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF}
};

void map(void)
{
  tgi_sprite(&mapSprite);
}
