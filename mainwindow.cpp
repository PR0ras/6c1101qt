#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"test.h"
#include"calibration.h"
#include<iostream>

//QSettings *configIniRead = new QSettings("test.ini", QSettings::IniFormat);
//QString test1 = configIniRead->value("/CH/4100").toString();
Config  _Config;

int k=0,j=0;//计数，多少个数据
struct Messure_Nu_info			//保存谱线中的核素信息  某些变量没用  可以注释掉
{
    unsigned int Exist_Nuclide_num;
    unsigned int Last_Exist_Nuclide_num;
    int Record_GetData_Times;
    int Total_Data[1024];
    int Peak_Area[200];
    BOOL Search_Peak_Flag;
    BOOL Correct_Flag;

    double cps;
    double m_doseRate;

    int temp_data[1024];
};

CCalibration Calibration;
Messure_Nu_info Me_Nu_info;

struct Me_Exist_Nu_info			//保存谱线中的核素信息 建议封装成全局变量
{
    unsigned int Probable_Nuclide[30];
    double Probablity;
    char/*wchar_t */Exist_Nuclide_Type[30];
    char/*wchar_t*/ Exist_Nuclide_Name[30];
};
Me_Exist_Nu_info Me_Exist_Nuclide[30];

struct Nuclide_info				//保存在核素库读出的信息  封装成全局变量或者在函数中定义为输出接口
{
//	wchar_t  N_type[30];
    char N_type[30];			//核素种类
//	CString N_type;
    char N_Name[30];			//核素名称
//	wchar_t N_Name[30];
    double N_Energy[30];			//核素能量
    double N_Proportion[30];		//核素分支比
    int N_En_num;				//核素库存在的核素个数
};

Nuclide_info Nuclide[50];
Nuclide_info Lib_Nu;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QVector<double> x(3000), y(3000);
    //_Config_File();
    //Config  _Config;
    _Config_File(_Config);
    //bool ok;
    //QString GG=_Config.activity_A.to(&ok,10);
    //QString s = QString::number(_Config.NTh_L, 10);
    //ui->label_5->setText(s);
    //设置缩放
    ui->qcustomplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

    ui->qcustomplot->xAxis->setRange(0, 1000);//设置X轴
    ui->qcustomplot->yAxis->setRange(0, 20000);//设置Y轴
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setName(QStringLiteral("原始数据"));
    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(1)->setName(QStringLiteral("本底数据"));
    ui->qcustomplot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->qcustomplot->legend->setFont(legendFont);
    ui->qcustomplot->legend->setSelectedFont(legendFont);
    ui->qcustomplot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

    connect(ui->qcustomplot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    // connect some interaction slots:
    connect(ui->qcustomplot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    connect(ui->qcustomplot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
    //connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
    // connect slot that shows a message in the status bar when a graph is clicked:
    connect(ui->qcustomplot, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));
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
    QTextStream txtInput(&f);
    QString lineStr;
    //QVector<double> x(3000), y(3000);

    int dec,ymax=0;
    int &i=k;
    k=0;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        dec=lineStr.toInt(&ok,10);//将String转换为Int
        //ui->label->setText(lineStr);
        m_y.push_back(dec);
        m_x.push_back(i);
        i++;
        if(ymax<dec)
        ymax=dec;
    }
    ymax+=1000;
    //ui->qcustomplot->xAxis->setRange(0, i);
    ui->qcustomplot->graph()->rescaleAxes(true);
    ui->qcustomplot->yAxis->setRange(0, ymax);
    //ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setData(m_x, m_y);
    QPen graphPen;
    graphPen.setColor(QColor(255,0,0));
    ui->qcustomplot->graph(0)->setPen(graphPen);
    ui->qcustomplot->replot();
}

void MainWindow::on_pushButton_2_clicked()
{
    //绘图
    QString file_name = QFileDialog::getOpenFileName(NULL,QStringLiteral("标题"),".","*.txt");
    QFile qf(file_name);//打开文件
    if(!qf.open(QIODevice::ReadOnly | QIODevice::Text))//异常处理
    {
        //QApplication::exit(0);
    }
    bool ok;
    int dec,ymax=0;
    int &i=j;
    i=0;
    QString lineStr;
    //QVector<double> x(3000), y(3000);
    QTextStream txtInput(&qf);
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        dec=lineStr.toInt(&ok,10);//将String转换为Int
        //ui->label->setText(lineStr);
        m1_y.push_back(dec);
        m1_x.push_back(i);
        i++;
        if(ymax<dec)
            ymax=dec;
    }
    ymax+=1000;
    //ui->qcustomplot->xAxis->setRange(0, i);
    ui->qcustomplot->graph()->rescaleAxes(true);
    ui->qcustomplot->yAxis->setRange(0, ymax);
    //ui->qcustomplot->graph()->setLineStyle((QCPGraph::LineStyle)i);//设置线性
    //ui->qcustomplot->addGraph();
    //ui->qcustomplot->graph(1)->setName(QStringLiteral("本底数据").arg(ui->qcustomplot->graphCount()-1));
    ui->qcustomplot->graph(1)->setData(m1_x, m1_y);
    QPen graphPen;
    graphPen.setColor(QColor(2,27,0));
    ui->qcustomplot->graph(1)->setPen(graphPen);
    ui->qcustomplot->replot();

}

