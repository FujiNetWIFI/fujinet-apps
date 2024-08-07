#ifndef WORDS_H
#define WORDS_H

#include <string.h>

#define MAX_LINES 8
#define LINE_LENGTH 40
#define BUFFER_SIZE 40

// void addWordToDisplay(const char* word);
void processBuffer(const char* buffer, bool more_data);

#endif