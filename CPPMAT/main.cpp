#include <iostream>
#include "cppmat.h"
#include "paraget.h"

using namespace std;

int main()
{

    ParaGet *p = new ParaGet("24inch");
    cout<<p->propara.zxflag<<endl;







//    cout << "Hello World!" << endl;

//    vector<double> num{1,2,3,4,5,6};
//    vector<vector<double>> myMatrix;

//    myMatrix=CPPMAT::creatmatrix(3,3,num);
//    CPPMAT::show_matrix(myMatrix);


    return 0;
}
