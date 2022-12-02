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

    // 对齐抽数算法
    static void align_extract(string pipesize, string dataPath1, string dataPath2, string correctPath1, string correctPath2, string outPath1, string outPath2);

    // 其他工具算法
    static vector<vector<double>> OpenDataFile(string dataPath,int lineNum);
    static vector<vector<double>> trip(const vector<vector<double>>&A,const vector<vector<double>>&B,int thre);
    static vector<vector<double>> combine_correct(string correctPath1,string correctPath2, ParaGet parameters);
    static vector<vector<double>> f_value(const vector<vector<double>>&bianxing_data_ori,const vector<vector<double>>&correct_data,ParaGet parameters);
    static vector<vector<double>> f_v2h(const vector<vector<double>>&bianxing_data_vol,ParaGet parameters);

};

#endif // BXTOOLKIT_H
