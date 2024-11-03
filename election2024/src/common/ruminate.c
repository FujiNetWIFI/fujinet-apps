/**
 * @brief Wait a spell... need to add __Z88DK__ define to makefile
 */

#ifdef __CMOC_VERSION__
#include <cmoc.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif /* CMOC_VERSION  */
void ruminate(void)
{
    sleep(900);
}
