#include <conio.h>
#include <stdint.h>
#include <stdio.h>

#include "main.h"

char *version = "1.0.0";

int main() {
	uint8_t result;

	clrscr();

	// call a 
	result = sum(1, 2);

	printf("main v%s\n", version);
	hello("template");
	printf("1+2 = %u\n", result);

	return 0;
}