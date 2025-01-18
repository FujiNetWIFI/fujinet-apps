/**
 * @brief Do any local filesystem housekeeping, like getting file type.
 */

#include <apple2.h>
#include <apple2_filetype.h>
#include <stdio.h>
#include <string.h>

char _ft[16];
char _aux[16];

void local_file_init(void)
{
    
    printf("Enter Destination Filetype: $");
    gets(_ft);

    if (!_ft[0])
        _filetype = PRODOS_T_BIN;
    else
        sscanf(_ft,"%x",&_filetype);


    printf("Enter Destination Auxtype: $");
    gets(_aux);

    if (!_aux[0])
        _auxtype = 0;
    else
        sscanf(_aux,"%x",&_auxtype);
}
