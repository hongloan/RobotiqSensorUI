/**************************************************
 * Author: Jean-Philippe Roberge ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#ifndef COMMUNICATIONHANDLER_H
#define COMMUNICATIONHANDLER_H

#include "visualizationform.h"
#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <mgl2/qt.h>
#include <mgl2/qmathgl.h>
#include <mgl2/fltk.h>
#include <QWidget>
#include <QTime>
#include <QFile>
#include <stdio.h>
#include <QtCore/QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QElapsedTimer>
#include "crc16lookuptable.h"


class CommunicationHandler : public QObject
{
    Q_OBJECT
public:
    CommunicationHandler(VisualizationForm *visualiationWindow, MainWindow *mainwnd);
public slots:
    void process();
    int ReadSensorID();
    bool SetSensorID(int DesiredID);
signals:
    void finished();
    void NewDataAvailable(int Datatype);
    void ErrorOpeningPort(int ErrorType);
    void ErrorPlottingFourierTransform(int ErrorType);
private:
    VisualizationForm* VF;
    MainWindow* MW;
    QFile* FilePointer;
    QTime timer,timer2,timer3,FrequencyTimer;
    long DynRange;
    int msec,count,count2,count3,CountForFrequency,LastSensorThatWasVisualized;
    qint64 NanoSecTimer;
    int NumberOfPorts,CurrentArmIndex,InitialValue[10][28],SensorNumberWhenErrorWasPrompted,CommunicationErrorCount,DataIni,Actual,SensorID;
    bool WrongCom,PortIsSet,PortConfigured,StaticValueTooHigh;
    mglData DataProximal,DataMedial,DataDistal, DataTempH, DataMean;
    mreal val[6];
    QString ParameterString;
    QByteArray ParameterStringByteArray;
    const char *ParameterPointer;
    CRC16LookupTable *CRCLookuptable;
    bool SetSensorIDSuccess;
    int crc;
    QElapsedTimer OneMsecTimer;
};


#endif // COMMUNICATIONHANDLER_H
