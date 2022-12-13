#include "bxtoolkit.h"

BXToolKit::BXToolKit()
{

}


// 对齐抽数算法
void BXToolKit::align_extract(string pipesize, string dataPath1, string dataPath2, string correctPath1, string correctPath2, string outPath1, string outPath2){

    cout<<"对齐抽数算法运行开始，请稍后......"<<endl;

    ParaGet *parameters = new ParaGet(pipesize);

    // 提取数据
    cout<<"正在提取数据..."<<endl;
    vector<vector<double>> PreData1;
    vector<vector<double>> PreData2;
    PreData1=OpenDataFile(dataPath1,parameters->datpara.line_num);
    if(!dataPath2.empty()){
        PreData2=OpenDataFile(dataPath2,parameters->datpara.line_num);
    }
    else {
        PreData2=PreData1;
    }
    cout<<"数据提取成功..."<<endl;


    double move_length=parameters->propara.move_length;     //传感器对齐
    // 基于帧记数处理数据 SW
    cout<<"正在基于帧记数处理数据..."<<endl;
    PreData1=trip(CPPMAT::getRow(PreData1,1),PreData1,0);
    PreData2=trip(CPPMAT::subMatrix(PreData2,1,1,1,move_length+5),CPPMAT::getColumns(PreData2,1,move_length+5),0);
    cout<<"基于帧记数处理数据完成..."<<endl;
//    //检查trip返回的Predata1&2
//    CPPMAT::show_matrix(CPPMAT::getColumn(PreData1,1));
//    CPPMAT::show_matrix(CPPMAT::getColumn(PreData2,1));
//    cout<<PreData1.size()<<"---"<<PreData1.at(0).size()<<endl;
//    cout<<PreData2.size()<<"---"<<PreData2.at(0).size()<<endl;

    //变形数据对齐
    cout<<"正在进行变形数据对齐..."<<endl;
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
    cout<<"传感器对齐完成..."<<endl;
//        //检查bianxing_data_ori
//        CPPMAT::show_matrix(CPPMAT::getRow(bianxing_data_ori,1));
//        cout<<bianxing_data_ori.size()<<"---"<<bianxing_data_ori.at(0).size()<<endl;
//        cout<<bianxing_data_ori[49999][34]<<endl;

    // 数据校准
    cout<<"正在进行数据校准..."<<endl;
    double volbase = parameters->propara.volbase;
    double bx_cha = parameters->propara.chanum;
    double a = parameters->propara.a;
    double r = parameters->propara.r;

    vector<vector<double>> data2hbase,data2hNobase,data4analysis;
    data4analysis=bianxing_data_ori;
//    //检查data4analysis
//    CPPMAT::show_matrix(CPPMAT::getRow(data4analysis,1));
//    cout<<data4analysis.size()<<"---"<<data4analysis.at(0).size()<<endl;
//    cout<<data4analysis[49999][34]<<endl;
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
        data2hNobase=f_v2h(bianxing_data_correct_vol,*parameters);
//        //检查 data2hNobase
//        CPPMAT::show_matrix(CPPMAT::getRow(data2hNobase,1));
//        cout<<data2hNobase.size()<<"---"<<data2hNobase.at(0).size()<<endl;

        data2hbase = f_baseValue(data2hNobase);
//        //检查 data2hbase
//        CPPMAT::show_matrix(CPPMAT::getRow(data2hbase,1));
//        cout<<data2hbase.size()<<"---"<<data2hbase.at(0).size()<<endl;

        cout<<"数据校准完成..."<<endl;
    }
    else {
        vector<vector<double>> bianxing_data_correct_vol = bianxing_data_ori;
        data2hNobase = f_v2h(bianxing_data_correct_vol,*parameters);
        data2hbase=f_baseValue(data2hNobase);

        cout<<"无校准数据..."<<endl;
    }




    // 变形数据存储
    cout<<"数据存储中..."<<endl;
    string filename=outPath1+dataPath1.substr(dataPath1.find_last_of("\\"),dataPath1.find_last_of("."));
    ofstream ofs(filename,ios::binary | ios::out);
    int len=data2hbase.at(0).size()*data2hbase.size();
    double* outputData = new double[len];
    for (int i = 0; i < data2hbase.size(); ++i) {
        for (int j = 0; j < data2hbase.at(0).size(); ++j) {
            outputData[j+i*data2hbase.at(0).size()]=data2hbase[i][j];
        }
    }
    ofs.write((const char*)outputData,sizeof(double)*len);
    delete[](outputData);
    ofs.close();
    cout<<"变形数据存储完成..."<<endl;

    string filename_data2hNobase = outPath2+dataPath1.substr(dataPath1.find_last_of("\\"),dataPath1.find_last_of(".")-dataPath1.find_last_of("\\"))+("_data2hNobase.bin");
    data2hNobase=CPPMAT::trans(data2hNobase);
    ofstream ofs_data2hNobase(filename_data2hNobase,ios::binary|ios::out);
    int len_data2hNobase = data2hNobase.size()*data2hNobase.at(0).size();
    double * outputData_data2hNobase = new double[len_data2hNobase];
    for (int i = 0; i < data2hNobase.size(); ++i) {
        for (int j = 0; j < data2hNobase.at(0).size(); ++j) {
            outputData_data2hNobase[j+i*data2hNobase.at(0).size()]=data2hNobase[i][j];
        }
    }
    ofs_data2hNobase.write((const char*)outputData_data2hNobase,sizeof(double)*len_data2hNobase);
    delete[](outputData_data2hNobase);
    ofs_data2hNobase.close();
    cout<<"变形数据data2hNobase存储完成..."<<endl;

    string filename_data4analysis = outPath2+dataPath1.substr(dataPath1.find_last_of("\\"),dataPath1.find_last_of(".")-dataPath1.find_last_of("\\"))+("_data4analysis.bin");
    data4analysis=CPPMAT::trans(data4analysis);
    ofstream ofs_data4analysis(filename_data4analysis,ios::binary|ios::out);
    int len_data4analysis = data4analysis.size()*data4analysis.at(0).size();
    double * outputData_data4analysis = new double[len_data4analysis];
    for (int i = 0; i < data4analysis.size(); ++i) {
        for (int j = 0; j < data4analysis.at(0).size(); ++j) {
            outputData_data4analysis[j+i*data4analysis.at(0).size()]=data4analysis[i][j];
        }
    }
    ofs_data4analysis.write((const char*)outputData_data4analysis,sizeof(double)*len_data4analysis);
    delete[](outputData_data4analysis);
    ofs_data4analysis.close();
    cout<<"变形数据data4analysis存储完成..."<<endl;


    // 辅助信息提取+存储
    double zx_chal = parameters->datpara.zxchal;
    double qj_chal = parameters->datpara.qjchal;
    double hx_chal = parameters->datpara.hxchal;
    double mile1_cha = parameters->datpara.mile1_cha;
    double mile2_cha = parameters->datpara.mile2_cha;
    double mile3_cha = parameters->datpara.mile3_cha;
    double time_chal = parameters->datpara.timechal;
    double vol_chal = parameters->datpara.volchal;
    double temp2_chal = parameters->datpara.temp2chal;

    // 优选里程存储
    vector<vector<double>> Assis;
    Assis.push_back(CPPMAT::getRow(PreData1,1).at(0));
    string filename2=outPath2+"\\mile.bin";
    int file2_len = Assis.at(0).size();
    double* outputData_file2 = new double[file2_len];
    for (int i = 0; i < file2_len; ++i) {
        outputData_file2[i]=Assis.at(0).at(i);
    }
    ofstream ofs2(filename2,ios::binary | ios::out);
    ofs2.write((const char*)outputData_file2,sizeof(double)*file2_len);
    delete[](outputData_file2);
    ofs2.close();
    cout<<"优选里程数据存储完成..."<<endl;

    // 三路里程存储
    Assis=CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,mile1_cha));
    Assis=CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,mile2_cha));
    Assis=CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,mile3_cha));
    Assis=CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,zx_chal));
    string filename3=outPath2+"\\zhou.bin";
    int file3_len = Assis.at(0).size();
    double* outputData_file3 = new double[file3_len];
    for (int i = 0; i < file3_len; ++i) {
        outputData_file3[i]=Assis.at(4).at(i);      //Assis 第五行为周向角数据
    }
    ofstream ofs3(filename3,ios::binary | ios::out);
    ofs3.write((const char*)outputData_file3,sizeof(double)*file3_len);
    delete[](outputData_file3);
    ofs3.close();
    cout<<"周向数据存储完成..."<<endl;

    //倾角，航向角，时间，电压，接口板温度存储
    Assis = CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,qj_chal));
    Assis = CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,hx_chal));
    Assis = CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,time_chal));
    Assis = CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,vol_chal));
    Assis = CPPMAT::matrix_overlaying_below(Assis,CPPMAT::getRow(PreData1,temp2_chal));
    string filename_Assis = outPath2+dataPath1.substr(dataPath1.find_last_of("\\"),dataPath1.find_last_of(".")-dataPath1.find_last_of("\\"))+("_Assis.bin");
    ofstream ofs_Assis(filename_Assis,ios::binary|ios::out);
    int len_Assis = Assis.size()*Assis.at(0).size();
    double * outputData_Assis = new double[len_Assis];
    for (int i = 0; i < Assis.size(); ++i) {
        for (int j = 0; j < Assis.at(0).size(); ++j) {
            outputData_Assis[j+i*Assis.at(0).size()]=Assis[i][j];
        }
    }
    ofs_Assis.write((const char*)outputData_Assis,sizeof(double)*len_Assis);
    delete[](outputData_Assis);
    ofs_Assis.close();
    cout<<"变形数据Assis存储完成..."<<endl;

