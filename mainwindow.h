#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"qcustomplot.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void titleDoubleClick(QMouseEvent *event);
    void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
    void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
    void selectionChanged();
    void mousePress();
    void mouseWheel();
    void addRandomGraph();
    void removeSelectedGraph();
    void removeAllGraphs();
    void contextMenuRequest(QPoint pos);
    void moveLegend();
    void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
    QImage image;

public:
    int data[4100];			//光滑后的数据
    int origindata[4100];	//原始数据
    int backdata[4100];		//本底数据
    int pure[4100];			//净数据
    int convertData[4100];	//中间转换数组
    void WeiCiSeven(int Beginch,int Endch);		//谱线光滑
    int AreaSearchPeakThree(int actual_Peak[4000],int actual_LBor[4000],int actual_RBor[4100],int actual_Peaknum);
    int LeftBorder[4100];						//左边界
    int RightBorder[4100];						//右边界
    QVector<double> m_x,m_y;
    QVector<double> mid_y,m1_x,m1_y ;
};

#endif // MAINWINDOW_H