void MainWindow::on_pushButton_10_clicked()
{
    QApplication::exit(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    //QString s = QString::number(_Config.NTh_L, 10);
    //ui->label_5->setText(s);
    Calibration.BDKC(pure,origindata,backdata);
    /*
    ((CSeries)m_chart.Series(0)).DeleteRange(0,4100);

    for(int i=0;i<4100;i++)
        ((CSeries)m_chart.Series(0)).AddXY(i,pure[i],NULL,RGB(255,0,255));
    */
}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "qcustomplot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->qcustomplot->replot();
    }
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "qcustomplot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->qcustomplot->replot();
    }
  }
}

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "qcustomplot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->qcustomplot->replot();
    }
  }
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->qcustomplot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->qcustomplot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->qcustomplot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->qcustomplot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->qcustomplot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->qcustomplot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->qcustomplot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->qcustomplot->graph(i);
    QCPPlottableLegendItem *item = ui->qcustomplot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeDrag(ui->qcustomplot->xAxis->orientation());
  else if (ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeDrag(ui->qcustomplot->yAxis->orientation());
  else
    ui->qcustomplot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeZoom(ui->qcustomplot->xAxis->orientation());
  else if (ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeZoom(ui->qcustomplot->yAxis->orientation());
  else
    ui->qcustomplot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::addRandomGraph()
{
  int n = 50; // number of points in graph
  double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
  double yOffset = (rand()/(double)RAND_MAX - 0.5)*10;
  double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
  QVector<double> x(n), y(n);
  for (int i=0; i<n; i++)
  {
    x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
    y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
  }

  ui->qcustomplot->addGraph();
  ui->qcustomplot->graph()->setName(QString("New graph %1").arg(ui->qcustomplot->graphCount()-1));
  ui->qcustomplot->graph()->setData(x, y);
  ui->qcustomplot->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
  if (rand()%100 > 50)
    ui->qcustomplot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
  QPen graphPen;
  graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
  graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
  ui->qcustomplot->graph()->setPen(graphPen);
  ui->qcustomplot->replot();
}

void MainWindow::removeSelectedGraph()
{
  if (ui->qcustomplot->selectedGraphs().size() > 0)
  {
    ui->qcustomplot->removeGraph(ui->qcustomplot->selectedGraphs().first());
    ui->qcustomplot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->qcustomplot->clearGraphs();
  ui->qcustomplot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->qcustomplot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (ui->qcustomplot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->qcustomplot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }

  menu->popup(ui->qcustomplot->mapToGlobal(pos));
}

void MainWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->qcustomplot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->qcustomplot->replot();
    }
  }
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QStringLiteral("Clicked on graph '%1' at data point #%4 with value %5.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
  ui->statusBar->showMessage(message, 25000);
}

int MainWindow::AreaSearchPeakThree(int actual_Peak[4100],int actual_LBor[4100],int actual_RBor[4100],int actual_Peaknum)		//对称零面积寻峰
{		//寻到的峰位 左边界 右边界 峰位个数	   需要自己封装到dll

    int Peak[4100];
    int temp_PeakPos[512],searchLeft = 0,searchRight = 0;
    int actual_temp_num = 0,LeftBor = 0,RightBor = 0;
    int temp_PeakPos_num = 0,L_Bor_num = 0,R_Bor_num = 0,temp_peak = 0,temp_peak_num = 0;
    float G[13],d = 0,f = 2.5,D = 0.0,H = 4.0;
    float SS[4100],y1[4100],y2[4100];
    actual_Peaknum = 0;

    memset(SS,0,sizeof(SS));
    memset(actual_Peak,0,sizeof(actual_Peak));
    memset(actual_LBor,0,sizeof(actual_LBor));
    memset(actual_RBor,0,sizeof(actual_RBor));

    for(int j=0;j<13;j++)
    {
        G[j] = (float)exp(-4.0*log(2.0)*(j/H)*(j/H));
        D = D+G[j];
    }
    d = (float)(D/13.0);

    memset(y1,0,sizeof(y1));
    memset(y2,0,sizeof(y2));

    for(int i=2;i<4100-10;i++)
    {
        for(int j=0;j<13;j++)
        {
            y1[i-2] = y1[i-2] + data[i+j]*(G[j]-d);
            y2[i-2] = y2[i-2] + data[i+j]*(G[j]-d)*(G[j]-d);
        }

        SS[i-2] = y1[i-2]/(float)sqrt(y2[i-2]);
        if(SS[i-2]>f)
        {
            temp_PeakPos[temp_PeakPos_num] = i;
            temp_PeakPos_num++;
        }
    }

    for(int i=0;i<temp_PeakPos_num-1;i++)
    {
        temp_peak = temp_PeakPos[i];
        if((data[temp_peak]>data[temp_peak+1])&&(data[temp_peak]>data[temp_peak-1]))
        {
            if((data[temp_peak]>data[temp_peak+4])&&(data[temp_peak]>data[temp_peak-4]))
            {
                Peak[temp_peak_num] = temp_peak;
                temp_peak_num++;
            }
        }
    }

    for(int i=0;i<temp_peak_num;i++)
    {
        int peakch = Peak[i];
        searchLeft = peakch;
        searchRight = peakch;
        float L_min_SS = 0,R_min_SS = 0;
        L_min_SS = SS[searchLeft];
        R_min_SS = SS[searchRight];

        for(int i_left=0;i_left<35;i_left++)
        {
            if((searchLeft - i_left) == 0)	break;
            else if(L_min_SS > SS[searchLeft-i_left])
            {
                L_min_SS = SS[searchLeft - i_left];

                if((SS[searchLeft - i_left - 1] > L_min_SS)
                    && (SS[searchLeft - i_left + 1] > L_min_SS)
                    && L_min_SS < 0)
                {
                    LeftBor = searchLeft - i_left;
                    break;
                }
            }
        }

        for(int i_right=0;i_right<35;i_right++)
        {
            if((searchRight + i_right) == 511)	break;
            else if(R_min_SS > SS[searchRight + i_right])
            {
                R_min_SS = SS[searchRight + i_right];

                if((SS[searchRight + i_right - 1] > R_min_SS)
                    && (SS[searchRight + i_right + 1] > R_min_SS)
                    && R_min_SS < 0)
                {
                    RightBor = searchRight + i_right;
                    break;
                }
            }
        }

        LeftBorder[L_Bor_num] = LeftBor;
        L_Bor_num++;
        RightBorder[R_Bor_num] = RightBor;
        R_Bor_num++;

    }

    for(int i=0;i<temp_peak_num;i++)
    {
        if((RightBorder[i] - LeftBorder[i])>_Config.Min && (RightBorder[i] - LeftBorder[i])<_Config.Max)
        {
            actual_Peak[actual_temp_num] = Peak[i];
            actual_LBor[actual_temp_num] = LeftBorder[i];
            actual_RBor[actual_temp_num] = RightBorder[i];
            actual_temp_num++;
        }
    }

    actual_Peaknum = actual_temp_num;
    return actual_Peaknum;
}



void MainWindow::on_pushButton_4_clicked()
{
    memcpy(Calibration.KC,pure,sizeof(pure));
    Calibration.DQD(_Config.Rn_C1,_Config.Rn_C2,_Config.Rn_C3,_Config.NL_L,_Config.NL_R,_Config.N1_L,_Config.N1_R,_Config.N2_L,_Config.N2_R);
    /*
     * CString strDQD,strD;
    strDQD.Format("%d",Calibration.Nu);		//动态库设置的变量 有重复
    strD = "NU值：" + strDQD;
    this->SetDlgItemTextA(IDC_STATIC_NU,strD);
    */
}

void MainWindow::on_pushButton_5_clicked()
{
    memcpy(Calibration.KC,pure,sizeof(pure));
    Calibration.GDXZ(_Config.high_A1,_Config.high_A2,_Config.high_A3,_Config.high_B1,_Config.high_B2,_Config.high_B3);
    /*
    ((CSeries)m_chart.Series(0)).DeleteRange(0,4100);

    for(int i=0;i<4100;i++)
        ((CSeries)m_chart.Series(0)).AddXY(i,Calibration.GD[i],NULL,RGB(255,0,255));
    */
}

void MainWindow::on_pushButton_6_clicked()
{
    memcpy(Calibration.KC,pure,sizeof(pure));

    Calibration.NU = Calibration.Nu;

    Calibration.HLJS(_Config.capacity_A1,_Config.capacity_A2,_Config.capacity_A3,_Config.capacity_B1,_Config.capacity_B2,_Config.capacity_B3,
        _Config.capacity_C1,_Config.capacity_C2,_Config.capacity_C3,_Config.NTh_L,_Config.NTh_R,_Config.NK_L,_Config.NK_R);
    /*CString strCU,strCTh,strCK;
    strCU.Format("%4f",Calibration.CU);
    strCU = "U含量：" + strCU + "%";
    SetDlgItemTextA(IDC_STATIC_U,strCU);

    strCTh.Format("%4f",Calibration.CTh);
    strCTh = "U含量：" + strCTh + "%";
    SetDlgItemTextA(IDC_STATIC_Th,strCTh);

    strCK.Format("%4f",Calibration.CK);
    strCK = "U含量：" + strCK + "%";
    SetDlgItemTextA(IDC_STATIC_K,strCK);*/
}


void MainWindow::on_pushButton_8_clicked()
{
    int sum = 0;
    for(int i=0;i<4100;i++)
    {
        sum +=  origindata[i];
    }

    double dose;
    //CString strDose;

    dose = _Config.dose_A * sum * sum + _Config.dose_B * sum + _Config.dose_C;
    //strDose.Format("%4f",dose);
    //strDose = "剂量：" + strDose;
    //SetDlgItemText(IDC_STATIC_DOSE,strDose);
}