//    cout<<data2hNobase.size()<<"---"<<data2hNobase.at(0).size()<<endl;
//    cout<<data4analysis.size()<<"---"<<data4analysis.at(0).size()<<endl;
//    cout<<Assis.size()<<"---"<<Assis.at(0).size()<<endl;
//    CPPMAT::show_matrix(CPPMAT::getColumn(data2hNobase,1));

    cout<<"对齐抽数算法运行完成！"<<endl;

}

// 有效性算法
vector<double> BXToolKit::DataValidAnalyse(string pipesize,string data2hNobase_Path, string data4analysis_Path, string Assis_Path, string outPath, double Window1, double Window2, double Pos_in, double Pos_out, int Flag_valid, int Flag_inout){
    cout<<"有效性算法运行开始，请稍后......"<<endl;

    ParaGet *parameters = new ParaGet(pipesize);

    // 准备写日志文件
    BXToolKit::writeLog(outPath,"开始进行有效性分析;");

    // 读文件
    vector<vector<double>> data2hNobase,data4analysis,Assis;
    data2hNobase=CPPMAT::trans(BXToolKit::readDataFileByRowNum(data2hNobase_Path,parameters->propara.chanum));
    data4analysis = CPPMAT::trans(BXToolKit::readDataFileByRowNum(data4analysis_Path,parameters->propara.chanum));
    Assis = CPPMAT::trans(BXToolKit::readDataFileByRowNum(Assis_Path,10));

//    // 检查 data2hNobase,data4analysis,Assis
//    cout<<data2hNobase.size()<<"---"<<data2hNobase.at(0).size()<<endl;
//    cout<<data4analysis.size()<<"---"<<data4analysis.at(0).size()<<endl;
//    cout<<Assis.size()<<"---"<<Assis.at(0).size()<<endl;
//    CPPMAT::show_matrix(CPPMAT::getRow(data2hNobase,1));

    BXToolKit::writeLog(outPath,"数据加载完毕;");
    cout<<"数据加载完毕..."<<endl;

    // 分解数据
    vector<vector<double>> Mile,mile11,mile21,mile31,zx1,qj1,hx1,Time,vol1,temp2,licheng,mile1,mile2,mile3,zx,qj,hx,vol,temp,speed;
    Mile=CPPMAT::getColumn(Assis,1);
    mile11=CPPMAT::getColumn(Assis,2);
    mile21=CPPMAT::getColumn(Assis,3);
    mile31=CPPMAT::getColumn(Assis,4);
    zx1=CPPMAT::getColumn(Assis,5);
    qj1=CPPMAT::getColumn(Assis,6);
    hx1=CPPMAT::getColumn(Assis,7);
    Time=CPPMAT::getColumn(Assis,8);
    vol1=CPPMAT::getColumn(Assis,9);
    temp2=CPPMAT::getColumn(Assis,10);

    // 里程二次处理+导出
    licheng = CPPMAT::multiply_num(Mile,parameters->datpara.delt_mile);
    string outputPath_licheng = outPath+"\\OptimizedMile.bin";
    BXToolKit::writeDataFile(outputPath_licheng,licheng);

    mile1=BXToolKit::trip_mor(mile11,0);
    string outputPath_mile1 = outPath+"\\Mile1.bin";
    BXToolKit::writeDataFile(outputPath_mile1,mile1);

    mile2=BXToolKit::trip_mor(mile21,0);
    string outputPath_mile2 = outPath+"\\Mile2.bin";
    BXToolKit::writeDataFile(outputPath_mile2,mile2);

    mile3=BXToolKit::trip_mor(mile31,0);
    string outputPath_mile3 = outPath+"\\Mile3.bin";
    BXToolKit::writeDataFile(outputPath_mile3,mile3);

    BXToolKit::writeLog(outPath,"里程信息处理完毕;");
    cout<<"里程信息处理完毕..."<<endl;

    // 辅助信息二次处理
    cout<<"二次处理辅助信息..."<<endl;
    Window2 =  Window2/0.002;       // 换算成里程点数

    // 周向角处理
    zx=BXToolKit::trip_ass(zx1,0);
    if(parameters->propara.zxflag==2){
        zx=CPPMAT::minus(CPPMAT::creatmatrix_Nums(zx.size(),zx.at(0).size(),360),zx);
    }
    string outputPath_zx = outPath+"\\ZhouXiangJiao.bin";
    BXToolKit::writeDataFile(outputPath_zx,zx);

    // 倾角处理
    qj = BXToolKit::trip_ass(qj1,0);
    if(parameters->propara.qjflag==2){
        qj = CPPMAT::multiply_num(qj,-1);
    }
    string outputPath_qj = outPath+"\\QingJiao.bin";
    BXToolKit::writeDataFile(outputPath_qj,qj);

    // 航向角处理
    hx = BXToolKit::trip_ass(hx1,0);
    if(parameters->propara.hxflag==2){
        hx = CPPMAT::multiply_num(hx,-1);
    }
    string outputPath_hx = outPath+"\\HangXiangJiao.bin";
    BXToolKit::writeDataFile(outputPath_hx,hx);

    // 电压处理
    vol = BXToolKit::trip_ass(vol1,0);
    string outputPath_vol = outPath+"\\DianYa.bin";
    BXToolKit::writeDataFile(outputPath_vol,vol);

    // 温度处理
    temp = BXToolKit::trip_ass(temp2,0);
    string outputPath_temp = outPath+"\\WenDu.bin";
    BXToolKit::writeDataFile(outputPath_temp,temp);

    //速度处理
    if(Time.back().at(0)<Time.at(Time.size()-2).at(0)){
        Time[Time.size()-1][0]=Time.at(Time.size()-2).at(0)+1;
    }
    speed=BXToolKit::vel(Mile,Time,Window2);
    speed=CPPMAT::multiply_num(speed,parameters->datpara.vtime);

    string outputPath_speed=outPath+"\\speed.bin";
    BXToolKit::writeDataFile(outputPath_speed,speed);


    BXToolKit::writeLog(outPath,"辅助信息处理完毕;");
    cout<<"辅助信息二次处理完毕..."<<endl;



    // 内径计算 内径最小
    cout<<"开始内径计算（计算时间较长，大约需要一分钟，请稍后）..."<<endl;
    double step = round(Window1/parameters->datpara.delt_mile);
    tuple<vector<vector<vector<double>>>,vector<vector<double>>,vector<vector<double>>,double> ployfitdia_return = BXToolKit::ployfitdia(data2hNobase,zx,*parameters,step);
    vector<vector<double>> D_fit,D_min;
    D_fit = get<1>(ployfitdia_return);
    D_min = get<2>(ployfitdia_return);

    string outputPath_D_fit = outPath+"\\FittingInnerDiameter.bin";
    BXToolKit::writeDataFile(outputPath_D_fit,D_fit);

    string outputPath_D_min = outPath+"\\MinimumInnerDiameter.bin";
    BXToolKit::writeDataFile(outputPath_D_min,D_min);

    BXToolKit::writeLog(outPath,"内径计算完毕;");
    cout<<"内径计算完毕..."<<endl;

    //TODO: 有效性分析
    cout<<"开始有效性分析..."<<endl;
    vector<double> result_validana_bx;
    double licheng_in,licheng_out,licheng1;

    if(Flag_valid==1){
        if(Flag_inout==1){//进管文件
            if(Pos_in!=0){
                licheng_in=ceil(Pos_in/parameters->datpara.delt_mile-Mile.at(0).at(0))+1;
                if((licheng_in<1)||(licheng_in>Mile.size())){
                    licheng_in=1;
                }
            }
            else{
                licheng_in=1;
            }
            licheng_out=Mile.size();
        }
        else if(Flag_inout==2){//出管文件
            if(Pos_out!=0){
                licheng_out-ceil(Pos_out/parameters->datpara.delt_mile-Mile.at(0).at(0))+1;
                if((licheng_out<1)||(licheng_out>Mile.size())){
                    licheng_out=Mile.size();
                }
            }
            else {
                licheng_out = Mile.size();
            }
            licheng_in=1;
        }
        else if (Flag_inout==3){//一个文件
            if(Pos_in!=0){
                licheng_in=ceil(Pos_in/parameters->datpara.delt_mile-Mile.at(0).at(0))+1;
                if((licheng_in<1)||(licheng_in>Mile.size())){
                    licheng_in=1;
                }
            }
            else {
                licheng_in=1;
            }
            if(Pos_out!=0){
                licheng_out=ceil(Pos_out/parameters->datpara.delt_mile-Mile.at(0).at(0))+1;
                if((licheng_out<1)||(licheng_out>Mile.size())){
                    licheng_out=Mile.size();
                }
            }
            else {
                licheng_out=Mile.size();
            }
        }
        else {//中间文件
            licheng_in=1;
            licheng_out=Mile.size();
        }

        licheng1=Mile.back().at(0)*parameters->datpara.delt_mile;

        result_validana_bx = BXToolKit::validana_BX(data4analysis,Mile,zx1,qj1,hx1,vol1,temp2,speed,licheng_in,licheng_out,*parameters);

        BXToolKit::writeLog(outPath,"有效性计算完毕;");
        cout<<"有效性计算完毕..."<<endl;

    }
    else{
        licheng1=0;
        result_validana_bx=vector<double>(18,0);

        BXToolKit::writeLog(outPath,"不计算有效性,完毕;");
        cout<<"不计算有效性..."<<endl;
    }
    result_validana_bx.insert(result_validana_bx.begin(),licheng1);

    cout<<"有效性算法运行完成......"<<endl;
    return result_validana_bx;

}

