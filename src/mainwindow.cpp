/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "visualizationform.h"
#include "aboutrobotiqsensorui.h"
#include "communicationhandler.h"
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Griper1 = new Gripper; Griper2 = new Gripper; Griper3 = new Gripper;
    Griper1->GripperEnabled=true;
    ui->numberOfSensorsArm1Label->setVisible(true); ui->numberOfSensorsArm1SpinBox->setVisible(true);
    ui->numberOfSensorsArm2Label->setVisible(false); ui->numberOfSensorsArm2SpinBox->setVisible(false);
    ui->numberOfSensorsArm3Label->setVisible(false); ui->numberOfSensorsArm3SpinBox->setVisible(false);

    ui->configurationGroupBox->setStyleSheet("QGroupBox#configurationGroupBox { border: 2px solid black; border-radius: 10px; margin-top: 0.5em} QGroupBox::Title { subcontrol-origin: margin; background-color: light grey; padding:0 3px}");
    ui->dataGroupBox->setStyleSheet("QGroupBox#dataGroupBox { border: 2px solid black; border-radius: 10px; margin-top: 0.5em} QGroupBox::Title { subcontrol-origin: margin; background-color: light grey; padding:0 3px 0 3px }");
    ui->communicationSettingsGroupBox->setStyleSheet("QGroupBox#communicationSettingsGroupBox { border: 2px solid black; border-radius: 10px; margin-top: 0.5em} QGroupBox::Title { subcontrol-origin: margin; background-color: light grey; padding:0 3px 0 3px }");
    ui->arm1LoggingSettingsGroupBox->setStyleSheet("QGroupBox#arm1LoggingSettingsGroupBox { border: 2px solid black; border-radius: 10px; margin-top: 0.5em} QGroupBox::Title { subcontrol-origin: margin; background-color: light grey; padding:0 3px 0 3px }");
    ui->arm2LoggingSettingsGroupBox->setStyleSheet("QGroupBox#arm2LoggingSettingsGroupBox { border: 2px solid black; border-radius: 10px; margin-top: 0.5em} QGroupBox::Title { subcontrol-origin: margin; background-color: light grey; padding:0 3px 0 3px }");
    ui->arm3LoggingSettingsGroupBox->setStyleSheet("QGroupBox#arm3LoggingSettingsGroupBox { border: 2px solid black; border-radius: 10px; margin-top: 0.5em} QGroupBox::Title { subcontrol-origin: margin; background-color: light grey; padding:0 3px 0 3px }");
    ui->configurationGroupBox->move(20,27);
    ui->dataGroupBox->move(20,160);
    ui->communicationSettingsGroupBox->move(330,160);
    ui->PortSelectionComboBox->setDisabled(true);
    ui->directoryLocationLineEdit->setDisabled(true);
    ui->browseDirectoryPushButton->setDisabled(true);
    ui->stopAcquisitionPushButton->setDisabled(true);
    ui->enableVisualizationCheckBox->setChecked(true);
    EnableDataVisualization=true; EnableDatalogging=false; DataKind=0;
    FilePath.append(QDir::homePath());
    FilePath.append("/LogFile.csv");
    ManualCommunicationSettings=false;
    ui->directoryLocationLineEdit->setText(FilePath);
    NumberOfArms=1; NumberOfSensors[0]=10; NumberOfSensors[1]=10; NumberOfSensors[2]=10;
    ui->loggingSettingsPushButton->setEnabled(false);
    ui->ConfigurationTabWidget->setTabEnabled(1,false);
    ui->arm2LoggingSettingsGroupBox->setVisible(false);
    ui->arm3LoggingSettingsGroupBox->setVisible(false);
    ui->ClearAllArm2PushButton->setVisible(false);ui->ClearAllArm3PushButton->setVisible(false);
    ui->SelectAllArm2PushButton->setVisible(false);ui->SelectAllArm3PushButton->setVisible(false);
    ui->arm1LoggingSettingsGroupBox->move(28,75); ui->SelectAllArm1PushButton->move(550,100);ui->ClearAllArm1PushButton->move(550,130);
    ui->Arm1Sensor1CheckBox->setChecked(true); ui->Arm2Sensor1CheckBox->setChecked(true); ui->Arm3Sensor1CheckBox->setChecked(true);
    ListOfSensorsPerArms[0][0]=-1;ListOfSensorsPerArms[0][1]=-1;ListOfSensorsPerArms[0][2]=-1;ListOfSensorsPerArms[0][3]=-1;ListOfSensorsPerArms[0][4]=-1;
    ListOfSensorsPerArms[0][5]=-1;ListOfSensorsPerArms[0][6]=-1;ListOfSensorsPerArms[0][7]=-1;ListOfSensorsPerArms[0][8]=-1;ListOfSensorsPerArms[0][9]=-1;ListOfSensorsPerArms[0][10]=-1;
    ListOfSensorsPerArms[1][0]=-1;ListOfSensorsPerArms[1][1]=-1;ListOfSensorsPerArms[1][2]=-1;ListOfSensorsPerArms[1][3]=-1;ListOfSensorsPerArms[1][4]=-1;
    ListOfSensorsPerArms[1][5]=-1;ListOfSensorsPerArms[1][6]=-1;ListOfSensorsPerArms[1][7]=-1;ListOfSensorsPerArms[1][8]=-1;ListOfSensorsPerArms[1][9]=-1;ListOfSensorsPerArms[1][10]=-1;
    ListOfSensorsPerArms[2][0]=-1;ListOfSensorsPerArms[2][1]=-1;ListOfSensorsPerArms[2][2]=-1;ListOfSensorsPerArms[2][3]=-1;ListOfSensorsPerArms[2][4]=-1;
    ListOfSensorsPerArms[2][5]=-1;ListOfSensorsPerArms[2][6]=-1;ListOfSensorsPerArms[2][7]=-1;ListOfSensorsPerArms[2][8]=-1;ListOfSensorsPerArms[2][9]=-1;ListOfSensorsPerArms[2][10]=-1;
    NumberOfSensorsToLogArm1=0;NumberOfSensorsToLogArm2=0;NumberOfSensorsToLogArm3=0; NumberOfDetectedPorts=0;
    ui->AdressSettingsArm2PushButton->setVisible(false); ui->AdressSettingsArm3PushButton->setVisible(false);
    AdressSettingWndArm1= new AdressSettingWindow;
    AdressSettingWndArm1->setFixedSize(820,500);
    AdressSettingWndArm1->setWindowTitle("Adress Settings");
    connect(AdressSettingWndArm1,SIGNAL(AdressSettingsWindowClosedSignal()),this,SLOT(ReactToAdressSettingsWindowClosedArm1()));
    connect(AdressSettingWndArm1,SIGNAL(AdressConfigurationIsInvalid(int)),this,SLOT(PromptErrorToScreen(int)));
    AdressSettingWndArm2= new AdressSettingWindow;
    AdressSettingWndArm2->setFixedSize(820,500);
    AdressSettingWndArm2->setWindowTitle("Adress Settings");
    connect(AdressSettingWndArm2,SIGNAL(AdressSettingsWindowClosedSignal()),this,SLOT(ReactToAdressSettingsWindowClosedArm2()));
    connect(AdressSettingWndArm2,SIGNAL(AdressConfigurationIsInvalid(int)),this,SLOT(PromptErrorToScreen(int)));
    AdressSettingWndArm3= new AdressSettingWindow;
    AdressSettingWndArm3->setFixedSize(820,500);
    AdressSettingWndArm3->setWindowTitle("Adress Settings");
    connect(AdressSettingWndArm3,SIGNAL(AdressSettingsWindowClosedSignal()),this,SLOT(ReactToAdressSettingsWindowClosedArm3()));
    connect(AdressSettingWndArm3,SIGNAL(AdressConfigurationIsInvalid(int)),this,SLOT(PromptErrorToScreen(int)));
    ui->Port2Label->setVisible(false); ui->Port2SelectionComboBox->setVisible(false); ui->Port2SelectionComboBox->setDisabled(true);
    ui->Port3Label->setVisible(false); ui->Port3SelectionComboBox->setVisible(false); ui->Port3SelectionComboBox->setDisabled(true);
    ui->communicationSettingsGroupBox->setFixedHeight(111);
    ui->numberOfSensorsArm1SpinBox->setVisible(false);ui->numberOfSensorsArm2SpinBox->setVisible(false);ui->numberOfSensorsArm3SpinBox->setVisible(false);
    ChangeIDForm= new ChangeSensorIDForm;
    ChangeIDForm->setWindowTitle("Change Sensor ID");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_numberOfArmsSpinBox_valueChanged(int arg1)
{
    if(arg1==1)
    {
        ui->configurationGroupBox->setFixedHeight(85);
        ui->numberOfSensorsArm1Label->setVisible(true);
        ui->numberOfSensorsArm2Label->setVisible(false);
        ui->numberOfSensorsArm2SpinBox->setVisible(false);
        ui->numberOfSensorsArm3Label->setVisible(false);
        ui->numberOfSensorsArm3SpinBox->setVisible(false);
        ui->SelectAllArm2PushButton->setVisible(false); ui->ClearAllArm2PushButton->setVisible(false);
        ui->dataGroupBox->move(20,160);
        ui->communicationSettingsGroupBox->move(330,160);
        NumberOfArms=1;
        ui->arm2LoggingSettingsGroupBox->setVisible(false);
        ui->arm3LoggingSettingsGroupBox->setVisible(false);
        ui->arm1LoggingSettingsGroupBox->move(28,75); ui->SelectAllArm1PushButton->move(550,100); ui->ClearAllArm1PushButton->move(550,130);
        ui->selectDataTypeLabel->move(30,10);
        ui->dataTypeToLogComboBox->move(232,15);
        ui->AdressSettingsArm2PushButton->setVisible(false);
        Griper2->GripperEnabled=false; Griper3->GripperEnabled=false;
        ui->Port2Label->setVisible(false); ui->Port2SelectionComboBox->setVisible(false);
        ui->communicationSettingsGroupBox->setFixedHeight(111);
    }
    else if (arg1==2)
    {
        ui->configurationGroupBox->setFixedHeight(115);
        ui->numberOfSensorsArm1Label->setVisible(true);
        ui->numberOfSensorsArm2Label->setVisible(true);
        ui->numberOfSensorsArm3Label->setVisible(false);
        ui->numberOfSensorsArm3SpinBox->setVisible(false);
        ui->SelectAllArm2PushButton->setVisible(true);ui->ClearAllArm2PushButton->setVisible(true);
        ui->SelectAllArm3PushButton->setVisible(false); ui->ClearAllArm3PushButton->setVisible(false);
        ui->dataGroupBox->move(20,160);
        ui->communicationSettingsGroupBox->move(330,160);
        NumberOfArms=2;
        ui->arm2LoggingSettingsGroupBox->setVisible(true);
        ui->arm3LoggingSettingsGroupBox->setVisible(false);
        ui->arm1LoggingSettingsGroupBox->move(28,75); ui->SelectAllArm1PushButton->move(550,100); ui->ClearAllArm1PushButton->move(550,130);
        ui->selectDataTypeLabel->move(30,10);
        ui->dataTypeToLogComboBox->move(232,15);
        ui->AdressSettingsArm2PushButton->setVisible(true);ui->AdressSettingsArm3PushButton->setVisible(false);
        Griper2->GripperEnabled=true; Griper3->GripperEnabled=false;
        ui->Port2Label->setVisible(true); ui->Port2SelectionComboBox->setVisible(true);
        ui->Port3Label->setVisible(false); ui->Port3SelectionComboBox->setVisible(false);
        ui->communicationSettingsGroupBox->setFixedHeight(141);
    }
    else if (arg1==3)
    {
        ui->configurationGroupBox->setFixedHeight(145);
        ui->numberOfSensorsArm1Label->setVisible(true);
        ui->numberOfSensorsArm2Label->setVisible(true);
        ui->numberOfSensorsArm3Label->setVisible(true);
        ui->SelectAllArm2PushButton->setVisible(true); ui->ClearAllArm2PushButton->setVisible(true);
        ui->SelectAllArm3PushButton->setVisible(true); ui->ClearAllArm3PushButton->setVisible(true);
        ui->dataGroupBox->move(20,190);
        ui->communicationSettingsGroupBox->move(330,130);
        NumberOfArms=3;
        ui->arm3LoggingSettingsGroupBox->setVisible(true);
        ui->AdressSettingsArm2PushButton->setVisible(true); ui->AdressSettingsArm3PushButton->setVisible(true);
        Griper3->GripperEnabled=true;
        ui->Port2Label->setVisible(true); ui->Port2SelectionComboBox->setVisible(true);
        ui->Port3Label->setVisible(true); ui->Port3SelectionComboBox->setVisible(true);
        ui->communicationSettingsGroupBox->setFixedHeight(171);
    }
}


