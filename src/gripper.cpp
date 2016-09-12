/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "gripper.h"


Gripper::Gripper()
{
    GripperEnabled=false;
    SensorAdress[0]=1;SensorAdress[1]=2;SensorAdress[2]=3;SensorAdress[3]=4;SensorAdress[4]=5;
    SensorAdress[5]=6;SensorAdress[6]=7;SensorAdress[7]=8;SensorAdress[8]=9;SensorAdress[9]=10;
}
