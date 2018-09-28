#include "rfidreader.h"

#include <fcntl.h>
#include <unistd.h>



RFIDReader::RFIDReader(QObject *parent)

   : QObject(parent)
{
    auto file = new QFile();
    file->setFileName(fileName);

    if( !file->exists() ){
        qWarning("file does not exist");
        return;
    }

    qDebug()<<fileName.toUtf8().data();
    fd = open(fileName.toUtf8().data(), O_RDONLY|O_NONBLOCK);
    if( fd==-1 ){
        qWarning("can not open file");
        return;
    }

    notifier = new QSocketNotifier( fd,
                                    QSocketNotifier::Read,
                                    this);

    connect( notifier,
             &QSocketNotifier::activated,
             this,
             &RFIDReader::handle_readNotification );
}

RFIDReader::~RFIDReader()
{
    if( fd>=0 ){
        close(fd);
    }
}



void RFIDReader::handle_readNotification(int /*socket*/)
{
    qDebug()<<"notifs";

    uchar buf;
    while( read(fd,&buf,sizeof(buf))>0 ){

        qDebug()<<buf;

    }
}
