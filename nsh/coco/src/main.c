/**
 * @brief   NSH
 * @author  Thomas Cherryhomes
 * @email   thom dot cherryhomes at gmail dot com
 * @license gpl v. 3, see LICENSE for details.
 * @verbose The main program 
 */

#include "nsh.h"
#include "parse.h"

int main(void)
{
    while (nsh())
        parse();
    
    return 0;
}
