/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "changesensoridform.h"
#include "ui_changesensoridform.h"
#include "communicationhandler.h"


ChangeSensorIDForm::ChangeSensorIDForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeSensorIDForm)
{
    ui->setupUi(this);
    SensorID=-1;
    ui->IDToSetLineEdit->setInputMask("000"); ui->IDToSetLineEdit->setMaxLength(3);
    PortIsSet=false;
//    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
//    {
//        qDebug() << "Name        : " << info.portName();
//        qDebug() << "Description : " << info.description();
//        qDebug() << "Manufacturer: " << info.manufacturer();
//        qDebug() << "Standard BAUD Rates:" << info.standardBaudRates();
//        if(info.description()=="USBUARTalex") //We found it so we set it:
//        {
//            qint32 custombaudrate=460800;
//            serial.setPort(info);
//            PortIsSet=true;
//            serial.setBaudRate(custombaudrate);
//            serial.setParity(QSerialPort::NoParity);
//            serial.setStopBits(QSerialPort::OneStop);
//            serial.setDataBits(QSerialPort::Data8);
//            serial.setFlowControl(QSerialPort::NoFlowControl);
//        }
//    }
}

ChangeSensorIDForm::~ChangeSensorIDForm()
{
    delete ui;
}

void ChangeSensorIDForm::on_SetNewIDPushButton_clicked()
{
    CommunicationHandler *InitializationAgent = new CommunicationHandler(0,0);


    //Begin with CRC16 calculation:


    if (ui->IDToSetLineEdit->text().toInt() > 127)
    {
        QMessageBox::information(0, QString("Error"), QString("An ID cannot be set to a value greather than 127."), QMessageBox::Ok);
        ui->IDToSetLineEdit->setText("");
    }
    else //If the ID is valid:
    {
        SetIDSuccess=InitializationAgent->SetSensorID(ui->IDToSetLineEdit->text().toInt());
        if (SetIDSuccess==true)
        {
            QMessageBox::information(0, QString("Ok"), QString("The new ID has been set."), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(0, QString("Error"), QString("Unable to set sensor ID, make sure it is properly connected."), QMessageBox::Ok);
        }
    }
    if (serial.isOpen()==true)
    {
        serial.close();
    }
}

void ChangeSensorIDForm::on_ReadIDPushButton_clicked()
{
    CommunicationHandler *InitializationAgent = new CommunicationHandler(0,0);
    SensorID=InitializationAgent->ReadSensorID();

    if (serial.open(QIODevice::ReadWrite))
    {

        if (SensorID!=-1)
        {
            ui->CurrentIDLabel->setText(QString::number(SensorID));
        }
        else
        {
            QMessageBox::information(0, QString("Error"), QString("Unable to read sensor ID, make sure it is properly connected."), QMessageBox::Ok);
            ui->CurrentIDLabel->setText("Error");
        }
    }
    else
    {
        qDebug() << "The port is currently unavailable for reading-writting.";
    }

    if (serial.isOpen()==true)
    {
        serial.close(); PortIsSet=false;
    }
}

