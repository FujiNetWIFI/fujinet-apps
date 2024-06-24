#include <string.h>
#include <stdio.h>

void ellipsizeString(char *src, char *dst, size_t maxLen)
{
    size_t srcLen = strlen(src);
    if (maxLen == 0)
    {
        return; // No space even for a null terminator
    }
    if (srcLen + 1 <= maxLen)
    {
        // The source string, plus null terminator, fits in the destination
        strcpy(dst, src);
    }
    else if (maxLen <= 4)
    {
        // Not enough space for any characters plus an ellipsis, so just truncate
        strncpy(dst, src, maxLen - 1);
        dst[maxLen - 1] = '\0';
    }
    else
    {
        // Add an ellipsis and as much of the source string as will fit
        strcpy(dst, "...");
        strncpy(dst + 3, src + srcLen - (maxLen - 4), maxLen - 4);
        dst[maxLen - 1] = '\0';
    }
}