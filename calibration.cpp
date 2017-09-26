// Calibration.cpp : 实现文件

#include "calibration.h"
#include "math.h"
#include <string.h>
#define E 2.718281828         //定义自然常数e


// CCalibration

CCalibration::CCalibration()
{
}

//此处可能需要补充内容

CCalibration::~CCalibration()
{
}


// CCalibration 成员函数

int CCalibration::BDKC(int pure[1024],int YS[1024],int BD[1024])           //本底扣除函数
{
   for(int j=0;j<1024;j++)
    {
      KC[j]=YS[j]-BD[j];             //定义本底扣除的算法
    }

   memcpy(pure,KC,sizeof(KC));
   return 0;
//	return KC[1024];                 //存在问题：函数返回值是否正确
}


int CCalibration::DQD(double c1,double c2,double c3,int m,int m1,int k,int k1,int n,int n1)            //大气氡扣除函数
{

    NL=N1=N2=0;                      //赋初值
   for(m;m<=m1;m++)
    {
        NL+=KC[m];                   //NL的值为低能窗的计数之和,具体道址有待进一步确定
    }
   for(k;k<=k1;k++)
    {
        N1+=KC[k];                   //N1的值为U能窗的计数之和,放射性核素Bi-214，具体道址有待进一步确定
    }
   for(n;n<=n1;n++)
    {
        N2+=KC[n];                   //N2的值为Th能窗的计数之和,放射性核素Tl-208，具体道址有待进一步确定
    }
    Nu=(N1+N2*c3/c1-NL/c1)/(1-c2/c1);//求得陆地U组分在U能窗的计数，此处只是对能窗总计数进行了修正，存在问题：是否需要对谱线各道计数进行修正？
    return 0;
}


double CCalibration::HLJS(double A1,double A2,double A3,double B1,double B2,double B3,double C1,double C2,double C3,int t,int t1,int k,int k1)//含量计算函数
{

    NU=Nu;        //赋初值
    NTh=NK=0;

   for(t;t<=t1;t++)
    {
        NTh+=KC[t];                 //Th能窗净计数，放射性核素Tl-208，道址有待进一步确定
    }
   for(k;k<=k1;k++)
    {
        NK+=KC[k];                  //K能窗净计数，放射性核素K-40，道址有待进一步确定
    }
    CU=A1*NU+B1*NTh+C1*NK;          //U含量计算
    CTh=A2*NU+B2*NTh+C2*NK;         //Th含量计算
    CK=A3*NU+B3*NTh+C3*NK;          //K含量计算
    return 0;                       //存在问题：函数返回值是否正确

}


double CCalibration::GDXZ(double a,double b,double c,double d,double e,double f)        //高度修正函数
{
  H=20;                                //标准大气压、标准温度下的飞行高度
  for(int r=0;r<1024;r++)
  {
     p=a*r*r*r*r*r+b*r*r*r*r+c*r*r*r+d*r*r+e*r+f;                    //p是与道址有关的函数，具体形式待实验后修改
     Rh=KC[r];
     double X,Y;
     Y=p*(H-60);
     X=pow(E,Y);
     R60=Rh*X;                         //高度修正公式
     GD[r]=R60;                        //将修正后的数据存入数组

  }
  return 0;                            //返回值待定

}
