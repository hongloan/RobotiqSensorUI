/**************************************************
 * Author: Jean-Philippe Roberge Ing. jr., M.Sc.A.*
 * Date of creation: August 14th 2013             *
 * Affiliations: Control and Robotics Laboratory  *
 *************************************************/

#include <qapplication.h>
#include "visualizationform.h"
#include "ui_visualizationform.h"


VisualizationForm::VisualizationForm(QWidget *parent, MainWindow *mainwnd) :
    QWidget(parent),
    ui(new Ui::VisualizationForm)
{
    ui->setupUi(this);
    MW = mainwnd;
#if 0
    DataStatic = new mglData(42);
    DataStaticAllSensor[0]= new mglData(42);DataStaticAllSensor[1]= new mglData(42);DataStaticAllSensor[2]= new mglData(42);DataStaticAllSensor[3]= new mglData(42);DataStaticAllSensor[4]= new mglData(42);
    DataStaticAllSensor[5]= new mglData(42);DataStaticAllSensor[6]= new mglData(42);DataStaticAllSensor[7]= new mglData(42);DataStaticAllSensor[8]= new mglData(42);DataStaticAllSensor[9]= new mglData(42);
    SumDataStatic = new mglData(42); SumOfSquaredDataStatic = new mglData(42); MeanDataStatic= new mglData(42); STDDataStatic = new mglData(42);
#else
    DataStatic.Create(42);
    DataStaticAllSensor[0].Create(42);DataStaticAllSensor[1].Create(42);DataStaticAllSensor[2].Create(42);DataStaticAllSensor[3].Create(42);DataStaticAllSensor[4].Create(42);
    DataStaticAllSensor[5].Create(42);DataStaticAllSensor[6].Create(42);DataStaticAllSensor[7].Create(42);DataStaticAllSensor[8].Create(42);DataStaticAllSensor[9].Create(42);
    SumDataStatic.Create(42); SumOfSquaredDataStatic.Create(42); MeanDataStatic.Create(42); STDDataStatic.Create(42);
#endif
    NumberOfStaticData =0; DynamicDataViewMode=0;
    ui->STFACheckbox->setChecked(false);
    timer.start();
    DataDynamicResized=new mglData(6000);
    DataDynamicFourier = new mglData(2048); DataDynamicFourier.Create(2048,1);
    DataDynamicFourier2 = new mglData(1024); DataDynamicFourier2.Create(1024,1);
    DataDynamicFourier3 = new mglData(512); DataDynamicFourier3.Create(512,1);
    DataDynamicFourier4 = new mglData(256); DataDynamicFourier4.Create(256,1);
    DataDynamicY=new mglData(6000);
    LastDataDynamicY = new mglData(6000);
    DataTime=new mglData(6000);
    DataHighSensitivity=new mglData(4);
    DataTable= new mglData(20);
    ResetButtonIsPushed=false;
    AllSensorsFirstTime=true;
    PassNumberAllSensors=0;
    SumOfAllTaxelsAvg=0; SumOfAllTaxelsStd=0;
    CurrentFrequency=0;
    STFAIsReady=false;
    VisualizationIsLocked=false;
    grHigh= new mglGraph(0,780,500);
    grHigh->SetRanges(0,1,0,300,0,300);
    grHigh->Light(true);
    grHigh->Rotate(180,180);
    grHigh->SetOrigin(1,0);
    val[0]=0; val[1]=1;
    val2[0]=0;val2[1]=1000;val2[2]=2000;val2[3]=3000;val2[4]=4000;val2[5]=5000;
    val3[0]=512; val3[1]=1024; val3[2]=1536; val3[3]=2048;
    grAll= new mglGraph(0,780,500);
    grAll->Light(true);
    grDyn= new mglGraph(0,780,500);
    grDyn->SetRanges(0,5000,-1,1,0,10);
    grDyn->SetTicks('x',1000,0);
    grDyn->SetTicksVal('x',mglData(6,val2),"\\0\n\\1\n\\2\n\\3\n\\4\n\\5");
    grDyn->Light(true);
    grDyn2= new mglGraph(0,780,250);
    grDyn2->SetRanges(0,5000,-1,1,0,10);
    grDyn2->SetTicks('x',1000,0);
    grDyn2->SetTicksVal('x',mglData(6,val2),"\\0\n\\1\n\\2\n\\3\n\\4\n\\5");
    grDyn2->Light(true);
    grFourier = new mglGraph(0,780,250);
    grFourier->SetRanges(0,2048,0,20);
    grFourier->SetTicksVal('x',mglData(4,val3),"\\125\n\\250\n\\375\n\\500");
    gr= new mglGraph(0,780,500);
    gr->SetRanges(0,3,0,6,-800,5000); gr->Rotate(60,250); gr->Light(true);
    gr->SetTuneTicks(true);
    gr->SetTicks('x',1,0); gr->SetTicks('y',1,0);
    w=gr->GetWidth(); h=gr->GetHeight();
    this->setFixedSize(w+200,h+70);
    DataFreeze=false;
    ui->testLabel->setFixedSize(w,h);
    ui->testLabel->move(175,45);
    ui->testLabel_2->setFixedSize(w,250);
    ui->testLabel_2->move(175,45);
    ui->Title->move(w/2-10,4);
    buf = new uchar[4*w*h];
    buf2 = new uchar[4*w*h];
    VisualizationForm::PlotData(MW->DataKind);
    closed=0;
    CurrentSensorToVisualize=1;
    CurrentArmToVisualize=0;
//    CurrentArmTypeToVisualize=MW->Griper1->GripperType;
    ui->visualizeArm1CheckBox->setChecked(true);
    ui->SensorArm2ComboBox->setDisabled(true);
    ui->SensorArm3ComboBox->setDisabled(true);
    ui->TaxelVal1->setVisible(false); ui->TaxelVal2->setVisible(false); ui->TaxelVal3->setVisible(false); ui->TaxelVal4->setVisible(false);  ui->TaxelVal5->setVisible(false); ui->TaxelVal6->setVisible(false);
    ui->TaxelVal7->setVisible(false); ui->TaxelVal8->setVisible(false); ui->TaxelVal9->setVisible(false); ui->TaxelVal10->setVisible(false); ui->TaxelVal11->setVisible(false); ui->TaxelVal12->setVisible(false);
    ui->TaxelVal13->setVisible(false); ui->TaxelVal14->setVisible(false); ui->TaxelVal15->setVisible(false); ui->TaxelVal16->setVisible(false); ui->TaxelVal17->setVisible(false); ui->TaxelVal18->setVisible(false);
    ui->TaxelVal19->setVisible(false); ui->TaxelVal20->setVisible(false);
    ui->LineTableDown->setVisible(false); ui->lineTableUp->setVisible(false); ui->LineTableRight->setVisible(false); ui->LineTableLeft->setVisible(false);
    ui->STDVal1->setVisible(false);ui->STDVal2->setVisible(false);ui->STDVal3->setVisible(false);ui->STDVal4->setVisible(false);ui->STDVal5->setVisible(false);ui->STDVal6->setVisible(false);
    ui->STDVal7->setVisible(false);ui->STDVal8->setVisible(false);ui->STDVal9->setVisible(false);ui->STDVal10->setVisible(false);ui->STDVal11->setVisible(false);ui->STDVal12->setVisible(false);
    ui->STDVal13->setVisible(false);ui->STDVal14->setVisible(false);ui->STDVal15->setVisible(false);ui->STDVal16->setVisible(false);ui->STDVal17->setVisible(false);ui->STDVal18->setVisible(false);
    ui->STDVal19->setVisible(false);ui->STDVal20->setVisible(false);
    ui->LineTableDown2->setVisible(false); ui->LineTableLeft2->setVisible(false); ui->LineTableRight2->setVisible(false); ui->lineTableUp2->setVisible(false); ui->StandardDeviationLabel->setVisible(false); ui->AverageValuesCheckBox->setVisible(false);
    ui->FreezePushButton->setVisible(true);
    ui->SumOfAllTaxelsAvgLabel->setVisible(false); ui->SumOfAllTaxelsAvgValueLabel->setVisible(false); ui->SumOfAllTaxelsStdLabel->setVisible(false); ui->SumOfAllTaxelsStdValueLabel->setVisible(false);
    ui->Sensor1Label->setVisible(false);ui->Sensor2Label->setVisible(false);ui->Sensor3Label->setVisible(false);ui->Sensor4Label->setVisible(false);ui->Sensor5Label->setVisible(false);
    ui->Sensor6Label->setVisible(false);ui->Sensor7Label->setVisible(false);ui->Sensor8Label->setVisible(false);ui->Sensor9Label->setVisible(false);ui->Sensor10Label->setVisible(false);
    ui->resetPushButton->move(10,420);
    ui->ExportGraphPushButton->setDisabled(true);
    FFTin = (double*) fftw_malloc(sizeof(double)*4096);
    FFTout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*4096);
    FFTplan = fftw_plan_dft_r2c_1d(4096, FFTin, FFTout, FFTW_ESTIMATE); // This is the plan for 1000 Hz
    FFTin2 = (double*) fftw_malloc(sizeof(double)*2048);
    FFTout2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*2048);
    FFTplan2 = fftw_plan_dft_r2c_1d(2048, FFTin2, FFTout2, FFTW_ESTIMATE); // This is the plan for 500 Hz
    FFTin3 = (double*) fftw_malloc(sizeof(double)*1024);
    FFTout3 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*1024);
    FFTplan3 = fftw_plan_dft_r2c_1d(1024, FFTin3, FFTout3, FFTW_ESTIMATE); // This is the plan for 250 Hz
    FFTin4 = (double*) fftw_malloc(sizeof(double)*512);
    FFTout4 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*512);
    FFTplan4 = fftw_plan_dft_r2c_1d(512, FFTin4, FFTout4, FFTW_ESTIMATE); // This is the plan for 125 Hz

    if (MW->NumberOfArms==1)
    {
        ui->arm2Label->setVisible(false); ui->sensorArm2Label->setVisible(false); ui->SensorArm2ComboBox->setVisible(false);
        ui->arm3Label->setVisible(false); ui->sensorArm3Label->setVisible(false); ui->SensorArm3ComboBox->setVisible(false);
        ui->visualizeArm2CheckBox->setVisible(false);
        ui->visualizeArm3CheckBox->setVisible(false);
        if (MW->EnableDatalogging==false)
        {
            for(int i=0;i<MW->NumberOfSensors[0];i++)
            {
                ui->SensorArm1ComboBox->addItem(QString::number(MW->Griper1->SensorAdress[i]));
            }
        }
        else
        {
            for(int i=0;i<MW->NumberOfSensorsToLogArm1;i++)
            {
                ui->SensorArm1ComboBox->addItem(QString::number(MW->ListOfSensorsPerArms[0][i]));
            }
        }

        ArmsSensorToVisualize[0]=1;ArmsSensorToVisualize[1]=-1;ArmsSensorToVisualize[2]=-1;
    }
    else if (MW->NumberOfArms==2)
    {
        ui->arm3Label->setVisible(false); ui->sensorArm3Label->setVisible(false); ui->SensorArm3ComboBox->setVisible(false); ui->visualizeArm3CheckBox->setVisible(false);
        if (MW->EnableDatalogging==false)
        {
            for(int i=0;i<MW->NumberOfSensors[0];i++)
            {
                ui->SensorArm1ComboBox->addItem(QString::number(MW->Griper1->SensorAdress[i]));
            }
            for(int i=0;i<MW->NumberOfSensors[1];i++)
            {
                ui->SensorArm2ComboBox->addItem(QString::number(MW->Griper2->SensorAdress[i]));
            }
            ArmsSensorToVisualize[0]=1;ArmsSensorToVisualize[1]=1;ArmsSensorToVisualize[2]=-1;
        }
        else
        {
            for(int i=0;i<MW->NumberOfSensorsToLogArm1;i++)
            {
                ui->SensorArm1ComboBox->addItem(QString::number(MW->ListOfSensorsPerArms[0][i]));
            }
            for(int i=0;i<MW->NumberOfSensorsToLogArm2;i++)
            {
                ui->SensorArm2ComboBox->addItem(QString::number(MW->ListOfSensorsPerArms[1][i]));
            }
        }
    }
    else if (MW->NumberOfArms==3)
    {
        if (MW->EnableDatalogging==false)
        {
            for(int i=0;i<MW->NumberOfSensors[0];i++)
            {
                ui->SensorArm1ComboBox->addItem(QString::number(MW->Griper1->SensorAdress[i]));
            }
            for(int i=0;i<MW->NumberOfSensors[1];i++)
            {
                ui->SensorArm2ComboBox->addItem(QString::number(MW->Griper2->SensorAdress[i]));
            }
            for(int i=0;i<MW->NumberOfSensors[2];i++)
            {
                ui->SensorArm3ComboBox->addItem(QString::number(MW->Griper3->SensorAdress[i]));
            }
        }
        else
        {
            for(int i=0;i<MW->NumberOfSensorsToLogArm1;i++)
            {
                ui->SensorArm1ComboBox->addItem(QString::number(MW->ListOfSensorsPerArms[0][i]));
            }
            for(int i=0;i<MW->NumberOfSensorsToLogArm2;i++)
            {
                ui->SensorArm2ComboBox->addItem(QString::number(MW->ListOfSensorsPerArms[1][i]));
            }
            for(int i=0;i<MW->NumberOfSensorsToLogArm3;i++)
            {
                ui->SensorArm3ComboBox->addItem(QString::number(MW->ListOfSensorsPerArms[2][i]));
            }
        }
        ArmsSensorToVisualize[0]=1;ArmsSensorToVisualize[1]=1;ArmsSensorToVisualize[2]=1;
    }
    if (MW->EnableDatalogging)
    {
        ui->ViewComboBox->setCurrentIndex(MW->DataKind);
        ui->ViewComboBox->setEnabled(false);
    }
    if (MW->DataKind == 0) // Static Data
    {
        ui->resetPushButton->setVisible(true);
        ui->textNoticeLabel->setVisible(false);
        ui->displayValuesCheckbox->setVisible(true);
        ui->CurrentFrequencyLabel->setVisible(false);
        ui->STFACheckbox->setVisible(false);
    }
    else if (MW->DataKind==1) // Dynamic Data
    {
        ui->resetPushButton->setVisible(false);
        ui->textNoticeLabel->setVisible(false);
        ui->displayValuesCheckbox->setVisible(false);
        ui->CurrentFrequencyLabel->setVisible(true);
        ui->STFACheckbox->setVisible(true);
        ui->MaxCountLabel->setVisible(false); ui->MaxCountSpinBox->setVisible(false);
        ui->RotateDownPushButton->setVisible(false); ui->RotateLeftPushButton->setVisible(false);
        ui->RotateRightPushButton->setVisible(false); ui->RotateUpPushButton->setVisible(false);
    }
    else if (MW->DataKind==2) // High Sensitivity Data
    {
        ui->resetPushButton->setVisible(true);
        ui->textNoticeLabel->setVisible(false);
        ui->displayValuesCheckbox->setVisible(false);
        ui->CurrentFrequencyLabel->setVisible(false);
        ui->STFACheckbox->setVisible(false);
    }
    else if (MW->DataKind==3) // All Sensors Data
    {
        ui->resetPushButton->setVisible(true);
        ui->textNoticeLabel->setVisible(false);
        ui->displayValuesCheckbox->setVisible(false);
        ui->Sensor1Label->setVisible(true);ui->Sensor2Label->setVisible(true);ui->Sensor3Label->setVisible(true);ui->Sensor4Label->setVisible(true);ui->Sensor5Label->setVisible(true);
        ui->Sensor6Label->setVisible(true);ui->Sensor7Label->setVisible(true);ui->Sensor8Label->setVisible(true);ui->Sensor9Label->setVisible(true);ui->Sensor10Label->setVisible(true);
        ui->Sensor1Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[0]));ui->Sensor2Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[1]));
        ui->Sensor3Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[2]));ui->Sensor4Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[3]));
        ui->Sensor5Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[4]));ui->Sensor6Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[5]));
        ui->Sensor7Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[6]));ui->Sensor8Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[7]));
        ui->Sensor9Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[7]));ui->Sensor10Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[9]));
        ui->CurrentFrequencyLabel->setVisible(false); ui->STFACheckbox->setVisible(false);
    }
    if(MW->DataKind==1 && MW->EnableDatalogging==true)
    {
        ui->textNoticeLabel->setVisible(true);
    }
    QShortcut *shortcut = new QShortcut(QKeySequence("p"), this);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_FreezePushButton_clicked()));
}

