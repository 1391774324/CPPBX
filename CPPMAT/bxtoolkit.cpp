#include "bxtoolkit.h"

BXToolKit::BXToolKit()
{

}


// 对齐抽数算法
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

//    //检查trip返回的Predata1&2
//    CPPMAT::show_matrix(CPPMAT::getColumn(PreData1,1));
//    CPPMAT::show_matrix(CPPMAT::getColumn(PreData2,1));
//    cout<<PreData1.size()<<"---"<<PreData1.at(0).size()<<endl;
//    cout<<PreData2.size()<<"---"<<PreData2.at(0).size()<<endl;

    //变形数据对齐
    int bx_start = parameters->datpara.bx_start;
    int bx_end = parameters->datpara.bx_end;
    int bx_flag = parameters->propara.bx_flag;
    int bx_circle = parameters->propara.bx_circle;
    //变形数据提取
    vector<vector<double>> bianxing_data_ori1=CPPMAT::trans(CPPMAT::getRows(PreData1,bx_start,(bx_end-bx_start+1)));
    vector<vector<double>> bianxing_data_ori2=CPPMAT::trans(CPPMAT::getRows(PreData2,bx_start,(bx_end-bx_start+1)));

//    //检查bianxing_data_ori1&2
//    CPPMAT::show_matrix(CPPMAT::getColumn(bianxing_data_ori1,1));
//    CPPMAT::show_matrix(CPPMAT::getColumn(bianxing_data_ori2,1));
//    cout<<bianxing_data_ori1.size()<<"---"<<bianxing_data_ori1.at(0).size()<<endl;
//    cout<<bianxing_data_ori2.size()<<"---"<<bianxing_data_ori2.at(0).size()<<endl;

    // 传感器对齐
    vector<vector<double>> bianxing_data_ori=CPPMAT::creatmatrix(bianxing_data_ori1.size(),bianxing_data_ori1.at(0).size());
    if(bx_circle==2){
        if(bx_flag==2){     //偶数圈在前
            for (int i = 2; i <= bianxing_data_ori.at(0).size(); i+=2) {
                bianxing_data_ori=CPPMAT::changeColumn(bianxing_data_ori,i,CPPMAT::getColumn(bianxing_data_ori1,i));
            }
            vector<vector<double>> bianxing_data_ori_temp=CPPMAT::matrix_overlaying_below(CPPMAT::subMatrix_step(bianxing_data_ori1,move_length+1,1,bianxing_data_ori1.size(),1,2,bianxing_data_ori1.at(0).size()),CPPMAT::subMatrix_step(bianxing_data_ori2,1,1,move_length,1,2,bianxing_data_ori2.at(0).size()));
            for (int i = 1; i <= bianxing_data_ori.at(0).size(); i+=2) {
                bianxing_data_ori=CPPMAT::changeColumn(bianxing_data_ori,i,CPPMAT::getColumn(bianxing_data_ori_temp,(i+1)/2));
            }
        }
        else{               //奇数圈在前
            for (int i = 1; i <= bianxing_data_ori.at(0).size(); i+=2) {
                bianxing_data_ori=CPPMAT::changeColumn(bianxing_data_ori,i,CPPMAT::getColumn(bianxing_data_ori1,i));
            }
            vector<vector<double>> bianxing_data_ori_temp=CPPMAT::matrix_overlaying_below(CPPMAT::subMatrix_step(bianxing_data_ori1,move_length+1,1,bianxing_data_ori1.size(),2,2,bianxing_data_ori1.at(0).size()),CPPMAT::subMatrix_step(bianxing_data_ori2,1,1,move_length,2,2,bianxing_data_ori2.at(0).size()));
            for (int i = 2; i <= bianxing_data_ori.at(0).size(); i+=2) {
                bianxing_data_ori=CPPMAT::changeColumn(bianxing_data_ori,i,CPPMAT::getColumn(bianxing_data_ori_temp,i/2));
            }
        }
    }
    else {
        bianxing_data_ori=bianxing_data_ori1;
    }

