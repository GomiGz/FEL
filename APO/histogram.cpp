#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
//#include <iostream>
#include <math.h>


typedef struct {

	unsigned char red, green, blue;

} Pixel;

int check(int number) {
	if (number > 255) {
		return 255;
	}
	else if (number < 0) {
		return 0;
	}
	else {
		return number;
	}
}

int main(int argc, char* argv[])
{
	printf("Zacatek\n");

	FILE* file = fopen(argv[1], "rb");
	int x, y, color_value;
	char format[8];
	int histogram[5] = { 0, 0, 0, 0, 0 };

	if (!fgets(format, sizeof(format), file)) {
		exit(1);
	}

	if (fscanf(file, "%d %d %d\n", &x, &y, &color_value) != 3) {
		exit(2);
	}

	int size = x*y;

	Pixel* data = (Pixel*)malloc(sizeof(Pixel)*size);

	if (fread(data, sizeof(Pixel), size, file) != size) {
		exit(3);
	}
	fclose(file);

	printf("Nacteno\n");

	Pixel* pixels = (Pixel*)malloc(sizeof(Pixel)*size);
	file = fopen("output.ppm", "wb");
	if (fprintf(file, "%s%d\n%d\n%d\n", format, x, y, color_value) < 0) {
		exit(4);
	}

	for (int i = 0; i < size; i++) {
		double h_value = 0;
		if ((i%x == 0) || (i%x == (x - 1)) || (i<x) || (i>(size - x))) {
			pixels[i] = data[i];
			h_value = round(0.2126*data[i].red + 0.7152*data[i].green + 0.0722*data[i].blue);
		}
		else {
			pixels[i].red = check(5 * data[i].red - data[i - 1].red - data[i + 1].red - data[i + x].red - data[i - x].red);
			h_value += 0.2126*pixels[i].red;

			pixels[i].green = check(5 * data[i].green - data[i - 1].green - data[i + 1].green - data[i + x].green - data[i - x].green);
			h_value += 0.7152*pixels[i].green;

			pixels[i].blue = check(5 * data[i].blue - data[i - 1].blue - data[i + 1].blue - data[i + x].blue - data[i - x].blue);
			h_value += 0.0722*pixels[i].blue;
		}
		h_value = round(h_value);
		if (h_value < 51) {
			histogram[0]++;
		}
		else if (h_value < 102) {
			histogram[1]++;
		}
		else if (h_value < 153) {
			histogram[2]++;
		}
		else if (h_value < 204) {
			histogram[3]++;
		}
		else {
			histogram[4]++;
		}
	}

	printf("Spocteno\n");

	if (fwrite(pixels, sizeof(Pixel), size, file) != size) {
		exit(5);
	}
	FILE* file_hist = fopen("output.txt", "w+");
	if (fprintf(file_hist, "%d %d %d %d %d", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4]) < 0) {
		exit(6);
	}
	fclose(file);
	fclose(file_hist);
	free(pixels);
	free(data);
	return 0;
}