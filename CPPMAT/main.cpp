#include <iostream>
#include "cppmat.h"
#include "paraget.h"
#include "bxtoolkit.h"
using namespace std;

int main()
{

    string pipesize = "10inch";
    string dataPath1 = "D:\\ZeYuWorkspace\\bxdata\\BX20221125-111613-1_000.dat1IN.bin";
    string dataPath2 = "D:\\ZeYuWorkspace\\bxdata\\BX20221125-111613-1_000.dat1IN.bin";
    string correctPath1 = "";
    string correctPath2 = "";
    string outPath1="";
    string outPath2="";

    BXToolKit::align_extract(pipesize,dataPath1,dataPath2,correctPath1,correctPath2,outPath1,outPath2);









//    cout << "Hello World!" << endl;

//    vector<double> num{1,2,3,4,5,6};
//    vector<vector<double>> myMatrix;

//    myMatrix=CPPMAT::creatmatrix(3,3,num);
//    CPPMAT::show_matrix(myMatrix);


    return 0;
}
