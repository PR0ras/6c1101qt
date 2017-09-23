#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include"test.h"

QSettings *configIniRead = new QSettings("test.ini", QSettings::IniFormat);
QString test1 = configIniRead->value("/CH/CH_1024").toString();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置缩放
    ui->qcustomplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    ui->qcustomplot->xAxis->setRange(0, 1000);//设置X轴
    ui->qcustomplot->yAxis->setRange(0, 20000);//设置Y轴
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fn1 = QFileDialog::getOpenFileName(NULL,QStringLiteral("标题"),".","*.txt");
    QFile f(fn1);//打开文件
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))//异常处理
    {
        //QApplication::exit(0);
    }

    bool ok;
    int dec=test1.toInt(&ok,10);//将String转换为Int
    QTextStream txtInput(&f);
    QString lineStr,sum;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        sum +=lineStr;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(NULL,QStringLiteral("标题"),".","*.txt");
    QFile qf(file_name);//打开文件
    if(!qf.open(QIODevice::ReadOnly | QIODevice::Text))//异常处理
    {
        //QApplication::exit(0);
    }
    bool ok;
    int dec=test1.toInt(&ok,10);//将String转换为Int
    QTextStream txtInput(&qf);
    QString lineStr,sum;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        sum +=lineStr;
    }
}
