/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "adresssettingwindow.h"
#include "ui_adresssettingwindow.h"


AddressSettingWindow::AddressSettingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddressSettingWindow)
{
    ui->setupUi(this);
    ui->Distal1LineEdit->setInputMask("00"); ui->Distal1LineEdit->setMaxLength(2);
    ui->Distal2LineEdit->setInputMask("00"); ui->Distal2LineEdit->setMaxLength(2);
    ui->Distal3LineEdit->setInputMask("00"); ui->Distal3LineEdit->setMaxLength(2);
    ui->Medial1LineEdit->setInputMask("00"); ui->Medial1LineEdit->setMaxLength(2);
    ui->Medial2LineEdit->setInputMask("00"); ui->Medial2LineEdit->setMaxLength(2);
    ui->Medial3LineEdit->setInputMask("00"); ui->Medial3LineEdit->setMaxLength(2);
    ui->Proximal1LineEdit->setInputMask("00"); ui->Proximal1LineEdit->setMaxLength(2);
    ui->Proximal2LineEdit->setInputMask("00"); ui->Proximal2LineEdit->setMaxLength(2);
    ui->Proximal3LineEdit->setInputMask("00"); ui->Proximal3LineEdit->setMaxLength(2);
    ui->PalmLineEdit->setInputMask("00"); ui->PalmLineEdit->setMaxLength(2);
    Addresses[0]=1;Addresses[1]=2;Addresses[2]=3;Addresses[3]=4;Addresses[4]=5;
    Addresses[5]=6;Addresses[6]=7;Addresses[7]=8;Addresses[8]=9;Addresses[9]=10;
    AddressConfiguration = new int[10];
    AddressConfiguration=Addresses;
    AddressesAreValid=true;
}

AddressSettingWindow::~AddressSettingWindow()
{
    delete ui;
}

void AddressSettingWindow::on_RestoreDefaultConfigurationPushButton_clicked()
{
    ui->Distal1LineEdit->setText("1"); ui->Distal2LineEdit->setText("4"); ui->Distal3LineEdit->setText("7");
    ui->Medial1LineEdit->setText("2"); ui->Medial2LineEdit->setText("5"); ui->Medial3LineEdit->setText("8");
    ui->Proximal1LineEdit->setText("3"); ui->Proximal2LineEdit->setText("6"); ui->Proximal3LineEdit->setText("9");
    ui->PalmLineEdit->setText("10");
}

void AddressSettingWindow::on_ApplyConfigurationPushButton_clicked(bool checked)
{
    if (checked==false)
    {
        Addresses[0]=ui->Distal1LineEdit->text().toInt(); Addresses[1]=ui->Medial1LineEdit->text().toInt();
        Addresses[2]=ui->Proximal1LineEdit->text().toInt(); Addresses[3]=ui->Distal2LineEdit->text().toInt();
        Addresses[4]=ui->Medial2LineEdit->text().toInt(); Addresses[5]=ui->Proximal2LineEdit->text().toInt();
        Addresses[6]=ui->Distal3LineEdit->text().toInt(); Addresses[7]=ui->Medial3LineEdit->text().toInt();
        Addresses[8]=ui->Proximal3LineEdit->text().toInt(); Addresses[9]=ui->PalmLineEdit->text().toInt();
        for (int i=0;i<10;i++)
        {
            for (int j=0;j<10;j++)
            {
                if(Addresses[i]==Addresses[j] && i!=j)
                {
                    AddressesAreValid=false;
                }
            }
        }
        if (AddressesAreValid==false)
        {
            emit AddressConfigurationIsInvalid(5);
            AddressSettingWindow::on_RestoreDefaultConfigurationPushButton_clicked();
            AddressesAreValid=true;
        }
        else
        {
            QMessageBox::information(0, QString("Notification"), QString("This configuration has been properly applied."), QMessageBox::Ok);
        }
    }
}


void AddressSettingWindow::closeEvent(QCloseEvent *event)
{
    emit AddressSettingsWindowClosedSignal();
    event->accept();
    ui->Distal1LineEdit->setText(QString::number(Addresses[0])); ui->Distal2LineEdit->setText(QString::number(Addresses[3])); ui->Distal3LineEdit->setText(QString::number(Addresses[6]));
    ui->Medial1LineEdit->setText(QString::number(Addresses[1])); ui->Medial2LineEdit->setText(QString::number(Addresses[4])); ui->Medial3LineEdit->setText(QString::number(Addresses[7]));
    ui->Proximal1LineEdit->setText(QString::number(Addresses[2])); ui->Proximal2LineEdit->setText(QString::number(Addresses[5])); ui->Proximal3LineEdit->setText(QString::number(Addresses[8]));
    ui->PalmLineEdit->setText(QString::number(Addresses[9]));
}
