#include "module3.h"
#include "qdebug.h"



module3::module3(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{

    showFullScreen();
    resize(3840,2160);

    q = new question(this,PATH);
    connect(q,SIGNAL(result(int,int)),this,SLOT(getResult(int,int)));
    q->resize(size());
    q->hide();



}


void module3::init()
{
 showQuestion(1);

}



void module3::getResult(int questionId, int answer)
{
    qDebug()<<questionId<<answer;
    q->hide();

    q->showQuestion(1);
}




void module3::showQuestion(int id)
{
    q->showQuestion(id);
}




question::question(QLabel *parent, QString PATH):QLabel(parent),PATH(PATH)
{

    vp = new mpvWidget(this);
    vp->resize(size());
    vp->setLoop(false);
    vp->setProperty("pause", false);
    vp->setProperty("keep-open",true);
    vp->show();

    int yb = 1731;
    int yh = 1129;

    int y0;
    int d0 = 220;

    circleButton * cb0 = new circleButton(this,QColor::fromRgb(0xFA,0xA9,0x2A),0);
    buttons.push_back(cb0);
    int x0 = 491;y0 = yb;
    cb0->move(x0-d0/2,y0-d0/2);

    circleButton * cb1 = new circleButton(this,QColor::fromRgb(0xF4,0x79,0x41),1);
    buttons.push_back(cb1);
    x0 = 979;y0 = yh;
    cb1->move(x0-d0/2,y0-d0/2);

    circleButton * cb2 = new circleButton(this,QColor::fromRgb(0x06,0x71,0xB8),2);
    buttons.push_back(cb2);
    x0 = 1440;y0 = yb;
    cb2->move(x0-d0/2,y0-d0/2);


    circleButton * cb3 = new circleButton(this,QColor::fromRgb(0xFC,0x48,0x61),3);
    buttons.push_back(cb3);
    x0 = 1924;y0 = yh;
    cb3->move(x0-d0/2,y0-d0/2);

    circleButton * cb4 = new circleButton(this,QColor::fromRgb(0xB7,0x47,0xB5),4);
    buttons.push_back(cb4);
    x0 = 2408; y0 = yb;
    cb4->move(x0-d0/2,y0-d0/2);


    circleButton * cb5 = new circleButton(this,QColor::fromRgb(0x8E,0x32,0x8B),5);
    buttons.push_back(cb5);
    x0 = 2894; y0 = yh;
    cb5->move(x0-d0/2,y0-d0/2);

    circleButton * cb6 = new circleButton(this,QColor::fromRgb(0x89,0x8E,0x91),6);
    buttons.push_back(cb6);
    x0 = 3373; y0 = yb;
    cb6->move(x0-d0/2,y0-d0/2);


    for (auto b:buttons)
    {
        b->resize(d0,d0);
        b->hide();

        connect(b,SIGNAL(clicked(int)),this,SLOT(showChoice(int)));
    }



    target.setParent(this);
    target.hide();


    show();





}

void question::showChoice(int choice)
{
    answerId = choice;
    disconnect(vp,SIGNAL(videoOver()),0,0);
    for (auto b:buttons)
        b->hide();

    connect(vp,SIGNAL(videoOver()),this,SLOT(provideResults()));
    vp->loadFile(PATH+"question"+QString::number(id)+"stats.mp4");

    showTarget(choice);
}

void question::showTarget(uint choice)
{
    if((choice<0)||(choice>=buttons.size()))
        return;

    int d0 = buttons[choice]->width();

    target.resize(2.3*d0,2.3*d0);

    //compute corresponding circle center
    int x0 = buttons[choice]->x()+buttons[choice]->width()/2;
    int y0 = buttons[choice]->y()+buttons[choice]->height()/2;

    target.move(x0-target.width()/2,y0-target.height()/2);

    target.setPixmap(QPixmap(PATH+"target.png").scaledToHeight(target.height()));

    target.show();
}

void question::showQuestion(int nuid)
{

    target.hide();
    id = nuid;

    disconnect(vp,SIGNAL(videoOver()),0,0);
    for(auto b:buttons)
        connect(vp,SIGNAL(videoOver()),b,SLOT(show()));

    vp->loadFile(PATH+"question"+QString::number(id)+".mp4");

     show();
}

void question::provideResults()
{
    emit result(id,answerId);
}

void question::resizeEvent(QResizeEvent *)
{
    vp->resize(size());
}