void MainWindow::on_enableLoggingCheckBox_toggled(bool checked)
{
    if(checked==true)
    {
        ui->directoryLocationLineEdit->setEnabled(true);
        ui->browseDirectoryPushButton->setEnabled(true);
        ui->loggingSettingsPushButton->setEnabled(true);
        ui->ConfigurationTabWidget->setTabEnabled(1,true);
    }
    else
    {
        ui->directoryLocationLineEdit->setDisabled(true);
        ui->browseDirectoryPushButton->setDisabled(true);
        ui->loggingSettingsPushButton->setDisabled(true);
        ui->ConfigurationTabWidget->setTabEnabled(1,false);
    }
}



void MainWindow::on_startAcquisitionPushButton_clicked()
{

    GetLoggingAndVisualizationInfo();
    if ((ui->PortSelectionComboBox->currentIndex()==ui->Port2SelectionComboBox->currentIndex() && ui->PortSelectionComboBox->currentIndex()!=0) || (ui->PortSelectionComboBox->currentIndex()==ui->Port3SelectionComboBox->currentIndex() && ui->PortSelectionComboBox->currentIndex()!=0) || (ui->Port2SelectionComboBox->currentIndex() == ui->Port3SelectionComboBox->currentIndex() && ui->Port2SelectionComboBox->currentIndex()!=0))
    {
        this->PromptErrorToScreen(6);
        ui->PortSelectionComboBox->setCurrentIndex(0);
        return;
    }
    if (ui->enableLoggingCheckBox->isChecked()==true && ui->enableVisualizationCheckBox->isChecked()==true)
    {
        DataKind=ui->dataTypeToLogComboBox->currentIndex();
    }
    else if (ui->enableVisualizationCheckBox->isChecked()==true && ui->enableLoggingCheckBox->isChecked()==false)
    {
        DataKind=0; // By default, when the users start visualization without logging, Static Data are shown first
    }
    else
    {
        DataKind=ui->dataTypeToLogComboBox->currentIndex();
    }
    VisualizationForm *visualiationWindow=new VisualizationForm(NULL,this);
    visualiationWindow->setWindowTitle("Tactile Sensor Visualisation Page");
    QThread* thread = new QThread;

    CommunicationHandler* comm = new CommunicationHandler(visualiationWindow,this);
    comm->moveToThread(thread);
    if (ui->enableVisualizationCheckBox->isChecked()==true)
    {
        visualiationWindow->show();
        connect(comm,SIGNAL(NewDataAvailable(int)),visualiationWindow,SLOT(PlotData(int)));
    }
    connect(comm,SIGNAL(ErrorOpeningPort(int)),this,SLOT(PromptErrorToScreen(int)));
    connect(comm,SIGNAL(ErrorPlottingFourierTransform(int)),this,SLOT(PromptErrorToScreen(int)));
    connect(thread, SIGNAL(started()), comm, SLOT(process()));
    connect(comm, SIGNAL(finished()), thread, SLOT(quit()));
    connect(comm, SIGNAL(finished()), comm, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(visualiationWindow,SIGNAL(VFClosed()),this,SLOT(ToggleStartStopButton()));
    connect(this,SIGNAL(StopButtonPressed()),visualiationWindow,SLOT(CloseVF()));
    thread->start();
    thread->setPriority(QThread::TimeCriticalPriority);
    ui->startAcquisitionPushButton->setDisabled(true);
    ui->stopAcquisitionPushButton->setEnabled(true);
    ui->enableLoggingCheckBox->setDisabled(true);
    ui->enableVisualizationCheckBox->setDisabled(true);
    ui->ConfigurationTabWidget->setTabEnabled(1,false);
    ui->numberOfArmsSpinBox->setDisabled(true);
    ui->numberOfSensorsArm1SpinBox->setDisabled(true);
    ui->numberOfSensorsArm2SpinBox->setDisabled(true);
    ui->numberOfSensorsArm3SpinBox->setDisabled(true);
    ui->loggingSettingsPushButton->setDisabled(true);
    ui->browseDirectoryPushButton->setDisabled(true);
    ui->communicationSettingsGroupBox->setDisabled(true);
}

void MainWindow::on_stopAcquisitionPushButton_clicked()
{
    if(ui->stopAcquisitionPushButton->isEnabled())
    {
        ui->communicationSettingsGroupBox->setDisabled(true);
        ui->stopAcquisitionPushButton->setDisabled(true);
        ui->startAcquisitionPushButton->setEnabled(true);
        if (ui->enableLoggingCheckBox->isChecked()==true)
        {
            ui->ConfigurationTabWidget->setTabEnabled(1,true);
        }
        ui->numberOfArmsSpinBox->setDisabled(false);
        ui->numberOfSensorsArm1SpinBox->setDisabled(false);
        ui->numberOfSensorsArm2SpinBox->setDisabled(false);
        ui->numberOfSensorsArm3SpinBox->setDisabled(false);
        if(ui->enableLoggingCheckBox->isChecked()==true)
        {
            ui->loggingSettingsPushButton->setDisabled(false);
            ui->browseDirectoryPushButton->setDisabled(false);
        }
        ui->communicationSettingsGroupBox->setDisabled(false);
    }
    else
    {
        ui->stopAcquisitionPushButton->setEnabled(true);
        ui->startAcquisitionPushButton->setDisabled(true);
    }
    emit StopButtonPressed();
    ui->enableLoggingCheckBox->setEnabled(true);
    ui->enableVisualizationCheckBox->setEnabled(true);
}

void MainWindow::on_enableVisualizationCheckBox_clicked(bool checked)
{
    if(checked==true)
    {
        ui->startAcquisitionPushButton->setText("Start Acquisition and Visualization");
        ui->startAcquisitionPushButton->setEnabled(true);
    }
    else if(checked==false && ui->enableLoggingCheckBox->isChecked()==false)
    {
        ui->startAcquisitionPushButton->setDisabled(true);
        ui->startAcquisitionPushButton->setText("Start Acquisition");
    }
    else if(checked==false && ui->enableLoggingCheckBox->isChecked()==true)
    {
        ui->startAcquisitionPushButton->setEnabled(true);
        ui->startAcquisitionPushButton->setText("Start Acquisition");
    }
}

void MainWindow::on_enableLoggingCheckBox_clicked(bool checked)
{
    if(checked==true && ui->enableVisualizationCheckBox->isChecked()==true)
    {
        ui->startAcquisitionPushButton->setEnabled(true);
        ui->startAcquisitionPushButton->setText("Start Acquisition and Visualization");
    }
    else if(checked==true && ui->enableVisualizationCheckBox->isChecked()==false)
    {
        ui->startAcquisitionPushButton->setEnabled(true);
        ui->startAcquisitionPushButton->setText("Start Acquisition");
    }
    else if (checked==false && ui->enableVisualizationCheckBox->isChecked()==true)
    {
        ui->startAcquisitionPushButton->setEnabled(true);
        ui->startAcquisitionPushButton->setText("Start Acquisition and Visualization");
    }
    else if (checked==false && ui->enableVisualizationCheckBox->isChecked()==false)
    {
        ui->startAcquisitionPushButton->setDisabled(true);
    }
    if(checked==true)
    {
        DataKind=ui->dataTypeToLogComboBox->currentIndex();
    }
    else
    {
        DataKind=0; //Static: Default Value
    }
}

void MainWindow::on_actionAbout_Haptics_UI_triggered()
{
    mglGraph *test = new mglGraph;
    test->Alpha(true);
    test->Light(true);
    test->Box();
    AboutRobotiqSensorUI *AboutForm = new AboutRobotiqSensorUI();
    AboutForm->setWindowTitle("About Robotiq Sensor UI");
    AboutForm->setFixedSize(380,120);
    AboutForm->show();
}

void MainWindow::ToggleStartStopButton()
{
    if(ui->startAcquisitionPushButton->isEnabled()==false)
    {
        ui->startAcquisitionPushButton->setEnabled(true);
        ui->stopAcquisitionPushButton->setEnabled(false);
        ui->enableLoggingCheckBox->setEnabled(true);
        ui->enableVisualizationCheckBox->setEnabled(true);
        ui->communicationSettingsGroupBox->setDisabled(false);
        if (ui->enableLoggingCheckBox->isChecked()==true)
        {
            ui->ConfigurationTabWidget->setTabEnabled(1,true);
        }
        ui->numberOfArmsSpinBox->setDisabled(false);
        ui->numberOfSensorsArm1SpinBox->setDisabled(false);
        ui->numberOfSensorsArm2SpinBox->setDisabled(false);
        ui->numberOfSensorsArm3SpinBox->setDisabled(false);

        if(ui->enableLoggingCheckBox->isChecked()==true)
        {
            ui->loggingSettingsPushButton->setDisabled(false);
            ui->browseDirectoryPushButton->setDisabled(false);
        }
    }
}


void MainWindow::on_browseDirectoryPushButton_clicked()
{
    FilePath = QFileDialog::getSaveFileName(this,tr("Enter where you want to save the log file:"),QDir::currentPath(),"*.csv");
    if (FilePath.isEmpty()==false)
    {
        ui->directoryLocationLineEdit->setText(FilePath);
    }
}

void MainWindow::on_enableLoggingCheckBox_clicked()
{
    EnableDatalogging=ui->enableLoggingCheckBox->isChecked();
}

void MainWindow::on_numberOfSensorsArm1SpinBox_valueChanged(int arg1)
{
    NumberOfSensors[0]=arg1;
}

void MainWindow::on_numberOfSensorsArm2SpinBox_valueChanged(int arg1)
{
    NumberOfSensors[1]=arg1;
}

void MainWindow::on_numberOfSensorsArm3SpinBox_valueChanged(int arg1)
{
    NumberOfSensors[2]=arg1;
}

void MainWindow::on_loggingSettingsPushButton_clicked()
{
    ui->ConfigurationTabWidget->setCurrentIndex(1);
}

void MainWindow::on_dataTypeToLogComboBox_currentIndexChanged(int index)
{
    DataKind=index;
}

void MainWindow::PromptErrorToScreen(int ErrorType)
{
    if (ErrorType==1)
    {
        QMessageBox::information(0, QString("Error"), QString("No RS-485 Port has been found."), QMessageBox::Ok);
    }
    else if(ErrorType==2)
    {
        QMessageBox::information(0, QString("Error"), QString("The port is currently unavailable for reading and writting. It might be already opened by another application."), QMessageBox::Ok);
    }
    else if (ErrorType==3)
    {
        QMessageBox::information(0, QString("Error"), QString("Cannot communicate with that arm or sensor. Make sure they are well connected."), QMessageBox::Ok);
    }
    else if (ErrorType==4)
    {
        QMessageBox::information(0, QString("Error"), QString("No serial port has been detected."), QMessageBox::Ok);
    }
    else if (ErrorType==5)
    {
        QMessageBox::information(0, QString("Error"), QString("Address configuration is invalid: two or more than two adresses are the same. Default configuration is restored."), QMessageBox::Ok);
    }
    else if (ErrorType==6)
    {
        QMessageBox::information(0, QString("Error"), QString("This port cannot be selected: it's already in used."), QMessageBox::Ok);
    }
    else if (ErrorType==7)
    {
        QMessageBox::information(0, QString("Error"), QString("To plot the Fast Fourier Transform (FFT), you need to acquire data at a minimal rate of 125 Hz."), QMessageBox::Ok);
    }
}


void MainWindow::on_ManuallySelectCommunicationPortCheckBox_clicked(bool checked)
{
    NumberOfDetectedPorts=0;
    if (checked==true)
    {
        ui->AutoDetectCommunicationSettingsCheckBox->setChecked(false);
        ui->PortSelectionComboBox->setEnabled(true); ui->Port2SelectionComboBox->setEnabled(true); ui->Port3SelectionComboBox->setEnabled(true);

        ui->PortSelectionComboBox->clear(); ui->Port2SelectionComboBox->clear(); ui->Port3SelectionComboBox->clear();
        ui->PortSelectionComboBox->addItem("Select..."); ui->Port2SelectionComboBox->addItem("Select..."); ui->Port3SelectionComboBox->addItem("Select...");

        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) // We list available serial ports, but we don't open any:
        {
            if (ui->PortSelectionComboBox->isEnabled()==false)
            {
                ui->PortSelectionComboBox->setEnabled(true); ui->Port2SelectionComboBox->setEnabled(true); ui->Port3SelectionComboBox->setEnabled(true);
                ui->PortSelectionComboBox->clear(); ui->Port2SelectionComboBox->clear(); ui->Port3SelectionComboBox->clear();
                ui->PortSelectionComboBox->addItem("Select...");ui->Port2SelectionComboBox->addItem("Select...");ui->Port3SelectionComboBox->addItem("Select...");
            }
            ui->PortSelectionComboBox->addItem(info.portName());ui->Port2SelectionComboBox->addItem(info.portName()); ui->Port3SelectionComboBox->addItem(info.portName());
            qDebug() << "Name        : " << info.portName();
            NumberOfDetectedPorts++;
        }
        if(NumberOfDetectedPorts==0)
        {
            ui->PortSelectionComboBox->clear();ui->Port2SelectionComboBox->clear();ui->Port3SelectionComboBox->clear();
            ui->PortSelectionComboBox->addItem("No serial port detected"); ui->Port2SelectionComboBox->addItem("No serial port detected"); ui->Port3SelectionComboBox->addItem("No serial port detected");
            ui->PortSelectionComboBox->setDisabled(true); ui->Port2SelectionComboBox->setDisabled(true); ui->Port3SelectionComboBox->setDisabled(true);
            ui->ManuallySelectCommunicationPortCheckBox->setChecked(false);
            ui->AutoDetectCommunicationSettingsCheckBox->setChecked(true);
            PromptErrorToScreen(4);
        }
        else if (NumberOfDetectedPorts==1)
        {
            ui->Port2SelectionComboBox->clear();ui->Port3SelectionComboBox->clear();
            ui->Port2SelectionComboBox->addItem("One serial port detected"); ui->Port3SelectionComboBox->addItem("One serial port detected");
            ui->Port2SelectionComboBox->setDisabled(true); ui->Port3SelectionComboBox->setDisabled(true);
        }
        else if (NumberOfDetectedPorts==2)
        {
            ui->Port3SelectionComboBox->clear();
            ui->Port3SelectionComboBox->addItem("Two serial ports detected");
            ui->Port3SelectionComboBox->setDisabled(true);
        }
        ManualCommunicationSettings=true;
    }
    else
    {
        ui->AutoDetectCommunicationSettingsCheckBox->setChecked(true);
        ui->PortSelectionComboBox->setEnabled(false);ui->Port2SelectionComboBox->setEnabled(false);ui->Port3SelectionComboBox->setEnabled(false);
        ManualCommunicationSettings=false;
    }
}

