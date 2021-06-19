#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

namespace PGM 
{

    struct PGM_FILE 
    {
        string pgmType;
        unsigned int maxVal;
        unsigned int width;
        unsigned int height;
        short int** data;
    };

    void skipComments(ifstream& file) 
    {
        string line;
        char ch = ' ';

        while (isspace(ch)) 
        {
            file.get(ch);
        }

        if (ch == '#') 
        {
            getline(file, line);
        }
        else 
        {
            file.seekg(-1, file.cur);
        }
    }

    void encodeFile(PGM_FILE& pgmFile, string newFileName) 
    {
        ofstream file;
        file.open(newFileName, ofstream::binary);

        if (file.is_open()) 
        {
            file << pgmFile.pgmType << endl;
            file << "# encoded file, created by Piotr Fijol." << endl;
            file << pgmFile.width << "  " << pgmFile.height << endl;
            file << pgmFile.maxVal << endl;

            short int s0 = 160;
            short int s1 = 160;
            short int e = 160;

            for (int i = 0; i < pgmFile.height; i++) 
            {
                for (int j = 0; j < pgmFile.width; j++) 
                {
                    if (j % 12 == 0 && j != 0)
                        file << endl;
                    file << e << "  ";
                    e = pgmFile.data[i][j] - floor((s0 + s1) / 2);
                    s0 = s1;
                    s1 = pgmFile.data[i][j];
                }
                file << " " << endl;
            }

            file.close();
        }
    }

    void decodeFile(PGM_FILE& pgmFile, string newFileName) 
    {
        ofstream file;
        file.open(newFileName, ifstream::binary);

        if (file.is_open()) 
        {
            file << pgmFile.pgmType << endl;
            file << "# decoded file, created by Piotr Fijol." << endl;
            file << pgmFile.width << "  " << pgmFile.height << endl;
            file << pgmFile.maxVal << endl;


            short int s0 = 160;
            short int s1 = 160;
            short int s;


            for (int i = 0; i < pgmFile.height; i++) 
            {
                for (int j = 0; j < pgmFile.width; j++) 
                {
                    if (i == 0 && j == 0)
                        continue;
                    if (j % 12 == 0 && j != 0)
                        file << endl;

                    s = (pgmFile.data[i][j] + floor((s0 + s1) / 2));

                    file << s << "  ";
                    s0 = s1;
                    s1 = s;
                }
                file << " " << endl;
            }


            file.close();

        }
    }


    void processFile(PGM_FILE& pgmFile, string fileName) 
    {

        ifstream file;
        file.open(fileName, ios::binary);

        if (file.is_open()) 
        {

            getline(file, pgmFile.pgmType);

            if (pgmFile.pgmType == "P5") 
            {

                skipComments(file);

                file >> pgmFile.width;
                file >> pgmFile.height;
                file >> pgmFile.maxVal;

                pgmFile.data = new short int* [pgmFile.height];

                stringstream buffer;
                buffer << file.rdbuf();
                string data = buffer.str();

                for (int i = 0; i < pgmFile.height; i++) 
                {
                    pgmFile.data[i] = new short int[pgmFile.width];
                    for (int j = 0; j < pgmFile.width; j++) 
                    {
                        pgmFile.data[i][j] = data[i * pgmFile.width + j];
                    }

                }
            } 
            else 
            {
                file.close();

                file.open(fileName);

                getline(file, pgmFile.pgmType);
                
                skipComments(file);

                file >> pgmFile.width;
                file >> pgmFile.height;
                file >> pgmFile.maxVal;

                pgmFile.data = new short int* [pgmFile.height];


                for (int i = 0; i < pgmFile.height; i++) 
                {
                    pgmFile.data[i] = new short int[pgmFile.width];
                    for (int j = 0; j < pgmFile.width; j++) 
                    {
                        file >> pgmFile.data[i][j];
                    }

                }
                file.close();
            }
        }
        else 
        {
            cout << "Error occured while opening file"; exit(1);
        }

    }

}