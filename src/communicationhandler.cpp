/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include "communicationhandler.h"


CommunicationHandler::CommunicationHandler(VisualizationForm *visualiationWindow, MainWindow *mainwnd)
{
    //Initializations:
    VF=visualiationWindow; MW=mainwnd;
    FilePointer=new QFile;
    msec=0; DynRange=5000; count=0;count2=0;count3=0;CountForFrequency=0; NanoSecTimer=0;
    CRCLookuptable = new CRC16LookupTable(); LastSensorThatWasVisualized=-1; NumberOfPorts=0; CurrentArmIndex=0;
    StaticValueTooHigh=false;
}

void CommunicationHandler::process()
{
    PortConfigured=false; WrongCom=false; SensorNumberWhenErrorWasPrompted=0,CommunicationErrorCount=0;
    DataProximal = new mglData(54); DataMedial = new mglData(24); DataDistal = new mglData(25); DataTempH = new mglData(42); DataMean = new mglData(5);
    DataProximal.Create(6,9); DataMedial.Create(4,6); DataDistal.Create(5,5); DataIni=0; Actual=0;
    DataProximal.a[0]=0; DataProximal.a[1]=0; DataProximal.a[2]=0; DataProximal.a[3]=0; DataProximal.a[4]=0; DataProximal.a[5]=0; DataProximal.a[6]=0;
    DataProximal.a[11]=0; DataProximal.a[12]=0; DataProximal.a[17]=0; DataProximal.a[18]=0; DataProximal.a[23]=0; DataProximal.a[24]=0; DataProximal.a[29]=0;
    DataProximal.a[30]=0; DataProximal.a[35]=0; DataProximal.a[36]=0;
    DataProximal.a[41]=0; DataProximal.a[42]=0; DataProximal.a[47]=0; DataProximal.a[48]=0;
    DataProximal.a[49]=0;DataProximal.a[50]=0;DataProximal.a[51]=0;DataProximal.a[52]=0;DataProximal.a[53]=0;

    DataMedial.a[0]=0; DataMedial.a[1]=0; DataMedial.a[2]=0; DataMedial.a[3]=0; DataMedial.a[4]=0;
    DataMedial.a[7]=0; DataMedial.a[8]=0; DataMedial.a[11]=0; DataMedial.a[12]=0; DataMedial.a[15]=0; DataMedial.a[16]=0;
    DataMedial.a[19]=0; DataMedial.a[20]=0; DataMedial.a[21]=0; DataMedial.a[22]=0; DataMedial.a[23]=0;
    DataDistal.a[0]=0; DataDistal.a[1]=0; DataDistal.a[2]=0; DataDistal.a[3]=0; DataDistal.a[4]=0; DataDistal.a[5]=0;
    DataDistal.a[9]=0; DataDistal.a[10]=0; DataDistal.a[14]=0; DataDistal.a[15]=0; DataDistal.a[19]=0; DataDistal.a[20]=0;
    DataDistal.a[21]=0; DataDistal.a[22]=0; DataDistal.a[23]=0; DataDistal.a[24]=0;
    QSerialPort serial[3]; PortIsSet=false; MW->Griper1->GripperEnabled=false; MW->Griper2->GripperEnabled=false; MW->Griper3->GripperEnabled=false;

    //We start by detecting which port communicate with the sensors:
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Standard BAUD Rates:" << info.standardBaudRates();
        if(MW->ManualCommunicationSettings==false && info.description()=="USBUARTalex") //We found it so we set it:
        {
            serial[NumberOfPorts].setPort(info);NumberOfPorts++;
            PortIsSet=true;
        }
        else if(MW->ManualCommunicationSettings==true && info.portName().compare(MW->ManualSelectedPortArm1)==0)
        {
            serial[0].setPort(info);NumberOfPorts++;
            PortIsSet=true;
        }
        else if(MW->ManualCommunicationSettings==true && info.portName().compare(MW->ManualSelectedPortArm2)==0)
        {
            serial[1].setPort(info);NumberOfPorts++;
            PortIsSet=true;
        }
        else if(MW->ManualCommunicationSettings==true && info.portName().compare(MW->ManualSelectedPortArm3)==0)
        {
            serial[2].setPort(info);NumberOfPorts++;
            PortIsSet=true;
        }
    }
    if(MW->NumberOfArms!=NumberOfPorts)
    {
        MW->NumberOfArms=NumberOfPorts; //This is needed to be consistent, and to avoid having more arms than available ports
        VF->on_ViewComboBox_currentIndexChanged(0);
    }
    if(NumberOfPorts==1)
    {
        MW->Griper1->GripperEnabled=true;MW->Griper2->GripperEnabled=false;MW->Griper3->GripperEnabled=false;
    }
    else if (NumberOfPorts==2)
    {
        MW->Griper1->GripperEnabled=true;MW->Griper2->GripperEnabled=true;MW->Griper3->GripperEnabled=false;
    }
    else if (NumberOfPorts==3)
    {
        MW->Griper1->GripperEnabled=true;MW->Griper2->GripperEnabled=true;MW->Griper3->GripperEnabled=true;
    }
    else
    {
        MW->Griper1->GripperEnabled=false;MW->Griper2->GripperEnabled=false;MW->Griper3->GripperEnabled=false;
    }

    if(PortIsSet==false)
    {
        emit ErrorOpeningPort(1);
    }

    //We check that the port is open for writting and reading, and we configure it:
    for(int PortIndex=0;PortIndex<NumberOfPorts;PortIndex++)
    {
        if (serial[PortIndex].open(QIODevice::ReadWrite))
        {
            qint32 custombaudrate=9600;
            QByteArray message;
            QByteArray TempResp;
            serial[PortIndex].setBaudRate(custombaudrate);
            serial[PortIndex].setParity(QSerialPort::NoParity);
            serial[PortIndex].setStopBits(QSerialPort::OneStop);
            serial[PortIndex].setDataBits(QSerialPort::Data8);
            serial[PortIndex].setFlowControl(QSerialPort::NoFlowControl);
            PortConfigured=true;
            message.clear();
            TempResp.clear();
        }
        else
        {
            qDebug() << "The port is currently unavailable for reading-writting.";
            if (PortIsSet==true)
            {
                emit ErrorOpeningPort(2);
            }
            PortConfigured=false;
        }
    }

    while(VF->closed==0 && PortConfigured==true) // Do this loop until the visualization window is closed
    {
        //If data logging is enabled, we create the log file:
        if(MW->EnableDatalogging==true)
        {
            FilePointer->setFileName(MW->FilePath);
            FilePointer->open(QIODevice::Append);
        }
        QTextStream OutputStream(FilePointer);

        if (PortConfigured==true && (MW->DataKind==0 || MW->DataKind==3)) // If the port has been successfully configured and we want static data OR all sensors:
        {
            QByteArray message; QByteArray TempData;
            int RawStaticData[28]; VF->DataStatic.Create(6,9);int i=0,x=0,SensorIndex=0;
            VF->DataStaticAllSensor[0].Create(6,9);VF->DataStaticAllSensor[1].Create(6,9);VF->DataStaticAllSensor[2].Create(6,9);VF->DataStaticAllSensor[3].Create(6,9);VF->DataStaticAllSensor[4].Create(6,9);
            VF->DataStaticAllSensor[5].Create(6,9);VF->DataStaticAllSensor[6].Create(6,9);VF->DataStaticAllSensor[7].Create(6,9);VF->DataStaticAllSensor[8].Create(6,9);VF->DataStaticAllSensor[9].Create(6,9);
            while (VF->closed==0 && (MW->DataKind==0 || MW->DataKind==3))
            {
                while(MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]!=-1)
                {
                    if (VF->ResetButtonIsPushed==true)
                    {
                        x=0;
                        VF->ResetButtonIsPushed=false;
                    }
                    //                    message.append((char)(250&0xFF));
                    message.append((char)(115&0xFF));
                    //                    message.append((char)(((CRCLookuptable->CRC16Table[MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]])&0xFF00)>>8)); // CRC16 low byte
                    //                    message.append((char)((CRCLookuptable->CRC16Table[MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]])&0xFF)); // CRC16 high byte
                    serial[CurrentArmIndex].write(message);
                    serial[CurrentArmIndex].waitForBytesWritten(100);
                    message.clear();

                    while (serial[CurrentArmIndex].waitForReadyRead(100))
                    {
                        TempData += serial[CurrentArmIndex].readAll();
                    }
                    if(TempData.length()==112)
                    {
                        if (VF->CurrentSensorToVisualize ==2)
                        {
                            for(i=0;i<=54;i=i+2)
                            {
                                RawStaticData[i/2]= (static_cast<unsigned int>(TempData[i]) & 0xFF)*256+(static_cast<unsigned int>(TempData[i+1]) & 0xFF);
                            }
                        }
                        else if(VF->CurrentSensorToVisualize==1)
                        {
                            for(i=0;i<=54;i=i+2)
                            {
                                RawStaticData[i/2]= (static_cast<unsigned int>(TempData[i+56]) & 0xFF)*256+(static_cast<unsigned int>(TempData[i+57]) & 0xFF);
                            }
                        }
                    }
                    //                    else
                    //                    {
                    //                        WrongCom=true;
                    //                    }
                    qDebug() << "Tempdata length: " << TempData.length();

                    if (x<4 && MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]==VF->CurrentSensorToVisualize && MW->DataKind==0)
                    {
                        memcpy(InitialValue[SensorIndex],RawStaticData, sizeof RawStaticData);
                        x++;
                    }
                    else if (MW->DataKind==3 && x<34)
                    {
                        memcpy(InitialValue[SensorIndex],RawStaticData, sizeof RawStaticData);
                        x++;
                    }
                    else
                    {
                        if (TempData.length()==112) //Proximal
                        {
                            DataProximal.a[7]=RawStaticData[0]-InitialValue[SensorIndex][0];   //0
                            DataProximal.a[8]=RawStaticData[1]-InitialValue[SensorIndex][1];   //1
                            DataProximal.a[9]=RawStaticData[2]-InitialValue[SensorIndex][2];   //2
                            DataProximal.a[10]=RawStaticData[3]-InitialValue[SensorIndex][3];  //3

                            DataProximal.a[13]=RawStaticData[4]-InitialValue[SensorIndex][4];  //4
                            DataProximal.a[14]=RawStaticData[5]-InitialValue[SensorIndex][5];  //5
                            DataProximal.a[15]=RawStaticData[6]-InitialValue[SensorIndex][6];  //6
                            DataProximal.a[16]=RawStaticData[7]-InitialValue[SensorIndex][7];  //7

                            DataProximal.a[19]=RawStaticData[8]-InitialValue[SensorIndex][8];  //8
                            DataProximal.a[20]=RawStaticData[9]-InitialValue[SensorIndex][9];  //9
                            DataProximal.a[21]=RawStaticData[10]-InitialValue[SensorIndex][10];  //10
                            DataProximal.a[22]=RawStaticData[11]-InitialValue[SensorIndex][11];  //11

                            DataProximal.a[25]=RawStaticData[12]-InitialValue[SensorIndex][12];   //12
                            DataProximal.a[26]=RawStaticData[13]-InitialValue[SensorIndex][13];   //13
                            DataProximal.a[27]=RawStaticData[14]-InitialValue[SensorIndex][14];   //14
                            DataProximal.a[28]=RawStaticData[15]-InitialValue[SensorIndex][15];  //15

                            DataProximal.a[31]=RawStaticData[16]-InitialValue[SensorIndex][16];  //16
                            DataProximal.a[32]=RawStaticData[17]-InitialValue[SensorIndex][17];  //17
                            DataProximal.a[33]=RawStaticData[18]-InitialValue[SensorIndex][18];  //18
                            DataProximal.a[34]=RawStaticData[19]-InitialValue[SensorIndex][19];  //19

                            DataProximal.a[37]=RawStaticData[20]-InitialValue[SensorIndex][20];   //20
                            DataProximal.a[38]=RawStaticData[21]-InitialValue[SensorIndex][21];   //21
                            DataProximal.a[39]=RawStaticData[22]-InitialValue[SensorIndex][22];   //22
                            DataProximal.a[40]=RawStaticData[23]-InitialValue[SensorIndex][23];  //23

                            DataProximal.a[43]=RawStaticData[24]-InitialValue[SensorIndex][24];  //24
                            DataProximal.a[44]=RawStaticData[25]-InitialValue[SensorIndex][25];  //25
                            DataProximal.a[45]=RawStaticData[26]-InitialValue[SensorIndex][26];  //26
                            DataProximal.a[46]=RawStaticData[27]-InitialValue[SensorIndex][27];  //27
                        }
                        if (MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]==VF->CurrentSensorToVisualize && StaticValueTooHigh==false)
                        {
                            if (TempData.length()==40)
                            {
                                VF->DataTable[0]=DataProximal[7];VF->DataTable[1]=DataProximal[13];VF->DataTable[2]=DataProximal[19];VF->DataTable[3]=DataProximal[25];VF->DataTable[4]=DataProximal[31];
                                VF->DataTable[5]=DataProximal[8];VF->DataTable[6]=DataProximal[14];VF->DataTable[7]=DataProximal[20];VF->DataTable[8]=DataProximal[26];VF->DataTable[9]=DataProximal[32];
                                VF->DataTable[10]=DataProximal[9];VF->DataTable[11]=DataProximal[15];VF->DataTable[12]=DataProximal[21];VF->DataTable[13]=DataProximal[27];VF->DataTable[14]=DataProximal[33];
                                VF->DataTable[15]=DataProximal[10];VF->DataTable[16]=DataProximal[16];VF->DataTable[17]=DataProximal[22];VF->DataTable[18]=DataProximal[28];VF->DataTable[19]=DataProximal[34];
                            }
                        }
                        if (MW->EnableDatalogging==true && x>=4)
                        {
                            if (TempData.length()==40) //Proximal
                            {
                                OutputStream << QTime::currentTime().toString() << ":" <<QTime::currentTime().msec() << "," << "G" << CurrentArmIndex+1 << "," << "S" << MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex] << "," << DataProximal.a[7] << "," << DataProximal.a[8] << "," << DataProximal.a[9] << "," << DataProximal.a[10] << "," << DataProximal.a[13] << "," << DataProximal.a[14] << "," << DataProximal.a[15] << "," << DataProximal.a[16]<< "," << DataProximal.a[19] << "," << DataProximal.a[20]<< "," << DataProximal.a[21] << "," << DataProximal.a[22]<< "," << DataProximal.a[25] << "," << DataProximal.a[26]<< "," << DataProximal.a[27] << "," << DataProximal.a[28]<< "," << DataProximal.a[31] << "," << DataProximal.a[32] << "," << DataProximal.a[33] << "," << DataProximal.a[34] <<"\n";
                            }
                        }

                        for(i=0;i<47;i++)
                        {
                            if(DataProximal.a[i]<10) // To reduce noise level
                            {
                                DataProximal.a[i]=0;
                            }
                        }
                        if(MW->DataKind==0 && MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]==VF->CurrentSensorToVisualize && CurrentArmIndex==VF->CurrentArmToVisualize && StaticValueTooHigh==false && WrongCom==false)
                        {
                            if (VF->VisualizationIsLocked==false && TempData.length()==112)
                            {
                                VF->DataStatic=DataProximal.Resize(36,54);
                                emit NewDataAvailable(0);
                            }
                            CommunicationErrorCount=0;
                        }
                        else if (MW->DataKind==3 && CurrentArmIndex==VF->CurrentArmToVisualize && StaticValueTooHigh==false && WrongCom==false)
                        {
                            if (TempData.length()==40)
                            {
                                VF->DataStaticAllSensor[SensorIndex]=DataProximal.Resize(36,42);
                            }
                            if (VF->VisualizationIsLocked==false)
                            {
                                emit NewDataAvailable(3);
                            }
                            CommunicationErrorCount=0;
                        }
                        else if (WrongCom==true)
                        {
                            if (SensorNumberWhenErrorWasPrompted!=VF->CurrentSensorToVisualize && CommunicationErrorCount>10)
                            {
                                emit ErrorOpeningPort(3);
                                qDebug() << "We had a communication problem";
                                SensorNumberWhenErrorWasPrompted=VF->CurrentSensorToVisualize;
                            }
                            if(SensorNumberWhenErrorWasPrompted!=VF->CurrentSensorToVisualize && MW->DataKind!=3)
                            {
                                CommunicationErrorCount++;
                            }
                            else
                            {
                                CommunicationErrorCount=0;
                            }
                            WrongCom=false;
                        }
                    }
                    StaticValueTooHigh=false;
                    TempData.clear();
                    SensorIndex++;
                }
                SensorIndex=0;
                if (MW->EnableDatalogging==true && MW->NumberOfArms>(CurrentArmIndex+1))
                {
                    CurrentArmIndex++;
                }
                else if (MW->EnableDatalogging==true && MW->NumberOfArms==(CurrentArmIndex+1) && MW->NumberOfArms!=1)
                {
                    CurrentArmIndex=0; // Then we go back to the first arm.
                }
                else if (MW->EnableDatalogging==false && MW->EnableDataVisualization==true)
                {
                    CurrentArmIndex=VF->CurrentArmToVisualize;
                }
            }
        }
        else if (PortConfigured==true && MW->DataKind==1) // Port is configured and we want dynamic data:
        {
            QByteArray message1; QByteArray message2; QByteArray TempData; int i=0, SensorIndex=0;
            timer.start(); VF->DataDynamicY.Create(6000,1);VF->DataDynamicResized.Create(6000,1); VF->DataTime.Create(6000,1); msec=timer.elapsed();timer2.start(); timer3.start(); FrequencyTimer.start(); OneMsecTimer.start();

            while (VF->closed==0 && MW->DataKind==1)
            {
                while(MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]!=-1)
                {
                    if (LastSensorThatWasVisualized!=VF->CurrentSensorToVisualize)
                    {
                        VF->grDyn->SetRanges(0,5000,-1,1,0,10);
                        VF->grDyn->SetTicks('x',1000,0);
                        VF->grDyn2->SetRanges(0,5000,-1,1,0,10);
                        VF->grDyn2->SetTicks('x',1000,0);
                        val[0]=0;val[1]=1000;val[2]=2000;val[3]=3000;val[4]=4000;val[5]=5000;
                        VF->grDyn->SetTicksVal('x',mglData(6,val),"\\0\n\\1\n\\2\n\\3\n\\4\n\\5");
                        VF->grDyn2->SetTicksVal('x',mglData(6,val),"\\0\n\\1\n\\2\n\\3\n\\4\n\\5");
                        VF->DataDynamicY=new mglData(6000); VF->DataTime=new mglData(6000);
                        DynRange=5000; count=0; count2=0; count3=0; timer2.restart(); timer3.restart();
                        LastSensorThatWasVisualized=VF->CurrentSensorToVisualize;
                    }
                    //                    message1.append((char)(250&0xFF));
                    NanoSecTimer=OneMsecTimer.nsecsElapsed()+700000;
                    if (SensorIndex<=1)
                    {
                        message1.append((char)((100)&0xFF));
                    }
                    //                    else if (SensorIndex>1 && SensorIndex<=3)
                    //                    {
                    //                        message1.append((char)((101)&0xFF));
                    //                    }
                    serial[CurrentArmIndex].write(message1);
                    serial[CurrentArmIndex].waitForBytesWritten(20);
                    //                    if (serial[CurrentArmIndex].waitForReadyRead(20))
                    //                    {
                    //                        TempData = serial[CurrentArmIndex].readAll();
                    //                        CommunicationErrorCount=0;
                    //                    }
                    //                    else
                    //                    {
                    //                        WrongCom=true;
                    //                    }

                    while(OneMsecTimer.nsecsElapsed()<NanoSecTimer)
                    {
                        //Do nothing
                    }


                    if (serial[CurrentArmIndex].waitForReadyRead(20))
                    {
                        TempData += serial[CurrentArmIndex].readAll();
                    }

                    qDebug() << "TempData size: " << TempData.size();
                    while(i<TempData.size())
                    {
                        if(TempData.size() == 4)
                        {

                            VF->DataDynamicY[count]= (double)static_cast<signed qint16>((TempData[(VF->CurrentSensorToVisualize)*2-2] & 0xFF)*256 + (TempData[(VF->CurrentSensorToVisualize)*2-1] & 0xFF))*((1.024*2) / 65535);
                            if(MW->EnableDatalogging==true)
                            {
                                OutputStream << QTime::currentTime().toString() << ":" <<QTime::currentTime().msec() << "," << "G" << CurrentArmIndex+1 << "," <<  "D" << MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex] << "," << VF->DataDynamicY[count] << "\n";
                                qDebug() << count << " RawDynamicData: " << VF->DataDynamicY[count]; //This is useful to check the input data rate
                            }
                            if (MW->EnableDataVisualization==true && count%5==0)
                            {
                                VF->DataDynamicResized[count2]=VF->DataDynamicY[count];
                                VF->DataTime.a[count2]=timer2.elapsed();
                                count2++;
                            }
                            count++; count3++; CountForFrequency++;

                            if (MW->EnableDataVisualization==true && timer2.elapsed()>DynRange)
                            {
                                ParameterString="\\"+ QString::number(DynRange/1000)+"\n\\" + QString::number(DynRange/1000+1)+"\n\\"+ QString::number(DynRange/1000+2)+"\n\\"+ QString::number(DynRange/1000+3)+"\n\\"+ QString::number(DynRange/1000+4)+"\n\\"+ QString::number(DynRange/1000+5)+"";
                                ParameterStringByteArray=ParameterString.toLocal8Bit();
                                ParameterPointer=ParameterStringByteArray.data();
                                val[0]=DynRange;val[1]=DynRange+1000;val[2]=DynRange+2000;val[3]=DynRange+3000;val[4]=DynRange+4000;val[5]=DynRange+5000;
                                VF->grDyn->SetRanges(DynRange,DynRange+5000,-1,1,0,10);
                                VF->grDyn->SetTicks('x',1000,0);
                                VF->grDyn->SetTicksVal('x',mglData(6,val),ParameterPointer);
                                VF->grDyn2->SetRanges(DynRange,DynRange+5000,-1,1,0,10);
                                VF->grDyn2->SetTicks('x',1000,0);
                                VF->grDyn2->SetTicksVal('x',mglData(6,val),ParameterPointer);
                                DynRange+=5000;
                                count2=0; count=0;
                                VF->DataTime=new mglData(6000);
                            }
                            else if (MW->EnableDataVisualization==true && timer3.elapsed()>1000)
                            {
                                if (count3>=800)
                                {
                                    if ((count-count3)>=0)
                                    {
                                        for (int h=count;h<4096;h++)
                                        {
                                            VF->FFTin[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<(count-count3);h++)
                                        {
                                            VF->FFTin[h+count3+(4096-count)]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<count3;h++)
                                        {
                                            VF->FFTin[h]=VF->DataDynamicY[count-count3+h];
                                        }
                                    }
                                    else
                                    {
                                        for (int h=count;h<(4096+count-count3);h++)
                                        {
                                            VF->FFTin[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<=(count3-count);h++)
                                        {
                                            VF->FFTin[h]=VF->DataDynamicY[4095+count-count3+h];
                                        }
                                        for (int h=0; h<count; h++)
                                        {
                                            VF->FFTin[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                    }
                                }
                                else if (count3 >=400 && count3 < 800)
                                {
                                    if ((count-count3)>=0)
                                    {
                                        for (int h=count;h<2048;h++)
                                        {
                                            VF->FFTin2[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<(count-count3);h++)
                                        {
                                            VF->FFTin2[h+count3+(2048-count)]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<count3;h++)
                                        {
                                            VF->FFTin2[h]=VF->DataDynamicY[count-count3+h];
                                        }
                                    }
                                    else
                                    {
                                        for (int h=count;h<(2048+count-count3);h++)
                                        {
                                            VF->FFTin2[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<=(count3-count);h++)
                                        {
                                            VF->FFTin2[h]=VF->DataDynamicY[2048+count-count3+h];
                                        }
                                        for (int h=0; h<count; h++)
                                        {
                                            VF->FFTin2[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                    }
                                }
                                else if (count3 >=200 && count3 < 400)
                                {
                                    if ((count-count3)>=0)
                                    {
                                        for (int h=count;h<1024;h++)
                                        {
                                            VF->FFTin3[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<(count-count3);h++)
                                        {
                                            VF->FFTin3[h+count3+(1024-count)]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<count3;h++)
                                        {
                                            VF->FFTin3[h]=VF->DataDynamicY[count-count3+h];
                                        }
                                    }
                                    else
                                    {
                                        for (int h=count;h<(1024+count-count3);h++)
                                        {
                                            VF->FFTin3[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<=(count3-count);h++)
                                        {
                                            VF->FFTin3[h]=VF->DataDynamicY[1024+count-count3+h];
                                        }
                                        for (int h=0; h<count; h++)
                                        {
                                            VF->FFTin3[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                    }
                                }
                                else if (count3 >=100 && count3 < 200)
                                {
                                    if ((count-count3)>=0)
                                    {
                                        for (int h=count;h<512;h++)
                                        {
                                            VF->FFTin4[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<(count-count3);h++)
                                        {
                                            VF->FFTin4[h+count3+(512-count)]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<count3;h++)
                                        {
                                            VF->FFTin4[h]=VF->DataDynamicY[count-count3+h];
                                        }
                                    }
                                    else
                                    {
                                        for (int h=count;h<(512+count-count3);h++)
                                        {
                                            VF->FFTin4[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                        for (int h=0;h<=(count3-count);h++)
                                        {
                                            VF->FFTin4[h]=VF->DataDynamicY[512+count-count3+h];
                                        }
                                        for (int h=0; h<count; h++)
                                        {
                                            VF->FFTin4[h+count3-count]=VF->DataDynamicY[h];
                                        }
                                    }
                                }
                                VF->FourierCount=count3;
                                if (count3 >=100)
                                {
                                    VF->STFAIsReady=true;
                                }
                                else if (count3 <125 && VF->DynamicDataViewMode==1)
                                {
                                    VF->STFAIsReady=false;
                                    emit ErrorPlottingFourierTransform(7);
                                    VF->RevertSTFA();
                                }
                                count3=0;
                                timer3.restart();
                            }
                            i=i+4;

                        }
                        else
                        {
                            i=i+1;
                        }
                    }

                    i=0;
                    TempData.clear();
                    if ((timer.elapsed()-msec)>100 && (MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]==VF->CurrentSensorToVisualize && CurrentArmIndex==VF->CurrentArmToVisualize) && WrongCom==false) // Emit a signal to refresh plot
                    {
                        VF->CurrentFrequency=(CountForFrequency*1000)/FrequencyTimer.elapsed();
                        for (int g=count2;g<5000;g++)
                        {
                            VF->DataDynamicResized[g]=NAN; // Don't plot "0" points
                            VF->DataTime[g]=NAN;
                        }
                        if (VF->VisualizationIsLocked==false)
                        {
                            emit NewDataAvailable(1);
                        }
                        msec=timer.elapsed();
                        CommunicationErrorCount=0;
                        CountForFrequency=0; FrequencyTimer.restart();
                    }
                    else if (WrongCom==true)
                    {
                        if (SensorNumberWhenErrorWasPrompted!=VF->CurrentSensorToVisualize && CommunicationErrorCount>10)
                        {
                            emit ErrorOpeningPort(3);
                            qDebug() << "We had a communication problem";
                            SensorNumberWhenErrorWasPrompted=VF->CurrentSensorToVisualize;
                        }
                        if (SensorNumberWhenErrorWasPrompted!=VF->CurrentSensorToVisualize)
                        {
                            CommunicationErrorCount++;
                        }
                        else
                        {
                            CommunicationErrorCount=0;
                        }
                        WrongCom=false;
                    }
                    message1.clear();message2.clear(); SensorIndex++;

                    while(OneMsecTimer.nsecsElapsed()<(NanoSecTimer+280000))
                    {
                        //Do nothing
                    }

                    OneMsecTimer.restart();
                }
                SensorIndex=0;
                if (MW->EnableDatalogging==true && MW->NumberOfArms>(CurrentArmIndex+1))
                {
                    CurrentArmIndex++;
                }
                else if (MW->EnableDatalogging==true && MW->NumberOfArms==(CurrentArmIndex+1) && MW->NumberOfArms!=1)
                {
                    CurrentArmIndex=0; // Then we go back to the first arm.
                }
                else if (MW->EnableDatalogging==false && MW->EnableDataVisualization==true)
                {
                    CurrentArmIndex=VF->CurrentArmToVisualize;
                }
            }
            qDebug() << "Ellapsed Time:" << timer.elapsed();
            VF->grDyn->SetRanges(0,5000,-1,1,0,10);
            VF->grDyn->SetTicks('x',1000,0);
            VF->grDyn2->SetRanges(0,5000,-1,1,0,10);
            VF->grDyn2->SetTicks('x',1000,0);
            val[0]=0;val[1]=1000;val[2]=2000;val[3]=3000;val[4]=4000;val[5]=5000;
            VF->grDyn->SetTicksVal('x',mglData(6,val),"\\0\n\\1\n\\2\n\\3\n\\4\n\\5");
            VF->grDyn2->SetTicksVal('x',mglData(6,val),"\\0\n\\1\n\\2\n\\3\n\\4\n\\5");
            VF->DataDynamicY=new mglData(6000); VF->DataTime=new mglData(6000);
            DynRange=5000; count=0; count2=0;
        }
        else if (PortConfigured==true && MW->DataKind==2) // High Sensitivity Data
        {
            QByteArray messageH; QByteArray TempDataH;
            int RawHighSensitivityData[12]; int i=0,x1=0,x2=0,SensorIndex=0;
            while (VF->closed==0 && MW->DataKind==2)
            {
                while(MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]!=-1 && MW->DataKind==2 && VF->closed==0)
                {
                    if (VF->ResetButtonIsPushed==true)
                    {
                        x1=0; x2=0;
                        VF->ResetButtonIsPushed=false;
                    }
                    messageH.append((char)(250&0xFF));
                    messageH.append((char)((MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex])&0xFF));
                    messageH.append((char)(((CRCLookuptable->CRC16Table[MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]])&0xFF00)>>8));
                    messageH.append((char)((CRCLookuptable->CRC16Table[MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]])&0xFF));
                    serial[CurrentArmIndex].write(messageH);
                    serial[CurrentArmIndex].waitForBytesWritten(20);
                    messageH.clear();

                    while (serial[CurrentArmIndex].waitForReadyRead(20))
                    {
                        TempDataH += serial[CurrentArmIndex].readAll();
                    }
                    if((static_cast<unsigned int>(TempDataH[0]) & 0xFF)*256+(static_cast<unsigned int>(TempDataH[1]) & 0xFF)==(250*256+MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]))
                    {
                        for(i=0;i<=22;i=i+2)
                        {
                            RawHighSensitivityData[i/2]= (static_cast<unsigned int>(TempDataH[i+2]) & 0xFF)*256+(static_cast<unsigned int>(TempDataH[i+3]) & 0xFF);
                        }
                    }
                    else
                    {
                        WrongCom=true;
                    }
                    TempDataH.clear();
                    if (x1<5)
                    {
                        memcpy(InitialValue[SensorIndex],RawHighSensitivityData, sizeof RawHighSensitivityData);
                        x1++;
                    }
                    DataTempH.a[0]=RawHighSensitivityData[0]-InitialValue[SensorIndex][0];
                    DataTempH.a[1]=RawHighSensitivityData[1]-InitialValue[SensorIndex][1];
                    DataTempH.a[2]=RawHighSensitivityData[2]-InitialValue[SensorIndex][2];
                    DataTempH.a[3]=RawHighSensitivityData[3]-InitialValue[SensorIndex][3];
                    DataTempH.a[4]=RawHighSensitivityData[4]-InitialValue[SensorIndex][4];
                    DataTempH.a[5]=RawHighSensitivityData[5]-InitialValue[SensorIndex][5];
                    DataTempH.a[6]=RawHighSensitivityData[6]-InitialValue[SensorIndex][6];
                    DataTempH.a[7]=RawHighSensitivityData[7]-InitialValue[SensorIndex][7];
                    DataTempH.a[8]=RawHighSensitivityData[8]-InitialValue[SensorIndex][8];
                    DataTempH.a[9]=RawHighSensitivityData[9]-InitialValue[SensorIndex][9];
                    DataTempH.a[10]=RawHighSensitivityData[10]-InitialValue[SensorIndex][10];
                    DataTempH.a[11]=RawHighSensitivityData[11]-InitialValue[SensorIndex][11];

                    for(i=0;i<2;i++)
                    {
                        if(DataTempH.a[i]<10) // To reduce noise level
                        {
                            DataTempH.a[i]=0;
                        }
                    }

                    DataTempH.a[2]=DataTempH.a[5]+DataTempH.a[8];
                    if (x1==5)
                    {
                        DataIni=DataTempH.a[2];
                    }

                    if(x2<6 && x1==5)
                    {
                        DataTempH[2]=DataTempH[2]-DataIni;
                        DataMean[x2]=DataTempH[2];
                        x2++;
                    }

                    if(x2==6 && x1==5)
                    {
                        for (int j=0;j<=3;j++)
                        {
                            Actual=Actual+DataMean[j];
                        }
                        Actual=(Actual-DataIni)/6;

                        for (int j=0;j<=2;j++)
                        {
                            DataMean[j]=DataMean[j+1];
                        }
                        DataMean[3]=DataTempH[2];
                    }

                    if(MW->ListOfSensorsPerArms[CurrentArmIndex][SensorIndex]==VF->CurrentSensorToVisualize && WrongCom==false)
                    {
                        if(Actual>300) // Ceil the value
                        {
                            Actual=300;
                        }
                        else if (Actual<-50) // Floor the value
                        {
                            Actual=-50;
                        }
                        VF->DataHighSensitivity[0]=Actual;VF->DataHighSensitivity[1]=Actual;
                        VF->DataHighSensitivity[2]=Actual;VF->DataHighSensitivity[3]=Actual;
                        if (VF->VisualizationIsLocked==false)
                        {
                            emit NewDataAvailable(2);
                        }
                        CommunicationErrorCount=0;
                        Actual=0;
                    }
                    else if (WrongCom==true)
                    {
                        if (SensorNumberWhenErrorWasPrompted!=VF->CurrentSensorToVisualize && CommunicationErrorCount>6)
                        {
                            emit ErrorOpeningPort(3);
                            qDebug() << "We had a communication problem";
                            SensorNumberWhenErrorWasPrompted=VF->CurrentSensorToVisualize;
                        }
                        if(SensorNumberWhenErrorWasPrompted!=VF->CurrentSensorToVisualize)
                        {
                            CommunicationErrorCount++;
                        }
                        else
                        {
                            CommunicationErrorCount=0;
                        }
                        WrongCom=false;
                    }
                    SensorIndex++;
                }
                SensorIndex=0;
                CurrentArmIndex=VF->CurrentArmToVisualize;
            }
        }
    }
    if (FilePointer->isOpen()==true)
    {
        FilePointer->flush();
    }
    FilePointer->close();
    for (int NumberOfOpenPorts=0;NumberOfOpenPorts<NumberOfPorts;NumberOfOpenPorts++)
    {
        serial[NumberOfOpenPorts].close();
    }
    emit finished();
}


int CommunicationHandler::ReadSensorID()
{
    QSerialPort serial; PortIsSet=false; SensorID=-1;

    //We start by detecting which port communicate with the sensors:
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Standard BAUD Rates:" << info.standardBaudRates();
        if(info.description()=="RS-485 Port") //We found it so we set it:
        {
            serial.setPort(info); PortIsSet=true;
        }
    }

    //We check that the port is open for writting and reading, and we configure it:
    if (serial.open(QIODevice::ReadWrite))
    {
        qint32 custombaudrate=460800;
        QByteArray TempResp;QByteArray message;
        serial.setBaudRate(custombaudrate);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setDataBits(QSerialPort::Data8);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        PortConfigured=true;

        message.append((char)(250&0xFF)); serial.write(message); serial.waitForBytesWritten(20); message.clear();
        message.append((char)(254&0xFF)); serial.write(message); serial.waitForBytesWritten(20); message.clear();
        message.append((char)(194&0xFF)); serial.write(message); serial.waitForBytesWritten(20); message.clear();
        message.append((char)(144&0xFF)); serial.write(message); serial.waitForBytesWritten(20);
        message.clear();
        while (serial.waitForReadyRead(200))
        {
            TempResp += serial.readAll();
        }

        if ((static_cast<unsigned int>(TempResp[0]) & 0xFF)==250)
        {
            SensorID= (static_cast<unsigned int>(TempResp[1]) & 0xFF);
        }
        else
        {
            return SensorID;
        }
        TempResp.clear();
    }
    if (serial.isOpen())
    {
        serial.close();
    }
    return SensorID;
}

bool CommunicationHandler::SetSensorID(int DesiredID)
{
    QSerialPort serial; PortIsSet=false; SetSensorIDSuccess=false;

    //We start by detecting which port communicate with the sensors:
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Standard BAUD Rates:" << info.standardBaudRates();
        if(info.description()=="RS-485 Port") //We found it so we set it:
        {
            serial.setPort(info); PortIsSet=true;
        }
    }

    crc=37250^DesiredID;
    for(int j=0;j<8;j++)
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

    //We check that the port is open for writting and reading, and we configure it:
    if (serial.open(QIODevice::ReadWrite))
    {
        qint32 custombaudrate=460800;
        QByteArray TempResp;QByteArray message;
        serial.setBaudRate(custombaudrate);
        serial.setParity(QSerialPort::NoParity);
        serial.setStopBits(QSerialPort::OneStop);
        serial.setDataBits(QSerialPort::Data8);
        serial.setFlowControl(QSerialPort::NoFlowControl);
        PortConfigured=true;
        message.append((char)(250&0xFF)); serial.write(message); serial.waitForBytesWritten(20); message.clear();
        message.append((char)(253&0xFF)); serial.write(message); serial.waitForBytesWritten(20); message.clear();
        message.append((char)(DesiredID&0xFF)); serial.write(message); serial.waitForBytesWritten(20); message.clear();
        message.append((char)(((crc)&0xFF))); serial.write(message); serial.waitForBytesWritten(20); message.clear();
        message.append((char)(((crc)&0xFF00)>>8)); serial.write(message); serial.waitForBytesWritten(20); message.clear();
        QTime dieTime= QTime::currentTime().addSecs(1);
        while( QTime::currentTime() < dieTime )
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        while (serial.waitForReadyRead(200))
        {
            TempResp += serial.readAll();
        }
        if ((static_cast<unsigned int>(TempResp[0]) & 0xFF)==250 && (static_cast<unsigned int>(TempResp[1]) & 0xFF)==DesiredID+5)
        {
            SetSensorIDSuccess=true;
        }
        else
        {
            SetSensorIDSuccess=false;
        }
        TempResp.clear();
    }
    if (serial.isOpen())
    {
        serial.close();
    }
    return SetSensorIDSuccess;
}
