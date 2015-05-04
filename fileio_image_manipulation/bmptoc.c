// Anna McAbee
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


#define DATA_ARR_LEN (0x36 + 240 * 160 * 4)
char data_arr[DATA_ARR_LEN];

int main(int argc, char *argv[]) {

	if (argc != 2) { 
		return -1; //exit
	}

	
	char *filename = argv[1]; 

	FILE *infile = fopen(filename, "r");
	if (!infile) {
		printf("File doesn't exist"); 
		return -1;
	}


	fread(data_arr, 1, DATA_ARR_LEN, infile);
	fclose(infile);

	unsigned int width = *(unsigned int*)(data_arr + 0x12);

	unsigned int height = *(unsigned int*)(data_arr + 0x16);

	char filenamePlain[100];
	strcpy(filenamePlain, filename);
	int dotIndex = strlen(filename) - 4;
	filenamePlain[dotIndex] = '\0'; 

	char filenameH[100];
	strcpy(filenameH, filename);
	filenameH[dotIndex] = '\0'; 
	sprintf(filenameH,"%s.h", filenameH);

	char filenameUpper[100];
	strcpy(filenameUpper, filename);
	filenameUpper[dotIndex] = '\0';
	int i = 0;
	while (filenameUpper[i])
	{
		filenameUpper[i] = toupper(filenameUpper[i]);
		i++;
	}

	FILE *outfile = fopen(filenameH,"w");
	fprintf(outfile, "#define %s_WIDTH %d\n", filenameUpper, width);
	fprintf(outfile, "#define %s_HEIGHT %d\n", filenameUpper, height);
	fprintf(outfile, "const unsigned short %s_data[%d];\n", filenamePlain, height*width);
	fclose(outfile);



	char filenameC[100];
	strcpy(filenameC, filename);
	filenameC[dotIndex] = '\0'; 
	sprintf(filenameC,"%s.c", filenameC);



	FILE *cfile = fopen(filenameC, "w");
	fprintf(cfile, "const unsigned short %s_data[%d] = {\n", filenamePlain, height*width);
	unsigned int *ptr = (unsigned int*) (data_arr + 0x36); 
	for(int h = height - 1; h >= 0; h--)
	{
		for (int w = 0; w < width; w++)
		{
			unsigned int pix = ptr[width*h+w];
			unsigned char red = (pix >> (2 * 8)) & 0xff;
			unsigned char green = (pix >> (1 * 8)) & 0xff;
			unsigned char blue = pix & 0xff;

			blue >>= 3;
			green >>= 3;
			red >>= 3;

			unsigned int pixel = (blue << (2 * 5)) | (green << (1 * 5)) |(red);
			fprintf(cfile, "0x%x, \n", pixel);
		}

	}
	fprintf(cfile,"\n};");
	fclose(cfile);

	return 0;
}