VisualizationForm::~VisualizationForm()
{
    delete ui;
}


void VisualizationForm::PlotData(int TypeOfData)
{
    VisualizationIsLocked=true;
    if (TypeOfData!=3 && ui->Sensor1Label->isVisible()==true)
    {
        ui->Sensor1Label->setVisible(false); ui->Sensor2Label->setVisible(false); ui->Sensor3Label->setVisible(false); ui->Sensor4Label->setVisible(false);
        ui->Sensor5Label->setVisible(false); ui->Sensor6Label->setVisible(false); ui->Sensor7Label->setVisible(false); ui->Sensor8Label->setVisible(false);
        ui->Sensor9Label->setVisible(false); ui->Sensor10Label->setVisible(false);
    }
    if(TypeOfData==0 && DataFreeze==false) // Static Data
    {
        gr->Alpha(false);
        gr->Clf();
        gr->Surf(DataStatic,"#, {B,0}{b,0.17}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 15");
        //gr->Axis();
//        gr->Label('z',"counts",0,"r");
        gr->GetBGRN(buf,4*w*h);
        pic = QPixmap::fromImage(QImage(buf,w,h,QImage::Format_RGB32));
        ui->testLabel->setPixmap(pic);

        if (ui->displayValuesCheckbox->isChecked()==true)
        {
            if (timer.elapsed()>=1000 && NumberOfStaticData!=0 && NumberOfStaticData!=1 && ui->AverageValuesCheckBox->isChecked()==true) // 0.5 second
            {
                SumOfAllTaxelsAvg=round((SumDataStatic[0]/NumberOfStaticData+SumDataStatic[1]/NumberOfStaticData+SumDataStatic[2]/NumberOfStaticData+SumDataStatic[3]/NumberOfStaticData+SumDataStatic[4]/NumberOfStaticData+SumDataStatic[5]/NumberOfStaticData+SumDataStatic[6]/NumberOfStaticData+SumDataStatic[7]/NumberOfStaticData+SumDataStatic[8]/NumberOfStaticData+SumDataStatic[9]/NumberOfStaticData+SumDataStatic[10]/NumberOfStaticData+SumDataStatic[11]/NumberOfStaticData+SumDataStatic[12]/NumberOfStaticData+SumDataStatic[13]/NumberOfStaticData+SumDataStatic[14]/NumberOfStaticData+SumDataStatic[15]/NumberOfStaticData+SumDataStatic[16]/NumberOfStaticData+SumDataStatic[17]/NumberOfStaticData+SumDataStatic[18]/NumberOfStaticData+SumDataStatic[19]/NumberOfStaticData))/20;
                SumOfAllTaxelsStd=sqrt(((NumberOfStaticData*SumOfSquaredDataStatic[0]-pow(SumDataStatic[0],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[1]-pow(SumDataStatic[1],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[2]-pow(SumDataStatic[2],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[3]-pow(SumDataStatic[3],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[4]-pow(SumDataStatic[4],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[5]-pow(SumDataStatic[5],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[6]-pow(SumDataStatic[6],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[7]-pow(SumDataStatic[7],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[8]-pow(SumDataStatic[8],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[9]-pow(SumDataStatic[9],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[10]-pow(SumDataStatic[10],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[11]-pow(SumDataStatic[11],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[12]-pow(SumDataStatic[12],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[13]-pow(SumDataStatic[13],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[14]-pow(SumDataStatic[14],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[15]-pow(SumDataStatic[15],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[16]-pow(SumDataStatic[16],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[17]-pow(SumDataStatic[17],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[18]-pow(SumDataStatic[18],2))/(NumberOfStaticData*(NumberOfStaticData-1)))+((NumberOfStaticData*SumOfSquaredDataStatic[19]-pow(SumDataStatic[19],2))/(NumberOfStaticData*(NumberOfStaticData-1))));
                ui->TaxelVal1->setText(QString::number(round(SumDataStatic[0]/NumberOfStaticData)));ui->TaxelVal2->setText(QString::number(round(SumDataStatic[1]/NumberOfStaticData)));ui->TaxelVal3->setText(QString::number(round(SumDataStatic[2]/NumberOfStaticData)));ui->TaxelVal4->setText(QString::number(round(SumDataStatic[3]/NumberOfStaticData)));
                ui->TaxelVal5->setText(QString::number(round(SumDataStatic[4]/NumberOfStaticData)));ui->TaxelVal6->setText(QString::number(round(SumDataStatic[5]/NumberOfStaticData)));ui->TaxelVal7->setText(QString::number(round(SumDataStatic[6]/NumberOfStaticData)));ui->TaxelVal8->setText(QString::number(round(SumDataStatic[7]/NumberOfStaticData)));
                ui->TaxelVal9->setText(QString::number(round(SumDataStatic[8]/NumberOfStaticData)));ui->TaxelVal10->setText(QString::number(round(SumDataStatic[9]/NumberOfStaticData)));ui->TaxelVal11->setText(QString::number(round(SumDataStatic[10]/NumberOfStaticData)));ui->TaxelVal12->setText(QString::number(round(SumDataStatic[11]/NumberOfStaticData)));
                ui->TaxelVal13->setText(QString::number(round(SumDataStatic[12]/NumberOfStaticData)));ui->TaxelVal14->setText(QString::number(round(SumDataStatic[13]/NumberOfStaticData)));ui->TaxelVal15->setText(QString::number(round(SumDataStatic[14]/NumberOfStaticData)));ui->TaxelVal16->setText(QString::number(round(SumDataStatic[15]/NumberOfStaticData)));
                ui->TaxelVal17->setText(QString::number(round(SumDataStatic[16]/NumberOfStaticData)));ui->TaxelVal18->setText(QString::number(round(SumDataStatic[17]/NumberOfStaticData)));ui->TaxelVal19->setText(QString::number(round(SumDataStatic[18]/NumberOfStaticData)));ui->TaxelVal20->setText(QString::number(round(SumDataStatic[19]/NumberOfStaticData)));
                ui->STDVal1->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[0]-pow(SumDataStatic[0],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal2->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[1]-pow(SumDataStatic[1],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal3->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[2]-pow(SumDataStatic[2],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal4->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[3]-pow(SumDataStatic[3],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal5->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[4]-pow(SumDataStatic[4],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal6->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[5]-pow(SumDataStatic[5],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal7->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[6]-pow(SumDataStatic[6],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal8->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[7]-pow(SumDataStatic[7],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal9->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[8]-pow(SumDataStatic[8],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal10->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[9]-pow(SumDataStatic[9],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal11->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[10]-pow(SumDataStatic[10],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal12->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[11]-pow(SumDataStatic[11],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal13->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[12]-pow(SumDataStatic[12],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal14->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[13]-pow(SumDataStatic[13],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal15->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[14]-pow(SumDataStatic[14],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal16->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[15]-pow(SumDataStatic[15],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal17->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[16]-pow(SumDataStatic[16],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal18->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[17]-pow(SumDataStatic[17],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal19->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[18]-pow(SumDataStatic[18],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal20->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[19]-pow(SumDataStatic[19],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->SumOfAllTaxelsAvgValueLabel->setText(QString::number(SumOfAllTaxelsAvg)); ui->SumOfAllTaxelsStdValueLabel->setText(QString::number(SumOfAllTaxelsStd));
                SumDataStatic[0]=0;SumDataStatic[1]=0;SumDataStatic[2]=0;SumDataStatic[3]=0;SumDataStatic[4]=0;SumDataStatic[5]=0;
                SumDataStatic[6]=0;SumDataStatic[7]=0;SumDataStatic[8]=0;SumDataStatic[9]=0;SumDataStatic[10]=0;SumDataStatic[11]=0;
                SumDataStatic[12]=0;SumDataStatic[13]=0;SumDataStatic[14]=0;SumDataStatic[15]=0;SumDataStatic[16]=0;SumDataStatic[17]=0;
                SumDataStatic[18]=0;SumDataStatic[19]=0;
                SumOfSquaredDataStatic[0]=0;SumOfSquaredDataStatic[1]=0;SumOfSquaredDataStatic[2]=0;SumOfSquaredDataStatic[3]=0;SumOfSquaredDataStatic[4]=0;SumOfSquaredDataStatic[5]=0;
                SumOfSquaredDataStatic[6]=0;SumOfSquaredDataStatic[7]=0;SumOfSquaredDataStatic[8]=0;SumOfSquaredDataStatic[9]=0;SumOfSquaredDataStatic[10]=0;SumOfSquaredDataStatic[11]=0;
                SumOfSquaredDataStatic[12]=0;SumOfSquaredDataStatic[13]=0;SumOfSquaredDataStatic[14]=0;SumOfSquaredDataStatic[15]=0;SumOfSquaredDataStatic[16]=0;SumOfSquaredDataStatic[17]=0;
                SumOfSquaredDataStatic[18]=0;SumOfSquaredDataStatic[19]=0;
                NumberOfStaticData=0; timer.restart();
            }
            else if (ui->AverageValuesCheckBox->isChecked()==false)
            {
                ui->TaxelVal1->setText(QString::number(DataTable[0]));ui->TaxelVal2->setText(QString::number(DataTable[1]));ui->TaxelVal3->setText(QString::number(DataTable[2]));ui->TaxelVal4->setText(QString::number(DataTable[3]));
                ui->TaxelVal5->setText(QString::number(DataTable[4]));ui->TaxelVal6->setText(QString::number(DataTable[5]));ui->TaxelVal7->setText(QString::number(DataTable[6]));ui->TaxelVal8->setText(QString::number(DataTable[7]));
                ui->TaxelVal9->setText(QString::number(DataTable[8]));ui->TaxelVal10->setText(QString::number(DataTable[9]));ui->TaxelVal11->setText(QString::number(DataTable[10]));ui->TaxelVal12->setText(QString::number(DataTable[11]));
                ui->TaxelVal13->setText(QString::number(DataTable[12]));ui->TaxelVal14->setText(QString::number(DataTable[13]));ui->TaxelVal15->setText(QString::number(DataTable[14]));ui->TaxelVal16->setText(QString::number(DataTable[15]));
                ui->TaxelVal17->setText(QString::number(DataTable[16]));ui->TaxelVal18->setText(QString::number(DataTable[17]));ui->TaxelVal19->setText(QString::number(DataTable[18]));ui->TaxelVal20->setText(QString::number(DataTable[19]));
                ui->STDVal1->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[0]-pow(SumDataStatic[0],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal2->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[1]-pow(SumDataStatic[1],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal3->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[2]-pow(SumDataStatic[2],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal4->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[3]-pow(SumDataStatic[3],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal5->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[4]-pow(SumDataStatic[4],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal6->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[5]-pow(SumDataStatic[5],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal7->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[6]-pow(SumDataStatic[6],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal8->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[7]-pow(SumDataStatic[7],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal9->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[8]-pow(SumDataStatic[8],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal10->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[9]-pow(SumDataStatic[9],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal11->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[10]-pow(SumDataStatic[10],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal12->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[11]-pow(SumDataStatic[11],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal13->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[12]-pow(SumDataStatic[12],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal14->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[13]-pow(SumDataStatic[13],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal15->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[14]-pow(SumDataStatic[14],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal16->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[15]-pow(SumDataStatic[15],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal17->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[16]-pow(SumDataStatic[16],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal18->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[17]-pow(SumDataStatic[17],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal19->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[18]-pow(SumDataStatic[18],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->STDVal20->setText(QString::number(sqrt((NumberOfStaticData*SumOfSquaredDataStatic[19]-pow(SumDataStatic[19],2))/(NumberOfStaticData*(NumberOfStaticData-1)))));
                ui->SumOfAllTaxelsAvgValueLabel->setText(QString::number(round((DataTable[0]+DataTable[1]+DataTable[2]+DataTable[3]+DataTable[4]+DataTable[5]+DataTable[6]+DataTable[7]+DataTable[8]+DataTable[9]+DataTable[10]+DataTable[11]+DataTable[12]+DataTable[13]+DataTable[14]+DataTable[15]+DataTable[16]+DataTable[17]+DataTable[18]+DataTable[19])/20)));
            }
            else
            {
                SumDataStatic[0]+=DataTable[0];SumDataStatic[1]+=DataTable[1];SumDataStatic[2]+=DataTable[2];SumDataStatic[3]+=DataTable[3];SumDataStatic[4]+=DataTable[4];SumDataStatic[5]+=DataTable[5];
                SumDataStatic[6]+=DataTable[6];SumDataStatic[7]+=DataTable[7];SumDataStatic[8]+=DataTable[8];SumDataStatic[9]+=DataTable[9];SumDataStatic[10]+=DataTable[10];SumDataStatic[11]+=DataTable[11];
                SumDataStatic[12]+=DataTable[12];SumDataStatic[13]+=DataTable[13];SumDataStatic[14]+=DataTable[14];SumDataStatic[15]+=DataTable[15];SumDataStatic[16]+=DataTable[16];SumDataStatic[17]+=DataTable[17];
                SumDataStatic[18]+=DataTable[18];SumDataStatic[19]+=DataTable[19];
                SumOfSquaredDataStatic[0]+= pow(DataTable[0],2);SumOfSquaredDataStatic[1]+= pow(DataTable[1],2);SumOfSquaredDataStatic[2]+= pow(DataTable[2],2);SumOfSquaredDataStatic[3]+= pow(DataTable[3],2);
                SumOfSquaredDataStatic[4]+= pow(DataTable[4],2);SumOfSquaredDataStatic[5]+= pow(DataTable[5],2);SumOfSquaredDataStatic[6]+= pow(DataTable[6],2);SumOfSquaredDataStatic[7]+= pow(DataTable[7],2);
                SumOfSquaredDataStatic[8]+= pow(DataTable[8],2);SumOfSquaredDataStatic[9]+= pow(DataTable[9],2);SumOfSquaredDataStatic[10]+= pow(DataTable[10],2);SumOfSquaredDataStatic[11]+= pow(DataTable[11],2);
                SumOfSquaredDataStatic[12]+= pow(DataTable[12],2);SumOfSquaredDataStatic[13]+= pow(DataTable[13],2);SumOfSquaredDataStatic[14]+= pow(DataTable[14],2);SumOfSquaredDataStatic[15]+= pow(DataTable[15],2);
                SumOfSquaredDataStatic[16]+= pow(DataTable[16],2);SumOfSquaredDataStatic[17]+= pow(DataTable[17],2);SumOfSquaredDataStatic[18]+= pow(DataTable[18],2);SumOfSquaredDataStatic[19]+= pow(DataTable[19],2);
                NumberOfStaticData++;
            }
        }
        qApp->processEvents();
    }
    else if(TypeOfData==1 && DataFreeze==false) // Dynamic Data
    {
        if (DynamicDataViewMode==0)
        {
            grDyn->Clf();
            grDyn->Axis();
            grDyn->Label('y',"mV",0);
            grDyn->Label('x',"sec",0);
            grDyn->Plot(DataTime,DataDynamicResized);
            grDyn->GetBGRN(buf,4*w*h);
            pic = QPixmap::fromImage(QImage(buf,w,h,QImage::Format_RGB32));
            ui->testLabel->setPixmap(pic);
            ui->CurrentFrequencyLabel->setText("Estimated sampling frequency: " + QString::number(CurrentFrequency) + "Hz");
            qApp->processEvents();
        }
        else if (DynamicDataViewMode==1) // Fast Fourier Transform is displayed
        {
            grDyn->Clf();grDyn2->Clf();
            grDyn2->Axis();
            grDyn2->Label('y',"mV",0);
            grDyn2->Label('x',"sec",0);
            grDyn2->Plot(DataTime,DataDynamicResized);
            grDyn2->GetBGRN(buf,4*w*250);
            pic = QPixmap::fromImage(QImage(buf,w,250,QImage::Format_RGB32));
            ui->testLabel->setPixmap(pic);

            if (STFAIsReady==true)
            {
                grFourier->Clf();
                if (FourierCount>=800)
                {
                    fftw_execute(FFTplan);
                    for (int h=0; h<2048;h++)
                    {
                        DataDynamicFourier[h] = sqrt(pow(FFTout[h][0],2)+pow(FFTout[h][1],2)); // This measures the amplitude of the Fourier Transform, for each frequency
                    }
                    val3[0]=512; val3[1]=1024; val3[2]=1536; val3[3]=2048;
                    grFourier->SetRanges(0,2048,0,20);
                    grFourier->SetTicksVal('x',mglData(4,val3),"\\125\n\\250\n\\375\n\\500");
                    grFourier->Plot(DataDynamicFourier);

                    if (DataDynamicFourier.Maximal()>20)
                    {
                        grFourier->SetRange('y',0,DataDynamicFourier.Maximal()*1.5);
                    }
                    else
                    {
                        grFourier->SetRange('y',0,20);
                    }
                }
                else if (FourierCount>=400 && FourierCount<800)
                {
                    fftw_execute(FFTplan2);
                    for (int h=0; h<1024;h++)
                    {
                        DataDynamicFourier2[h] = sqrt(pow(FFTout2[h][0],2)+pow(FFTout2[h][1],2)); // This measures the amplitude of the Fourier Transform, for each frequency
                    }
                    val3[0]=256; val3[1]=512; val3[2]=768; val3[3]=1024;
                    grFourier->SetTicksVal('x',mglData(4,val3),"\\62.5\n\\125\n\\187.5\n\\250");
                    grFourier->SetRanges(0,1024,0,20);
                    grFourier->Plot(DataDynamicFourier2);
                    if (DataDynamicFourier2.Maximal()>20)
                    {
                        grFourier->SetRange('y',0,DataDynamicFourier2.Maximal());
                    }
                    else
                    {
                        grFourier->SetRange('y',0,20);
                    }
                }
                else if (FourierCount>=200 && FourierCount<400)
                {
                    fftw_execute(FFTplan3);
                    for (int h=0; h<512;h++)
                    {
                        DataDynamicFourier3[h] = sqrt(pow(FFTout3[h][0],2)+pow(FFTout3[h][1],2)); // This measures the amplitude of the Fourier Transform, for each frequency
                    }
                    val3[0]=128; val3[1]=256; val3[2]=384; val3[3]=512;
                    grFourier->SetRanges(0,512,0,20);
                    grFourier->SetTicksVal('x',mglData(4,val3),"\\31.25\n\\62.5\n\\93.75\n\\125");
                    grFourier->Plot(DataDynamicFourier3);

                    if (DataDynamicFourier3.Maximal()>20)
                    {
                        grFourier->SetRange('y',0,DataDynamicFourier3.Maximal());
                    }
                    else
                    {
                        grFourier->SetRange('y',0,20);
                    }
                }
                else if (FourierCount>=100 && FourierCount<200)
                {
                    fftw_execute(FFTplan4);
                    for (int h=0; h<256;h++)
                    {
                        DataDynamicFourier4[h] = sqrt(pow(FFTout4[h][0],2)+pow(FFTout4[h][1],2)); // This measures the amplitude of the Fourier Transform, for each frequency
                    }
                    val3[0]=64; val3[1]=128; val3[2]=192; val3[3]=256;
                    grFourier->SetRanges(0,256,0,20);
                    grFourier->SetTicksVal('x',mglData(4,val3),"\\15.625\n\\31.25\n\\46.875\n\\62.5");
                    grFourier->Plot(DataDynamicFourier4);

                    if (DataDynamicFourier4.Maximal()>20)
                    {
                        grFourier->SetRange('y',0,DataDynamicFourier4.Maximal());
                    }
                    else
                    {
                        grFourier->SetRange('y',0,20);
                    }
                }

                grFourier->Axis();
                grFourier->Label('x', "Hertz",0);
                grFourier->Label('y', "Amplitude", 0);
                grFourier->GetBGRN(buf2,4*w*250);
                pic2 = QPixmap::fromImage(QImage(buf2,w,250,QImage::Format_RGB32));
                ui->testLabel_2->setPixmap(pic2);

                STFAIsReady=false;
            }
            ui->CurrentFrequencyLabel->setText("Estimated sampling frequency: " + QString::number(CurrentFrequency) + "Hz");
            qApp->processEvents();
        }
    }
    else if(TypeOfData==2 && DataFreeze==false) // High Sensitivity Data
    {
        grHigh->Clf();
        grHigh->Area(DataHighSensitivity,"g");
        grHigh->Axis();
        grHigh->Label('y',"Sensitive counts",0);
        grHigh->Box();
        grHigh->SetTicks('x',1,1);
        grHigh->SetTicksVal('x', mglData(2,val), "\\ \n\\ ");
        grHigh->GetBGRN(buf,4*w*h);
        pic = QPixmap::fromImage(QImage(buf,w,h,QImage::Format_RGB32));
        ui->testLabel->setPixmap(pic);
        qApp->processEvents();
    }
    else if(TypeOfData==3 && DataFreeze==false) // All Sensors Data
    {
        if (AllSensorsFirstTime==true)
        {
            ui->Sensor1Label->setVisible(true);ui->Sensor2Label->setVisible(true);ui->Sensor3Label->setVisible(true);ui->Sensor4Label->setVisible(true);ui->Sensor5Label->setVisible(true);
            ui->Sensor6Label->setVisible(true);ui->Sensor7Label->setVisible(true);ui->Sensor8Label->setVisible(true);ui->Sensor9Label->setVisible(true);ui->Sensor10Label->setVisible(true);
            AllSensorsFirstTime=false;
        }

        grAll->NewFrame();

        //Seventh subplot:
        grAll->InPlot(-0.1,0.15,0.8,1.05,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(0,240);
        grAll->Surf(DataStaticAllSensor[6],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //Eighth subplot:
        grAll->InPlot(0,0.25,0.45,0.70,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(0,240);
        grAll->Surf(DataStaticAllSensor[7],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //Ninth subplot:
        grAll->InPlot(0.1,0.35,0.10,0.35,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(0,240);
        grAll->Surf(DataStaticAllSensor[8],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //First subplot:
        grAll->InPlot(0.38,0.63,0.94,1.19,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(180,90);
        grAll->Surf(DataStaticAllSensor[0],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //Second subplot:
        grAll->InPlot(0.38,0.63,0.54,0.79,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(180,90);
        grAll->Surf(DataStaticAllSensor[1],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //Third subplot:
        grAll->InPlot(0.38,0.63,0.14,0.39,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(180,90);
        grAll->Surf(DataStaticAllSensor[2],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //Fourth subplot:
        grAll->InPlot(0.86,1.11,0.80,1.05,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(0,300);
        grAll->Surf(DataStaticAllSensor[3],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //Fifth subplot:
        grAll->InPlot(0.76,1.01,0.45,0.70,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(0,300);
        grAll->Surf(DataStaticAllSensor[4],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //Sixth subplot:
        grAll->InPlot(0.66,0.91,0.1,0.35,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(1,2,1);
        grAll->Rotate(0,300);
        grAll->Surf(DataStaticAllSensor[5],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        grAll->InPlot(0.38,0.63,-0.23,0.02,true);
        grAll->SetRanges(0,3,0,4,-1500,11000);
        grAll->Aspect(0.5,0.5,0.6);
        grAll->Rotate(0,0);
        grAll->Surf(DataStaticAllSensor[9],"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        //Eleventh subplot (Dummy plot to avoid Mathgl lib's bug):
        grAll->InPlot(-10,-9.99,-10,-9.99,true);
        grAll->Rotate(180,30);
        grAll->Light(true);
        grAll->Surf(DataStatic,"#, {B,0}{b,0.14}{c,0.25}{y,0.35}{r,0.55}{R,0.85}", "meshnum 4");

        grAll->GetBGRN(buf,4*w*h);
        pic = QPixmap::fromImage(QImage(buf,w,h,QImage::Format_RGB32));
        ui->testLabel->setPixmap(pic);
        qApp->processEvents();
    }
    VisualizationIsLocked=false;
}

void VisualizationForm::closeEvent(QCloseEvent *event)
{
    closed=1;
    emit VFClosed();
    event->accept();
    MW->DataKind=0; // Default Data Type is Static
}

void VisualizationForm::CloseVF()
{
    close();
    closed=1;
    emit VFClosed();
}

void VisualizationForm::on_ViewComboBox_currentIndexChanged(int index)
{
    DynamicDataViewMode=0; ui->STFACheckbox->setChecked(false); ui->testLabel_2->setVisible(false);
    ui->testLabel->move(175,45);
    MW->DataKind=index;
    AllSensorsFirstTime=true;
    if (index ==1 && MW->EnableDatalogging==true)
    {
        ui->textNoticeLabel->setVisible(true);
        ui->CurrentFrequencyLabel->setVisible(true);
        ui->STFACheckbox->setVisible(true);
    }
    if (index ==0 || index==2 || index==3)
    {
        ui->resetPushButton->setVisible(true);
        ui->CurrentFrequencyLabel->setVisible(false);
        ui->STFACheckbox->setVisible(false);
    }
    else
    {
        ui->resetPushButton->setVisible(false);
        ui->CurrentFrequencyLabel->setVisible(true);
        ui->STFACheckbox->setVisible(true);
    }
    if (index==0)
    {
        ui->MaxCountLabel->setVisible(true);ui->MaxCountSpinBox->setVisible(true);
        MW->GetLoggingAndVisualizationInfo();
        MW->NumberOfSensorsToLogArm1=1;
        CurrentSensorToVisualize=MW->ListOfSensorsPerArms[0][0];
        CurrentArmToVisualize=0; ui->visualizeArm1CheckBox->setChecked(true); ui->visualizeArm2CheckBox->setChecked(false); ui->visualizeArm3CheckBox->setChecked(false);
        ui->SensorArm1ComboBox->setCurrentIndex(0);
        ui->displayValuesCheckbox->setVisible(true);
        ui->CurrentFrequencyLabel->setVisible(false);
        ui->STFACheckbox->setVisible(false);
        ui->RotateDownPushButton->setVisible(true); ui->RotateLeftPushButton->setVisible(true); ui->RotateRightPushButton->setVisible(true); ui->RotateUpPushButton->setVisible(true);
    }
    else if(index==3)
    {
        ui->MaxCountLabel->setVisible(false);ui->MaxCountSpinBox->setVisible(false);
        ui->RotateDownPushButton->setVisible(false);ui->RotateLeftPushButton->setVisible(false); ui->RotateRightPushButton->setVisible(false); ui->RotateUpPushButton->setVisible(false);
        CurrentArmToVisualize=0;ui->visualizeArm1CheckBox->setChecked(true); ui->visualizeArm2CheckBox->setChecked(false); ui->visualizeArm3CheckBox->setChecked(false);
        MW->NumberOfSensorsToLogArm1=10; MW->NumberOfSensorsToLogArm2=10; MW->NumberOfSensorsToLogArm3=10;
        MW->ListOfSensorsPerArms[0][0]=MW->Griper1->SensorAdress[0];MW->ListOfSensorsPerArms[0][1]=MW->Griper1->SensorAdress[1];MW->ListOfSensorsPerArms[0][2]=MW->Griper1->SensorAdress[2];MW->ListOfSensorsPerArms[0][3]=MW->Griper1->SensorAdress[3];
        MW->ListOfSensorsPerArms[0][4]=MW->Griper1->SensorAdress[4];MW->ListOfSensorsPerArms[0][5]=MW->Griper1->SensorAdress[5];MW->ListOfSensorsPerArms[0][6]=MW->Griper1->SensorAdress[6];MW->ListOfSensorsPerArms[0][7]=MW->Griper1->SensorAdress[7];
        MW->ListOfSensorsPerArms[0][8]=MW->Griper1->SensorAdress[8];MW->ListOfSensorsPerArms[0][9]=MW->Griper1->SensorAdress[9];
        MW->ListOfSensorsPerArms[1][0]=MW->Griper2->SensorAdress[0];MW->ListOfSensorsPerArms[1][1]=MW->Griper2->SensorAdress[1];MW->ListOfSensorsPerArms[1][2]=MW->Griper2->SensorAdress[2];MW->ListOfSensorsPerArms[1][3]=MW->Griper2->SensorAdress[3];
        MW->ListOfSensorsPerArms[1][4]=MW->Griper2->SensorAdress[4];MW->ListOfSensorsPerArms[1][5]=MW->Griper2->SensorAdress[5];MW->ListOfSensorsPerArms[1][6]=MW->Griper2->SensorAdress[6];MW->ListOfSensorsPerArms[1][7]=MW->Griper2->SensorAdress[7];
        MW->ListOfSensorsPerArms[1][8]=MW->Griper2->SensorAdress[8];MW->ListOfSensorsPerArms[1][9]=MW->Griper2->SensorAdress[9];
        MW->ListOfSensorsPerArms[2][0]=MW->Griper3->SensorAdress[0];MW->ListOfSensorsPerArms[2][1]=MW->Griper3->SensorAdress[1];MW->ListOfSensorsPerArms[2][2]=MW->Griper3->SensorAdress[2];MW->ListOfSensorsPerArms[2][3]=MW->Griper3->SensorAdress[3];
        MW->ListOfSensorsPerArms[2][4]=MW->Griper3->SensorAdress[4];MW->ListOfSensorsPerArms[2][5]=MW->Griper3->SensorAdress[5];MW->ListOfSensorsPerArms[2][6]=MW->Griper3->SensorAdress[6];MW->ListOfSensorsPerArms[2][7]=MW->Griper3->SensorAdress[7];
        MW->ListOfSensorsPerArms[2][8]=MW->Griper3->SensorAdress[8];MW->ListOfSensorsPerArms[2][9]=MW->Griper3->SensorAdress[9];
        ui->displayValuesCheckbox->setVisible(false);ui->displayValuesCheckbox->setChecked(false);
        ui->TaxelVal1->setVisible(false);ui->TaxelVal2->setVisible(false);ui->TaxelVal3->setVisible(false);ui->TaxelVal4->setVisible(false);
        ui->TaxelVal5->setVisible(false);ui->TaxelVal6->setVisible(false);ui->TaxelVal7->setVisible(false);ui->TaxelVal8->setVisible(false);
        ui->TaxelVal9->setVisible(false);ui->TaxelVal10->setVisible(false);ui->TaxelVal11->setVisible(false);ui->TaxelVal12->setVisible(false);
        ui->TaxelVal13->setVisible(false);ui->TaxelVal14->setVisible(false);ui->TaxelVal15->setVisible(false);ui->TaxelVal16->setVisible(false);
        ui->TaxelVal17->setVisible(false);ui->TaxelVal18->setVisible(false);ui->TaxelVal19->setVisible(false);ui->TaxelVal20->setVisible(false);
        ui->LineTableDown->setVisible(false);ui->lineTableUp->setVisible(false);ui->LineTableLeft->setVisible(false);ui->LineTableRight->setVisible(false);
        ui->STDVal1->setVisible(false);ui->STDVal2->setVisible(false);ui->STDVal3->setVisible(false);ui->STDVal4->setVisible(false);ui->STDVal5->setVisible(false);ui->STDVal6->setVisible(false);
        ui->STDVal7->setVisible(false);ui->STDVal8->setVisible(false);ui->STDVal9->setVisible(false);ui->STDVal10->setVisible(false);ui->STDVal11->setVisible(false);ui->STDVal12->setVisible(false);
        ui->STDVal13->setVisible(false);ui->STDVal14->setVisible(false);ui->STDVal15->setVisible(false);ui->STDVal16->setVisible(false);ui->STDVal17->setVisible(false);ui->STDVal18->setVisible(false);
        ui->STDVal19->setVisible(false);ui->STDVal20->setVisible(false);
        ui->StandardDeviationLabel->setVisible(false); ui->AverageValuesCheckBox->setVisible(false); ui->LineTableDown2->setVisible(false); ui->LineTableLeft2->setVisible(false); ui->LineTableRight2->setVisible(false); ui->lineTableUp2->setVisible(false);
        ui->FreezePushButton->setVisible(true); ui->SumOfAllTaxelsAvgLabel->setVisible(false); ui->SumOfAllTaxelsAvgValueLabel->setVisible(false); ui->SumOfAllTaxelsStdLabel->setVisible(false); ui->SumOfAllTaxelsStdValueLabel->setVisible(false);
        ui->AverageValuesCheckBox->setChecked(false);
    }
    else
    {
        ui->RotateDownPushButton->setVisible(false); ui->RotateLeftPushButton->setVisible(false); ui->RotateRightPushButton->setVisible(false); ui->RotateUpPushButton->setVisible(false);
        ui->MaxCountLabel->setVisible(false);ui->MaxCountSpinBox->setVisible(false);
        MW->GetLoggingAndVisualizationInfo();
        MW->NumberOfSensorsToLogArm1=1;
        CurrentSensorToVisualize=MW->ListOfSensorsPerArms[0][0];
        CurrentArmToVisualize=0;ui->visualizeArm1CheckBox->setChecked(true); ui->visualizeArm2CheckBox->setChecked(false); ui->visualizeArm3CheckBox->setChecked(false);
        ui->SensorArm1ComboBox->setCurrentIndex(0);
        ui->displayValuesCheckbox->setVisible(false); ui->displayValuesCheckbox->setChecked(false);
        ui->TaxelVal1->setVisible(false);ui->TaxelVal2->setVisible(false);ui->TaxelVal3->setVisible(false);ui->TaxelVal4->setVisible(false);
        ui->TaxelVal5->setVisible(false);ui->TaxelVal6->setVisible(false);ui->TaxelVal7->setVisible(false);ui->TaxelVal8->setVisible(false);
        ui->TaxelVal9->setVisible(false);ui->TaxelVal10->setVisible(false);ui->TaxelVal11->setVisible(false);ui->TaxelVal12->setVisible(false);
        ui->TaxelVal13->setVisible(false);ui->TaxelVal14->setVisible(false);ui->TaxelVal15->setVisible(false);ui->TaxelVal16->setVisible(false);
        ui->TaxelVal17->setVisible(false);ui->TaxelVal18->setVisible(false);ui->TaxelVal19->setVisible(false);ui->TaxelVal20->setVisible(false);
        ui->LineTableDown->setVisible(false);ui->lineTableUp->setVisible(false);ui->LineTableLeft->setVisible(false);ui->LineTableRight->setVisible(false);
        ui->STDVal1->setVisible(false);ui->STDVal2->setVisible(false);ui->STDVal3->setVisible(false);ui->STDVal4->setVisible(false);ui->STDVal5->setVisible(false);ui->STDVal6->setVisible(false);
        ui->STDVal7->setVisible(false);ui->STDVal8->setVisible(false);ui->STDVal9->setVisible(false);ui->STDVal10->setVisible(false);ui->STDVal11->setVisible(false);ui->STDVal12->setVisible(false);
        ui->STDVal13->setVisible(false);ui->STDVal14->setVisible(false);ui->STDVal15->setVisible(false);ui->STDVal16->setVisible(false);ui->STDVal17->setVisible(false);ui->STDVal18->setVisible(false);
        ui->STDVal19->setVisible(false);ui->STDVal20->setVisible(false);

        ui->StandardDeviationLabel->setVisible(false); ui->AverageValuesCheckBox->setVisible(false); ui->LineTableDown2->setVisible(false); ui->LineTableLeft2->setVisible(false); ui->LineTableRight2->setVisible(false); ui->lineTableUp2->setVisible(false);
        ui->FreezePushButton->setVisible(true); ui->SumOfAllTaxelsAvgLabel->setVisible(false); ui->SumOfAllTaxelsAvgValueLabel->setVisible(false); ui->SumOfAllTaxelsStdLabel->setVisible(false); ui->SumOfAllTaxelsStdValueLabel->setVisible(false);
        ui->AverageValuesCheckBox->setChecked(false);
    }

    if (index==0 || index==1) // Static or Dynamic Data Kind
    {
        if(MW->NumberOfArms==1)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->SensorArm1ComboBox->setVisible(true);
            ui->sensorArm1Label->setVisible(true);
            ui->arm1Label->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(false);
            ui->SensorArm2ComboBox->setVisible(false);
            ui->sensorArm2Label->setVisible(false);
            ui->arm2Label->setVisible(false);
            ui->visualizeArm3CheckBox->setVisible(false);
            ui->SensorArm3ComboBox->setVisible(false);
            ui->sensorArm3Label->setVisible(false);
            ui->arm3Label->setVisible(false);
        }
        else if(MW->NumberOfArms==2)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->SensorArm1ComboBox->setVisible(true);
            ui->sensorArm1Label->setVisible(true);
            ui->arm1Label->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(true);
            ui->SensorArm2ComboBox->setVisible(true);
            ui->sensorArm2Label->setVisible(true);
            ui->arm2Label->setVisible(true);
            ui->visualizeArm3CheckBox->setVisible(false);
            ui->SensorArm3ComboBox->setVisible(false);
            ui->sensorArm3Label->setVisible(false);
            ui->arm3Label->setVisible(false);
        }
        else if (MW->NumberOfArms==3)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->SensorArm1ComboBox->setVisible(true);
            ui->sensorArm1Label->setVisible(true);
            ui->arm1Label->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(true);
            ui->SensorArm2ComboBox->setVisible(true);
            ui->sensorArm2Label->setVisible(true);
            ui->arm2Label->setVisible(true);
            ui->visualizeArm3CheckBox->setVisible(true);
            ui->SensorArm3ComboBox->setVisible(true);
            ui->sensorArm3Label->setVisible(true);
            ui->arm3Label->setVisible(true);
        }
        ui->Sensor1Label->setVisible(false);ui->Sensor2Label->setVisible(false);ui->Sensor3Label->setVisible(false);ui->Sensor4Label->setVisible(false);ui->Sensor5Label->setVisible(false);
        ui->Sensor6Label->setVisible(false);ui->Sensor7Label->setVisible(false);ui->Sensor8Label->setVisible(false);ui->Sensor9Label->setVisible(false);ui->Sensor10Label->setVisible(false);
    }
    else if (index==2) // High Sensitivity Data Kind
    {
        if(MW->NumberOfArms==1)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->SensorArm1ComboBox->setVisible(true);
            ui->sensorArm1Label->setVisible(true);
            ui->arm1Label->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(false);
            ui->SensorArm2ComboBox->setVisible(false);
            ui->sensorArm2Label->setVisible(false);
            ui->arm2Label->setVisible(false);
            ui->visualizeArm3CheckBox->setVisible(false);
            ui->SensorArm3ComboBox->setVisible(false);
            ui->sensorArm3Label->setVisible(false);
            ui->arm3Label->setVisible(false);
        }
        else if (MW->NumberOfArms==2)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->SensorArm1ComboBox->setVisible(true);
            ui->sensorArm1Label->setVisible(true);
            ui->arm1Label->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(true);
            ui->SensorArm2ComboBox->setVisible(true);
            ui->sensorArm2Label->setVisible(true);
            ui->arm2Label->setVisible(true);
            ui->visualizeArm3CheckBox->setVisible(false);
            ui->SensorArm3ComboBox->setVisible(false);
            ui->sensorArm3Label->setVisible(false);
            ui->arm3Label->setVisible(false);
        }
        else if (MW->NumberOfArms==3)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->SensorArm1ComboBox->setVisible(true);
            ui->sensorArm1Label->setVisible(true);
            ui->arm1Label->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(true);
            ui->SensorArm2ComboBox->setVisible(true);
            ui->sensorArm2Label->setVisible(true);
            ui->arm2Label->setVisible(true);
            ui->visualizeArm3CheckBox->setVisible(true);
            ui->SensorArm3ComboBox->setVisible(true);
            ui->sensorArm3Label->setVisible(true);
            ui->arm3Label->setVisible(true);
        }
        ui->Sensor1Label->setVisible(false);ui->Sensor2Label->setVisible(false);ui->Sensor3Label->setVisible(false);ui->Sensor4Label->setVisible(false);ui->Sensor5Label->setVisible(false);
        ui->Sensor6Label->setVisible(false);ui->Sensor7Label->setVisible(false);ui->Sensor8Label->setVisible(false);ui->Sensor9Label->setVisible(false);ui->Sensor10Label->setVisible(false);
    }
    else if (index==3) //All sensors static Data kind
    {
        if (MW->NumberOfArms==1)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(false);
            ui->visualizeArm3CheckBox->setVisible(false);
        }
        else if(MW->NumberOfArms==2)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(true);
            ui->visualizeArm3CheckBox->setVisible(false);
        }
        else if(MW->NumberOfArms==3)
        {
            ui->visualizeArm1CheckBox->setVisible(true);
            ui->visualizeArm2CheckBox->setVisible(true);
            ui->visualizeArm3CheckBox->setVisible(true);
        }
        ui->SensorArm1ComboBox->setVisible(false);
        ui->SensorArm2ComboBox->setVisible(false);
        ui->SensorArm3ComboBox->setVisible(false);
        ui->sensorArm1Label->setVisible(false);
        ui->sensorArm2Label->setVisible(false);
        ui->sensorArm3Label->setVisible(false);
        if (ui->visualizeArm1CheckBox->isChecked()==true)
        {
            ui->Sensor1Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[0]));ui->Sensor2Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[1]));
            ui->Sensor3Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[2]));ui->Sensor4Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[3]));
            ui->Sensor5Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[4]));ui->Sensor6Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[5]));
            ui->Sensor7Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[6]));ui->Sensor8Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[7]));
            ui->Sensor9Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[8]));ui->Sensor10Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[9]));
        }
        else if (ui->visualizeArm2CheckBox->isChecked()==true)
        {
            ui->Sensor1Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[0]));ui->Sensor2Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[1]));
            ui->Sensor3Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[2]));ui->Sensor4Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[3]));
            ui->Sensor5Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[4]));ui->Sensor6Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[5]));
            ui->Sensor7Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[6]));ui->Sensor8Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[7]));
            ui->Sensor9Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[8]));ui->Sensor10Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[9]));
        }
        else if (ui->visualizeArm3CheckBox->isChecked()==true)
        {
            ui->Sensor1Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[0]));ui->Sensor2Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[1]));
            ui->Sensor3Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[2]));ui->Sensor4Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[3]));
            ui->Sensor5Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[4]));ui->Sensor6Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[5]));
            ui->Sensor7Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[6]));ui->Sensor8Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[7]));
            ui->Sensor9Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[8]));ui->Sensor10Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[9]));
        }
    }
    if (index==1)
    {
        ui->CurrentFrequencyLabel->setVisible(true);
        ui->STFACheckbox->setVisible(true);
        ui->STFACheckbox->setChecked(true);
        this->on_STFACheckbox_clicked(true);
    }

    DataFreeze=false;
    ui->FreezePushButton->setText("Pause");
    ui->ExportGraphPushButton->setDisabled(true);
}

void VisualizationForm::on_resetPushButton_clicked()
{
    ResetButtonIsPushed=true;
}

void VisualizationForm::on_visualizeArm1CheckBox_clicked(bool checked)
{
    if (checked==true)
    {
        ui->visualizeArm1CheckBox->setChecked(true);
        ui->visualizeArm2CheckBox->setChecked(false);
        ui->visualizeArm3CheckBox->setChecked(false);
        ui->SensorArm1ComboBox->setEnabled(true);
        ui->SensorArm2ComboBox->setDisabled(true);
        ui->SensorArm3ComboBox->setDisabled(true);
        ui->Sensor1Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[0]));ui->Sensor2Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[1]));
        ui->Sensor3Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[2]));ui->Sensor4Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[3]));
        ui->Sensor5Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[4]));ui->Sensor6Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[5]));
        ui->Sensor7Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[6]));ui->Sensor8Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[7]));
        ui->Sensor9Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[8]));ui->Sensor10Label->setText("Sensor " + QString::number(MW->Griper1->SensorAdress[9]));
        CurrentArmToVisualize=0;
    }
    else
    {
        ui->visualizeArm1CheckBox->setChecked(true);
    }
}

