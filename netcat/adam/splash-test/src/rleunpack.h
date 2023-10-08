#ifndef RLEUNPACK_H
#define RLEUNPACK_H

/**
 * @brief RLE unpack routines for images processed by Convert9918, modified for z88dk
 * @author Tursillion
 * @param p The start VDP address
 * @param buf The source buffer in RLE format
 * @param nMax the maximum # of bytes to unpack
 */

void RLEUnpack(unsigned int p, unsigned char *buf, unsigned int nMax);

#endif /* RLEUNPACK_H */
