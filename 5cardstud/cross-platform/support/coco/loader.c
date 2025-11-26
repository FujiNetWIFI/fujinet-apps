#include <cmoc.h>
#include <coco.h>



/// @brief Invokes the CoCo BASIC RUNM command
/// @param filename
void runm(const char *filename)
{
    // This reproduces the state when executing RUNM"FILE in BASIC
    // by filling in the command line buffer and jumping directly
    // to the Basic RUN procedure

    // Set beginning of (compressed) command: M"
    *((uint16_t *)0x2dd) = 0x4D22;

    // Add the filename to the command
    strcpy(0x2df, filename);

    // Set command pointer
    *((uint16_t *)0xa6) = 0x2dd; // set CHARAD

    // Jump to "RUNM" command
    asm
    {
        // Observed this value in actual RUNM. Not working unless 4Dxx is set
        ldd     #$4D1C

        // Jump to RUN procedure
        jmp     $AE75
    }
}

#define PROGRAM_TO_RUN "FCS"

// Load appropriate binary depending on CoCo version
void main(void)
{
    initCoCoSupport();
    runm(isCoCo3 ? PROGRAM_TO_RUN "3" : PROGRAM_TO_RUN "12");
}