void VisualizationForm::on_visualizeArm2CheckBox_clicked(bool checked)
{
    if (checked==true)
    {
        ui->visualizeArm1CheckBox->setChecked(false);
        ui->visualizeArm2CheckBox->setChecked(true);
        ui->visualizeArm3CheckBox->setChecked(false);
        ui->SensorArm1ComboBox->setDisabled(true);
        ui->SensorArm2ComboBox->setEnabled(true);
        ui->SensorArm3ComboBox->setDisabled(true);
        ui->Sensor1Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[0]));ui->Sensor2Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[1]));
        ui->Sensor3Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[2]));ui->Sensor4Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[3]));
        ui->Sensor5Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[4]));ui->Sensor6Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[5]));
        ui->Sensor7Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[6]));ui->Sensor8Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[7]));
        ui->Sensor9Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[8]));ui->Sensor10Label->setText("Sensor " + QString::number(MW->Griper2->SensorAdress[9]));
        CurrentArmToVisualize=1;
    }
    else
    {
        ui->visualizeArm2CheckBox->setChecked(true);
    }
}

void VisualizationForm::on_visualizeArm3CheckBox_clicked(bool checked)
{
    if (checked==true)
    {
        ui->visualizeArm1CheckBox->setChecked(false);
        ui->visualizeArm2CheckBox->setChecked(false);
        ui->visualizeArm3CheckBox->setChecked(true);
        ui->SensorArm1ComboBox->setDisabled(true);
        ui->SensorArm2ComboBox->setDisabled(true);
        ui->SensorArm3ComboBox->setEnabled(true);
        ui->Sensor1Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[0]));ui->Sensor2Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[1]));
        ui->Sensor3Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[2]));ui->Sensor4Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[3]));
        ui->Sensor5Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[4]));ui->Sensor6Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[5]));
        ui->Sensor7Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[6]));ui->Sensor8Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[7]));
        ui->Sensor9Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[8]));ui->Sensor10Label->setText("Sensor " + QString::number(MW->Griper3->SensorAdress[9]));
        CurrentArmToVisualize=2;
    }
    else
    {
        ui->visualizeArm3CheckBox->setChecked(true);
    }
}

