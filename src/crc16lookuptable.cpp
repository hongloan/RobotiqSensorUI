/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: October 11th 2013            *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "crc16lookuptable.h"


CRC16LookupTable::CRC16LookupTable() //CRC-16: Generation of a lookup table for 0 to 255
{
    for (i=0;i<256;i++)
    {
        crc=831^i; // This is 0x033f=831, which is CRC16 for 250
        for(j=0;j<8;j++)
        {
            if(crc & 0x0001)
            {
                crc=(crc>>1)^0xa001;
            }
            else
            {
                crc>>=1;
            }
        }
        CRC16Table.insert(i,((crc & 0x00FF)<<8)+((crc & 0xFF00)>>8));
    }
}
