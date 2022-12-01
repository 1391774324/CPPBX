#include "bxtoolkit.h"

BXToolKit::BXToolKit()
{

}



void BXToolKit::align_extract(string pipesize, string dataPath1, string dataPath2, string correctPath1, string correctPath2, string outPath1, string outPath2){
    ParaGet *parameters = new ParaGet(pipesize);

    // 提取数据
    vector<vector<double>> PreData1;
    vector<vector<double>> PreData2;
    PreData1=OpenDataFile(dataPath1,parameters->datpara.line_num);
    if(!dataPath2.empty()){
        PreData2=OpenDataFile(dataPath2,parameters->datpara.line_num);
    }
    else {
        PreData2=PreData1;
    }

    double move_length=parameters->propara.move_length;     //传感器对齐
    // 基于帧记数处理数据 SW
    PreData1=trip(CPPMAT::getRow(PreData1,1),PreData1,0);
    PreData2=trip(CPPMAT::subMatrix(PreData2,1,1,1,move_length+5),CPPMAT::getColumns(PreData2,1,move_length+5),0);



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
        double* byteData = new double[lineNum];
        file.read((char*)byteData,sizeof(double)*lineNum);
        for (int j = 0; j < lineNum; ++j) {

            v.push_back(byteData[j]);
//            char* ch=new char[8];
//            file.read(ch,sizeof(double));
//            double d;
//            memcpy(&d,ch,8);
//            v.push_back(d);
//            delete[](ch);
        }
        delete[](byteData);
        A.push_back(v);
    }

    A=CPPMAT::trans(A);
    return A;

}

vector<vector<double>> BXToolKit::trip(const vector<vector<double>>&datain1,const vector<vector<double>>&datain2,int thre){
    vector<vector<double>> dataout;
    dataout=datain2;
    for (int i = 2; i < datain1.at(0).size(); ++i) {
        if((datain1[0][i-1]-datain1[0][i-2]<0)&&(datain1[0][i]-datain1[0][i-2]>0)){
            vector<vector<double>> temp;
            temp=CPPMAT::plus(CPPMAT::getColumn(dataout,i-1),CPPMAT::getColumn(dataout,i+1));
            temp=CPPMAT::multiply_num(temp,0.5);
            dataout=CPPMAT::changeColumn(dataout,i,temp);
        }
    }
    return dataout;
}