tuple<double,double> BXToolKit::mulcha_invalid(const vector<vector<double>> data1,double lobo_mfl_1,double upbo_mfl_1,double loss_sensor,double window1,double thresh1){
    double dat1_num=data1.at(0).size();
    double row=data1.size();

    //连续数据缺失
    vector<vector<double>> invalidFlag1,invalidFlag2,invalidChanel,piece_together;
    vector<double> tempVector;
    invalidFlag1=CPPMAT::creatmatrix(row,1);
    invalidFlag2=CPPMAT::creatmatrix(row,1);
    //超限缺失
    for (int i = 1; i <=row ; ++i) {
        invalidChanel=CPPMAT::creatmatrix(dat1_num,1);
        tempVector=CPPMAT::getRow(data1,i).at(0);
        for (int ii = 0; ii < tempVector.size(); ++ii) {
            if((tempVector.at(ii)<lobo_mfl_1)||(tempVector.at(ii)>upbo_mfl_1)){
                invalidChanel[ii][0]=1;
            }
        }
        for (int j = 1; j <= dat1_num-(loss_sensor-1); ++j) {
            if(CPPMAT::matrix_equal(CPPMAT::getRows(invalidChanel,j,loss_sensor),CPPMAT::creatmatrix_ones(loss_sensor,1))){
                invalidFlag1[i-1][0]=1;
                break;
            }
        }

        for (int j = 1; j <= loss_sensor-1; ++j) {
            piece_together=CPPMAT::matrix_overlaying_below(CPPMAT::getRows(invalidChanel,invalidChanel.size()-(loss_sensor-1)+j,(loss_sensor-1)-j+1),CPPMAT::getRows(invalidChanel,1,j));
            if(CPPMAT::matrix_equal(piece_together,CPPMAT::creatmatrix_ones(loss_sensor,1))){
                invalidFlag1[i-1][0]=1;
                break;
            }
        }

    }

    for (int i = 1; i <= row-window1; ++i) {
        invalidChanel=CPPMAT::creatmatrix_ones(dat1_num,1);
        for (int k = 1; k <= dat1_num; ++k) {
            for (int j = i; j <= i+window1-1; ++j) {
                if(abs(data1[j-1][k-1]-data1[j][k-1])>=thresh1){
                    invalidChanel[k-1][0]=0;
                    break;
                }
            }
        }
        for (int ii = 1; ii <= dat1_num-(loss_sensor-1); ++ii) {
            if(CPPMAT::matrix_equal(CPPMAT::getRows(invalidChanel,ii,loss_sensor),CPPMAT::creatmatrix_ones(loss_sensor,1))){
                for (int n = i; n <=i+window1-1 ; ++n) {
                    invalidFlag2[n-1][0]=1;
                }
                break;
            }
        }
        for (int ii = 1; ii <= loss_sensor-1; ++ii) {
            piece_together=CPPMAT::matrix_overlaying_below(CPPMAT::getRows(invalidChanel,invalidChanel.size()-(loss_sensor-1)+ii,(loss_sensor-1)-ii+1),CPPMAT::getRows(invalidChanel,1,ii));
            if(CPPMAT::matrix_equal(piece_together,CPPMAT::creatmatrix_ones(loss_sensor,1))){
                for (int n = i; n <= i+window1-1; ++n) {
                    invalidFlag2[n-1][0]=1;
                    break;
                }
            }
        }
    }

    double N1 = CPPMAT::numOfMember(invalidFlag1,1)+CPPMAT::numOfMember(invalidFlag2,1);

    invalidFlag1=CPPMAT::creatmatrix(row,dat1_num);
    invalidFlag2=CPPMAT::creatmatrix(row,dat1_num);
    //超限缺失

    for (int i = 1; i <=row ; ++i) {
    tempVector=CPPMAT::getRow(data1,i).at(0);
    for (int ii = 0; ii < tempVector.size(); ++ii) {
        if((tempVector.at(ii)<lobo_mfl_1)||(tempVector.at(ii)>upbo_mfl_1)){
            invalidFlag1[i-1][0]=1;
        }
    }
    }
    //平滑缺失
    int flag;
    for (int i = 1; i <= row-window1; ++i) {
        for (int k = 1; k <= dat1_num; ++k) {
            flag=1;
            for (int j = i; j <= i+window1-1; ++j) {
                if(abs(data1[j-1][k-1]-data1[j][k-1])>=thresh1){
                    flag=0;
                    break;
                }
            }
            if(flag==1){
                for (int n = i; n <= i+window1-1; ++n) {
                    invalidFlag2[n-1][k-1]=1;
                }
            }

        }
    }
    double N2=CPPMAT::numOfMember(invalidFlag1,1)+CPPMAT::numOfMember(invalidFlag2,1);
    double con_loss_1=N1/row;
    double discon_loss_1=N2/(dat1_num*row);
    tuple<double,double> result = make_tuple(con_loss_1,discon_loss_1);
    return result;
}

