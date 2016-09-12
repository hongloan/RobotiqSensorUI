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
class AdressSettingWindow;
}

class AdressSettingWindow : public QWidget
{
    Q_OBJECT 
public:
    explicit AdressSettingWindow(QWidget *parent = 0);
    ~AdressSettingWindow();
    int *AdressConfiguration;
signals:
    void AdressSettingsWindowClosedSignal();
    void AdressConfigurationIsInvalid(int ErrorType);
private slots:
    void on_RestoreDefaultConfigurationPushButton_clicked();

    void on_ApplyConfigurationPushButton_clicked(bool checked);
private:
    Ui::AdressSettingWindow *ui;
    void closeEvent(QCloseEvent *event);
    int Adresses[10];
    bool AdressesAreValid;
};

#endif // ADRESSSETTINGWINDOW_H
