/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "gripper.h"

Gripper::Gripper()
{
    GripperEnabled=false;
    for (int i = 0; i < 10; ++i)
        SensorAddress[i] = i + 1;
}
