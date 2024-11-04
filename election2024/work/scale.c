#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("scale <max_value> <screen_x_max>");
		return 1;
	}

	float mx = atof(argv[1]);
	float sx = atof(argv[2]);

	printf("int scale[%d]=\n",(int)floor(sx));
	printf("{\n");
	for (int i=0; i<sx; i++)
	{
		printf("%d,",(int)floor(i * (mx/sx))); 
	}

	printf("\n}\n");

	return 0;
}
