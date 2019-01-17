#include "module0.h"
#include "qdebug.h"
#include "qpushbutton.h"




module0::module0(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{

    showFullScreen();
    resize(3840,2160);

    setPixmap(QPixmap(PATH+"background.png").scaled(width(),height()));

    b1 = new picButton(this,0,PATH+"button1.png",PATH+"button1on.png","1");
    b1->move(width()/5-b1->width()/2,height()/2);
    connect(b1,SIGNAL(clicked(QString)),this,SIGNAL(goModule1()));
    connect(b1,SIGNAL(clicked(QString)),this,SLOT(disableButtons()));
    b1->show();

    b2 = new picButton(this,0,PATH+"button2.png",PATH+"button2on.png","2");
    b2->move(2*width()/5-b2->width()/2,height()/2);
    connect(b2,SIGNAL(clicked(QString)),this,SIGNAL(goModule2()));
    connect(b2,SIGNAL(clicked(QString)),this,SLOT(disableButtons()));
    b2->show();

    b3 = new picButton(this,0,PATH+"button3.png",PATH+"button3on.png","2");
    b3->move(3*width()/5-b3->width()/2,height()/2);
    connect(b3,SIGNAL(clicked(QString)),this,SIGNAL(goModule3()));
    connect(b3,SIGNAL(clicked(QString)),this,SLOT(disableButtons()));
    b3->show();

    b4 = new picButton(this,0,PATH+"button4.png",PATH+"button4on.png","2");
    b4->move(4*width()/5-b4->width()/2,height()/2);
    connect(b4,SIGNAL(clicked(QString)),this,SIGNAL(goModule4()));
    connect(b4,SIGNAL(clicked(QString)),this,SLOT(disableButtons()));
    b4->show();


}


void module0::disableButtons()
{
    b1->disable();
    b2->disable();
    b3->disable();
    b4->disable();
}


void module0::init()
{
    b1->enable();
    b2->enable();
    b3->enable();
    b4->enable();
}

void module0::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<ev->pos();

}
