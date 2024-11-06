#include "portwrapper_ma.h"

PortWrapper_ma::PortWrapper_ma(QObject *parent)
    : QObject{parent} {

}

QList<QSerialPortInfo> PortWrapper_ma::getSerialPortList(){

    portList = QSerialPortInfo::availablePorts();
    return portList;
}


void PortWrapper_ma::openPort(int portNr){
    serial.setPort(portList[portNr]);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);

    connect(&serial, &QIODevice::readyRead, this, &PortWrapper_ma::readData);

    serial.open(QIODevice::ReadWrite);
}

void PortWrapper_ma::readData(){

    if(serial.bytesAvailable())
    {
        recByteArray.append(serial.readLine()+ '\n');
    }
}

void PortWrapper_ma::selectAsciiMode(){
    sendChar('0');
}

void PortWrapper_ma::sendChar(char sendC){
    QDataStream dataStream(&serial);

    qDebug() <<  sendC;
    dataStream << (quint8)sendC;
}

QString PortWrapper_ma::getRecString(){
    QString recString;
    recString.append(recByteArray);

    return recString;
}
