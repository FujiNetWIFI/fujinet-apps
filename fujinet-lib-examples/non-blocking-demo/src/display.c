#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "display.h"

char left_over[40] = "";
uint8_t current_x = 0;
uint8_t current_y = 0;

// Directly update the console
void addWordToDisplay(const char* word) {
    int i = 0;
    int word_length = strlen(word);
    
    // Check if the word fits in the current line
    int spaceLeft = LINE_LENGTH - current_x;
    if (spaceLeft < word_length + 1) { // +1 for space or newline
        // Move to the next line, wrapping to the top
        current_y = (current_y + 1) % MAX_LINES;
        current_x = 0;

        // blank the rest of the new line, no point doing the word that will be printed
        gotoxy(word_length, current_y);
        for (i = word_length; i < LINE_LENGTH; i++) {
            cputc(' ');
        }
    }
    // fake a space
    if (current_x > 0) current_x++;
    cputsxy(current_x, current_y, word);
    current_x += word_length;
}

void processBuffer(const char* buffer, bool more_data) {
    char temp_buffer[BUFFER_SIZE * 2]; // Temporary buffer to handle concatenation and processing
    char* start;
    char* end;
    char temp;

    strcpy(temp_buffer, left_over);    // Start with left_over
    strcat(temp_buffer, buffer);       // Concatenate new buffer content
    temp_buffer[BUFFER_SIZE * 2 - 1] = '\0'; // Ensure null-termination

    // Clear left_over for the new processing round
    left_over[0] = '\0';

    start = temp_buffer;
    end = temp_buffer;
    while (*end != '\0') {
        // Find the end of the current word
        while (*end != ' ' && *end != '\0') {
            end++;
        }
        
        // Check if we reached the end of the buffer without finding a space
        if (*end == '\0') {
            if (!more_data) {
                // no more data, so this is final word, not one that needs to be completed in next block
                addWordToDisplay(start);
            } else {
                // The word is incomplete; copy it to left_over and break
                strcpy(left_over, start);
            }
            break;
        } else {
            // We found a complete word, or we're at the end; add it to the display
            // Temporarily null-terminate the word for display
            temp = *end;
            *end = '\0';
            addWordToDisplay(start);
            *end = temp; // Restore the original character
            
            // Move to the start of the next word
            end++;
            start = end;
        }
    }
}