void MainWindow::on_AutoDetectCommunicationSettingsCheckBox_clicked(bool checked)
{
    if (checked==true)
    {
        ui->ManuallySelectCommunicationPortCheckBox->setChecked(false);
        ui->PortSelectionComboBox->setDisabled(true);ui->Port2SelectionComboBox->setDisabled(true);ui->Port3SelectionComboBox->setDisabled(true);
        ManualCommunicationSettings=false;
    }
    else
    {
        ui->ManuallySelectCommunicationPortCheckBox->setChecked(true);
        ui->PortSelectionComboBox->setDisabled(false);ui->Port2SelectionComboBox->setDisabled(false);ui->Port3SelectionComboBox->setDisabled(false);
        ManualCommunicationSettings=true;
    }
}


void MainWindow::on_SelectAllArm1PushButton_clicked()
{
    ui->Arm1Sensor1CheckBox->setChecked(true);ui->Arm1Sensor2CheckBox->setChecked(true);ui->Arm1Sensor3CheckBox->setChecked(true);ui->Arm1Sensor4CheckBox->setChecked(true);
    ui->Arm1Sensor5CheckBox->setChecked(true);ui->Arm1Sensor6CheckBox->setChecked(true);ui->Arm1Sensor7CheckBox->setChecked(true);ui->Arm1Sensor8CheckBox->setChecked(true);
    ui->Arm1Sensor9CheckBox->setChecked(true);ui->Arm1Sensor10CheckBox->setChecked(true);
}

