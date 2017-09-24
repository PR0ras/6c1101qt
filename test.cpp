#include"Config.h"
#include "mainwindow.h"

Config _Config;

void _Config_File(void)
{
    //QSettings *configIniRead = new QSettings("test.ini", QSettings::IniFormat);
    //QString test1 = configIniRead->value("/CH/CH_1024").toString();
    //bool ok;
    //int CH=test1.toInt(&ok,10);
    //文件操作
    //QString test1;
    QString fn1 = QFileDialog::getOpenFileName(NULL,QStringLiteral("标题"),".","*.txt");
    QFile f(fn1);//打开文件
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))//异常处理
    {
        //QApplication::exit(0);
    }

    //将String转换为Int
    QTextStream txtInput(&f);
    QString lineStr,sum;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        sum +=lineStr;
    }
    //ini操作
    QSettings *configIniRead = new QSettings("test.ini", QSettings::IniFormat);//读取ini
    _Config.CH = configIniRead->value("/CH/CH_1024").toInt();//
    _Config.dose_A = configIniRead->value("/Y/dose_A").toFloat();
    _Config.dose_B = configIniRead->value("/Y/dose_B").toFloat();
    _Config.dose_C = configIniRead->value("/Y/dose_C").toFloat();
    _Config.energy_A = configIniRead->value("/E/energy_A").toFloat();
    _Config.energy_B = configIniRead->value("/E/energy_B").toFloat();
    _Config.energy_C = configIniRead->value("/E/energy_C").toFloat();
    _Config.detect_A = configIniRead->value("/Z/detect_A").toFloat();
    _Config.detect_B = configIniRead->value("/Z/detect_B").toFloat();
    _Config.detect_C = configIniRead->value("/Z/detect_C").toFloat();
    _Config.detect_D = configIniRead->value("/Z/detect_D").toFloat();
    _Config.detect_E = configIniRead->value("/Z/detect_E").toFloat();
    _Config.detect_F = configIniRead->value("/Z/detect_F").toFloat();
    _Config.activity_A = configIniRead->value("/Q/activity_A").toFloat();
    _Config.activity_B = configIniRead->value("/Q/activity_B").toFloat();
    _Config.activity_C = configIniRead->value("/Q/activity_C").toFloat();
    _Config.activity_D = configIniRead->value("/Q/activity_D").toFloat();
    _Config.activity_E = configIniRead->value("/Q/activity_E").toFloat();
    _Config.activity_F = configIniRead->value("/Q/activity_F").toFloat();
    _Config.capacity_A1 = configIniRead->value("/C/capacity_A1").toFloat();
    _Config.capacity_A2 = configIniRead->value("/C/capacity_A2").toFloat();
    _Config.capacity_A3 = configIniRead->value("/C/capacity_A3").toFloat();
    _Config.capacity_B1 = configIniRead->value("/C/capacity_B1").toFloat();
    _Config.capacity_B2 = configIniRead->value("/C/capacity_B2").toFloat();
    _Config.capacity_B3 = configIniRead->value("/C/capacity_B3").toFloat();
    _Config.capacity_C1 = configIniRead->value("/C/capacity_C1").toFloat();
    _Config.capacity_C2 = configIniRead->value("/C/capacity_C2").toFloat();
    _Config.capacity_C3 = configIniRead->value("/C/capacity_C3").toFloat();
    _Config.NTh_L = configIniRead->value("/C/NTh_L").toInt();
    _Config.NTh_R = configIniRead->value("/C/NTh_R").toInt();
    _Config.NK_L = configIniRead->value("/C/NK_L").toInt();
    _Config.NK_R = configIniRead->value("/C/NK_R").toInt();
    _Config.high_A1 = configIniRead->value("/P/high_A1").toFloat();
    _Config.high_A2 = configIniRead->value("/P/high_A2").toFloat();
    _Config.high_A3 = configIniRead->value("/P/high_A3").toFloat();
    _Config.high_B1 = configIniRead->value("/P/high_B1").toFloat();
    _Config.high_B2 = configIniRead->value("/P/high_B2").toFloat();
    _Config.high_B3 = configIniRead->value("/P/high_B3").toFloat();
    _Config.high_C1 = configIniRead->value("/P/high_C1").toFloat();
    _Config.high_C2 = configIniRead->value("/P/high_C2").toFloat();
    _Config.high_C3 = configIniRead->value("/P/high_C3").toFloat();
    _Config.Rn_C1 = configIniRead->value("/Rn/Rn_C1").toFloat();
    _Config.Rn_C2 = configIniRead->value("/Rn/Rn_C2").toFloat();
    _Config.Rn_C3 = configIniRead->value("/Rn/Rn_C3").toFloat();
    _Config.NL_L = configIniRead->value("/Rn/NL_L").toInt();
    _Config.NL_R = configIniRead->value("/Rn/NL_R").toInt();
    _Config.N1_L = configIniRead->value("/Rn/N1_L").toInt();
    _Config.N1_R = configIniRead->value("/Rn/N1_R").toInt();
    _Config.N2_L = configIniRead->value("/Rn/NL_L").toInt();
    _Config.N2_R = configIniRead->value("/Rn/N2_R").toInt();
    _Config.Min = configIniRead->value("/X/Min").toInt();
    _Config.Max = configIniRead->value("/X/Max").toInt();
    _Config.smooth_Times = configIniRead->value("/S/smooth_Times").toInt();
    delete configIniRead;
}
