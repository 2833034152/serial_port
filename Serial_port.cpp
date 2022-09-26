#include "Serial_port.h"
#include "ui_Serial_port.h"
#include <QMessageBox>
#include "m_lineEdit.h"
#include <QDebug>
#include <QFileDialog>
#include <QSerialPort>
#include <qDebug>

#include <QSerialPort>

#include <QSerialPortInfo>

#include <windef.h>


Serial_port::Serial_port(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Serial_port)
{
    qDebug() << "Serial_port start to struct!";
    ui->setupUi(this);
    time = new QTimer(this);
    InitPort();
    //设置默认值
    ui->PortBox->setCurrentIndex(1);
    ui->BaudBox->setCurrentIndex(7);
    ui->StopBox->setCurrentIndex(0);
    ui->DataBox->setCurrentIndex(3);
    ui->ParityBox->setCurrentIndex(0);

    ui->sendButton->setEnabled(false);
    ui->sHexRadio->setEnabled(false);
    ui->sTextRadio->setChecked(true);
    ui->sTextRadio->setEnabled(false);
    ui->rTextRadio->setChecked(true);
    ui->rHexRadio->setChecked(false);
    ui->reDisplay->setChecked(true);

    connect(ui->reSendCheck, &QCheckBox::stateChanged,
            this, &Serial_port::timeToSend);         //自动重发
    connect(time, &QTimer::timeout, this, &Serial_port::on_sendButton_clicked);//

    connect(ui->rHexRadio, &QRadioButton::toggled, this, &Serial_port::Mdisplay);

    connect(ui->m_txt, SIGNAL(clicked()), this, SLOT(handleLineEditClicked()));


}

int Serial_port::sendBytes = 0;
int Serial_port::receBytes = 0;

