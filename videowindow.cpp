#include "videowindow.h"
#define screenSaverTimer 20


videoWindow::videoWindow(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{
    showFullScreen();
    setFixedSize(504,1008);


    mpvWidget *bg = new mpvWidget(this);
    bg->setLoop(true);
    bg->resize(504,1008);
    bg->move(0,0);
    bg->show();
    bg->loadFile(PATH+"bg.mp4");
    bg->setProperty("mute",true);


    vp = new mpvWidget(this);
    vp->setLoop(false);
    vp->resize(504,1008);
    vp->move((width()-vp->width())/2,0);
    vp->show();
    vp->loadFile(PATH+"video.mp4");
    vp->setLoop(true);
    vp->setProperty("pause", false);
    connect(vp,SIGNAL(videoOver()),this,SLOT(playingOver()));
    connect(vp,SIGNAL(videoOver()),this,SIGNAL(donePlaying()));
    stopped = false;

    goSaverVideo();
}





void videoWindow::playingOver()
{

    qDebug()<<"done playing";
    goSaverVideo();

}


void videoWindow::unblockPlayer()
{
    stopped = false;
    goSaverVideo();

}


void videoWindow::goSaverVideo()
{

    if(stopped)
    {
        vp->stop();


    }
    else
    {
       /* vp->resize(504,284);
        vp->move(0,(height()-vp->height())/2);
        QString dir = PATH+"records/";
        QDirIterator it(dir, QStringList() << "*.mp4", QDir::Files, QDirIterator::Subdirectories);

        QStringList dirList;
        while (it.hasNext())
            dirList.append(it.next());

        if(dirList.size()>0)
        {
            int randVid = qrand() % dirList.size();

            vp->loadFile(dirList[randVid]);

        }
        else
            vp->loadFile(PATH+"screensaver.mp4");
*/
        vp->setLoop(true);
        vp->setProperty("mute",true);
        vp->loadFile(PATH+"screensaver.mp4");


    }
}



void videoWindow::startQuestionVideo(uint question)
{
    qDebug()<<"start "<<question;

    vp->resize(504,1008);
    vp->move(0,0);

    vp->loadFile(PATH+"Question"+QString::number(question)+".mp4");
    vp->setLoop(false);
    vp->setProperty("mute",false);
    stopped = true;
}


void videoWindow::startVideo(QString videoUrl)
{
    vp->resize(504,1008);
    vp->move(0,0);
    vp->loadFile(PATH+videoUrl);
    vp->setLoop(true);
    vp->setProperty("mute",true);

}




void videoWindow::startIntro()
{


    vp->resize(504,1008);
    vp->move(0,0);
    vp->loadFile(PATH+"introGlobal.mp4");
    vp->setLoop(false);
    vp->setProperty("mute",false);

}

