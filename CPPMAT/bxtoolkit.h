#ifndef BXTOOLKIT_H
#define BXTOOLKIT_H

#include <iostream>
#include "paraget.h"
#include "cppmat.h"
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

class BXToolKit
{
public:
    BXToolKit();

    static void align_extract(string pipesize, string dataPath1, string dataPath2, string correctPath1, string correctPath2, string outPath1, string outPath2);
    static vector<vector<double>> OpenDataFile(string dataPath,int lineNum);
};

#endif // BXTOOLKIT_H
