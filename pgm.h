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

                file.open(fileName, file.binary);

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
