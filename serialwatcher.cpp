#include "serialwatcher.h"

#include <fcntl.h>
#include <unistd.h>




#define maxPage 14

serialWatcher::serialWatcher(QObject *parent,int pageTops,int timeoutValue)

    : QObject(parent),pageTops(pageTops),timeoutValue(timeoutValue)
{

    savingTimer = new QTimer(this);
    connect(savingTimer,SIGNAL(timeout()),this,SLOT(timeOut()));
    timeOut();
    ready = true;
    pageStatus.resize(2,false);

    port = new QSerialPort;
    port->setPortName("/dev/ttyACM0");


    if(port->open(QIODevice::ReadWrite))
        qDebug()<<"serial created";
    else
        qDebug()<<"serial error";


    port->setBaudRate(QSerialPort::Baud9600)

            && port->setDataBits(QSerialPort::Data8)

            && port->setParity(QSerialPort::NoParity)

            && port->setStopBits(QSerialPort::OneStop)

            && port->setFlowControl(QSerialPort::NoFlowControl);




    connect(port, SIGNAL(readyRead()), this, SLOT(readData()));

    tops = 0;
    page =  0;
}




void serialWatcher::timeOut()
{
    saving = true;
    tops = 0;
    page =  0;
    emit gotTimeOut();
}


void serialWatcher::readData()
{

    const QByteArray data = port->readAll();
    uchar b = data.at(0);



    if(saving)
    {
        page =  0;
        saving = false;
        emit exitTimeOut();
         savingTimer->start(timeoutValue);//restart timing
        return;
    }


    if((!ready))
        return;

    savingTimer->start(timeoutValue);//restart timing


 //   qDebug()<<saving;









    if(b==155)
    {
        tops ++;


    }
    else if(b==255)
    {



        tops--;
    }



    //qDebug()<<tops;

    if(tops>pageTops)
    {
        tops=0;


        if(page<maxPage)
        {
            page++;
            emit nuPage(page);

        }

    }
    else if(tops<-pageTops)
    {
        tops=0;

        if(page>0)
        {
            page --;
            emit nuPage(page);
        }

    }




}

void serialWatcher::getStatus(int id,bool status)
{
    if((id>0)&&(id<=2))
    {
        pageStatus[id-1]=status;
        //qDebug()<<pageStatus;
    }

    ready = true;

    for(auto s:pageStatus)
    {
        if(s)
        {
            ready = false;

        }
    }

    // qDebug()<<"status: "<<ready;
}



serialWatcher::~serialWatcher()
{
    qDebug()<<"closed";

}


