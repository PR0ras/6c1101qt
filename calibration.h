#ifndef CALIBRATION_H
#define CALIBRATION_H

#endif // CALIBRATION_H
// CCalibration 命令目标

class CCalibration
{
public:
//定义用于本底扣除函数的变量
   int BD[1024],YS[1024],KC[1024];     //定义3个数组，分别用来存放本底数据、原始测量数据和扣除本底后的数据

//定义用于大气氡扣除函数的变量
   //int c1,c2,c3;                       //定义谱线比系数
   int Nu,NL,N1,N2;                       //NL-经本底和散射校正后的低能窗特征峰面积计数，N1-经本底和散射校正后的U能窗的特征峰面积计数，N2-经本底和散射校正后的Th能窗的特征峰面积计数

//定义用于含量修正的变量
   //double A1,A2,A3,B1,B2,B3,C1,C2,C3;  //定义九个换算系数A1=1,A2=0,A3=0,B1=0,B2=1,B3=0,C1=0,C2=0,C3=1
   int NU,NTh,NK;                      //定义U、Th、K各能窗净计数，注：注意NU与Nu的区分
   double CU,CTh,CK;                   //定义U、Th、K的含量

//定义用于高度修正的变量
   double R60,Rh;                      //定义60m高度上修正后的计数率R60和实际飞行高度h时修正后的计数率Rh
   double H;                           //标准大气压，标准温度下的飞行高度(m)
   double p;                           //高度衰减系数，单位：m
   double GD[1024];                    //声明一个用于存放高度校正后数据的数组，该数组可能需要定义为全局变量

public:
    CCalibration();
    //此处可能需要补充内容
    virtual ~CCalibration();

public:
    int BDKC(int pure[1024],int YS[1024],int BD[1024]);                                                 //定义用于本底扣除的函数
    int DQD(double c1,double c2,double c3,int m,int m1,int k,int k1,int n,int n1);                         //定义用于扣除大气氡的函数
    double HLJS(double A1,double A2,double A3,double B1,double B2,double B3,double C1,double C2,double C3,int t,int t1,int k,int k1);    //定义用于含量计算的函数
    double GDXZ(double a,double b,double c,double d,double e,double f);                     //定义高度修正所用函数

};


