/**
 * @brief   #FujiNet Checkers
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE.md for details.
 * @verbose Constants (defines)
 */

/**
 * @brief important addresses
 */
#define DLIST_ADDR   0x0600      // Display list address in memory
#define PMG_ADDR     0x7C00      // PM/G base address
#define CHSET_ADDR   0x7A00      // Character set base address
#define BOARDPF_ADDR 0x8000      // Board playfield address
#define MSGPF_ADDR   0x8200      // Message playfield address.
#define MSG ((unsigned char*)0x8200)
#define BOARDPF ((unsigned char*)0x8000)

/**
 * @brief playfield colors
 */
#define COLOR0       0x36        // top pieces
#define COLOR1       0x88        // cursor
#define COLOR2       0x0e        // bottom pieces
#define COLOR3       0x26        // board frame
#define COLOR4       0x00        // background
#define PCOLR        0x34        // Board color (using players 0-3)

/**
 * @brief board background positions (using players)
 */
#define HPOSM1      0x38        // Board frame left (M1/M2/M3)
#define HPOSP0      0x3C        // Board segment 1
#define HPOSP1      0x5C        // Board segment 2
#define HPOSP2      0x7C        // Board segment 3
#define HPOSP3      0x9C        // Board segment 4
#define HPOSM0      0xBC        // Board frame right (M0)

/**
 * @brief Player/missile sizes
 */
#define SIZEP0      0x03       // All four players are quad size (4 color clocks wide)
#define SIZEM       0xFF       // All four missiles are quad size (4 color clocks wide)

/**
 * @brief Other Registers
 */
#define GPRIOR      0x14      // PM/G behind playfield objects
#define GRACTL      0x1B      // enable player and missile DMA to GTIA
#define SDMCTL      0x2E      // normalpf missiles players 2-line dlist
