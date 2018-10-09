#include "showrunner.h"

#include <fcntl.h>
#include <unistd.h>
#include <qstringlist.h>
#include <linux/input.h>
#include "qbytearray.h"
#include "qprocess.h"

#define TIMEOUT 10000

#define FULLSCREEN false
#define SHOW false


showRunner::showRunner(QObject *parent, QList<QWidget *> widgetList, QString PATH, int speed, serialWatcher *serialwatch)
    : QObject(parent),widgetList(widgetList),PATH(PATH),speed(speed),serialwatch(serialwatch)
{

    // videoThread.start();

    codeBuf.clear();
    codes.clear();
    static const int arr0[] = {11,11,11,5,42,33,42,33,4,10,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr1[] = {11,11,11,5,10,2,42,18,8,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr2[] = {11,11,11,5,8,4,10,6,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr3[] = {11,11,11,5,9,42,46,42,30,42,48,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr4[] = {11,11,11,5,42,46,3,42,46,9,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr5[] = {11,11,11,5,8,11,6,4,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr6[] = {11,11,11,5,9,3,7,42,33,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr7[] = {11,11,11,5,7,42,33,10,9,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr8[] = {11,11,11,5,7,42,33,42,32,9,42,30,3,42,18,42,32,5,42,46,9,11};
    static const int arr9[] = {11,11,11,5,42,46,42,33,9,9,42,30,3,42,18,42,32,5,42,46,9,11};

    //
    //
    //
    //
    //
    //

    std::vector<uchar> vec0 (arr0, arr0 + sizeof(arr0) / sizeof(arr0[0]) );
    std::vector<uchar> vec1 (arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
    std::vector<uchar> vec2 (arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]) );
    std::vector<uchar> vec3 (arr3, arr3 + sizeof(arr3) / sizeof(arr3[0]) );
    std::vector<uchar> vec4 (arr4, arr4 + sizeof(arr4) / sizeof(arr4[0]) );
    std::vector<uchar> vec5 (arr5, arr5 + sizeof(arr5) / sizeof(arr5[0]) );
    std::vector<uchar> vec6 (arr6, arr6 + sizeof(arr6) / sizeof(arr6[0]) );
    std::vector<uchar> vec7 (arr7, arr7 + sizeof(arr7) / sizeof(arr7[0]) );
    std::vector<uchar> vec8 (arr8, arr8 + sizeof(arr8) / sizeof(arr8[0]) );
    std::vector<uchar> vec9 (arr9, arr9 + sizeof(arr9) / sizeof(arr9[0]) );


    codes.push_back(vec0);
    codes.push_back(vec1);
    codes.push_back(vec2);
    codes.push_back(vec3);
    codes.push_back(vec4);
    codes.push_back(vec5);
    codes.push_back(vec6);
    codes.push_back(vec7);
    codes.push_back(vec8);
    codes.push_back(vec9);


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
    //startShow(0);

}





#define imgCount 0

#define videoCount 2

void showRunner::startShow(int show)
{

    codeBuf.clear();
    if(show>9)
        return;
    if(show<0)
        return;

    if(activeShow == show)
    {
        qDebug()<<"reset time out";
        RFIDtimeout->start(TIMEOUT);//just restart
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
    RFIDtimeout->start(TIMEOUT);

    for(auto w:widgetList)
    {
        if(SHOW)
        {
            if(FULLSCREEN)
            {
                w->showFullScreen();
                w->showFullScreen();
                w->showFullScreen();
            }
            else
            {
                w->show();
                w->show();
                w->show();
            }
        }
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
        //sv1->moveToThread(&videoThread);
        videos.push_back(sv1);
        connect(serialwatch,SIGNAL(goForward()),sv1,SLOT(goForward()));
        connect(serialwatch,SIGNAL(goBackward()),sv1,SLOT(goBackward()));
        //
    }





}


void showRunner::stopShow()
{

    if(activeShow == -1)
        return;

    qDebug()<<"stop show";

    activeShow = -1;
    codeBuf.clear();
    for(auto w:widgetList)
    {
        if(SHOW)
        {
            if(FULLSCREEN)
            {
                w->showFullScreen();
                w->showFullScreen();
                w->showFullScreen();
            }
            else
            {
                w->show();
                w->show();
                w->show();
            }
        }
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
    // qDebug()<<"notifs";

    struct input_event ev;


    while(read(fd, &ev, sizeof(struct input_event))>0)
    {
        // qDebug()<<ev.code;

        if((ev.type == 1)&&(ev.value==0))
        {
            if((ev.code == 96)||(ev.code == 28))
            {
                //qDebug()<<"enter";

                bool okcode=false;
                for(int i = 0;i<codes.size();i++)
                {
                    if(codeBuf == codes[i])
                    {
                        qDebug()<<"code"<<i;
                        startShow(i);
                        okcode = true;
                        break;
                    }


                }



                if(not okcode)
                {
                    QString sbuf="";
                    for(auto b:codeBuf)
                    {

                        sbuf.append(QString::number((int)b)+" ");
                    }
                    qDebug()<<sbuf;
                    qDebug()<<"FORCE reset time out";
                    RFIDtimeout->start(TIMEOUT);//just restart


                }

                codeBuf.clear();
            }
            else
            {
                ushort buf = ev.code;
                qDebug()<<buf;
                codeBuf.push_back(buf);
            }
        }
    }

}













