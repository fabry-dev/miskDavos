#include "module1.h"
#include "qdebug.h"
#include "qpushbutton.h"


module1::module1(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{
    
    // showFullScreen();
    resize(3840,2160);
    
    
    
    vp = new mpvWidget(this);
    vp->resize(size());
    vp->setLoop(false);
    vp->setProperty("pause", false);
    vp->setProperty("keep-open",true);
    
    vp->hide();

    
    
    videoSlide = new QPropertyAnimation(vp,"pos");
    videoSlide->setDuration(500);
    videoSlide->setEasingCurve(QEasingCurve::InCurve);
    

    
    home = new picButton(this,0,PATH+"home.png",PATH+"homeOn.png","");
    connect(home,SIGNAL(clicked(QString)),this,SIGNAL(goHome()));
    home->move(width()-50-home->width(),50);
    home->show();
    home->raise();
    
    

    
    

    
    vp->raise();//video player on top of everything

    

}


void module1::init()
{

}



void module1::showVideo(int videoId)
{

    disconnect(videoSlide,0,0,0);
    
    QString videoName = PATH+"video"+QString::number(videoId)+".mp4";

    vp->move(width(),0);
    vp->show();
    vp->loadFile(videoName);
    
    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(0,0));
    
    QTimer::singleShot(200,videoSlide,SLOT(start()));

}

void module1::hideVideo()
{
    disconnect(videoSlide,0,0,0);
    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(width(),0));
    connect(videoSlide,SIGNAL(finished()),vp,SLOT(hide()));
    videoSlide->start();
    
}

void module1::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<"mod1"<<ev->pos();
    
}
