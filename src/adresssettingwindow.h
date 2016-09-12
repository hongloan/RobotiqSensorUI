/**************************************************
 * Author: Jean-Philippe Roberge ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#ifndef ADRESSSETTINGWINDOW_H
#define ADRESSSETTINGWINDOW_H

#include <QWidget>
#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>


namespace Ui {
class AddressSettingWindow;
}

class AddressSettingWindow : public QWidget
{
    Q_OBJECT 
public:
    explicit AddressSettingWindow(QWidget *parent = 0);
    ~AddressSettingWindow();
    int *AddressConfiguration;
signals:
    void AddressSettingsWindowClosedSignal();
    void AddressConfigurationIsInvalid(int ErrorType);
private slots:
    void on_RestoreDefaultConfigurationPushButton_clicked();

    void on_ApplyConfigurationPushButton_clicked(bool checked);
private:
    Ui::AddressSettingWindow *ui;
    void closeEvent(QCloseEvent *event);
    int Addresses[10];
    bool AddressesAreValid;
};

#endif // ADRESSSETTINGWINDOW_H
