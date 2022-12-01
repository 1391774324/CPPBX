#include "cppmat.h"

CPPMAT::CPPMAT()
{

}

//创建 h行l列的矩阵，并将初始各值设定为0
vector<vector<double>> CPPMAT::creatmatrix(int h,int l)
{
    vector<vector<double>> v;
    for (int i = 0; i < h; i++)
    {
        vector<double>v1(l,0);
        v.push_back(v1);
    }
    return v;
}

//创建h行l列赋值（默认0）矩阵；
vector<vector<double>> CPPMAT::creatmatrix(int h,int l, vector<double> elements){
    vector<vector<double>> v;
    int index=0;
    for (int i = 0; i < h; i++)
    {
        vector<double>v1(l,0);
        for (int j = 0; j < l; ++j) {
            if(index<elements.size()){
                v1[j]=elements.at(index);
                index++;
            }
        }
        v.push_back(v1);
    }
    return v;
}

//矩阵A+矩阵B=矩阵C，并返回
vector<vector<double>> CPPMAT::plus(const vector<vector<double>>&A,const vector<vector<double>>&B)
{
    int h=A.size();
    int l=A[0].size();
    vector<vector<double>> C;
    C=creatmatrix( h, l);

    for(int i=0;i<h;i++)
    {
        for (int j = 0; j < l; j++)
        {
            C[i][j]=A[i][j]+B[i][j];
            if (abs(C[i][j])<M_epsilon)
            {
                C[i][j]=0.0;
            }
        }
    }
    return C;
}

//矩阵A-矩阵B=矩阵C，并返回
vector<vector<double>> CPPMAT::minus(const vector<vector<double>>&A,const vector<vector<double>>&B)
{
    int h=A.size();
    int l=A[0].size();
    vector<vector<double>> C;
    C=creatmatrix( h, l);

    for(int i=0;i<h;i++)
    {
        for (int j = 0; j < l; j++)
        {
            C[i][j]=A[i][j]-B[i][j];
            if (abs(C[i][j])<M_epsilon)
            {
                C[i][j]=0.0;
            }
        }
    }
    return C;
}

//矩阵A*矩阵B=矩阵C，并返回
vector<vector<double>> CPPMAT::multiply(const vector<vector<double>>&A,const vector<vector<double>>&B)
{
    int A_h=A.size();
    int A_l=A[0].size();
    int B_h=B.size();
    int B_l=B[0].size();
    if(A_l !=B_h)
    {
        cout<<"两矩阵维数无法相乘"<<endl;
        exit(0);
    }
    vector<vector<double>> C=creatmatrix(A_h,B_l);
    for (int i = 0; i < A_h; i++)
    {
        for (int j = 0; j < B_l; j++)
        {
            C[i][j]=0;
            for (int k = 0; k < A_l; k++)
            {
                C[i][j] +=A[i][k]*B[k][j];
            }
            if (abs(C[i][j])<M_epsilon)
            {
                C[i][j]=0.0;
            }
            //cout<<C[i][j]<<"\t";
        }
        //cout<<endl;
    }
    return C;
}

//矩阵点乘A.*B
vector<vector<double>> CPPMAT::multiply_dot(const vector<vector<double>>&A,const vector<vector<double>>&B){
    int A_h=A.size();
    int A_l=A[0].size();
    int B_h=B.size();
    int B_l=B[0].size();
    if((A_l !=B_l)||(A_h!=B_h))
    {
        cout<<"两矩阵维数无法点乘"<<endl;
        exit(0);
    }
    vector<vector<double>> C=creatmatrix(A_h,A_l);
    for (int i = 0; i < A_h; ++i) {
        for (int j = 0; j < A_h; ++j) {
            C[i][j]=A[i][j]*B[i][j];
        }
    }
    return C;
}


//矩阵A*num=矩阵B，并返回
vector<vector<double>> CPPMAT::multiply_num(const vector<vector<double>>&A,double num)
{
    int A_h=A.size();
    int A_l=A[0].size();
    vector<vector<double>> B=creatmatrix(A_h,A_l);
    for (int i = 0; i < A_h; i++)
    {
        for (int j = 0; j < A_l; j++)
        {
            B[i][j]=num*A[i][j];
        }
    }
    return B;
}

