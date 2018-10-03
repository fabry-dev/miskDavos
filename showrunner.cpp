#include "showrunner.h"

#include <fcntl.h>
#include <unistd.h>
#include <qstringlist.h>
#include <linux/input.h>
#include "qbytearray.h"
#include "qprocess.h"

#define timeout 3000



showRunner::showRunner(QObject *parent, QList<QWidget *> widgetList, QString PATH, int speed, serialWatcher *serialwatch)
    : QObject(parent),widgetList(widgetList),PATH(PATH),speed(speed),serialwatch(serialwatch)
{


    codeBuf.clear();
    code0.clear();
    code1.clear();
    code2.clear();

    static const int arr0[] = {11,11,11,5,42,33,42,33,4,10,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr1[] = {11,11,11,5,10,2,42,18,8,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr2[] = {11,11,11,5,8,4,10,6,42,30,3,42,18,42,32,5,42,46,9,11};


    std::vector<uchar> vec0 (arr0, arr0 + sizeof(arr0) / sizeof(arr0[0]) );
    std::vector<uchar> vec1 (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
    std::vector<uchar> vec2 (arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]) );



    code0=vec0;
    code1=vec1;
    code2=vec2;



    auto file = new QFile();
    file->setFileName(fileName);

    if( !file->exists() ){
        qWarning("file does not exist");
        //return;
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





    for(int i = 0;i<widgetList.size();i++)
    {
        bgImg.load(PATH+"insert"+QString::number(i)+".png");
        QLabel *lbl = new QLabel(widgetList[i]);
        bgLbls.push_back(lbl);
        lbl->resize(1920,1080);
        lbl->setPixmap(bgImg);
        lbl->hide();
    }



    stopShow();
    //startShow(1);

}





#define imgCount 0

#define videoCount 2

void showRunner::startShow(int show)
{




    codeBuf.clear();
    if(show>2)
        return;
    if(show<0)
        return;

    if(activeShow == show)
    {
        qDebug()<<"reset time out";
        RFIDtimeout->start(timeout);//just restart
        return;
    }
    else if(activeShow != -1)//already playing something, let us stop first.
    {

        stopShow();
        return;
    }



    activeShow = show;



    QString contentPath = "content"+QString::number(show)+"/";


    for(auto lbl:bgLbls)
    {
        lbl->hide();
    }
    qDebug()<<"start time out count";
    RFIDtimeout->start(timeout);

    for(auto w:widgetList)
    {
        // w->showFullScreen();
        //   w->showFullScreen();
        //   w->showFullScreen();
        // w->show();
        // w->show();
        // w->show();
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





    std::vector<int>videoPos;
    std::vector<int>videoWidth;

    std::vector<QString>videoNames;

    for (int i = 0;i<videoCount;i++)
    {
        videoPos.push_back(totalWidth);
        QString videoName = PATH+contentPath+"video"+QString::number(i)+".mov";
        // videoName = (QString)"/home/fred/Downloads/content0/"+"video"+QString::number(i)+".mov";
        videoNames.push_back(videoName);
        videoWidth.push_back(getVideoWidth(videoName));
        totalWidth+=videoWidth.at(i);

        qDebug()<<videoName<<videoPos.at(i);
    }









    if(totalWidth<widgetList.size()*1920)
        totalWidth = widgetList.size()*1920;

    for(int i =0;i<imgCount;i++)
    {
        slideWindow *sw =   new slideWindow(NULL,PATH,widgetList,x0s.at(i),totalWidth,names.at(i),speed,i);
        connect(serialwatch,SIGNAL(goBackward()),sw,SLOT(goBackward()));
        connect(serialwatch,SIGNAL(goForward()),sw,SLOT(goForward()));

        photos.push_back(sw);

    }


    for (int i = 0;i<videoCount;i++)
    {
        slidevideo *sv1 = new slidevideo(NULL,PATH,widgetList,videoPos.at(i),videoWidth.at(i),totalWidth,videoNames.at(i),speed);
        videos.push_back(sv1);
        connect(serialwatch,SIGNAL(goForward()),sv1,SLOT(goForward()));
        connect(serialwatch,SIGNAL(goBackward()),sv1,SLOT(goBackward()));

    }





}




void showRunner::stopShow()
{

    if(activeShow == -1)
        return;

    activeShow = -1;
    codeBuf.clear();
    for(auto w:widgetList)
    {
        //  w->showFullScreen();
        //  w->showFullScreen();
        //  w->showFullScreen();
        // w->show();
        //  w->show();
        // w->show();
    }

    for(auto lbl:bgLbls)
    {
        lbl->show();
    }

    RFIDtimeout->stop();

    for (auto v:videos)
    {
        v->shutdown();
    }

    for (auto p:photos)
    {
        p->deleteLater();
    }

    photos.clear();
    videos.clear();





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


int showRunner::getVideoWidth(QString name)
{
    QProcess process;
    process.start("mediainfo --Inform=\"Video;%Width%\" "+name);
    process.waitForFinished(-1); // will wait forever until finished

    QString stdout = process.readAllStandardOutput();
    QStringList params = stdout.split("\n");
    bool test=false;int val;
    if(params.size()>0)
    {
        val = params[0].toInt(&test);
    }

    if(test)
        return val;
    else
        return 1920;//default value
}





void showRunner::handle_readNotification(int /*socket*/)
{
    //  qDebug()<<"notifs";





    struct input_event ev;


    read(fd, &ev, sizeof(struct input_event));

    if((ev.type == 1)&&(ev.value==0))
    {
        if((ev.code == 96)||(ev.code == 28))
        {
            //qDebug()<<"enter";


            if(codeBuf == code0)
            {
                qDebug()<<"code0";

                startShow(0);

            }
            else if(codeBuf == code1)
            {
                qDebug()<<"code1";
                startShow(1);
            }
            else if(codeBuf == code2)
            {
                qDebug()<<"code2";
                startShow(2);
            }
            else
            {
                for(auto b:codeBuf)
                    qDebug()<<(int)b;
            }

            codeBuf.clear();
        }
        else
        {
            uchar buf = ev.code;
            codeBuf.push_back(buf);
        }
    }


}













