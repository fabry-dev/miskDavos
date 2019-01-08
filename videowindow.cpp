#include "videowindow.h"
#include "qdebug.h"



videoWindow::videoWindow(QLabel *parent, QString PATH,int id) : QLabel(parent),PATH(PATH),id(id)
{

    page = 2;
    vp = new mpvWidget(this);
      vp->setLoop(false);
    vp->setProperty("pause", false);
    vp->setProperty("keep-open",true);

    if(id == 1)
        vp->setProperty("vf","crop=1920:1080:0:0");
    else
        vp->setProperty("vf","crop=1920:1080:1920:0");


    connect(vp,SIGNAL(videoOver()),this,SLOT(playingOver()));
    connect(vp,SIGNAL(videoOver()),this,SIGNAL(donePlaying()));

    showFullScreen();

    goToPage(1);

}


void videoWindow::resizeEvent(QResizeEvent *event)
{

    vp->resize(this->size());
}


void videoWindow::playingOver()
{

   // qDebug()<<"done playing";
    emit updStatus(id,false);
}


void videoWindow::goToPage(int nuPage)
{
    // qDebug()<<page<<nuPage;

    QString videoName;

    if(page == nuPage)
        return;

    if(page>nuPage)
    {

        videoName = "Page"+QString::number(page)+"-"+QString::number(page-1)+".f4v";

    }
    else
    {
        videoName = "Page"+QString::number(page)+"-"+QString::number(page+1)+".f4v";
    }
    page = nuPage;

    startVideo(videoName);

}



void videoWindow::startVideo(QString videoUrl)
{
   // qDebug()<<videoUrl;
    emit updStatus(id,true);

    vp->loadFile(PATH+videoUrl);

    vp->setProperty("pause",false);

}



void videoWindow::initVideo()
{
    startVideo("video.mp4");

}


