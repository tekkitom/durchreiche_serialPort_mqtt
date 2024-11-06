#include "frmmain.h"
#include "ui_frmmain.h"

FrmMain::FrmMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FrmMain)
{
    ui->setupUi(this);
    mySerialPort = new PortWrapper_ma();

    connect(ui->btnListSerialPorts, &QPushButton::clicked, this, &FrmMain::listSerialPorts);
    connect(ui->btnConnectArduino, &QPushButton::clicked, this, &FrmMain::connectArduino);
    selectedSerialPort = 0xff;
}

FrmMain::~FrmMain()
{
    delete ui;
}

void FrmMain::listSerialPorts(){
    QList<QSerialPortInfo> serialPorts;
    QString outString;
    if(mySerialPort){
        serialPorts = mySerialPort->getSerialPortList();
        ui->lstSerialPorts->clear();
        for(int i = 0; i < serialPorts.size(); i++){
            outString = serialPorts[i].serialNumber() + ": " + serialPorts[i].portName() + " " + serialPorts[i].productIdentifier();
            ui->lstSerialPorts->addItem(outString);
        }
    }
    if(0xff>selectedSerialPort)
        ui->lstSerialPorts->setCurrentRow(selectedSerialPort);
}

void FrmMain::connectArduino(){
    int portNr = -1;

    portNr = ui->lstSerialPorts->currentRow();
    mySerialPort->openPort(portNr);
}
