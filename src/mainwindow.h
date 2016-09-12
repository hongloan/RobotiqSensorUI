/**************************************************
 * Author: Jean-Philippe Roberge ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "adresssettingwindow.h"
#include "gripper.h"
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QScrollArea>
#include <qthread.h>
#include <QtCore/QDebug>
#include <QFileDialog>
#include <QDir>
#include "changesensoridform.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int DataKind; // 0: Static, 1: Dynamic, 2: High Sensitivity, 3: All Sensors
    int NumberOfArms,NumberOfSensors[3],NumberOfSensorsToLogArm1,NumberOfSensorsToLogArm2,NumberOfSensorsToLogArm3,SensorToVisualize,ListOfSensorsPerArms[3][11];
    bool EnableDatalogging,EnableDataVisualization,ManualCommunicationSettings;
    QString FilePath,ManualSelectedPortArm1,ManualSelectedPortArm2,ManualSelectedPortArm3;
    Gripper *Griper1, *Griper2, *Griper3;
private slots:
    void on_numberOfArmsSpinBox_valueChanged(int arg1);
    void on_enableLoggingCheckBox_toggled(bool checked);
    void on_startAcquisitionPushButton_clicked();
    void on_stopAcquisitionPushButton_clicked();
    void on_enableVisualizationCheckBox_clicked(bool checked);
    void on_enableLoggingCheckBox_clicked(bool checked);
    void on_actionAbout_Haptics_UI_triggered();
    void on_browseDirectoryPushButton_clicked();
    void on_enableLoggingCheckBox_clicked();
    void on_numberOfSensorsArm1SpinBox_valueChanged(int arg1);
    void on_numberOfSensorsArm2SpinBox_valueChanged(int arg1);
    void on_numberOfSensorsArm3SpinBox_valueChanged(int arg1);
    void on_loggingSettingsPushButton_clicked();
    void on_dataTypeToLogComboBox_currentIndexChanged(int index);
    void on_ManuallySelectCommunicationPortCheckBox_clicked(bool checked);
    void on_AutoDetectCommunicationSettingsCheckBox_clicked(bool checked);
    void on_SelectAllArm1PushButton_clicked();
    void on_ClearAllArm1PushButton_clicked();
    void on_SelectAllArm2PushButton_clicked();
    void on_ClearAllArm2PushButton_clicked();
    void on_SelectAllArm3PushButton_clicked();
    void on_ClearAllArm3PushButton_clicked();
    void on_AdressSettingsArm1PushButton_clicked();
    void on_AdressSettingsArm2PushButton_clicked();
    void on_AdressSettingsArm3PushButton_clicked();
    void on_PortSelectionComboBox_currentIndexChanged(int index);
    void on_Port2SelectionComboBox_currentIndexChanged(int index);
    void on_Port3SelectionComboBox_currentIndexChanged(int index);
    void on_actionChange_sensor_ID_restricted_triggered();

public slots:
    void ToggleStartStopButton();
    void PromptErrorToScreen(int ErrorType);
    void ReactToAdressSettingsWindowClosedArm1();
    void ReactToAdressSettingsWindowClosedArm2();
    void ReactToAdressSettingsWindowClosedArm3();
    void GetLoggingAndVisualizationInfo();
signals:
    void StopButtonPressed();
private:
    Ui::MainWindow *ui;
    int NumberOfDetectedPorts;
    AdressSettingWindow *AdressSettingWndArm1, *AdressSettingWndArm2, *AdressSettingWndArm3;
    ChangeSensorIDForm *ChangeIDForm;
};

#endif // MAINWINDOW_H
