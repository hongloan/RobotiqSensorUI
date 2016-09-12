/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "adresssettingwindow.h"
#include "ui_adresssettingwindow.h"


AdressSettingWindow::AdressSettingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdressSettingWindow)
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
    Adresses[0]=1;Adresses[1]=2;Adresses[2]=3;Adresses[3]=4;Adresses[4]=5;
    Adresses[5]=6;Adresses[6]=7;Adresses[7]=8;Adresses[8]=9;Adresses[9]=10;
    AdressConfiguration = new int[10];
    AdressConfiguration=Adresses;
    AdressesAreValid=true;
}

AdressSettingWindow::~AdressSettingWindow()
{
    delete ui;
}

void AdressSettingWindow::on_RestoreDefaultConfigurationPushButton_clicked()
{
    ui->Distal1LineEdit->setText("1"); ui->Distal2LineEdit->setText("4"); ui->Distal3LineEdit->setText("7");
    ui->Medial1LineEdit->setText("2"); ui->Medial2LineEdit->setText("5"); ui->Medial3LineEdit->setText("8");
    ui->Proximal1LineEdit->setText("3"); ui->Proximal2LineEdit->setText("6"); ui->Proximal3LineEdit->setText("9");
    ui->PalmLineEdit->setText("10");
}

void AdressSettingWindow::on_ApplyConfigurationPushButton_clicked(bool checked)
{
    if (checked==false)
    {
        Adresses[0]=ui->Distal1LineEdit->text().toInt(); Adresses[1]=ui->Medial1LineEdit->text().toInt();
        Adresses[2]=ui->Proximal1LineEdit->text().toInt(); Adresses[3]=ui->Distal2LineEdit->text().toInt();
        Adresses[4]=ui->Medial2LineEdit->text().toInt(); Adresses[5]=ui->Proximal2LineEdit->text().toInt();
        Adresses[6]=ui->Distal3LineEdit->text().toInt(); Adresses[7]=ui->Medial3LineEdit->text().toInt();
        Adresses[8]=ui->Proximal3LineEdit->text().toInt(); Adresses[9]=ui->PalmLineEdit->text().toInt();
        for (int i=0;i<10;i++)
        {
            for (int j=0;j<10;j++)
            {
                if(Adresses[i]==Adresses[j] && i!=j)
                {
                    AdressesAreValid=false;
                }
            }
        }
        if (AdressesAreValid==false)
        {
            emit AdressConfigurationIsInvalid(5);
            AdressSettingWindow::on_RestoreDefaultConfigurationPushButton_clicked();
            AdressesAreValid=true;
        }
        else
        {
            QMessageBox::information(0, QString("Notification"), QString("This configuration has been properly applied."), QMessageBox::Ok);
        }
    }
}


void AdressSettingWindow::closeEvent(QCloseEvent *event)
{
    emit AdressSettingsWindowClosedSignal();
    event->accept();
    ui->Distal1LineEdit->setText(QString::number(Adresses[0])); ui->Distal2LineEdit->setText(QString::number(Adresses[3])); ui->Distal3LineEdit->setText(QString::number(Adresses[6]));
    ui->Medial1LineEdit->setText(QString::number(Adresses[1])); ui->Medial2LineEdit->setText(QString::number(Adresses[4])); ui->Medial3LineEdit->setText(QString::number(Adresses[7]));
    ui->Proximal1LineEdit->setText(QString::number(Adresses[2])); ui->Proximal2LineEdit->setText(QString::number(Adresses[5])); ui->Proximal3LineEdit->setText(QString::number(Adresses[8]));
    ui->PalmLineEdit->setText(QString::number(Adresses[9]));
}
