/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "aboutrobotiqsensorui.h"
#include "ui_aboutrobotiqsensorui.h"


AboutRobotiqSensorUI::AboutRobotiqSensorUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutRobotiqSensorUI)
{
    ui->setupUi(this);
    ui->aboutInformation1Label->setText("This is Robotiq Sensor User Interface, version 1.0");
    ui->aboutInformation2Label->setText("This software was developped by CoRo Laboratory");
    ui->aboutInformation3Label->setText(QString(169) + " Copyright 2013-2016 CoRo Laboratory. All Rights Reserved.");
}

AboutRobotiqSensorUI::~AboutRobotiqSensorUI()
{
    delete ui;
}
