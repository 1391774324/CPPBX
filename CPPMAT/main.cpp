#include <iostream>
#include "cppmat.h"
#include "paraget.h"
#include "bxtoolkit.h"
using namespace std;

int main()
{

    /* ******************     对齐抽数算法调用      **********************
    string pipesize = "10inch";
    string dataPath1 = "D:\\ZeYuWorkspace\\bxdata\\BX20221125-111613-1_000.dat1IN.bin";
    string dataPath2 = "D:\\ZeYuWorkspace\\bxdata\\BX20221125-111613-1_000.dat2IN.bin";
    string correctPath1 = "D:\\ZeYuWorkspace\\bxdata\\correct\\BX20221121-154134-1_000.dat1IN.bin";
    string correctPath2 = "D:\\ZeYuWorkspace\\bxdata\\correct\\BX20221121-154134-1_000.dat1IN.bin";
    string outPath1="D:\\ZeYuWorkspace\\bxoutput\\testdata";
    string outPath2="D:\\ZeYuWorkspace\\bxoutput\\testdata";

    BXToolKit::align_extract(pipesize,dataPath1,dataPath2,correctPath1,correctPath2,outPath1,outPath2);
*/

    /* ******************     有效性算法调用      ***********************/
    string pipesize = "10inch";
    string outPath =  "D:\\ZeYuWorkspace\\bxoutput\\testdata\\analysis";
    string data2hNobase_Path =  "D:\\ZeYuWorkspace\\bxoutput\\testdata\\BX20221125-111613-1_000.dat1IN_data2hNobase.bin";
    string data4analysis_Path = "D:\\ZeYuWorkspace\\bxoutput\\testdata\\BX20221125-111613-1_000.dat1IN_data4analysis.bin";
    string Assis_Path = "D:\\ZeYuWorkspace\\bxoutput\\testdata\\BX20221125-111613-1_000.dat1IN_Assis.bin";
    int Flag_valid = 1;
    double Pos_in = 0; //单位m
    double Pos_out = 0; //单位m
    double Window1 = 0.002; //单位m
    double Window2 = 0.002; //单位m
    int Flag_inout = 1;

    BXToolKit::DataValidAnalyse(pipesize,data2hNobase_Path,data4analysis_Path,Assis_Path,outPath,Window1,Window2,Pos_in,Pos_out,Flag_valid,Flag_inout);



    //测试circlefit
//    vector<double> x_data{-107.937879608015,-95.5341523935576,	-80.9977809390377,	-63.3901890576661,	-43.8287901881214,	-22.8984404369545,	-1.30020812261445,	20.2283163200301,	41.3628087443513,	61.0663969359885,	78.5062097330284,	94.3234269446122,	106.848176598943,	116.158260528276,	122.301392237517,	124.558708275625,	121.924455093875,	116.900463123445,	107.312968543382,	95.6804021041508,	80.3587203840495,	62.7266828255180,	43.7149119913823,	22.7368779015380,	1.29220302091274,	-19.9777208602227,	-40.9707666905716,	-60.4422629311365,	-78.0878536008025,	-93.9070587412946,	-106.116132043128,	-115.269274850067,	-121.884848374238,	-123.774539141859,	-122.217685126520,	-116.460109895302};
//    vector<double> y_data{60.8198694516837,	78.4726374740778,	94.5017325412632,	107.186955979267,	116.604606593656,	122.356734380626,	124.156191241667,	122.189353631347,	117.456061709487,	108.375395409362,	95.5750239305162,	80.8449545593578,	63.1898354906543,	43.6610197318561,	22.8880834363851,	1.30442342522481,	-20.1844626515275,	-41.1671515963197,	-60.4677501631823,	-78.5927683407108,	-93.7561277982073,	-106.065028211667,	-116.301638561152,	-121.493432607467,	-123.391788281486,	-120.675629168452,	-116.342797957355,	-107.267735993412,	-95.0657088395834,	-80.4880838479391,	-62.7569055471943,	-43.3268719832494,	-22.8101293712415,	-1.29621132507080,	20.2330065686412,	41.0120787453274};
//    vector<vector<double>> x,y;
//    x = CPPMAT::creatmatrix(1,x_data.size(),x_data);
//    y = CPPMAT::creatmatrix(1,y_data.size(),y_data);
//    BXToolKit::circle_r(x,y);


//    cout << "Hello World!" << endl;

//    vector<double> num{1,2,3,4,5,6,7,8,9};
//    vector<vector<double>> myMatrix;

//    myMatrix=CPPMAT::creatmatrix(3,3,num);
//    CPPMAT::show_matrix(CPPMAT::subMatrix_step(myMatrix,1,1,3,3,1,1));


    return 0;
}