//取子矩阵
vector<vector<double>> CPPMAT::subMatrix(const vector<vector<double>> &A,int r, int r_step, int c, int c_step){
    int A_h=A.size();
    int A_l=A.at(0).size();
    if(r>A_h||r+r_step-1>A_h){
       cout<<"取子矩阵行数大于原矩阵行数范围！"<<endl;
       exit(0);
    }
    else if(c>A_l||c+c_step-1>A_l){
       cout<<"取子矩阵列数大于原矩阵列数范围！"<<endl;
       exit(0);
    }
    else if(r<=0||c<=0||r+r_step+1<=0||c+c_step+1<=0){
        cout<<"取子矩阵行列参数应大于0！"<<endl;
        exit(0);
    }
    else if(r_step==0||c_step==0){
        cout<<"取子矩阵行列跨度不可等于0！"<<endl;
        exit(0);
    }

    vector<vector<double>> B;
    if(r_step>0){
        if(c_step>0){
            for (int i = r; i < r+r_step; ++i) {
                vector<double> v;
                for (int j = c; j < c+c_step; ++j) {
                    v.push_back(A[i-1][j-1]);
                }
                B.push_back(v);
            }
        }
        else if(c_step<0){
            for (int i = r; i < r+r_step; ++i) {
                vector<double> v;
                for (int j = c+c_step+1; j <= c; ++j) {
                    v.push_back(A[i-1][j-1]);
                }
                B.push_back(v);
            }
        }
    }
    else if(r_step<0){
        if(c_step>0){
            for (int i = r+r_step+1; i <= r; ++i) {
                vector<double> v;
                for (int j = c; j < c+c_step; ++j) {
                    v.push_back(A[i-1][j-1]);
                }
                B.push_back(v);
            }
        }
        else if(c_step<0){
            for (int i = r+r_step+1; i <= r; ++i) {
                vector<double> v;
                for (int j = c+c_step+1; j <= c; ++j) {
                    v.push_back(A[i-1][j-1]);
                }
                B.push_back(v);
            }
        }
    }
    return B;
}


//取行
vector<vector<double>> CPPMAT::getRow(const vector<vector<double>>&A, int num){
    if(num>A.size()){
        cout<<"取行向量操作超出矩阵最大行数！"<<endl;
        exit(0);
    }
    else if (num<=0){
        cout<<"取行向量操作参数应大于0！"<<endl;
        exit(0);
    }
    vector<vector<double>> row{A.at(num-1)};
    return row;
}

//取多个行向量
vector<vector<double>> CPPMAT::getRows(const vector<vector<double>>&A, int r,int r_step){
    int A_h=A.size();
    int A_l=A.at(0).size();
    if((r>A_h)||((r+r_step-1)>A_h)){
        cout<<"取行向量操作超出矩阵最大行数！"<<endl;
        exit(0);
    }
    else if (r<=0||r+r_step+1<=0){
        cout<<"取行向量操作参数应大于0！"<<endl;
        exit(0);
    }
    else if(r_step==0){
        cout<<"取行向量行跨度不可为0！"<<endl;
        exit(0);
    }
    vector<vector<double>>B;
    if(r_step>0){
        for (int i = r; i < r+r_step; ++i) {
            B.push_back(getRow(A,i).at(0));
        }
    }
    else{
        for (int i = r+r_step+1; i <=r ; ++i) {
            B.push_back(getRow(A,i).at(0));
        }
    }
    return B;
}

//取列
vector<vector<double>> CPPMAT::getColumn(const vector<vector<double>>&A, int num){
    if(num>A.at(0).size()){
        cout<<"取列向量操作超出矩阵最大列数！"<<endl;
        exit(0);
    }
    else if (num<=0){
        cout<<"取列向量操作参数应大于0！"<<endl;
        exit(0);
    }
    int A_h=A.size();
    int A_l=A.at(0).size();
    vector<vector<double>> column;
    for (int i = 0; i < A_h; ++i) {
        column.push_back(vector<double>{A[i][num-1]});
    }
    return column;
}

//取多个列向量
vector<vector<double>> CPPMAT::getColumns(const vector<vector<double>>&A, int c,int c_step){
    int A_h=A.size();
    int A_l=A.at(0).size();
    if(c>A_l||c+c_step-1>A_l){
        cout<<"取列向量操作超出矩阵最大列数！"<<endl;
        exit(0);
    }
    else if (c<=0||c+c_step+1<=0){
        cout<<"取列向量操作参数应大于0！"<<endl;
        exit(0);
    }
    else if(c_step==0){
        cout<<"取列向量列跨度不可为0！"<<endl;
        exit(0);
    }
    vector<vector<double>>B;
    if(c_step>0){
        for (int i = c; i < c+c_step; ++i) {
            if(i==c){
                B=getColumn(A,i);
            }
            else {
                B=matrix_overlaying_beside(B,getColumn(A,i));
            }
        }
    }
    else if(c_step<0){
        for (int i = c+c_step+1; i <= c; ++i) {
            if(i==c+c_step+1){
                B=getColumn(A,i);
            }
            else {
                B=matrix_overlaying_beside(B,getColumn(A,i));
            }
        }
    }
    return B;
}

//矩阵A与矩阵B上下叠加获得新的矩阵C,并返回
vector<vector<double>> CPPMAT::matrix_overlaying_below(const vector<vector<double>>&A,const vector<vector<double>>&B)
{
    //判断矩阵的列是否相等
    int A_h=A.size();
    int A_l=A[0].size();
    int B_h=B.size();
    int B_l=B[0].size();
    if (A_l != B_l)
    {
        cout<<"叠加的矩阵列数不相等"<<endl;
        exit(0);
    }
    //创建
    vector<vector<double>> C=creatmatrix(A_h+B_h,A_l);
    //将A传入
    for (int i = 0; i < A_h; i++)
    {
        for (int j = 0; j < A_l; j++)
        {
            C[i][j]=A[i][j];
        }
    }
    //将B传入
    for (int i = 0; i < B_h; i++)
    {
        for (int j = 0; j < B_l; j++)
        {
            C[i+A_h][j]=B[i][j];
        }
    }
    return C;
}

