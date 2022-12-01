#include "bxtoolkit.h"

BXToolKit::BXToolKit()
{

}



void BXToolKit::align_extract(string pipesize, string dataPath1, string dataPath2, string correctPath1, string correctPath2, string outPath1, string outPath2){
    ParaGet *parameters = new ParaGet(pipesize);
    vector<vector<double>> data1;
    data1=OpenDataFile(dataPath1,parameters->datpara.line_num);

}


vector<vector<double>> BXToolKit::OpenDataFile(string dataPath,int lineNum){
    ifstream file;
    file.open(dataPath, ios::binary|ios::in);
    if (file.is_open() == false) {	// 文件打开失败
        cout << "Open file  fail!" << endl;
        exit(0);
    }
    file.seekg(0,ios_base::end);
    streampos filesize=file.tellg();
    int dataSize_double=filesize/8;
    int len = dataSize_double/lineNum;
    file.seekg(0,ios_base::beg);



    vector<vector<double>> A;
    for (int i = 0; i < len; ++i) {
        vector<double> v;
        //double* byteData = new double[len];

        for (int j = 0; j < lineNum; ++j) {
            char* ch=new char[8];
            file.read(ch,sizeof(double));
            double d;
            memcpy(&d,ch,8);
            v.push_back(d);
            delete[](ch);
            //byteData[j]=atof(ch);
        }

        A.push_back(v);
    }

    A=CPPMAT::trans(A);
    return A;

}
