#ifndef TEST_H
#define TEST_H
#endif // TEST_H

struct Config
{
    int CH;//道数
    int Min,Max;		//寻峰容差
    float Rn_C1,Rn_C2,Rn_C3;		//Rn校正系数
    int NL_L,NL_R,N1_L,N1_R,N2_L,N2_R;		//Rn校正系数
    float dose_A,dose_B,dose_C;		//剂量校准系数
    float energy_A,energy_B,energy_C;		//能量校准系数
    float detect_A,detect_B,detect_C,detect_D,detect_E,detect_F;		//探测校准系数
    float activity_A,activity_B,activity_C,activity_D,activity_E,activity_F;		//活度校准系数
    int smooth_Times;		//光滑次数
    int NTh_L,NTh_R,NK_L,NK_R;		//含量边界
    float capacity_A1,capacity_A2,capacity_A3,capacity_B1,capacity_B2,capacity_B3,capacity_C1,capacity_C2,capacity_C3;		//含量系数
    float high_A1,high_A2,high_A3,high_B1,high_B2,high_B3,high_C1,high_C2,high_C3;	//高度校准

};

void _Config_File(void);
