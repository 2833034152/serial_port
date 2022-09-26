#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>
#include <QTimer>
#include <windows.h>
#include <QString>
#include <QDebug>
#include <dbt.h>
#include <devguid.h>
#include <setupapi.h>
#include <initguid.h>
#include <list>
using namespace std;

namespace Ui {
class Serial_port;
}

class Serial_port : public QMainWindow
{
    Q_OBJECT

public:
    explicit Serial_port(QWidget *parent = nullptr);
    void InitPort();
    ~Serial_port();

private slots:
    void readData();    //读取数据
    void timeToSend();  //定时发送

    void on_OpenButton_clicked();

    void on_r_clearButton_clicked();

    void on_s_clearButton_clicked();

    void on_sendButton_clicked();

    void StringToHex(QString str,QByteArray &senddata); //发送时进制转换
    char convertHexChart(char ch);  //16进制转换
    void readToHex();   //将读取的数据以16进制显示
    void Mdisplay();
    //void pasingData(QByteArray &array); //解析串口数据

    void handleLineEditClicked();

    void on_Playbutton_clicked();

private:
    Ui::Serial_port *ui;
    QSerialPort *serialPort;
    QTimer *time;
    static   int sendBytes;
    static   int receBytes;
};
#endif // SERIAL_PORT_H
