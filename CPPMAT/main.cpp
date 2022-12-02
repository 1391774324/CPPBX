#include <iostream>
#include "cppmat.h"
#include "paraget.h"
#include "bxtoolkit.h"
using namespace std;

int main()
{

    string pipesize = "10inch";
    string dataPath1 = "D:\\ZeYuWorkspace\\bxdata\\BX20221125-111613-1_000.dat1IN.bin";
    string dataPath2 = "D:\\ZeYuWorkspace\\bxdata\\BX20221125-111613-1_000.dat2IN.bin";
    string correctPath1 = "D:\\ZeYuWorkspace\\bxdata\\correct\\BX20221121-154134-1_000.dat1IN.bin";
    string correctPath2 = "D:\\ZeYuWorkspace\\bxdata\\correct\\BX20221121-154134-1_000.dat1IN.bin";
    string outPath1="D:\\ZeYuWorkspace\\bxoutput";
    string outPath2="D:\\ZeYuWorkspace\\bxoutput";

    BXToolKit::align_extract(pipesize,dataPath1,dataPath2,correctPath1,correctPath2,outPath1,outPath2);









//    cout << "Hello World!" << endl;

//    vector<double> num{1,2,3,4,5,6,7,8,9};
//    vector<vector<double>> myMatrix;

//    myMatrix=CPPMAT::creatmatrix(3,3,num);
//    CPPMAT::show_matrix(CPPMAT::subMatrix_step(myMatrix,1,1,3,3,1,1));


    return 0;
}