vector<vector<double>> BXToolKit::vel(const vector<vector<double>>&mile,const vector<vector<double>>&time,double step){
    vector<vector<double>> vel_output;
    vector<double> vel_output_data;
    int width=1;
    for(int i=1;i<=step;i++){
        double temp;
        if((time.at(1+step-1).at(0)-time.at(0).at(0))==0){
            if((mile.at(1+step-1).at(0)-mile.at(0).at(0))>0){
                temp=INT_MAX;
            }
            else if ((mile.at(1+step-1).at(0)-mile.at(0).at(0))==0) {
                temp=0;
            }else if((mile.at(1+step-1).at(0)-mile.at(0).at(0))<0){
                temp=INT_MIN;
            }
        }
        else{
            temp=(mile.at(1+step-1).at(0)-mile.at(0).at(0))/(time.at(1+step-1).at(0)-time.at(0).at(0));
        }

        if(temp==INT_MAX||temp==INT_MIN||temp<=0){
            width++;
        }

        vel_output_data.push_back(temp);
    }
    for (int i = 1; i <= mile.size()-step; ++i) {
        double temp;
        if((time.at(i+step-1).at(0)-time.at(i-1).at(0))==0){
            if((mile.at(i+step-1).at(0)-mile.at(i-1).at(0))>0){
                temp=INT_MAX;
            }
            else if ((mile.at(i+step-1).at(0)-mile.at(i-1).at(0))==0) {
                temp=0;
            }else if((mile.at(i+step-1).at(0)-mile.at(i-1).at(0))<0){
                temp=INT_MIN;
            }
        }
        else{
            temp=(mile.at(i+step-1).at(0)-mile.at(i-1).at(0))/(time.at(i+step-1).at(0)-time.at(i-1).at(0));
        }

        if(temp==INT_MAX||temp==INT_MIN||temp<=0){
            width++;
        }
        vel_output_data.push_back(temp);
    }

    vel_output_data=CPPMAT::smooth(vel_output_data,width);
    vel_output_data[0]=(mile.at(1+step-1).at(0)-mile.at(0).at(0))/(time.at(1+step-1).at(0)-time.at(0).at(0));
    vel_output=CPPMAT::creatmatrix(vel_output_data.size(),1,vel_output_data);

    return vel_output;

}