void VisualizationForm::on_SensorArm1ComboBox_currentIndexChanged(int index)
{
    if (MW->EnableDatalogging==true)
    {
        CurrentSensorToVisualize=MW->ListOfSensorsPerArms[0][index];
    }
    else
    {
        CurrentSensorToVisualize=MW->Griper1->SensorAdress[index];
        MW->ListOfSensorsPerArms[0][0]=CurrentSensorToVisualize;
    }
    ResetButtonIsPushed=true;
}


void VisualizationForm::on_displayValuesCheckbox_clicked(bool checked)
{
    if (checked==true)
    {
        ui->TaxelVal1->setVisible(true);ui->TaxelVal2->setVisible(true);ui->TaxelVal3->setVisible(true);ui->TaxelVal4->setVisible(true);
        ui->TaxelVal5->setVisible(true);ui->TaxelVal6->setVisible(true);ui->TaxelVal7->setVisible(true);ui->TaxelVal8->setVisible(true);
        ui->TaxelVal9->setVisible(true);ui->TaxelVal10->setVisible(true);ui->TaxelVal11->setVisible(true);ui->TaxelVal12->setVisible(true);
        ui->TaxelVal13->setVisible(true);ui->TaxelVal14->setVisible(true);ui->TaxelVal15->setVisible(true);ui->TaxelVal16->setVisible(true);
        ui->TaxelVal17->setVisible(true);ui->TaxelVal18->setVisible(true);ui->TaxelVal19->setVisible(true);ui->TaxelVal20->setVisible(true);
        ui->AverageValuesCheckBox->setVisible(true); ui->FreezePushButton->setVisible(true);
        ui->LineTableDown->setVisible(true);ui->lineTableUp->setVisible(true);ui->LineTableRight->setVisible(true);ui->LineTableLeft->setVisible(true);
        ui->SumOfAllTaxelsAvgLabel->setVisible(true); ui->SumOfAllTaxelsAvgValueLabel->setVisible(true); ui->SumOfAllTaxelsStdLabel->setVisible(false); ui->SumOfAllTaxelsStdValueLabel->setVisible(false);
    }
    else
    {
        ui->TaxelVal1->setVisible(false);ui->TaxelVal2->setVisible(false);ui->TaxelVal3->setVisible(false);ui->TaxelVal4->setVisible(false);
        ui->TaxelVal5->setVisible(false);ui->TaxelVal6->setVisible(false);ui->TaxelVal7->setVisible(false);ui->TaxelVal8->setVisible(false);
        ui->TaxelVal9->setVisible(false);ui->TaxelVal10->setVisible(false);ui->TaxelVal11->setVisible(false);ui->TaxelVal12->setVisible(false);
        ui->TaxelVal13->setVisible(false);ui->TaxelVal14->setVisible(false);ui->TaxelVal15->setVisible(false);ui->TaxelVal16->setVisible(false);
        ui->TaxelVal17->setVisible(false);ui->TaxelVal18->setVisible(false);ui->TaxelVal19->setVisible(false);ui->TaxelVal20->setVisible(false);
        ui->STDVal1->setVisible(false);ui->STDVal2->setVisible(false);ui->STDVal3->setVisible(false);ui->STDVal4->setVisible(false);ui->STDVal5->setVisible(false);ui->STDVal6->setVisible(false);
        ui->STDVal7->setVisible(false);ui->STDVal8->setVisible(false);ui->STDVal9->setVisible(false);ui->STDVal10->setVisible(false);ui->STDVal11->setVisible(false);ui->STDVal12->setVisible(false);
        ui->STDVal13->setVisible(false);ui->STDVal14->setVisible(false);ui->STDVal15->setVisible(false);ui->STDVal16->setVisible(false);ui->STDVal17->setVisible(false);ui->STDVal18->setVisible(false);
        ui->STDVal19->setVisible(false);ui->STDVal20->setVisible(false);
        ui->LineTableDown2->setVisible(false); ui->LineTableLeft2->setVisible(false); ui->LineTableRight2->setVisible(false); ui->lineTableUp2->setVisible(false);
        ui->LineTableDown->setVisible(false);ui->lineTableUp->setVisible(false);ui->LineTableRight->setVisible(false);ui->LineTableLeft->setVisible(false);
        ui->AverageValuesCheckBox->setChecked(false); ui->AverageValuesCheckBox->setVisible(false);
        ui->StandardDeviationLabel->setVisible(false);
        ui->SumOfAllTaxelsAvgLabel->setVisible(false); ui->SumOfAllTaxelsAvgValueLabel->setVisible(false); ui->SumOfAllTaxelsStdLabel->setVisible(false); ui->SumOfAllTaxelsStdValueLabel->setVisible(false);
    }
}