void MainWindow::on_ClearAllArm1PushButton_clicked()
{
    ui->Arm1Sensor1CheckBox->setChecked(false);ui->Arm1Sensor2CheckBox->setChecked(false);ui->Arm1Sensor3CheckBox->setChecked(false);ui->Arm1Sensor4CheckBox->setChecked(false);
    ui->Arm1Sensor5CheckBox->setChecked(false);ui->Arm1Sensor6CheckBox->setChecked(false);ui->Arm1Sensor7CheckBox->setChecked(false);ui->Arm1Sensor8CheckBox->setChecked(false);
    ui->Arm1Sensor9CheckBox->setChecked(false);ui->Arm1Sensor10CheckBox->setChecked(false);
}

void MainWindow::on_SelectAllArm2PushButton_clicked()
{
    ui->Arm2Sensor1CheckBox->setChecked(true);ui->Arm2Sensor2CheckBox->setChecked(true);ui->Arm2Sensor3CheckBox->setChecked(true);ui->Arm2Sensor4CheckBox->setChecked(true);
    ui->Arm2Sensor5CheckBox->setChecked(true);ui->Arm2Sensor6CheckBox->setChecked(true);ui->Arm2Sensor7CheckBox->setChecked(true);ui->Arm2Sensor8CheckBox->setChecked(true);
    ui->Arm2Sensor9CheckBox->setChecked(true);ui->Arm2Sensor10CheckBox->setChecked(true);
}


