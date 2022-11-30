#ifndef CPPMAT_H
#define CPPMAT_H

#include <iostream>
#include <vector>

#define M_epsilon 1e-12

using namespace std;

class CPPMAT
{
public:
    CPPMAT();
    vector<vector<double>> Matrix;

    //const double epsilon=1e-12;  //小于该数判断为0
    static vector<vector<double>> creatmatrix(int h,int l);     //创建h行l列0矩阵；
    static vector<vector<double>> creatmatrix(int h,int l, vector<double> elements);     //创建h行l列赋值（默认0）矩阵；
    static vector<vector<double>> plus(const vector<vector<double>>&A,const vector<vector<double>>&B);
    static vector<vector<double>> minus(const vector<vector<double>>&A,const vector<vector<double>>&B);
    static vector<vector<double>> multiply(const vector<vector<double>>&A,const vector<vector<double>>&B);
    static vector<vector<double>> multiply_num(const vector<vector<double>>&A,double num);
    static vector<vector<double>> matrix_overlaying_below(const vector<vector<double>>&A,const vector<vector<double>>&B);
    static vector<vector<double>> matrix_overlaying_beside(const vector<vector<double>>&A,const vector<vector<double>>&B);
    static vector<vector<double>> trans(const vector<vector<double>> &A);
    static vector<vector<double>> inverse(const vector<vector<double>> &A);
    static void show_matrix(const vector<vector<double>> &A);
};

#endif // CPPMAT_H
