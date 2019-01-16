#include "module2.h"
#include "qdebug.h"
#include "qpushbutton.h"


module2::module2(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{

    showFullScreen();
    resize(3840,2160);



    vp = new mpvWidget(this);
    vp->resize(size());
    vp->setLoop(false);
    vp->setProperty("pause", false);
    vp->setProperty("keep-open",true);

    vp->hide();
    connect(vp,SIGNAL(clicked(QPoint)),this,SLOT(getVideoClicked(QPoint)));


    videoSlide = new QPropertyAnimation(vp,"pos");
    videoSlide->setDuration(500);
    videoSlide->setEasingCurve(QEasingCurve::InCurve);

    colors.push_back(QColor::fromRgb(0x8E,0x31,0x8B));//pourpre
    colors.push_back(QColor::fromRgb(0x06,0x71,0xB7));//blue
    colors.push_back(QColor::fromRgb(0xF3,0x79,0x40));//orange
    colors.push_back(QColor::fromRgb(0xFB,0xA9,0x2B));//jaune




    loadCountries();

    vp->raise();//video player on top of everything

    for (auto b:exploreBubbles)
        b->hide();


    goExplore();


}


void module2::goCompare()
{
    for (auto b:exploreBubbles)
        b->hide();

}

void module2::goExplore()
{
 setPixmap(QPixmap(PATH+"exploreBackground.png").scaledToWidth(width()));
 for (auto b:exploreBubbles)
     b->show();
}




void module2::loadCountries()
{
    uint count = 0;

    QString countryName;
    int countrySize,countryValue,posX,posY,colorCode;
    bool test;
    QFile file(PATH+"countries.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no config file";

    }
    else
    {

        QTextStream in(&file);

        QString  line;
        QStringList params;

        while(!in.atEnd()) {
            line = in.readLine();
            params = (line.split(","));

            if(params.size()!=6)
                break;

            countryName = params[0];

            countryValue = params[1].toInt(&test);
            if(!test)
                break;

            countrySize = params[2].toInt(&test);
            if(!test)
                break;

            posX = params[3].toInt(&test);
            if(!test)
                break;

            posY = params[4].toInt(&test);
            if(!test)
                break;

            colorCode= params[5].toInt(&test);
            if((!test)&&(colorCode>=colors.size()))
                break;



            circleButton *b = new circleButton(this,colors[colorCode],count,QString::number(countryValue)+" "+countryName);
            exploreBubbles.push_back(b);
            countryNames.push_back(countryName);
            connect(b,SIGNAL(clicked(int)),this,SLOT(showVideo(int)));
            b->resize(QSize(countrySize,countrySize));
            b->move(posX,posY);
            b->show();
            count ++;
        }
        file.close();

    }

}


void module2::showVideo(int countryId)
{
    if(countryId>=countryNames.size())
        return;


    disconnect(videoSlide,0,0,0);

    QString videoName = PATH+countryNames[countryId]+".mp4";

    qDebug()<<videoName;
    vp->move(width(),0);
    vp->show();
    vp->loadFile(videoName);

    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(0,0));

    QTimer::singleShot(200,videoSlide,SLOT(start()));
    //videoSlide->start();

}



void module2::hideVideo()
{
    disconnect(videoSlide,0,0,0);
    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(width(),0));
    connect(videoSlide,SIGNAL(finished()),vp,SLOT(hide()));
    videoSlide->start();

}

void module2::getVideoClicked(QPoint pos)
{
    if((pos.x()>160)&&(pos.y()>120)&&(pos.x()<400)&&(pos.y()<255))
    {
        hideVideo();
    }

    //    qDebug()<<pos;
}

void module2::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<ev->pos();

}
