/**************************************************
 * Author: Jean-Philippe Roberge ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#ifndef ABOUTROBOTIQSENSORUI_H
#define ABOUTROBOTIQSENSORUI_H

#include <QWidget>

namespace Ui {
class AboutRobotiqSensorUI;
}

class AboutRobotiqSensorUI : public QWidget
{
    Q_OBJECT
public:
    explicit AboutRobotiqSensorUI(QWidget *parent = 0);
    ~AboutRobotiqSensorUI();
private:
    Ui::AboutRobotiqSensorUI *ui;
};

#endif // ABOUTROBOTIQSENSORUI_H