void MainWindow::on_ClearAllArm2PushButton_clicked()
{
    ui->Arm2Sensor1CheckBox->setChecked(false);ui->Arm2Sensor2CheckBox->setChecked(false);ui->Arm2Sensor3CheckBox->setChecked(false);ui->Arm2Sensor4CheckBox->setChecked(false);
    ui->Arm2Sensor5CheckBox->setChecked(false);ui->Arm2Sensor6CheckBox->setChecked(false);ui->Arm2Sensor7CheckBox->setChecked(false);ui->Arm2Sensor8CheckBox->setChecked(false);
    ui->Arm2Sensor9CheckBox->setChecked(false);ui->Arm2Sensor10CheckBox->setChecked(false);
}

void MainWindow::on_SelectAllArm3PushButton_clicked()
{
    ui->Arm3Sensor1CheckBox->setChecked(true);ui->Arm3Sensor2CheckBox->setChecked(true);ui->Arm3Sensor3CheckBox->setChecked(true);ui->Arm3Sensor4CheckBox->setChecked(true);
    ui->Arm3Sensor5CheckBox->setChecked(true);ui->Arm3Sensor6CheckBox->setChecked(true);ui->Arm3Sensor7CheckBox->setChecked(true);ui->Arm3Sensor8CheckBox->setChecked(true);
    ui->Arm3Sensor9CheckBox->setChecked(true);ui->Arm3Sensor10CheckBox->setChecked(true);
}

