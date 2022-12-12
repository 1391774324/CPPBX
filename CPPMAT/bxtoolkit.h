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
#include <iomanip>
#include <tuple>


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
    static vector<vector<double>> OpenDataFile(string dataPath,int lineNum);        //变形抽数入读文件函数（原始数据为行列转置数据。）
    static vector<vector<double>> trip(const vector<vector<double>>&A,const vector<vector<double>>&B,int thre);
    static vector<vector<double>> trip_mor(const vector<vector<double>>&datain, int thre);  // 列向量
    static vector<vector<double>> trip_ass(const vector<vector<double>>&datain, int thre);  // 列向量
    static vector<vector<double>> combine_correct(string correctPath1,string correctPath2, ParaGet parameters);
    static vector<vector<double>> f_value(const vector<vector<double>>&bianxing_data_ori,const vector<vector<double>>&correct_data,ParaGet parameters);
    static vector<vector<double>> f_v2h(const vector<vector<double>>&bianxing_data_vol,ParaGet parameters);
    static vector<vector<double>> f_baseValue(const vector<vector<double>>&data);
    static vector<vector<double>> readDataFileByRowNum(string dataPath, int rowNum);
    static tuple<vector<vector<vector<double>>>,vector<vector<double>>,vector<vector<double>>,double> ployfitdia(const vector<vector<double>>&data2hNobase,const vector<vector<double>>&CirAngle,ParaGet parameters, int step);
    static vector<vector<vector<double>>> polarsystem(const vector<vector<double>>&data,const vector<vector<double>>&datainfor,ParaGet parameters);
    static vector<vector<vector<double>>> circlesystem(const vector<vector<double>>&x,const vector<vector<double>>&y,double circle_a, double circle_b);
    static vector<vector<double>> outlier_circle(double r1,const vector<vector<double>>&x,const vector<vector<double>>&y);
    static double* circle_r(const vector<vector<double>>&A,const vector<vector<double>>&B);
    static double* circlefit(const vector<vector<double>>&A,const vector<vector<double>>&B);
    static double getangle(double PointCircumAngle,int j,double center_0,double one_angle,double chanum);
    static double min_fit(const vector<vector<double>>&x,const vector<vector<double>>&y,double x1, double y1);
    static void writeDataFile(string outputPath, const vector<vector<double>>&Data);
    static void writeLog (string logPath, string logData);

};

#endif // BXTOOLKIT_H
