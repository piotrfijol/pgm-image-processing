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
	{ 70,80,80 },
	{ 70,50,80 },
	{ 80,90,80 },
};


short int** convertPGMToASCII(short int** obraz, int width, int height)
{

	short int** obrazWynik = new short int* [pgmImage.height];

	for (int i = 0; i < height; i++) {
		obrazWynik[i] = new short int[width];
		for (int j = 0; j < width; j++) {
			obrazWynik[i][j] = floor(obraz[i][j] / nShade);
		}
	}

	return obrazWynik;
}


void showImage(short int** obraz)
{
	short int** img = convertPGMToASCII(obraz, pgmImage.width, pgmImage.height);
	const unsigned char tablicaOdcieni[SHADES] = { 0x2E , 0x2D, 0x3A , 0x3D, 0x2B, 0x23,  0xDB };

	int i,j;
	printf("%c",0xDA);

	for(j=0;j<pgmImage.width;j++)
		printf("%c ",0xC4);


	printf("%c\n",0xBF);

	for(i=0;i<pgmImage.height;i++)
	{
		printf("%c",0xB3);
		for (j = 0;j < pgmImage.width;j++) {
			if (img[i][j] > 6)
				printf("%c ", tablicaOdcieni[SHADES-1]);
			else if (img[i][j] < 0)
				printf("%c ", tablicaOdcieni[0]);
			else
				printf("%c ", tablicaOdcieni[img[i][j]]);

		}
		printf("%c\n",0xB3);
	}

	printf("%c",0xC0);
	for(j=0;j<pgmImage.width;j++)
		printf("%c ",0xC4);
	printf("%c\n",0xD9);

	getchar();
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


string repeat(char character, int n)
{
	string result;
	for (int i = 0; i < n; i++) {
		result += character;
		result += ' ';
	}
	return result;
}

void drawTextFrame(string text, char character)
{
	int len = pgmImage.width;
	int padding = (len - text.length())/2;

	cout << repeat(character, len) << endl;
	cout << '#' << repeat(' ', len-2) << '#' << endl;
	cout << repeat(' ', padding-1) << text << repeat(' ', padding-1) << endl;
	cout << '#' << repeat(' ', len-2) << '#' << endl;
	cout << repeat(character, len) << endl << endl;
}


int main(int argc, char** argv)
{
	PGM::processFile(pgmImage, argv[1]);
	nShade = pgmImage.maxVal / (SHADES - 1);

	drawTextFrame("Oryginalny obraz", '#');
	short int** obraz = pgmImage.data;
	showImage(obraz);

	drawTextFrame("Obraz po erozji", '#');
	short int** obrazErozja = imageErusion(pgmImage.data);
	showImage(obrazErozja);

	drawTextFrame("Obraz po dylatacji", '#');
	short int** obrazDylatacja = imageDilation(pgmImage.data);
	showImage(obrazDylatacja);

	drawTextFrame("Otwarcie", '#');
	showImage(imageDilation(obrazErozja));

	drawTextFrame("Zamkni�cie", '#');
	showImage(imageErusion(obrazDylatacja));

	return 0;
}
