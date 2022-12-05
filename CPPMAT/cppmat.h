#ifndef CPPMAT_H
#define CPPMAT_H

#include <iostream>
#include <vector>
#include <math.h>
#include <numeric>
#include <algorithm>

#define M_epsilon 1e-12

using namespace std;

class CPPMAT
{
public:
    CPPMAT();
    vector<vector<double>> Matrix;

    // Matrixs operating Functions
    static vector<vector<double>> creatmatrix(int h,int l);                                                                 //创建h行l列0矩阵；
    static vector<vector<double>> creatmatrix_ones(int h,int l);                                                            //创建h行l列1矩阵；
    static vector<vector<double>> creatmatrix_Nums(int h,int l,double num);                                                 //创建h行l列Nums矩阵；
    static vector<vector<double>> creatmatrix(int h,int l, vector<double> elements);                                        //创建h行l列赋值（默认0）矩阵；
    static vector<vector<double>> plus(const vector<vector<double>>&A,const vector<vector<double>>&B);                      //矩阵加法（A+B）
    static vector<vector<double>> minus(const vector<vector<double>>&A,const vector<vector<double>>&B);                     //矩阵减法（A-B）
    static vector<vector<double>> multiply(const vector<vector<double>>&A,const vector<vector<double>>&B);                  //矩阵乘法（A*B）
    static vector<vector<double>> multiply_dot(const vector<vector<double>>&A,const vector<vector<double>>&B);              //矩阵点乘（A.*B）
    static vector<vector<double>> multiply_num(const vector<vector<double>>&A,double num);                                  //矩阵数乘（n*A）
    static vector<vector<double>> divide_num(const vector<vector<double>>&A,double num);                                    //矩阵数除（A./n）
    static vector<vector<double>> sin_matrix(const vector<vector<double>>&A);                                               //矩阵sin
    static vector<vector<double>> cos_matrix(const vector<vector<double>>&A);                                               //矩阵cos
    static vector<vector<double>> subMatrix(const vector<vector<double>> &A,int r, int r_step, int c,int c_step);           //取子矩阵（原矩阵，行，行跨度，列，列跨度）
    static vector<vector<double>> subMatrix_step(const vector<vector<double>> &A,int r, int r_step, int r_end, int c,int c_step, int c_end);    //正序逆序取子矩阵
    static vector<vector<double>> getRow(const vector<vector<double>> &A, int num);                                         //取行向量
    static vector<vector<double>> getRows(const vector<vector<double>> &A, int r,int r_step);                               //取多个行向量
    static vector<vector<double>> getColumn(const vector<vector<double>> &A, int num);                                      //取列向量
    static vector<vector<double>> getColumns(const vector<vector<double>> &A, int c, int c_step);                           //取多个列向量
    static vector<vector<double>> matrix_overlaying_below(const vector<vector<double>>&A,const vector<vector<double>>&B);   //矩阵上下拼接
    static vector<vector<double>> matrix_overlaying_beside(const vector<vector<double>>&A,const vector<vector<double>>&B);  //矩阵左右拼接
    static vector<vector<double>> trans(const vector<vector<double>> &A);                                                   //矩阵转置
    static vector<vector<double>> inverse(const vector<vector<double>> &A);                                                 //矩阵求逆
    static vector<vector<double>> changeRow(const vector<vector<double>>&A,int row, const vector<vector<double>>&B);        //更改行向量
    static vector<vector<double>> changeColumn(const vector<vector<double>>&A,int column, const vector<vector<double>>&B);  //更改列向量
    static double matrix_sum (const vector<vector<double>> &A);                                                             //向量求和
    static double matrix_mean (const vector<vector<double>> &A);                                                            //均值
    static double matrix_variance (const vector<vector<double>> &A);                                                        //方差
    static double matrix_standard_deviation (const vector<vector<double>> &A);                                              //标准差
    static double matrix_mid (const vector<vector<double>> &A);                                                             //中位数
    static void show_matrix(const vector<vector<double>> &A);                                                               //打印矩阵

};

#endif // CPPMAT_H
