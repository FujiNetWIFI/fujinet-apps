/**
 * @brief Runs VT100C3 on a CoCo 3, VT100C2 on a CoCo 1/2.
 */

#include <cmoc.h>
#include <coco.h>

/* RUNM via the ROM: filename into the line buffer, CHARAD pointing at it,
   then BASIC's RUN at $AE75, which takes the ML path for 'M'. */
static void runm(const char *filename)
{
    *((uint16_t *) 0x2dd) = 0x4D22;
    strcpy((char *) 0x2df, filename);
    *((uint16_t *) 0xa6) = 0x2dd;

    asm
    {
        ldd     #$4D1C
        jmp     $AE75
    }
}

int main(void)
{
    initCoCoSupport();
    runm(isCoCo3 ? "VT100C3" : "VT100C2");
    return 0;
}