//矩阵A与矩阵B左右叠加，获得新的矩阵C，并返回
vector<vector<double>> CPPMAT::matrix_overlaying_beside(const vector<vector<double>>&A,const vector<vector<double>>&B)
{
    //判断矩阵的列是否相等
    int A_h=A.size();
    int A_l=A[0].size();
    int B_h=B.size();
    int B_l=B[0].size();
    if (A_h != B_h)
    {
        cout<<"叠加的矩阵行数不相等"<<endl;
        exit(0);
    }
    //创建
    vector<vector<double>> C=creatmatrix(A_h,A_l+B_l);
    //将A传入
    for (int i = 0; i < A_h; i++)
    {
        for (int j = 0; j < A_l; j++)
        {
            C[i][j]=A[i][j];
        }
    }
    //将B传入
    for (int i = 0; i < B_h; i++)
    {
        for (int j = 0; j < B_l; j++)
        {
            C[i][j+A_l]=B[i][j];
        }
    }
    return C;
}

//输入矩阵A，输出矩阵A的转置矩阵AT
vector<vector<double>> CPPMAT::trans(const vector<vector<double>> &A)
{
    vector<vector<double>> AT=creatmatrix(A[0].size(),A.size());
    int h=AT.size();
    int l=AT[0].size();
    for (int i = 0; i <h ; i++)
    {
        for (int j = 0; j < l; j++)
        {
            AT[i][j]=A[j][i];
        }
    }
    return AT;
}

//输入矩阵A,输出矩阵A的逆矩阵inv_A
vector<vector<double>> CPPMAT::inverse(const vector<vector<double>> &A)
{
    if (A.size() != A[0].size())
    {
        cout<<"输入矩阵维数不合法"<<endl;
        exit(0);
    }
    int n=A.size();
    vector<vector<double>> inv_A=creatmatrix(n,n);
    vector<vector<double>> L=creatmatrix(n,n);
    vector<vector<double>> U=creatmatrix(n,n);
    vector<vector<double>> inv_L=creatmatrix(n,n);
    vector<vector<double>> inv_U=creatmatrix(n,n);
//LU分解
    //L矩阵对角元素为1
    for (int i = 0; i < n; i++)
    {
        L[i][i] = 1;
    }
    //U矩阵第一行
    for (int i = 0; i < n; i++)
    {
        U[0][i]=A[0][i];
    }
    //L矩阵第一列
    for (int i = 1; i < n; i++)
    {
        L[i][0]=1.0*A[i][0]/A[0][0];
    }

    //计算LU上下三角
    for (int i = 1; i < n; i++)
    {
        //计算U（i行j列）
        for (int j = i; j < n; j++)
        {
            double tem = 0;
            for (int k = 0; k < i; k++)
            {
                tem += L[i][k] * U[k][j];
            }
            U[i][j] = A[i][j] - tem;
            if (abs(U[i][j])<M_epsilon)
            {
                U[i][j]=0.0;
            }
        }
        //计算L（j行i列）
        for (int j = i ; j < n; j++)
        {
            double tem = 0;
            for (int k = 0; k < i; k++)
            {
                tem += L[j][k] * U[k][i];
            }
            L[j][i] = 1.0*(A[j][i] - tem) / U[i][i];
            if (abs(L[i][j])<M_epsilon)
            {
                L[i][j]=0.0;
            }
        }

    }
    //L U剩余位置设为0
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(i>j)
            {
                U[i][j]=0.0;
            }
            else if(i<j)
            {
                L[i][j]=0.0;
            }
        }
    }
    //LU求逆
    //求矩阵U的逆
    for (int i=0;i<n;i++)
    {
        inv_U[i][i]=1/U[i][i];// U对角元素的值，直接取倒数
        for (int k=i-1;k>=0;k--)
        {
            double s=0;
            for (int j=k+1;j<=i;j++)
            {
                s=s+U[k][j]*inv_U[j][i];
            }
            inv_U[k][i]=-s/U[k][k];//迭代计算，按列倒序依次得到每一个值，
            if (abs(inv_U[k][i])<M_epsilon)
            {
                inv_U[k][i]=0.0;
            }
        }
    }
    //求矩阵L的逆
    for (int i=0;i<n;i++)
    {
        inv_L[i][i]=1; //L对角元素的值，直接取倒数，这里为1
        for (int k=i+1;k<n;k++)
        {
            for (int j=i;j<=k-1;j++)
            {
                inv_L[k][i]=inv_L[k][i]-L[k][j]*inv_L[j][i];
                if (abs(inv_L[k][i])<M_epsilon)
                {
                    inv_L[k][i]=0.0;
                }
            }
        }
    }
    inv_A=multiply(inv_U,inv_L);
    return inv_A;
}

void CPPMAT::show_matrix(const vector<vector<double>> &A)
{
    int h=A.size();
    int l=A[0].size();
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < l; j++)
        {
            cout<<A[i][j]<<"\t";
        }
        cout<<endl;
    }
}
