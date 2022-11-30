#ifndef PARAGET_H
#define PARAGET_H

#include <iostream>
#include <map>
#include <math.h>
#include <limits.h>

using namespace std;

class ParaGet       // 声明一个ParaGet类的parameter对象，构造函数中输入pipesize以初始化变形参数；
{
public:
    ParaGet(string pipesize);

    // parameters
    //double paraPrj;
    struct DATPARA{
        int line_num;
        int bx_end;
        int bx_start;
        int mile1_cha;
        int mile2_cha;
        int mile3_cha;
        int timechal;
        int vtime;
        int zxchal;
        int qjchal;
        int hxchal;
        int volchal;
        int temp1chal;
        int temp2chal;
        int temp3chal;
        double delt_mile;
    }datpara;
    struct PROPARA{
        double move_length;
        int bx_flag;
        int bx_circle;
        double chanum;
        double volbase;
        int a;
        int r;
        double d2;
        int zxflag;
        int qjflag;
        int hxflag;
        int center_0;
        double one_angle;
        double OD;
    }propara;
    struct PREPARA
    {
    double bianxing_thresh;
    int vstep;
    }prepara;
    struct VALPARA{
        double lobo_vol;
        double upbo_vol;
        double upbo_zhouxiang;
        double lobo_zhouxiang;
        double upbo_qingjiao;
        double lobo_qingjiao;
        double upbo_hangxiang;
        double lobo_hangxiang;
        double upbo_temp;
        double lobo_temp;
        double upbo_BX;
        double lobo_BX;
        double window_BX;
        double thresh_BX;
        double loss_sensor_BX;
    }valpara;
    struct DETPARA{
        struct BLEND{
            double od;
            double thresh1;
            double thresh2;
            double thresh3;
            double dis;
            //int interval1;
        }blend;
        struct WELD{
            double threshdiff;
            int vardata;
            int varstep;
            int dis;
            int step;
            int threshstep;
            int varseparate;
            int widthresh;
        }weld;
        int Con;
        int detect_thresh;
        struct TEE{
            double long_min;
            double long_max;
            double wide_min;
            double wide_max;
            int TH_STD;
            double area;
        }tee;
        struct TAP{
            double long_min;
            double long_max;
            double wide_min;
            double wide_max;
            double TH_STD;
            double area;
        }tap;
        struct FLAW{
            double con;
            double area;
            double TH_STD;
            double TH_DET;
        }flaw;
        struct OVAL{
            int smooth_window;
            int thresh;
        }oval;
    }detpara;
};

#endif // PARAGET_H
