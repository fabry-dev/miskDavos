#include "showrunner.h"

#include <fcntl.h>
#include <unistd.h>
#include <qstringlist.h>


#define timeout 3000

QStringList rfidTags = QStringList() << "12345" << "234567" << "789765";

showRunner::showRunner(QObject *parent, QList<QWidget *> widgetList, QString PATH, int speed, serialWatcher *serialwatch)
    : QObject(parent),widgetList(widgetList),PATH(PATH),speed(speed),serialwatch(serialwatch)
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
        qWarning("can not open RFID socket");
        //return;
    }

    notifier = new QSocketNotifier( fd,
                                    QSocketNotifier::Read,
                                    this);

    connect( notifier,
             &QSocketNotifier::activated,
             this,
             &showRunner::handle_readNotification );


    RFIDtimeout = new QTimer(this);
    RFIDtimeout->setSingleShot(true);
    connect(RFIDtimeout,SIGNAL(timeout()),this,SLOT(onTimeout()));




    testTimer = new QTimer(this);
    //testTimer->start(500);





    for(int i = 0;i<widgetList.size();i++)
    {
        bgImg.load(PATH+"insert"+QString::number(i)+".png");
        QLabel *lbl = new QLabel(widgetList[i]);
        bgLbls.push_back(lbl);
        lbl->resize(1920,1080);
        lbl->setPixmap(bgImg);
        lbl->hide();
    }


    //stopShow();
    //startShow(2);

}





#define imgCount 5

void showRunner::startShow(int show)
{

    if(show>2)
        return;
    if(show<0)
        return;

    QString contentPath = "content"+QString::number(show)+"/";


    for(auto lbl:bgLbls)
    {
        lbl->hide();
    }
    //  RFIDtimeout->start(timeout);

    for(auto w:widgetList)
    {
        w->showFullScreen();
        w->showFullScreen();
        w->showFullScreen();
        w->show();
        w->show();
        w->show();
    }
    std::vector<int> x0s;
    QStringList names;
    QImage buf;
    int totalWidth = 0;


    for(int i =0;i<imgCount;i++)
    {
        QString filename = PATH+contentPath+"img"+QString::number(i)+".jpg";
        buf.load(filename);
        buf = buf.scaledToHeight(1080);
        x0s.push_back(totalWidth);
        qDebug()<<totalWidth;
        totalWidth+= buf.width();
        names.push_back((QString)contentPath+"img"+QString::number(i)+".jpg");
    }

    int videoWidth = 1920;
    int videoPos = totalWidth;
    QString videoName = PATH+contentPath+"video0.mp4";
    totalWidth+=videoWidth;




    if(totalWidth<widgetList.size()*1920)
        totalWidth = widgetList.size()*1920;

    for(int i =0;i<imgCount;i++)
    {
        slideWindow *sw =   new slideWindow(NULL,PATH,widgetList,x0s.at(i),totalWidth,names.at(i),speed,i);
        connect(serialwatch,SIGNAL(goBackward()),sw,SLOT(goBackward()));
        connect(serialwatch,SIGNAL(goForward()),sw,SLOT(goForward()));
        connect(testTimer,SIGNAL(timeout()),sw,SLOT(goForward()));
        photos.push_back(sw);

    }
    slidevideo *sv = new slidevideo(NULL,PATH,widgetList,videoPos,videoWidth,totalWidth,videoName,speed);
    videos.push_back(sv);


    connect(serialwatch,SIGNAL(goForward()),sv,SLOT(goForward()));
    connect(serialwatch,SIGNAL(goBackward()),sv,SLOT(goBackward()));
    connect(testTimer,SIGNAL(timeout()),sv,SLOT(goForward()));
}


void showRunner::stopShow()
{

    for(auto w:widgetList)
    {
        w->showFullScreen();
        w->showFullScreen();
        w->showFullScreen();
        w->show();
        w->show();
        w->show();
    }

    for(auto lbl:bgLbls)
    {
        lbl->show();
    }

    RFIDtimeout->stop();
    for (auto v:videos)
    {

        v->deleteLater();
    }

    for (auto p:photos)
    {

        p->deleteLater();
    }

    photos.clear();
    videos.clear();

    //  restartGame->start(timeout);


}



showRunner::~showRunner()
{
    if( fd>=0 ){
        close(fd);
    }
}


void showRunner::onTimeout()
{
    qDebug()<<"time out RFID";
    stopShow();

}







void showRunner::handle_readNotification(int /*socket*/)
{
    qDebug()<<"notifs";

    uchar buf;
    while( read(fd,&buf,sizeof(buf))>0 ){
        qDebug()<<"RFID:";
        qDebug()<<buf;

        for(int i = 0;i<rfidTags.size();i++)
        {
            if((QString)buf == rfidTags[i])
            {
                qDebug()<<"start "<<i;
            }
        }


    }
}
