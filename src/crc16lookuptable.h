/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: October 11th 2013            *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#ifndef CRC16LOOKUPTABLE_H
#define CRC16LOOKUPTABLE_H

#include <QString>
#include <QHash>
#include <stdint.h>

class CRC16LookupTable
{
public:
    CRC16LookupTable();
    QHash<int, int> CRC16Table;
    uint16_t i,j,crc;
};

#endif // CRC16LOOKUPTABLE_H
