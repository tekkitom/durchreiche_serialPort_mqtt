#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include"portwrapper_ma.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class FrmMain;
}
QT_END_NAMESPACE

class FrmMain : public QWidget
{
    Q_OBJECT

public:
    FrmMain(QWidget *parent = nullptr);
    ~FrmMain();

private slots:
    void listSerialPorts();
    void connectArduino();
private:
    Ui::FrmMain *ui;
    PortWrapper_ma *mySerialPort;
    uint selectedSerialPort;
};
#endif // FRMMAIN_H
