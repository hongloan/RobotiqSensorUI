/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#ifndef CHANGESENSORIDFORM_H
#define CHANGESENSORIDFORM_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTime>


namespace Ui {
class ChangeSensorIDForm;
}

class ChangeSensorIDForm : public QWidget
{
    Q_OBJECT
public:
    explicit ChangeSensorIDForm(QWidget *parent = 0);
    ~ChangeSensorIDForm();
    int SensorID;
    bool SetIDSuccess;
private slots:
    void on_SetNewIDPushButton_clicked();
    void on_ReadIDPushButton_clicked();
private:
    Ui::ChangeSensorIDForm *ui;
    QSerialPort serial;
    bool PortIsSet;
};

#endif // CHANGESENSORIDFORM_H