void Serial_port::InitPort()
{
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        QSerialPort serial;
//      QextSerialPort("\\\\.\\COM12");
        serial.setPort(info);

        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
//        serial.setPortName(info.portName());

        int ret  =  serial.open(QIODevice::ReadWrite);
        qDebug() <<errno;
        qDebug() << strerror(errno);
        qDebug() << "ret: " << ret;


        QSerialPort port;
        qDebug() <<"port号"<<port.error();
       qDebug() << "last error:"<< GetLastError();
//       if(serial.open(QIODevice::ReadWrite))
         if(ret)
        {
            ui->PortBox->addItem(info.portName());
            //qDebug()<<info.portName();
            serial.close();
        }
        else{
            qDebug() << QString("connot open available serial")<<info.portName();
        }
    }

    QSerialPortInfo info_com1;
    //创建串口对象
    QSerialPort serial_com1;
    //设置串口名
    serial_com1.setPortName("COM1");
    //设置波特率
    serial_com1.setBaudRate(QSerialPort::Baud9600);
    //设置数据位数
    serial_com1.setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    serial_com1.setParity(QSerialPort::NoParity);
    //设置停止位
    serial_com1.setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial_com1.setFlowControl(QSerialPort::NoFlowControl);

    serial_com1.setPort(info_com1);
    //打开串口
  int ret1 =   serial_com1.open(QIODevice::ReadWrite);
  if(ret1)
      {
          ui->PortBox->addItem(info_com1.portName());
          qDebug() << QString("  open  serial_com1 success");
          //qDebug()<<info.portName();
      }
      else{
          qDebug() << QString("t open  serial_com1 failed");
      }




  HANDLE   hCom = CreateFile(("\\\\.\\COM1"),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
    if (hCom != INVALID_HANDLE_VALUE)
    {
       qDebug()<< "打开串口1成功";
//       CloseHandle(hCom);    //这句是释放内核资源， 即关闭串口

    }else
    {
        qDebug()<< "打开串口1失败";
//        CloseHandle(hCom);
    }
    HANDLE  hCom2 = CreateFile("\\\\.\\COM2",GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
    if (hCom2 != INVALID_HANDLE_VALUE)
    {
       qDebug()<< "打开串口2成功";

    }else
    {
        qDebug()<< "打开串口2失败";
    }

    QStringList baudList;   //波特率
    QStringList parityList; //校验位
    QStringList dataBitsList;   //数据位
    QStringList stopBitsList;   //停止位
    // 波特率    //波特率默认选择下拉第三项：9600
    baudList<<"1200"<<"2400"<<"4800"<<"9600"
           <<"14400"<<"19200"<<"38400"<<"56000"
          <<"57600"<<"115200";
    ui->BaudBox->addItems(baudList);
    ui->BaudBox->setCurrentIndex(3);
    // 校验      //校验默认选择无
    parityList<<"无"<<"奇"<<"偶";
    ui->ParityBox->addItems(parityList);
    ui->ParityBox->setCurrentIndex(0);
    // 数据位      //数据位默认选择8位
    dataBitsList<<"5"<<"6"<<"7"<<"8";
    ui->DataBox->addItems(dataBitsList);
    ui->DataBox->setCurrentIndex(3);
    // 停止位      //停止位默认选择1位
    stopBitsList<<"1"<<"2";
    ui->StopBox->addItems(stopBitsList);
    ui->StopBox->setCurrentIndex(0);
}

Serial_port::~Serial_port()
{
    delete serialPort;
    delete time;
    delete ui;

}
#if 0
void Serial_port::readData()
{
#if 1
  static  QByteArray buf;
    if (serialPort){
        qDebug() << "serialPort is:"<< serialPort;
        qDebug() << buf;

        buf += serialPort->readAll();

//        if(buf.endsWith("\r\n")){
            qDebug()<<"after readAll() ,buff:"<<buf;
            QString myStrTemp2 = QString::fromLocal8Bit(buf); //支持中文显示
             qDebug() <<"after fromLocal8Bit:"<< myStrTemp2;
             ui->textBrowser->insertPlainText(myStrTemp2);
             ui->textBrowser->append(myStrTemp2);
             buf.clear();
//        }
#else
        qDebug()<<"buf:"<< buf;
        if (!buf.isEmpty())
             qDebug() << "buf.isEmpty()<< is:"<< serialPort;
        {

            receBytes += buf.size();
            QString redata = QString("received:%1").arg(QString::number(receBytes));

            ui->sendlabel->setText(redata);
            QString myStrTemp = QString::fromLocal8Bit(buf); //支持中文显示


/*
            if(ui->reDisplay->isChecked())
            {
               QString str = ui->textBrowser->toPlainText();
               str +=myStrTemp;

                ui->textBrowser->insertPlainText(myStrTemp);
//          ui->textBrowser->clear();

                ui->textBrowser->append(str);
            }
          */
             buf.clear();

        #endif
    }
}
#else
void Serial_port::readData()
{
    QByteArray buf;
    if (serialPort){
   qDebug() <<QSerialPort::BaudRate();    //波特率
   qDebug()<< QSerialPort::StopBits();
   qDebug()<< QSerialPort::DataBits();
   qDebug()<< QSerialPort::Parity();//奇偶校验位



    buf = serialPort->readAll();
     qDebug()<<"buf:"<< buf;
      qDebug() << strerror(errno);
       qDebug() << "last error:"<< GetLastError();
    if (!buf.isEmpty())
    {
        qDebug() << "before ,receBytes:"<<receBytes;
        receBytes += buf.size();


        qDebug() << "after ,receBytes:"<<receBytes;

        QString redata = QString("received:%1").arg(QString::number(receBytes));
        ui->sendlabel->setText(redata);

        qDebug()<<"in function :buf:"<< buf;
        QString myStrTemp = QString::fromLocal8Bit(buf); //支持中文显示

        qDebug() <<"after fromLocal8Bit:"<< myStrTemp;
        if(ui->reDisplay->isChecked())
        {
//            ui->textBrowser->setText(myStrTemp);

            QString str = ui->textBrowser->toPlainText();
            qDebug() << "first str:"<< str;           //从文本框中获取的

            str +=myStrTemp;                          //myStrTemp是buf中的
            qDebug() << "after + : str:"<< str;

            ui->textBrowser->clear();      //清空文本框中内容
            ui->textBrowser->append(str);
             buf.clear();
        }
    }

    }

}

#endif
void Serial_port::timeToSend()
{
    if(ui->reSendCheck->isChecked())
    {
        if(time->isActive())
        {
            return;
        }
        else
        {
            int ms = ui->spinBox->value();
            time->start(ms);
        }
    }
    else
    {
        if(time->isActive())
        {
            time->stop();
        }
        else
        {
            return;
        }
    }

}

void Serial_port::on_OpenButton_clicked()
{
    if (ui->OpenButton->text() == tr("打开串口"))
    {
        serialPort = new QSerialPort;

        serialPort->setPortName(ui->PortBox->currentText());

        int tmp = serialPort->open(QIODevice::ReadWrite);
        qDebug() <<"打开串口:tmp"<<tmp;
        if(serialPort->open(QIODevice::ReadWrite))

          qDebug() << strerror(tmp);
          if(tmp)
        {
            //qDebug()<<ui->BaudBox->currentIndex();
            qDebug() <<"打开串口 - serialPort:"<<serialPort;
            switch (ui->BaudBox->currentIndex()) {
            case 0:
                serialPort->setBaudRate(QSerialPort::Baud1200);
                break;
            case 1:
                serialPort->setBaudRate(QSerialPort::Baud2400);
                break;
            case 2:
                serialPort->setBaudRate(QSerialPort::Baud4800);
                break;
            case 3:
                serialPort->setBaudRate(QSerialPort::Baud9600);
                break;
            case 4:
                serialPort->setBaudRate(QSerialPort::Baud19200);
                break;
            case 5:
                serialPort->setBaudRate(QSerialPort::Baud38400);
                break;
            case 6:
                serialPort->setBaudRate(QSerialPort::Baud57600);
                break;
            case 7:
                serialPort->setBaudRate(QSerialPort::Baud115200);
                break;
            default:
                break;
            }

            switch (ui->StopBox->currentIndex()) {
            case 0:
                serialPort->setStopBits(QSerialPort::OneStop);
                break;
            case 1:
                serialPort->setStopBits(QSerialPort::TwoStop);
                break;
            default:
                break;
            }

            switch (ui->DataBox->currentIndex()) {
            case 0:
                serialPort->setDataBits(QSerialPort::Data5);
                break;
            case 1:
                serialPort->setDataBits(QSerialPort::Data6);
                break;
            case 2:
                serialPort->setDataBits(QSerialPort::Data7);
                break;
            case 3:
                serialPort->setDataBits(QSerialPort::Data8);
                break;
            default:
                break;
            }

            switch (ui->ParityBox->currentIndex()) {
            case 0:
                serialPort->setParity(QSerialPort::NoParity);
                break;
            case 1:
                serialPort->setParity(QSerialPort::OddParity);
                break;
            case 2:
                serialPort->setParity(QSerialPort::EvenParity);
                break;
            default:
                break;
            }

            ui->OpenButton->setText(tr("关闭串口"));
            ui->PortBox->setEnabled(false);
            ui->BaudBox->setEnabled(false);
            ui->StopBox->setEnabled(false);
            ui->DataBox->setEnabled(false);
            ui->ParityBox->setEnabled(false);
            ui->sendButton->setEnabled(true);
            ui->sTextRadio->setEnabled(true);
            ui->sHexRadio->setEnabled(true);
            connect(serialPort, &QSerialPort::readyRead, this, &Serial_port::readData);
            connect(serialPort, &QSerialPort::readyRead, this, &Serial_port::readToHex);
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), serialPort->errorString());
        }

          serialPort->setBaudRate(QSerialPort::Baud115200);
          qDebug() << QSerialPort::BaudRate();    //波特率
          qDebug()<< QSerialPort::StopBits();
          qDebug()<< QSerialPort::DataBits();
          qDebug()<< QSerialPort::Parity();//奇偶校验位

    }
    else
    {
        serialPort->clear();
        serialPort->close();
        serialPort->deleteLater();

        ui->sendButton->setEnabled(false);
        ui->OpenButton->setText(tr("打开串口"));
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->DataBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->sHexRadio->setEnabled(false);
        ui->sTextRadio->setEnabled(false);

    }

}