void VisualizationForm::on_AverageValuesCheckBox_clicked(bool checked)
{
    if (checked==true)
    {
        ui->STDVal1->setVisible(true);ui->STDVal2->setVisible(true);ui->STDVal3->setVisible(true);ui->STDVal4->setVisible(true);ui->STDVal5->setVisible(true);ui->STDVal6->setVisible(true);
        ui->STDVal7->setVisible(true);ui->STDVal8->setVisible(true);ui->STDVal9->setVisible(true);ui->STDVal10->setVisible(true);ui->STDVal11->setVisible(true);ui->STDVal12->setVisible(true);
        ui->STDVal13->setVisible(true);ui->STDVal14->setVisible(true);ui->STDVal15->setVisible(true);ui->STDVal16->setVisible(true);ui->STDVal17->setVisible(true);ui->STDVal18->setVisible(true);
        ui->STDVal19->setVisible(true);ui->STDVal20->setVisible(true);

        ui->LineTableDown2->setVisible(true); ui->LineTableLeft2->setVisible(true); ui->LineTableRight2->setVisible(true); ui->lineTableUp2->setVisible(true); ui->StandardDeviationLabel->setVisible(true);
         ui->SumOfAllTaxelsAvgLabel->setVisible(true); ui->SumOfAllTaxelsAvgValueLabel->setVisible(true); ui->SumOfAllTaxelsStdLabel->setVisible(true); ui->SumOfAllTaxelsStdValueLabel->setVisible(true);
    }
    else
    {
        ui->STDVal1->setVisible(false);ui->STDVal2->setVisible(false);ui->STDVal3->setVisible(false);ui->STDVal4->setVisible(false);ui->STDVal5->setVisible(false);ui->STDVal6->setVisible(false);
        ui->STDVal7->setVisible(false);ui->STDVal8->setVisible(false);ui->STDVal9->setVisible(false);ui->STDVal10->setVisible(false);ui->STDVal11->setVisible(false);ui->STDVal12->setVisible(false);
        ui->STDVal13->setVisible(false);ui->STDVal14->setVisible(false);ui->STDVal15->setVisible(false);ui->STDVal16->setVisible(false);ui->STDVal17->setVisible(false);ui->STDVal18->setVisible(false);
        ui->STDVal19->setVisible(false);ui->STDVal20->setVisible(false);
        ui->LineTableDown2->setVisible(false); ui->LineTableLeft2->setVisible(false); ui->LineTableRight2->setVisible(false); ui->lineTableUp2->setVisible(false); ui->StandardDeviationLabel->setVisible(false);
        ui->SumOfAllTaxelsStdLabel->setVisible(false); ui->SumOfAllTaxelsStdValueLabel->setVisible(false);
    }
}

