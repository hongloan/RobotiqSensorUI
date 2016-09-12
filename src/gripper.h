/**************************************************
 * Author: Jean-Philippe Roberge ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#ifndef GRIPPER_H
#define GRIPPER_H

class Gripper
{
public:
    Gripper();
    bool GripperEnabled;
    int SensorAdress[10];
};

#endif // GRIPPER_H