void Serial_port::on_r_clearButton_clicked()
{
    ui->textBrowser->clear();

}

void Serial_port::on_s_clearButton_clicked()
{
    ui->lineEdit->clear();
}

void Serial_port::on_sendButton_clicked()
{
    //Latin1是ISO-8859-1的别名，有些环境下写作Latin-1。ISO-8859-1编码是单字节编码，向下兼容ASCII
   //其编码范围是0x00-0xFF，0x00-0x7F之间完全和ASCII一致，0x80-0x9F之间是控制字符，0xA0-0xFF之间是文字符号。
    QString str = ui->lineEdit->text();

    if(!str.isEmpty())
    {
        auto isHexSend = ui->sHexRadio->isChecked();

        int len = str.length();
        if(len%2 == 1)
        {
            str = str.insert(len-1,'0');
        }
        QByteArray senddata;
        if(isHexSend)
        {
            StringToHex(str,senddata);
            serialPort->write(senddata);

            if(serialPort->write(senddata)<0)
            {
                QMessageBox::critical(this, tr("Error"), serialPort->errorString());
            }
        }
        else
        {
            if(serialPort->write(ui->lineEdit->text().toLocal8Bit())<0)
            {
                QMessageBox::critical(this, tr("Error"), serialPort->errorString());
            }
        }

    }

}