//        //检查bianxing_data_ori
//        CPPMAT::show_matrix(CPPMAT::getRow(bianxing_data_ori,1));
//        cout<<bianxing_data_ori.size()<<"---"<<bianxing_data_ori.at(0).size()<<endl;
//        cout<<bianxing_data_ori[49999][34]<<endl;

    // 数据校准
    double volbase = parameters->propara.volbase;
    double bx_cha = parameters->propara.chanum;
    double a = parameters->propara.a;
    double r = parameters->propara.r;

    if(!correctPath1.empty()){
        vector<vector<double>> correctData=combine_correct(correctPath1,correctPath2,*parameters);
//        //检查 correctData
//        CPPMAT::show_matrix(CPPMAT::getRow(correctData,1));
//        cout<<correctData.size()<<"---"<<correctData.at(0).size()<<endl;
        vector<vector<double>> bianxing_data_correct_vol = f_value(bianxing_data_ori,correctData,*parameters);
//        //检查 bianxing_data_correct_vol
//        CPPMAT::show_matrix(CPPMAT::getRow(bianxing_data_correct_vol,1));
//        cout<<bianxing_data_correct_vol.size()<<"---"<<bianxing_data_correct_vol.at(0).size()<<endl;

        // 高度转换
        vector<vector<double>> data2hNobase=f_v2h(bianxing_data_correct_vol,*parameters);
        //检查 data2hNobase
        CPPMAT::show_matrix(CPPMAT::getRow(data2hNobase,1));
        cout<<data2hNobase.size()<<"---"<<data2hNobase.at(0).size()<<endl;

    }




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

    file.close();
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

vector<vector<double>> BXToolKit::combine_correct(string correctPath1,string correctPath2, ParaGet parameters){
    double line_num = parameters.datpara.line_num;
    double bx_cha=parameters.propara.chanum;
    double bx_start = parameters.datpara.bx_start;



    vector<vector<double>> PreData1,PreData2,standata1,standata2;
    PreData1=CPPMAT::trans(OpenDataFile(correctPath1,line_num));
    PreData2=CPPMAT::trans(OpenDataFile(correctPath2,line_num));

    int center1,center2;
    center1=floor(PreData1.size()/2);
    center2=floor(PreData2.size()/2);

//    cout<<center1<<"---"<<center2<<endl;
//    cout<<PreData1.size()<<"---"<<PreData2.size()<<endl;

    standata1=CPPMAT::subMatrix_step(PreData1,(center1-100),1,(center1+100),bx_start,1,bx_start+bx_cha/2-1);
    standata2=CPPMAT::subMatrix_step(PreData2,(center2-100),1,(center2+100),(bx_start+bx_cha/2),1,(bx_start+bx_cha-1));

    return CPPMAT::matrix_overlaying_beside(standata1,standata2);

}

vector<vector<double>> BXToolKit::f_value(const vector<vector<double>>&bianxing_data_ori,const vector<vector<double>>&correct_data,ParaGet parameters){
    vector<double> v;
    vector<vector<double>> chanl_correct,BiasValue,bianxing_data_correct;
    double volbase = parameters.propara.volbase;
    double bx_cha=parameters.propara.chanum;
    for (int i = 1; i <= bx_cha; ++i) {
        v.push_back(CPPMAT::matrix_mean(CPPMAT::getColumn(correct_data,i)));
    }
    chanl_correct.push_back(v);
    //BiasValue=CPPMAT::minus(chanl_correct,CPPMAT::multiply_num(CPPMAT::creatmatrix_ones(1,chanl_correct.at(0).size()),volbase));
    BiasValue=CPPMAT::minus(chanl_correct,CPPMAT::creatmatrix_Nums(1,chanl_correct.at(0).size(),volbase));
    for (int i = 1; i <= bx_cha; ++i) {
        if(i==1){
            bianxing_data_correct=CPPMAT::minus(CPPMAT::getColumn(bianxing_data_ori,i),CPPMAT::creatmatrix_Nums(bianxing_data_ori.size(),1,BiasValue.at(0).at(i-1)));
        }
        else {
            bianxing_data_correct=CPPMAT::matrix_overlaying_beside(bianxing_data_correct,CPPMAT::minus(CPPMAT::getColumn(bianxing_data_ori,i),CPPMAT::creatmatrix_Nums(bianxing_data_ori.size(),1,BiasValue.at(0).at(i-1))));
        }
    }
    return bianxing_data_correct;
}

vector<vector<double>> BXToolKit::f_v2h(const vector<vector<double>>&bianxing_data_vol,ParaGet parameters){
    double a = parameters.propara.a;
    double r = parameters.propara.r;
    int row=bianxing_data_vol.size();
    int col=bianxing_data_vol.at(0).size();
    vector<vector<double>> alpha,bianxing_data_h;
    alpha=CPPMAT::minus(CPPMAT::creatmatrix_Nums(row,col,4.5),bianxing_data_vol);
    alpha=CPPMAT::divide_num(alpha,0.055);
    alpha=CPPMAT::multiply_num(alpha,M_PI);
    alpha=CPPMAT::divide_num(alpha,180);


    bianxing_data_h=CPPMAT::multiply_num(CPPMAT::sin_matrix(alpha),a);
    bianxing_data_h=CPPMAT::plus(bianxing_data_h,CPPMAT::creatmatrix_Nums(bianxing_data_h.size(),bianxing_data_h.at(0).size(),r));
    return bianxing_data_h;
}
