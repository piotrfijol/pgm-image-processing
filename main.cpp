#include <stdio.h>
#include <iostream>
#include "pgm.h"

#define SHADES 7
#define EL_STR_WIDTH 3
#define EL_STR_HEIGHT 3

using namespace std;


PGM::PGM_FILE pgmImage;


unsigned char nShade;
int elStr[EL_STR_HEIGHT][EL_STR_WIDTH] = 
{
	{ 50,80,80 },
	{ 50,30,20 },
	{ 80,80,10 },
};


void showImage(short int** obraz)
{

	const char tablicaOdcieni[SHADES] = { 0x2E , 0x2D, 0x3A , 0x3D, 0x2B, 0x23,  0xDB };

	int i,j;
	printf("%c",0xDA); 
	
	for(j=0;j<pgmImage.width;j++) 
		printf("%c ",0xC4);
	
	
	printf("%c\n",0xBF);
	
	for(i=0;i<pgmImage.height;i++) 
	{
		printf("%c",0xB3);
		for (j = 0;j < pgmImage.width;j++) {
			if (obraz[i][j] > 6)
				printf("%c ", tablicaOdcieni[SHADES-1]);
			else if (obraz[i][j] < 0)
				printf("%c ", tablicaOdcieni[0]);
			else
				printf("%c ", tablicaOdcieni[obraz[i][j]]);

		}
		printf("%c\n",0xB3);
	}

	printf("%c",0xC0); 
	for(j=0;j<pgmImage.width;j++) 
		printf("%c ",0xC4);
	printf("%c\n",0xD9);  
}

short int** imageErusion(short int** obraz) 
{

	short int** obrazWynik = new short int* [pgmImage.height];

	for (int i = 0; i < pgmImage.height; i++)
		obrazWynik[i] = new short int[pgmImage.width];

	for (int i = 1; i < pgmImage.height - 1; i++) 
	{
		for (int j = 1; j < pgmImage.width - 1; j++) 
		{

			int min = obraz[i][j] - elStr[1][1];
			for (int k = -1; k < EL_STR_HEIGHT - 1; k++) 
			{
				for (int l = -1; l < EL_STR_WIDTH - 1; l++) 
				{
					if ((obraz[i + k][j + l] - elStr[k + 1][l + 1]) < min)
						min = obraz[i + k][j + l] - elStr[k + 1][l + 1];

					obrazWynik[i][j] = min;
				}
			}
		}
	}

	return obrazWynik;
}

short int** imageDilation(short int** obraz) 
{

	short int** obrazWynik = new short int* [pgmImage.height];

	for (int i = 0; i < pgmImage.height; i++)
		obrazWynik[i] = new short int[pgmImage.width];


	for (int i = 1; i < pgmImage.height - 1; i++) 
	{
		for (int j = 1; j < pgmImage.width - 1; j++) 
		{
			obrazWynik[i][j] = 0;

			int max = 0;
			for (int k = -1; k < EL_STR_HEIGHT-1; k++) 
			{
				for (int l = -1; l < EL_STR_WIDTH-1; l++) 
				{
					if ((obraz[i + k][j + l] + elStr[k + 1][l + 1]) > max)
						max = obraz[i + k][j + l] + elStr[k + 1][l + 1];

					obrazWynik[i][j] = max;
				}
			}
		}
	}
	return obrazWynik;
}

short int** convertPGMToASCII(short int** obraz, int width, int height) 
{
	
	short int** obrazWynik = new short int*[pgmImage.height];

	for (int i = 0; i < height; i++) {
		obrazWynik[i] = new short int[width];
		for (int j = 0; j < width; j++) {
			obrazWynik[i][j] = floor(obraz[i][j] / nShade);
		}
	}

	return obrazWynik;
}



int main()
{
	PGM::processFile(pgmImage, "cat.pgm");
	nShade = pgmImage.maxVal / (SHADES - 1);

	short int** obrazDylatacja = imageDilation(pgmImage.data);
	short int** obraz = convertPGMToASCII(obrazDylatacja, pgmImage.width, pgmImage.height);
	showImage(obraz);
	getchar();

	short int** obrazErozja = imageErusion(pgmImage.data);
	obraz = convertPGMToASCII(obrazErozja, pgmImage.width, pgmImage.height);
	showImage(obraz);
	getchar();

	obraz = convertPGMToASCII(pgmImage.data, pgmImage.width, pgmImage.height);
	showImage(obraz);
	getchar();


	return 0;
}