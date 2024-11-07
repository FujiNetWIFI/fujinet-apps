/**
 * @brief Tokenize data retrieved from server
 */

#ifdef _COCO_BASIC_
#include <cmoc.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif /* CMOC_VERSION */

char l1[16], l2[16], l3[16], l4[16], l5[16], l6[16], d[16], t[16], newsite[10];
char s1[8], s2[8], s3[8], s4[8], s5[8], s6[8];

int d1=1, d2=2, d3=3, d4=4, d5=5, d6=6;

void parse(char *s)
{
    char *p = NULL;
    const char delim[2] = "|";

    p = strtok(s, delim);
    strcpy(l1,p);
    p = strtok(NULL, delim);
    strcpy(s1,p);
    d1 = atoi(s1);

    p = strtok(NULL, delim);
    strcpy(l2,p);
    p = strtok(NULL, delim);
    strcpy(s2,p);
    d2 = atoi(s2);

    p = strtok(NULL, delim);
    strcpy(l3,p);
    p = strtok(NULL, delim);
    strcpy(s3,p);
    d3 = atoi(s3);

    p = strtok(NULL, delim);
    strcpy(l4,p);
    p = strtok(NULL, delim);
    strcpy(s4,p);
    d4 = atoi(s4);

    p = strtok(NULL, delim);
    strcpy(l5,p);
    p = strtok(NULL, delim);
    strcpy(s5,p);
    d5 = atoi(s5);

    p = strtok(NULL, delim);
    strcpy(l6,p);
    p = strtok(NULL, delim);
    strcpy(s6,p);
    d6 = atoi(s6);

    p = strtok(NULL, delim);
    strcpy(d,p);
    p = strtok(NULL, delim);
    strcpy(t,p);
    p = strtok(NULL, delim);
    strcpy(newsite,p);
}