void MainWindow::on_ClearAllArm3PushButton_clicked()
{
    ui->Arm3Sensor1CheckBox->setChecked(false);ui->Arm3Sensor2CheckBox->setChecked(false);ui->Arm3Sensor3CheckBox->setChecked(false);ui->Arm3Sensor4CheckBox->setChecked(false);
    ui->Arm3Sensor5CheckBox->setChecked(false);ui->Arm3Sensor6CheckBox->setChecked(false);ui->Arm3Sensor7CheckBox->setChecked(false);ui->Arm3Sensor8CheckBox->setChecked(false);
    ui->Arm3Sensor9CheckBox->setChecked(false);ui->Arm3Sensor10CheckBox->setChecked(false);
}

void MainWindow::on_AdressSettingsArm1PushButton_clicked()
{
    AdressSettingWndArm1->show();
    ui->AdressSettingsArm1PushButton->setDisabled(true);
    ui->AdressSettingsArm2PushButton->setDisabled(true);
    ui->AdressSettingsArm3PushButton->setDisabled(true);
}

void MainWindow::on_AdressSettingsArm2PushButton_clicked()
{
    AdressSettingWndArm2->show();
    ui->AdressSettingsArm1PushButton->setDisabled(true);
    ui->AdressSettingsArm2PushButton->setDisabled(true);
    ui->AdressSettingsArm3PushButton->setDisabled(true);
}