void VisualizationForm::on_FreezePushButton_clicked()
{
    if (DataFreeze==false)
    {
        DataFreeze=true;
        ui->FreezePushButton->setText("Unpause");
        ui->ExportGraphPushButton->setEnabled(true);
    }
    else
    {
        DataFreeze=false;
        ui->FreezePushButton->setText("Pause");
        ui->ExportGraphPushButton->setDisabled(true);
    }
}

void VisualizationForm::on_STFACheckbox_clicked(bool checked)
{
    if (checked==true)
    {
        DynamicDataViewMode=1; // Dynamic and stfa one upon each other
        ui->testLabel->move(175,-80);
        ui->testLabel_2->move(175,295);
        ui->testLabel_2->setVisible(true);
        grFourier->Clf();
        grFourier->Axis();
        grFourier->Label('x', "Hertz",0);
        grFourier->Label('y', "Amplitude", 0);
        grFourier->GetBGRN(buf2,4*w*250);
        pic2 = QPixmap::fromImage(QImage(buf2,w,250,QImage::Format_RGB32));
        ui->testLabel_2->setPixmap(pic2);
        qApp->processEvents();
        DataFreeze=false;
        ui->FreezePushButton->setText("Pause");
        ui->ExportGraphPushButton->setDisabled(true);
    }
    else
    {
        DynamicDataViewMode=0; // Only Dynamic Data are shown
        ui->testLabel->move(175,45);
        ui->testLabel_2->setVisible(false);
        DataFreeze=false;
        ui->FreezePushButton->setText("Pause");
        ui->ExportGraphPushButton->setDisabled(true);

    }
}