void Serial_port::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
           int hexdatalen = 0;
           int len = str.length();
           senddata.resize(len/2);
           char lstr,hstr;
           for(int i=0; i<len; )
           {
               //char lstr,
               hstr=str[i].toLatin1();
               if(hstr == ' ')
               {
                   i++;
                   continue;
               }
               i++;
               if(i >= len)
                   break;
               lstr = str[i].toLatin1();
               hexdata = convertHexChart(hstr);
               lowhexdata = convertHexChart(lstr);
               if((hexdata == 16) || (lowhexdata == 16))
                   break;
               else
                   hexdata = hexdata*16+lowhexdata;
               i++;
               senddata[hexdatalen] = (char)hexdata;
               hexdatalen++;
           }
           senddata.resize(hexdatalen);

}

char Serial_port::convertHexChart(char ch)
{
    if((ch >= '0') && (ch <= '9'))
                return ch-0x30;  // 0x30 对应 ‘0’
            else if((ch >= 'A') && (ch <= 'F'))
                return ch-'A'+10;
            else if((ch >= 'a') && (ch <= 'f'))
                return ch-'a'+10;
    //        else return (-1);
    else return ch-ch;//不在0-f范围内的会发送成0

}

void Serial_port::readToHex()
{
    QByteArray temp = serialPort->readAll();
    auto isShow = ui->reDisplay->isChecked();         //接收显示？
    QDataStream out(&temp,QIODevice::ReadOnly);    //将字节数组读入
    while(!out.atEnd())
    {
           qint8 outChar = 0;
           out>>outChar;   //每字节填充一次，直到结束
           //十六进制的转换
           QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));
           if(isShow){
               ui->textBrowser->insertPlainText(str.toUpper());//大写
               ui->textBrowser->insertPlainText(" ");//每发送两个字符后添加一个空格
               ui->textBrowser->moveCursor(QTextCursor::End);
           }
    }

}

void Serial_port::Mdisplay()
{
    if(ui->rHexRadio->isChecked())
    {
        disconnect(serialPort, &QSerialPort::readyRead, this, &Serial_port::readData);
        connect(serialPort, &QSerialPort::readyRead, this, &Serial_port::readToHex);
    }
    else
    {
        connect(serialPort, &QSerialPort::readyRead, this, &Serial_port::readData);
        disconnect(serialPort, &QSerialPort::readyRead, this, &Serial_port::readToHex);
    }

}

void Serial_port::handleLineEditClicked()
{
    //QString curPath = QDir::currentPath();
    QString curPath = "../QSerial_port";
    //QFile file;
    QString aFileName = QFileDialog::getOpenFileName(this,QString("选择文件"),curPath,QString("TEXT(*.txt)"));
    ui->m_txt->setText(aFileName);

}

void Serial_port::on_Playbutton_clicked()
{
    QString path = ui->m_txt->text();
    //qDebug()<<"是否成功获取"<<path_ce->text();

    if(path.isEmpty())
        return;
    QFile aFile(path);
    if(!aFile.exists())     //文件不存在
        qDebug() <<"the file not exist!!";
        //return;
    if(!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() <<"文件无法打开";
        //return;
    QTextStream aStream(&aFile);    //用文件流读取文件
    while (!aStream.atEnd())
    {
        QString qstr = aStream.readLine();   //读取文件的一行文本
        //qDebug()<<qstr.length();
        int len = qstr.length();
        if(len%2 == 1)
        {
            qstr = qstr.insert(len-1,'0');
        }
        QByteArray senddata;
        StringToHex(qstr,senddata);
        //qDebug()<<senddata.size();
        for(int i=0;i<senddata.size();i++)
        {
            QByteArray tmpsenddata=senddata.mid(i,40);
            i=i+39;
            //qDebug()<<tmpsenddata.length();
            serialPort->write(tmpsenddata);

            if(!serialPort->waitForBytesWritten())   //这一句很关键，决定是否能发送成功
            {
                 qDebug()<<"serial write error";
            }

            Sleep(1000);
        }

    }

}
