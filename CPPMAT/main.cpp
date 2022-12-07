#include <iostream>
#include "cppmat.h"
#include "paraget.h"
#include "bxtoolkit.h"
using namespace std;

int main()
{

    /* ******************     对齐抽数算法调用      *******************
    string pipesize = "10inch";
    string dataPath1 = "D:\\ZeYuWorkspace\\bxdata\\BX20221125-111613-1_000.dat1IN.bin";
    string dataPath2 = "";
    string correctPath1 = "";
    string correctPath2 = "";
    string outPath1="D:\\ZeYuWorkspace\\bxoutput\\testdata";
    string outPath2="D:\\ZeYuWorkspace\\bxoutput\\testdata";

    BXToolKit::align_extract(pipesize,dataPath1,dataPath2,correctPath1,correctPath2,outPath1,outPath2);
    */

    /* ******************     有效性算法调用      ******************* */
    string pipesize = "10inch";
    string outPath =  "D:\\ZeYuWorkspace\\bxoutput\\testdata\\analysis";
    string data2hNobase_Path =  "D:\\ZeYuWorkspace\\bxoutput\\testdata\\BX20221125-111613-1_000.dat1IN_data2hNobase.bin";
    string data4analysis_Path = "D:\\ZeYuWorkspace\\bxoutput\\testdata\\BX20221125-111613-1_000.dat1IN_data4analysis.bin";
    string Assis_Path = "D:\\ZeYuWorkspace\bxoutput\\testdata\\BX20221125-111613-1_000.dat1IN_Assis.bin";
    int Flag_valid = 1;
    double Pos_in = 0; //单位m
    double Pos_out = 0; //单位m
    double Window1 = 0.002; //单位m
    double Window2 = 0.002; //单位m
    int Flag_inout = 1;

    BXToolKit::DataValidAnalyse(pipesize,data2hNobase_Path,data4analysis_Path,Assis_Path,outPath,Window1,Window2,Pos_in,Pos_out,Flag_valid,Flag_inout);






//    cout << "Hello World!" << endl;

//    vector<double> num{1,2,3,4,5,6,7,8,9};
//    vector<vector<double>> myMatrix;

//    myMatrix=CPPMAT::creatmatrix(3,3,num);
//    CPPMAT::show_matrix(CPPMAT::subMatrix_step(myMatrix,1,1,3,3,1,1));


    return 0;
}
