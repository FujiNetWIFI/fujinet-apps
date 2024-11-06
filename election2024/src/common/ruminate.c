/**
 * @brief Wait a spell... need to add __Z88DK__ define to makefile
 */

#ifdef _COCO_BASIC_
#include <cmoc.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif /* CMOC_VERSION  */

void ruminate(void)
{
#if defined(_COCO_BASIC_)
	unsigned long i=0;
	for (i=0;i<100000;i++); /* seriously, cmoc, WTF? */
#elif defined(__PMD85__)
	sleep(640); /* PMD is slow even if it sleeps ... */
#else
    sleep(900);
#endif /* _COCO_BASIC_ */
}
