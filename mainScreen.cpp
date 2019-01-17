#include "mainScreen.h"






mainScreen::mainScreen(QLabel *parent,QString PATH,bool DEBUG) : QLabel(parent),PATH(PATH),DEBUG(DEBUG)
{

    showFullScreen();
    resize(3840,2160);



    md0 = new module0(this,PATH+"module0/");
    md0->show();



    md2 = new module2(this,PATH+"module2/");
    md2->hide();

    md3 = new module3(this,PATH+"module3/");
    md3->hide();

    md4 = new module4(this,PATH+"module4/");
    md4->hide();



    if(DEBUG)
    {

        QPushButton *pb0 = new QPushButton(md0);
        pb0->move(0,0);
        pb0->resize(200,200);
        pb0->setText("close app");
        connect(pb0,SIGNAL(clicked(bool)),qApp,SLOT(closeAllWindows()));
        pb0->show();
        pb0->raise();


        QPushButton *pb = new QPushButton(md2);
        pb->move(0,0);
        pb->resize(200,200);
        pb->setText("close app");
        connect(pb,SIGNAL(clicked(bool)),qApp,SLOT(closeAllWindows()));
        pb->show();
        pb->raise();

        QPushButton *pb2 = new QPushButton(md3);
        pb2->move(0,0);
        pb2->resize(200,200);
        pb2->setText("close app");
        connect(pb2,SIGNAL(clicked(bool)),qApp,SLOT(closeAllWindows()));
        pb2->show();
        pb2->raise();

        QPushButton *pb4 = new QPushButton(md4);
        pb4->move(0,0);
        pb4->resize(200,200);
        pb4->setText("close app");
        connect(pb4,SIGNAL(clicked(bool)),qApp,SLOT(closeAllWindows()));
        pb4->show();
        pb4->raise();


    }



    connect(md0,SIGNAL(goModule1()),this,SLOT(goModule1()));
    connect(md0,SIGNAL(goModule2()),this,SLOT(goModule2()));
    connect(md0,SIGNAL(goModule3()),this,SLOT(goModule3()));
    connect(md0,SIGNAL(goModule4()),this,SLOT(goModule4()));


    goaway = new QPropertyAnimation(NULL,"pos");
    goaway->setDuration(1000);
    goaway->setEasingCurve(QEasingCurve::InCurve);

    come = new QPropertyAnimation(NULL,"pos");
    come->setDuration(1000);
    come->setEasingCurve(QEasingCurve::InCurve);






}







void mainScreen::goModule0()
{

}
void mainScreen::goModule1()
{

}

void mainScreen::goModule2()
{
    disconnect(goaway,0,0,0);
    disconnect(come,0,0,0);

    goaway->setTargetObject(md0);
    goaway->setStartValue(md0->pos());
    goaway->setEndValue(QPoint(-width(),0));
    connect(goaway,SIGNAL(finished()),md0,SLOT(hide()));

    come->setTargetObject(md2);
    come->setStartValue(pos());
    come->setEndValue(QPoint(0,0));
    md2->init();
    md2->move(width(),0);
    md2->show();

    goaway->start();
    come->start();
}


void mainScreen::goModule3()
{
    disconnect(goaway,0,0,0);
    disconnect(come,0,0,0);

    goaway->setTargetObject(md0);
    goaway->setStartValue(md0->pos());
    goaway->setEndValue(QPoint(-width(),0));
    connect(goaway,SIGNAL(finished()),md0,SLOT(hide()));

    come->setTargetObject(md3);
    come->setStartValue(pos());
    come->setEndValue(QPoint(0,0));
    md3->init();
    md3->move(width(),0);
    md3->show();

    goaway->start();
    come->start();
}



void mainScreen::goModule4()
{
    disconnect(goaway,0,0,0);
    disconnect(come,0,0,0);

    goaway->setTargetObject(md0);
    goaway->setStartValue(md0->pos());
    goaway->setEndValue(QPoint(-width(),0));
    connect(goaway,SIGNAL(finished()),md0,SLOT(hide()));

    come->setTargetObject(md4);
    come->setStartValue(pos());
    come->setEndValue(QPoint(0,0));
    md4->init();
    md4->move(width(),0);
    md4->show();

    goaway->start();
    come->start();
}