void VisualizationForm::on_ExportGraphPushButton_clicked()
{
    PicturePath = QFileDialog::getSaveFileName(this,tr("Enter where you want to save the picture:"),QDir::currentPath()+"/Graph.eps","*.eps \n*.jpg \n*.png \n*.gif");
    QFileInfo PictureFileNameInfo(PicturePath);

    if (MW->DataKind==0) // Current view is set to Static Data
    {
        if(PictureFileNameInfo.suffix()=="eps")
        {
            gr->WriteEPS(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="jpg")
        {
            gr->WriteJPEG(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="png")
        {
            gr->WritePNG(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="gif")
        {
            gr->WriteGIF(PicturePath.toStdString().c_str());
        }
    }
    else if(MW->DataKind==1) // Current view is set to Dynamic Data
    {
        if (DynamicDataViewMode==0)
        {
            if(PictureFileNameInfo.suffix()=="eps")
            {
                grDyn->WriteEPS(PicturePath.toStdString().c_str());
            }
            else if (PictureFileNameInfo.suffix()=="jpg")
            {
                grDyn->WriteJPEG(PicturePath.toStdString().c_str());
            }
            else if (PictureFileNameInfo.suffix()=="png")
            {
                grDyn->WritePNG(PicturePath.toStdString().c_str());
            }
            else if (PictureFileNameInfo.suffix()=="gif")
            {
                grDyn->WriteGIF(PicturePath.toStdString().c_str());
            }
        }
        else if (DynamicDataViewMode==1)
        {
            if(PictureFileNameInfo.suffix()=="eps")
            {
                grDyn2->WriteEPS(PicturePath.toStdString().c_str());
                PicturePath = PictureFileNameInfo.absolutePath() + "/" + PictureFileNameInfo.baseName() + "_FFT.eps";
                grFourier->WriteEPS(PicturePath.toStdString().c_str());
            }
            else if (PictureFileNameInfo.suffix()=="jpg")
            {
                grDyn2->WriteJPEG(PicturePath.toStdString().c_str());
                PicturePath = PictureFileNameInfo.absolutePath() + "/" + PictureFileNameInfo.baseName() + "_FFT.jpg";
                grFourier->WriteJPEG(PicturePath.toStdString().c_str());
            }
            else if (PictureFileNameInfo.suffix()=="png")
            {
                grDyn2->WritePNG(PicturePath.toStdString().c_str());
                PicturePath = PictureFileNameInfo.absolutePath() + "/" + PictureFileNameInfo.baseName() + "_FFT.png";
                grFourier->WritePNG(PicturePath.toStdString().c_str());
            }
            else if (PictureFileNameInfo.suffix()=="gif")
            {
                grDyn2->WriteGIF(PicturePath.toStdString().c_str());
                PicturePath = PictureFileNameInfo.absolutePath() + "/" + PictureFileNameInfo.baseName() + "FFT.gif";
                grFourier->WriteGIF(PicturePath.toStdString().c_str());
            }
        }
    }
    else if(MW->DataKind==2) // Current view is set to High Sensitivity Data
    {
        if(PictureFileNameInfo.suffix()=="eps")
        {
            grHigh->WriteEPS(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="jpg")
        {
            grHigh->WriteJPEG(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="png")
        {
            grHigh->WritePNG(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="gif")
        {
            grHigh->WriteGIF(PicturePath.toStdString().c_str());
        }
    }
    else if(MW->DataKind==3) // Current view is set to All Sensors
    {
        if(PictureFileNameInfo.suffix()=="eps")
        {
            grAll->WriteEPS(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="jpg")
        {
            grAll->WriteJPEG(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="png")
        {
            grAll->WritePNG(PicturePath.toStdString().c_str());
        }
        else if (PictureFileNameInfo.suffix()=="gif")
        {
            grAll->WriteGIF(PicturePath.toStdString().c_str());
        }
    }
}

void VisualizationForm::RevertSTFA()
{
    DynamicDataViewMode=0; // Only Dynamic Data are shown
    ui->testLabel->move(175,45);
    ui->testLabel_2->setVisible(false);
    DataFreeze=false;
    ui->FreezePushButton->setText("Pause");
    ui->ExportGraphPushButton->setDisabled(true);
    ui->STFACheckbox->setChecked(false);
}

void VisualizationForm::on_MaxCountSpinBox_valueChanged(int arg1)
{
    gr->SetRanges(0,3,0,6,-800,arg1);
}

void VisualizationForm::on_RotateRightPushButton_clicked()
{
    gr->Rotate(1,0);
}

void VisualizationForm::on_RotateLeftPushButton_clicked()
{
    gr->Rotate(-1,0);
}

void VisualizationForm::on_RotateUpPushButton_clicked()
{
    gr->Rotate(0,-1);
}

void VisualizationForm::on_RotateDownPushButton_clicked()
{
    gr->Rotate(0,1);
}