void MainWindow::on_AdressSettingsArm3PushButton_clicked()
{
    AdressSettingWndArm3->show();
    ui->AdressSettingsArm1PushButton->setDisabled(true);
    ui->AdressSettingsArm2PushButton->setDisabled(true);
    ui->AdressSettingsArm3PushButton->setDisabled(true);
}

void MainWindow::ReactToAdressSettingsWindowClosedArm1()
{
    ui->AdressSettingsArm1PushButton->setEnabled(true);
    ui->AdressSettingsArm2PushButton->setEnabled(true);
    ui->AdressSettingsArm3PushButton->setEnabled(true);
    Griper1->SensorAdress[0]=AdressSettingWndArm1->AdressConfiguration[0];Griper1->SensorAdress[1]=AdressSettingWndArm1->AdressConfiguration[1];
    Griper1->SensorAdress[2]=AdressSettingWndArm1->AdressConfiguration[2];Griper1->SensorAdress[3]=AdressSettingWndArm1->AdressConfiguration[3];
    Griper1->SensorAdress[4]=AdressSettingWndArm1->AdressConfiguration[4];Griper1->SensorAdress[5]=AdressSettingWndArm1->AdressConfiguration[5];
    Griper1->SensorAdress[6]=AdressSettingWndArm1->AdressConfiguration[6];Griper1->SensorAdress[7]=AdressSettingWndArm1->AdressConfiguration[7];
    Griper1->SensorAdress[8]=AdressSettingWndArm1->AdressConfiguration[8];Griper1->SensorAdress[9]=AdressSettingWndArm1->AdressConfiguration[9];
}

void MainWindow::ReactToAdressSettingsWindowClosedArm2()
{
    ui->AdressSettingsArm1PushButton->setEnabled(true);
    ui->AdressSettingsArm2PushButton->setEnabled(true);
    ui->AdressSettingsArm3PushButton->setEnabled(true);
    Griper2->SensorAdress[0]=AdressSettingWndArm2->AdressConfiguration[0];Griper2->SensorAdress[1]=AdressSettingWndArm2->AdressConfiguration[1];
    Griper2->SensorAdress[2]=AdressSettingWndArm2->AdressConfiguration[2];Griper2->SensorAdress[3]=AdressSettingWndArm2->AdressConfiguration[3];
    Griper2->SensorAdress[4]=AdressSettingWndArm2->AdressConfiguration[4];Griper2->SensorAdress[5]=AdressSettingWndArm2->AdressConfiguration[5];
    Griper2->SensorAdress[6]=AdressSettingWndArm2->AdressConfiguration[6];Griper2->SensorAdress[7]=AdressSettingWndArm2->AdressConfiguration[7];
    Griper2->SensorAdress[8]=AdressSettingWndArm2->AdressConfiguration[8];Griper2->SensorAdress[9]=AdressSettingWndArm2->AdressConfiguration[9];
}

void MainWindow::ReactToAdressSettingsWindowClosedArm3()
{
    ui->AdressSettingsArm1PushButton->setEnabled(true);
    ui->AdressSettingsArm2PushButton->setEnabled(true);
    ui->AdressSettingsArm3PushButton->setEnabled(true);
    Griper3->SensorAdress[0]=AdressSettingWndArm3->AdressConfiguration[0];Griper3->SensorAdress[1]=AdressSettingWndArm3->AdressConfiguration[1];
    Griper3->SensorAdress[2]=AdressSettingWndArm3->AdressConfiguration[2];Griper3->SensorAdress[3]=AdressSettingWndArm3->AdressConfiguration[3];
    Griper3->SensorAdress[4]=AdressSettingWndArm3->AdressConfiguration[4];Griper3->SensorAdress[5]=AdressSettingWndArm3->AdressConfiguration[5];
    Griper3->SensorAdress[6]=AdressSettingWndArm3->AdressConfiguration[6];Griper3->SensorAdress[7]=AdressSettingWndArm3->AdressConfiguration[7];
    Griper3->SensorAdress[8]=AdressSettingWndArm3->AdressConfiguration[8];Griper3->SensorAdress[9]=AdressSettingWndArm3->AdressConfiguration[9];
}

void MainWindow::on_PortSelectionComboBox_currentIndexChanged(int index)
{
    if (index!=0)
    {
        ManualSelectedPortArm1=ui->PortSelectionComboBox->itemText(index);
    }
}

void MainWindow::on_Port2SelectionComboBox_currentIndexChanged(int index)
{
    if (index!=0)
    {
        ManualSelectedPortArm2=ui->Port2SelectionComboBox->itemText(index);
    }
}

void MainWindow::on_Port3SelectionComboBox_currentIndexChanged(int index)
{
    if (index!=0)
    {
        ManualSelectedPortArm3=ui->Port3SelectionComboBox->itemText(index);
    }
}