vector<vector<double>> BXToolKit::readDataFileByRowNum(string dataPath, int rowNum){
    ifstream file;
    file.open(dataPath, ios::binary|ios::in);
    if (file.is_open() == false) {	// 文件打开失败
        cout << "Open file  fail!" << endl;
        exit(0);
    }
    file.seekg(0,ios_base::end);
    streampos filesize=file.tellg();
    int dataSize_double=filesize/8;
    int row=rowNum;
    int column = dataSize_double/rowNum;
    file.seekg(0,ios_base::beg);

    vector<vector<double>> A;
    for (int i = 0; i < row; ++i) {
        vector<double> v;
        double * byteData = new double[column];
        file.read((char*)byteData,sizeof(double)*column);
        for (int j = 0; j < column; ++j) {
            v.push_back(byteData[j]);
        }
        delete[](byteData);
        A.push_back(v);
    }
    file.close();
    return A;
}

void BXToolKit::writeDataFile(string outputPath, const vector<vector<double>>&Data){

    ofstream ofs(outputPath,ios::binary|ios::out);
    int len=Data.size()*Data.at(0).size();
    double * outputData = new double[len];
    for (int i = 0; i < Data.size(); ++i) {
        for (int j = 0; j < Data.at(0).size(); ++j) {
            outputData[j+i*Data.at(0).size()]=Data[i][j];
        }
    }
    ofs.write((const char*)outputData,sizeof(double)*len);
    delete[](outputData);
    ofs.close();

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

vector<vector<double>> BXToolKit::trip_mor(const vector<vector<double>>&datain, int thre){ // 列向量
    if(datain.at(0).size()>1){
        cout<<"trip_mor 函数输入参数不是列向量！";
        exit(0);
    }
    vector<vector<double>> dataout = datain;

    for (int i = 2; i <= datain.size()-1; ++i) {
        if((datain[i-1][0]-datain[i-2][0])<thre && (datain[i][0]-datain[i-2][0])>=0){
            dataout[i-1][0]=(dataout[i-2][0]+dataout[i][0])/2;
        }
    }
    return dataout;
}

vector<vector<double>> BXToolKit::trip_ass(const vector<vector<double>>&datain, int thre){ // 列向量
    if(datain.at(0).size()>1){
        cout<<"trip_mor 函数输入参数不是列向量！";
        exit(0);
    }
    vector<vector<double>> dataout = datain;
    for (int i = 2; i <= datain.size()-1; ++i) {
        if((datain[i-1][0]-datain[i-2][0])<thre && (datain[i][0]-datain[i-1][0])>0){
            dataout[i-1][0]=(dataout[i-2][0]+dataout[i][0])/2;
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

vector<vector<double>> BXToolKit::f_baseValue(const vector<vector<double>>&data){
    vector<vector<double>> mid,bx_data_allbase;
    vector<double> mid_list;
    for (int i = 1; i <= data.at(0).size(); ++i) {
        mid_list.push_back(CPPMAT::matrix_mid(CPPMAT::getColumn(data,i)));
    }
    mid.push_back(mid_list);
    double aver = CPPMAT::matrix_mean(mid);
    for (int i = 1; i <= mid_list.size(); ++i) {
        double dis = mid_list.at(i-1)-aver;
        if(i==1){
            bx_data_allbase=CPPMAT::minus(CPPMAT::getColumn(data,i),CPPMAT::creatmatrix_Nums(data.size(),1,dis));
        }
        else {
            bx_data_allbase=CPPMAT::matrix_overlaying_beside(bx_data_allbase,CPPMAT::minus(CPPMAT::getColumn(data,i),CPPMAT::creatmatrix_Nums(data.size(),1,dis)));
        }

    }
    return bx_data_allbase;
}

tuple<vector<vector<vector<double>>>,vector<vector<double>>,vector<vector<double>>,double> BXToolKit::ployfitdia(const vector<vector<double>>&data2hNobase,const vector<vector<double>>&CirAngle,ParaGet parameters, int step){
    vector<vector<double>> outputMatrix,data,datainfor,d_fit,d_fit_min;
    vector<double> d_fit_data,d_fit_min_data;
    double x1,y1,r1;

    // 拟合圆直径计算
    data = data2hNobase;    //未经基值矫正的高度值

    // 转换极坐标
    datainfor = CirAngle;
    vector<vector<vector<double>>> Dcell = BXToolKit::polarsystem(data,datainfor,parameters);
    vector<vector<double>> x,y;
    int k=1;
    for (int i = 1; i <= Dcell.size(); i+=step) {
        x=CPPMAT::trans(CPPMAT::getColumn(Dcell.at(i-1),2));// 每个小矩阵的第二列转换成行向量
        y=CPPMAT::trans(CPPMAT::getColumn(Dcell.at(i-1),3));// 每个小矩阵的第三列转换成行向量
        double * circle_r_return = BXToolKit::circle_r(x,y);
        x1=circle_r_return[0];
        y1=circle_r_return[1];
        r1=circle_r_return[2];
        delete[](circle_r_return);

        d_fit_data.push_back(r1*2);
        d_fit_min_data.push_back(BXToolKit::min_fit(x,y,x1,y1));
    }
    d_fit = CPPMAT::creatmatrix(1,d_fit_data.size(),d_fit_data);
    d_fit_min = CPPMAT::creatmatrix(1,d_fit_min_data.size(),d_fit_min_data);

    tuple<vector<vector<vector<double>>>,vector<vector<double>>,vector<vector<double>>,double> result = make_tuple(Dcell,d_fit,d_fit_min,0);
    return result;

}

double BXToolKit::min_fit(const vector<vector<double>>&x,const vector<vector<double>>&y,double x1, double y1){
    vector<double> D;
    double td_num = x.at(0).size();
    for (int i = 1; i <= td_num/2; ++i) {
        D.push_back((sqrt(pow((x.at(0).at(i-1)-x1),2)+pow((y.at(0).at(i-1)-y1),2))+sqrt(pow((x.at(0).at((i+td_num/2)-1)-x1),2)+pow((y.at(0).at((i+td_num/2)-1)-y1),2))));
    }
    double d_min = *min_element(D.begin(),D.end());
    return d_min;
}

double* BXToolKit::circle_r(const vector<vector<double>>&x,const vector<vector<double>>&y){
    vector<vector<vector<double>>> circlesystem_return;
    vector<vector<double>> x_cell,y_cell,outlier,out_td,x_delete,y_delete;
    vector<double> x_delete_data,y_delete_data;
    double x1,y1,r1;
    x_delete = x;
    y_delete = y;

    while (true) {
        double* circlefit_return = circlefit(x_delete,y_delete);

        x1 = circlefit_return[0];
        y1 = circlefit_return[1];
        r1 = circlefit_return[2];
        delete[](circlefit_return);
        //检查circlefit_return
        //cout<<x1<<"---"<<y1<<"---"<<r1<<endl;

        circlesystem_return = BXToolKit::circlesystem(x_delete,y_delete,x1,y1);
        x_cell = circlesystem_return.at(0);
        y_cell = circlesystem_return.at(1);
        //检查circlesustem_return
//      CPPMAT::show_matrix(x_cell);
//      CPPMAT::show_matrix(y_cell);

        outlier = BXToolKit::outlier_circle(r1,x_cell,y_cell);
        if(!outlier.empty()){
           x_delete_data.clear();
           y_delete_data.clear();
           out_td = CPPMAT::getColumn(outlier,1);
           for (int i = 1; i <= x_delete.at(0).size(); ++i) {
               if(CPPMAT::ismember(out_td,i)){
                   continue;
               }
               else {
                   x_delete_data.push_back(x_delete.at(0).at(i-1));
                   y_delete_data.push_back(y_delete.at(0).at(i-1));
               }
           }
           x_delete = CPPMAT::creatmatrix(1,x_delete_data.size(),x_delete_data);
           y_delete = CPPMAT::creatmatrix(1,y_delete_data.size(),y_delete_data);

        }
        else {
            break;
        }

    }
    double * output = new double[3]{x1,y1,r1};
    //cout<<output[0]<<"---"<<output[1]<<"---"<<output[2]<<endl;
    return output;
}

vector<vector<double>> BXToolKit::outlier_circle(double R,const vector<vector<double>>&x_cell,const vector<vector<double>>&y_cell){
    vector<vector<double>> distance,lb,outlier1;
    vector<double> distance_data,outlier1_data;
    for (int i = 1; i <= x_cell.at(0).size(); ++i) {
        double x1 = x_cell.at(0).at(i-1);
        double y1 = y_cell.at(0).at(i-1);
        double l =sqrt((pow(x1,2)+pow(y1,2))-R);
        distance_data.push_back(l);
    }
    distance = CPPMAT::creatmatrix(distance_data.size(),1,distance_data);
    double sigma = CPPMAT::matrix_standard_deviation(distance,1);
    lb = CPPMAT::minus(distance,CPPMAT::creatmatrix_Nums(distance.size(),distance.at(0).size(),CPPMAT::matrix_mean(distance)));
    for (int i = 1; i <= lb.size(); ++i) {
        if((lb[i-1][0]<=-3*sigma)||(lb[i-1][0]>=3*sigma)){
            outlier1_data.push_back(i);
            outlier1_data.push_back(lb[i-1][0]);
        }
    }
    outlier1 = CPPMAT::creatmatrix(outlier1_data.size()/2,2,outlier1_data);

    return outlier1;

}

vector<vector<vector<double>>> BXToolKit::circlesystem(const vector<vector<double>>&x,const vector<vector<double>>&y,double circle_a, double circle_b){
    vector<vector<vector<double>>> output;
    vector<vector<double>> x_cell,y_cell;

    for (int i = 1; i <= x.at(0).size(); ++i) {
        x_cell = CPPMAT::minus(x,CPPMAT::creatmatrix_Nums(1,x.at(0).size(),circle_a));
        y_cell = CPPMAT::minus(y,CPPMAT::creatmatrix_Nums(1,y.at(0).size(),circle_b));
    }
    output.push_back(x_cell);
    output.push_back(y_cell);
    return output;
}

double* BXToolKit::circlefit(const vector<vector<double>>&x,const vector<vector<double>>&y){



    double n = x.at(0).size();
    vector<vector<double>> xx,yy,xy,A,B,a;
    xx=CPPMAT::multiply_dot(x,x);
    yy=CPPMAT::multiply_dot(y,y);
    xy=CPPMAT::multiply_dot(x,y);

    vector<double> A_data{CPPMAT::matrix_sum(x),CPPMAT::matrix_sum(y),n,CPPMAT::matrix_sum(xy),CPPMAT::matrix_sum(yy),CPPMAT::matrix_sum(y),CPPMAT::matrix_sum(xx),CPPMAT::matrix_sum(xy),CPPMAT::matrix_sum(x)};
    vector<double> B_data{-(CPPMAT::matrix_sum(CPPMAT::plus(xx,yy))),-(CPPMAT::matrix_sum(CPPMAT::plus(CPPMAT::multiply_dot(xx,y),CPPMAT::multiply_dot(yy,y)))),-(CPPMAT::matrix_sum(CPPMAT::plus(CPPMAT::multiply_dot(xx,x),CPPMAT::multiply_dot(xy,y))))};
    A = CPPMAT::creatmatrix(3,3,A_data);
    B = CPPMAT::creatmatrix(3,1,B_data);
    a = CPPMAT::multiply(CPPMAT::inverse(A),B);

    double xc,yc,R;
    xc=-0.5*a[0][0];
    yc=-0.5*a[1][0];
    R=sqrt(-(a[2][0]-pow(xc,2)-pow(yc,2)));



    double *output = new double[3]{xc,yc,R};
    return output;
}

vector<vector<vector<double>>> BXToolKit::polarsystem(const vector<vector<double>>&data2hNobase,const vector<vector<double>>&CirAngle,ParaGet parameters){
    vector<vector<vector<double>>> Dcell;
    vector<vector<double>> Q,D,PointData;
    double chanum = parameters.propara.chanum;
    double center_0 = parameters.propara.center_0;
    double one_angle = parameters.propara.one_angle;
    for (int i = 1; i <= data2hNobase.size(); ++i) {
        PointData = CPPMAT::getRow(data2hNobase,i);
        Q=CPPMAT::creatmatrix(PointData.at(0).size(),1,PointData.at(0));    //将当前行pointdata存为Q的第一列
        double PointCircumAngle = CirAngle[i-1][0];
        vector<double> Q2,Q3;
        for (int j = 1; j <= PointData.at(0).size(); ++j) {
            Q2.push_back((BXToolKit::getangle(PointCircumAngle,j,center_0,one_angle,chanum)/180*M_PI));
            Q3.push_back(PointCircumAngle+(i-center_0)*(one_angle));
        }
        Q=CPPMAT::matrix_overlaying_beside(Q,CPPMAT::creatmatrix(Q2.size(),1,Q2));
        Q=CPPMAT::matrix_overlaying_beside(Q,CPPMAT::creatmatrix(Q3.size(),1,Q3));

        D=CPPMAT::creatmatrix(PointData.at(0).size(),1,PointData.at(0));
        D=CPPMAT::matrix_overlaying_beside(D,CPPMAT::multiply_dot(CPPMAT::getColumn(Q,1),CPPMAT::sin_matrix(CPPMAT::getColumn(Q,2))));
        D=CPPMAT::matrix_overlaying_beside(D,CPPMAT::multiply_dot(CPPMAT::getColumn(Q,1),CPPMAT::cos_matrix(CPPMAT::getColumn(Q,2))));

        Dcell.push_back(D);
    }
    return Dcell;
}

double BXToolKit::getangle(double zhouxiang,int channel,double center_0,double one_angle,double chanum){
    double angle;
    if (channel>center_0){
        angle=zhouxiang+(channel-center_0)*one_angle;
    }

   else{
        angle=zhouxiang+(channel+chanum-center_0)*one_angle;
    }

   if (angle>360){
        angle=angle-360;
   }
   if (angle<0){
       angle=angle+360;
   }
   stringstream ss;
   ss.setf(ios::fixed);
   ss.precision(1);
   ss<<angle;
   angle = atof(ss.str().c_str());
   return angle;
}

void BXToolKit::writeLog(string logPath, string logData){
    string logFileName = logPath+("\\log.log");
    ofstream ofs(logFileName,ios::app);
    time_t sysTime = time(NULL);
    tm* sysLocalTime = localtime(&sysTime);
    ofs<<asctime(sysLocalTime)+logData<<endl;
    ofs.close();
}

vector<double> BXToolKit::validana_BX(const vector<vector<double>> data4analysis, const vector<vector<double>> Mile1, const vector<vector<double>> zx1, const vector<vector<double>> qj1, const vector<vector<double>> hx1, const vector<vector<double>> vol1, const vector<vector<double>> temp1, const vector<vector<double>> speed, double licheng_in, double licheng_out, ParaGet parameters){

    vector<vector<double>> Mile,vol,data_BX,zhouxiang,qingjiao,temp,vel;
    Mile=CPPMAT::getRows(Mile1,licheng_in,licheng_out-licheng_in+1);
    vol=CPPMAT::getRows(vol1,licheng_in,licheng_out-licheng_in+1);
    data_BX=CPPMAT::getRows(data4analysis,licheng_in,licheng_out-licheng_in+1);
    zhouxiang=CPPMAT::getRows(zx1,licheng_in,licheng_out-licheng_in+1);
    qingjiao=CPPMAT::getRows(qj1,licheng_in,licheng_out-licheng_in+1);
    temp=CPPMAT::getRows(temp1,licheng_in,licheng_out-licheng_in+1);
    vel=CPPMAT::getRows(speed,licheng_in,licheng_out-licheng_in+1);

    //判定参数
    double lobo_vol,upbo_vol,upbo_zhouxiang,lobo_zhouxiang,upbo_qingjiao,lobo_qingjiao,upbo_temp,lobo_temp,upbo_BX,lobo_BX,window_BX,thresh_BX,loss_sensor_BX;
    lobo_vol=parameters.valpara.lobo_vol;
    upbo_vol=parameters.valpara.upbo_vol;
    upbo_zhouxiang=parameters.valpara.upbo_zhouxiang;
    lobo_zhouxiang=parameters.valpara.lobo_zhouxiang;
    upbo_qingjiao=parameters.valpara.upbo_qingjiao;
    lobo_qingjiao=parameters.valpara.lobo_qingjiao;
    upbo_temp=parameters.valpara.upbo_temp;
    lobo_temp=parameters.valpara.lobo_temp;
    upbo_BX=parameters.valpara.upbo_BX;
    lobo_BX=parameters.valpara.lobo_BX;
    window_BX=parameters.valpara.window_BX;
    thresh_BX=parameters.valpara.thresh_BX;
    loss_sensor_BX=parameters.valpara.loss_sensor_BX;

    // 里程跳变异常 里程只能向下跳 12145   1232345
    double n_mile,licheng_invalid,licheng_invalid_num,SpeedAverage,SpeedMin,SpeedMax,vol_invalid,vol_ave,temp_invalid,temp_max,temp_min,temp_ave,zhouxiang_invalid,zhouxiang_max,zhouxiang_min,zhouxiang_ave,qingjiao_invalid;
    vector<vector<double>> mile_diff;
    vector<double> mile_vector;
    for (int i = 0; i < Mile.size(); ++i) {
        mile_vector.push_back(Mile.at(i).at(0));
    }

    mile_vector=CPPMAT::diff(mile_vector);
    mile_diff=CPPMAT::creatmatrix(mile_vector.size(),1,mile_vector);
    n_mile=CPPMAT::numOfMinusMember(mile_diff);
    licheng_invalid=n_mile/CPPMAT::getsize(Mile);
    licheng_invalid_num=n_mile;

    //速度均值
    SpeedAverage=CPPMAT::matrix_mean(vel);
    SpeedMin=CPPMAT::getMin(vel);
    SpeedMax=CPPMAT::getMax(vel);
    // 电压失效率和电压均值 %原始数据，上限，下限
    vol_invalid=(CPPMAT::numOfMember_lower(vol,lobo_vol)+CPPMAT::numOfMember_larger(vol,upbo_vol))/CPPMAT::getsize(vol);
    vol_ave=CPPMAT::matrix_mean(vol);
    //温度失效率和温度极值和温度均值
    temp_invalid=(CPPMAT::numOfMember_lower(temp,lobo_temp)+CPPMAT::numOfMember_larger(temp,upbo_temp))/CPPMAT::getsize(temp);
    temp_max=CPPMAT::getMax(temp);
    temp_min=CPPMAT::getMin(temp);
    temp_ave=CPPMAT::matrix_mean(temp);
    //周向角无效帧数和极值和均值
    zhouxiang_invalid=(CPPMAT::numOfMember_lower(zhouxiang,lobo_zhouxiang)+CPPMAT::numOfMember_larger(zhouxiang,upbo_zhouxiang))/CPPMAT::getsize(zhouxiang);
    zhouxiang_max=CPPMAT::getMax(zhouxiang);
    zhouxiang_min=CPPMAT::getMin(zhouxiang);
    zhouxiang_ave=CPPMAT::matrix_mean(zhouxiang);
    //倾角无效帧数
    qingjiao_invalid=(CPPMAT::numOfMember_lower(qingjiao,lobo_qingjiao)+CPPMAT::numOfMember_larger(qingjiao,upbo_qingjiao))/CPPMAT::getsize(qingjiao);

    tuple<double,double> result_mulcha_invalid=BXToolKit::mulcha_invalid(data_BX,lobo_BX,upbo_BX,loss_sensor_BX,window_BX,thresh_BX);
    double con_loss = get<0>(result_mulcha_invalid);
    double discon_loss = get<1>(result_mulcha_invalid);
    vector<double> output{temp_max,temp_min,temp_ave,temp_invalid,
                licheng_invalid,licheng_invalid_num,
                zhouxiang_max,zhouxiang_min,zhouxiang_ave,zhouxiang_invalid,
                qingjiao_invalid,vol_ave,vol_invalid,
                SpeedAverage,SpeedMin,SpeedMax,con_loss,discon_loss};
    return output;

}

