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

    vp2 = new mpvWidget(this);
    vp2->resize(size());
    vp2->setLoop(false);
    vp2->setProperty("pause", false);
    vp2->setProperty("keep-open",true);
    vp2->hide();

    connect(vp,SIGNAL(videoOver()),this,SLOT(hideVideo()));
     connect(vp2,SIGNAL(videoOver()),this,SLOT(hideVideo()));
    /*   videoSlide = new QPropertyAnimation(vp,"geometry");
    videoSlide->setDuration(500);
    videoSlide->setEasingCurve(QEasingCurve::InCurve);


    */
    home = new picButton(this,0,PATH+"home.png",PATH+"homeOn.png","");
    connect(home,SIGNAL(clicked(QString)),this,SIGNAL(goHome()));
    home->move(width()-50-home->width(),50);
    home->show();
    home->raise();
    
    

    setPixmap(QPixmap(PATH+"background.png").scaled(width(),height()));
    
    int y0 = 200;
    b1 = new picButton2(this,0,PATH+"button1.png",PATH+"button1on.png","1");
    b1->move((width()-b1->width())/3,height()/2-y0);
    b1->show();




    b2 = new picButton2(this,0,PATH+"button2.png",PATH+"button2on.png","2");
    b2->move(2*(width()-b2->width())/3,height()/2-y0);
    b2->show();


    skip = new picButton(vp,0,PATH+"skip.png",PATH+"skipon.png","");
    skip->move(width()-skip->width()-50,50);
    skip->show();

    skip2 = new picButton(vp2,0,PATH+"skip.png",PATH+"skipon.png","");
    skip2->move(width()-skip2->width()-50,50);
    skip2->show();

    connect(b1,SIGNAL(clicked(QString)),this,SLOT(showVideo(QString)));
    connect(b2,SIGNAL(clicked(QString)),this,SLOT(showVideo(QString)));
    connect(skip,SIGNAL(clicked(QString)),this,SLOT(hideVideo()));
    connect(skip2,SIGNAL(clicked(QString)),this,SLOT(hideVideo()));
    vp->raise();//video player on top of everything
    vp2->raise();//video player on top of everything
    

}


void module1::init()
{

}



void module1::showVideo(QString video)
{
    // disconnect(videoSlide,0,0,0);
    
    QString videoName = PATH+"video"+video+".mp4";

    /* vp->move(-width(),0);
    vp->show();
    vp->loadFile(videoName);
    
    if(video == "1")
    videoSlide->setStartValue(b1->geometry());
    else
    videoSlide->setStartValue(b2->geometry());


    videoSlide->setEndValue(geometry());
    
    QTimer::singleShot(200,videoSlide,SLOT(start()));*/

    // vp->command(QVariantList() << "seek" << 0 << "absolute");
    //   vp->setProperty("pause", false);

    if(video=="1")
    {
        vp->move(-width(),0);
        vp->show();
        vp->loadFile(videoName);
        vp->move(0,0);
        b1->hide();
        b2->hide();
        vp2->stop();

    }
    else
    {
        vp2->move(-width(),0);
        vp2->show();
        vp2->loadFile(videoName);
        vp2->move(0,0);
        b1->hide();
        b2->hide();
        vp->stop();

    }




}

void module1::hideVideo()
{


    /*
    disconnect(videoSlide,0,0,0);
    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(-width(),0));
    connect(videoSlide,SIGNAL(finished()),vp,SLOT(hide()));
    videoSlide->start();*/
    // vp->stop();
    vp->move(-width(),0);
    vp2->move(-width(),0);
    b1->show();
    b2->show();

}

void module1::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<"mod1"<<ev->pos();
    
}