void MainWindow::GetLoggingAndVisualizationInfo()
{
    NumberOfSensorsToLogArm1=0;NumberOfSensorsToLogArm2=0;NumberOfSensorsToLogArm3=0;
    ListOfSensorsPerArms[0][0]=-1;ListOfSensorsPerArms[0][1]=-1;ListOfSensorsPerArms[0][2]=-1;ListOfSensorsPerArms[0][3]=-1;ListOfSensorsPerArms[0][4]=-1;
    ListOfSensorsPerArms[0][5]=-1;ListOfSensorsPerArms[0][6]=-1;ListOfSensorsPerArms[0][7]=-1;ListOfSensorsPerArms[0][8]=-1;ListOfSensorsPerArms[0][9]=-1;ListOfSensorsPerArms[0][10]=-1;
    ListOfSensorsPerArms[1][0]=-1;ListOfSensorsPerArms[1][1]=-1;ListOfSensorsPerArms[1][2]=-1;ListOfSensorsPerArms[1][3]=-1;ListOfSensorsPerArms[1][4]=-1;
    ListOfSensorsPerArms[1][5]=-1;ListOfSensorsPerArms[1][6]=-1;ListOfSensorsPerArms[1][7]=-1;ListOfSensorsPerArms[1][8]=-1;ListOfSensorsPerArms[1][9]=-1;ListOfSensorsPerArms[1][10]=-1;
    ListOfSensorsPerArms[2][0]=-1;ListOfSensorsPerArms[2][1]=-1;ListOfSensorsPerArms[2][2]=-1;ListOfSensorsPerArms[2][3]=-1;ListOfSensorsPerArms[2][4]=-1;
    ListOfSensorsPerArms[2][5]=-1;ListOfSensorsPerArms[2][6]=-1;ListOfSensorsPerArms[2][7]=-1;ListOfSensorsPerArms[2][8]=-1;ListOfSensorsPerArms[2][9]=-1;ListOfSensorsPerArms[2][10]=-1;

    if(ui->Arm1Sensor1CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[0];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor2CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[1];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor3CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[2];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor4CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[3];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor5CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[4];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor6CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[5];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor7CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[6];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor8CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[7];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor9CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[8];
        NumberOfSensorsToLogArm1++;
    }
    if(ui->Arm1Sensor10CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[0][NumberOfSensorsToLogArm1]=Griper1->SensorAdress[9];
        NumberOfSensorsToLogArm1++;
    }

    if(ui->Arm2Sensor1CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[0];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor2CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[1];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor3CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[2];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor4CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[3];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor5CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[4];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor6CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[5];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor7CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[6];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor8CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[7];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor9CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[8];
        NumberOfSensorsToLogArm2++;
    }
    if(ui->Arm2Sensor10CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[1][NumberOfSensorsToLogArm2]=Griper2->SensorAdress[9];
        NumberOfSensorsToLogArm2++;
    }

    if(ui->Arm3Sensor1CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[0];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor2CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[1];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor3CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[2];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor4CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[3];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor5CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[4];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor6CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[5];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor7CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[6];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor8CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[7];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor9CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[8];
        NumberOfSensorsToLogArm3++;
    }
    if(ui->Arm3Sensor10CheckBox->isChecked()==true)
    {
        ListOfSensorsPerArms[2][NumberOfSensorsToLogArm3]=Griper3->SensorAdress[9];
        NumberOfSensorsToLogArm3++;
    }
    if(EnableDatalogging==false)
    {
        ListOfSensorsPerArms[0][0]=Griper1->SensorAdress[0];ListOfSensorsPerArms[0][1]=-1;ListOfSensorsPerArms[0][2]=-1;ListOfSensorsPerArms[0][3]=-1;ListOfSensorsPerArms[0][4]=-1;
        ListOfSensorsPerArms[0][5]=-1;ListOfSensorsPerArms[0][6]=-1;ListOfSensorsPerArms[0][7]=-1;ListOfSensorsPerArms[0][8]=-1;ListOfSensorsPerArms[0][9]=-1;ListOfSensorsPerArms[0][10]=-1;
        ListOfSensorsPerArms[1][0]=Griper2->SensorAdress[0];ListOfSensorsPerArms[1][1]=-1;ListOfSensorsPerArms[1][2]=-1;ListOfSensorsPerArms[1][3]=-1;ListOfSensorsPerArms[1][4]=-1;
        ListOfSensorsPerArms[1][5]=-1;ListOfSensorsPerArms[1][6]=-1;ListOfSensorsPerArms[1][7]=-1;ListOfSensorsPerArms[1][8]=-1;ListOfSensorsPerArms[1][9]=-1;ListOfSensorsPerArms[1][10]=-1;
        ListOfSensorsPerArms[2][0]=Griper3->SensorAdress[0];ListOfSensorsPerArms[2][1]=-1;ListOfSensorsPerArms[2][2]=-1;ListOfSensorsPerArms[2][3]=-1;ListOfSensorsPerArms[2][4]=-1;
        ListOfSensorsPerArms[2][5]=-1;ListOfSensorsPerArms[2][6]=-1;ListOfSensorsPerArms[2][7]=-1;ListOfSensorsPerArms[2][8]=-1;ListOfSensorsPerArms[2][9]=-1;ListOfSensorsPerArms[2][10]=-1;
        NumberOfSensorsToLogArm1=1; NumberOfSensorsToLogArm2=1; NumberOfSensorsToLogArm3=1;
    }
}

void MainWindow::on_actionChange_sensor_ID_restricted_triggered()
{
    bool ok;
        QString InputPassword = QInputDialog::getText(this, tr("Authentication"), tr("Enter the password:"), QLineEdit::Password,"", &ok);
        if (InputPassword.toLower()=="servicelevel" && ok == true)
        {
            ChangeIDForm->show();
        }
        else
        {
            QMessageBox::information(0, QString("Error"), QString("Wrong password"), QMessageBox::Ok);
        }
}
