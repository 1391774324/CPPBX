#ifndef BXTOOLKIT_H
#define BXTOOLKIT_H

#include <iostream>
#include "paraget.h"
#include "cppmat.h"
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>


using namespace std;

class BXToolKit
{
public:
    BXToolKit();

    // 对齐抽数算法
    static void align_extract(string pipesize, string dataPath1, string dataPath2, string correctPath1, string correctPath2, string outPath1, string outPath2);
    // 有效性算法
    static void DataValidAnalyse(string pipesize,string data2hNobase_Path, string data4analysis_Path, string Assis_Path, string outPath, double Window1, double Window2, double Pos_in, double Pos_out, int Flag_valid, int Flag_inout);

    // 其他工具算法
    static vector<vector<double>> OpenDataFile(string dataPath,int lineNum);
    static vector<vector<double>> trip(const vector<vector<double>>&A,const vector<vector<double>>&B,int thre);
    static vector<vector<double>> combine_correct(string correctPath1,string correctPath2, ParaGet parameters);
    static vector<vector<double>> f_value(const vector<vector<double>>&bianxing_data_ori,const vector<vector<double>>&correct_data,ParaGet parameters);
    static vector<vector<double>> f_v2h(const vector<vector<double>>&bianxing_data_vol,ParaGet parameters);
    static vector<vector<double>> f_baseValue(const vector<vector<double>>&data);
    static void writeLog (string logPath, string logData);

};

#endif // BXTOOLKIT_H
