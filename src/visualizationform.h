/**************************************************
 * Author: Jean-Philippe Roberge ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#ifndef VISUALIZATIONFORM_H
#define VISUALIZATIONFORM_H

#include <QWidget>
#include <mgl2/qt.h>
#include <mgl2/qmathgl.h>
#include <mgl2/fltk.h>
#include <qthread.h>
#include "mainwindow.h"
#include <QTime>
#include <fftw3.h>
#include <QFileInfo>
#include <QtCore/QDebug>
#include <QtCore/qmath.h>
#include <QCloseEvent>
#include <QShortcut>
#include <QFileDialog>

namespace Ui {
class VisualizationForm;
}

class VisualizationForm : public QWidget
{
    Q_OBJECT

public:
    explicit VisualizationForm(QWidget *parent = 0, MainWindow *mainwnd=0);
    ~VisualizationForm();
    mglData DataStatic, SumDataStatic, SumOfSquaredDataStatic, MeanDataStatic, STDDataStatic, DataTable, DataStaticAllSensor[10];
    mglData DataDynamicY, LastDataDynamicY, DataTime, DataDynamicResized, DataDynamicFourier, DataDynamicFourier2, DataDynamicFourier3, DataDynamicFourier4;
    mglData DataHighSensitivity;
    mglGraph *grDyn,*grDyn2;
    int SumOfAllTaxelsAvg, SumOfAllTaxelsStd;
    int closed,DynamicDataViewMode;
    int ArmsSensorToVisualize[3],CurrentSensorToVisualize,CurrentArmToVisualize;
    int CurrentFrequency, FourierCount;
    bool ResetButtonIsPushed,STFAIsReady,VisualizationIsLocked;
    double *FFTin,*FFTin2,*FFTin3,*FFTin4;
    fftw_complex *FFTout,*FFTout2,*FFTout3,*FFTout4;
    fftw_plan FFTplan,FFTplan2,FFTplan3,FFTplan4;
    QString PicturePath;

public slots:
    void on_ViewComboBox_currentIndexChanged(int index);
    void PlotData(int TypeOfData);
    void CloseVF();
    void RevertSTFA();

signals:
    void VFClosed();

private slots:
    void on_resetPushButton_clicked();
    void on_visualizeArm1CheckBox_clicked(bool checked);
    void on_visualizeArm2CheckBox_clicked(bool checked);
    void on_visualizeArm3CheckBox_clicked(bool checked);
    void on_SensorArm1ComboBox_currentIndexChanged(int index);
    void on_displayValuesCheckbox_clicked(bool checked);
    void on_AverageValuesCheckBox_clicked(bool checked);
    void on_FreezePushButton_clicked();
    void on_STFACheckbox_clicked(bool checked);
    void on_ExportGraphPushButton_clicked();
    void on_MaxCountSpinBox_valueChanged(int arg1);
    void on_RotateRightPushButton_clicked();
    void on_RotateLeftPushButton_clicked();
    void on_RotateUpPushButton_clicked();
    void on_RotateDownPushButton_clicked();

private:
    Ui::VisualizationForm *ui;
    mglGraph *gr,*grHigh,*grAll,*grFourier;
    uchar *buf, *buf2;
    int w,h,PassNumberAllSensors,NumberOfStaticData;
    QPixmap pic,pic2;
    void closeEvent(QCloseEvent *event);
    MainWindow* MW;
    int WndWidth, WndHeight;
    bool DataFreeze,AllSensorsFirstTime;
    QTime timer;
    mreal val[2],val2[6],val3[4];
};

#endif // VISUALIZATIONFORM_H
