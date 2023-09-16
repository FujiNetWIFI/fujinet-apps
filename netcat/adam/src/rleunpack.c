/**
 * @brief RLE unpack routines for images processed by Convert9918, modified for z88dk
 * @author Tursillion
 * @param p The start VDP address
 * @param buf The source buffer in RLE format
 * @param nMax the maximum # of bytes to unpack
 */

#include <video/tms99x8.h>

void RLEUnpack(unsigned int p, unsigned char *buf, unsigned int nMax)
{
  unsigned char z;
  int cnt;
  
  cnt = nMax;
  while (cnt > 0)
    {
      z=*buf;
      if (z&0x80)
	{
	  // run of bytes
	  buf++;
	  z&=0x7f;
	  vdp_vfill(p,*buf,z);
	  buf++;
	  p += z;
	}
      else
	{
	  // sequence of data
	  buf++;
	  vdp_vwrite(buf,p,z);
	  buf+=z;
	  p += z;
	}
      cnt-=z;
    }
}
