#include "picbutton2.h"

picButton2::picButton2(QWidget *parent, uint w, QString file, QString clickFile, QString msg):QLabel(parent),msg(msg)
{

    //setStyleSheet("border: 1px solid black");
    on = false;
    pic.load(file);
    if(clickFile!="")
        onpic.load(clickFile);
    else
        onpic=pic.copy();

    if(w>0)
    {
        pic = pic.scaledToWidth(w);
        onpic = onpic.scaledToWidth(w);
    }



    button = new QLabel(this);
    button->resize(pic.size());
    button->show();

    resize(button->size());




    button->setPixmap(pic);

    enable();





    anim1 = new QPropertyAnimation(this, "size");

    anim1->setDuration(1000);
    anim1->setEasingCurve(QEasingCurve::InCurve);
    anim1->setStartValue(size());
    anim1->setEndValue(size()*1.1);



    anim2 = new QPropertyAnimation(this, "size");

    anim2->setDuration(1000);
    anim2->setEasingCurve(QEasingCurve::InCurve);
    anim2->setEndValue(size());
    anim2->setStartValue(size()*1.1);



    connect(anim1,SIGNAL(finished()),anim2,SLOT(start()));
    connect(anim2,SIGNAL(finished()),anim1,SLOT(start()));
    anim1->start();


}


void picButton2::resizeEvent(QResizeEvent *event)
{
    button->resize(size());
    if(on)
        button->setPixmap(onpic.scaled(button->size()));
    else
        button->setPixmap(pic.scaled(button->size()));
}

void picButton2::moveEvent(QMoveEvent *event)
{

}







void picButton2::enable()
{
    enabled = true;

}

void picButton2::disable()
{

    enabled = false;
}

void picButton2::resetClick()
{
    if(!enabled)
        return;
    button->setPixmap(pic);
    on = false;
    emit clicked(msg);
}


void picButton2::mousePressEvent(QMouseEvent *ev)
{

    if(!enabled)
        return;
    button->setPixmap(onpic);
    on = true;
}



void picButton2::mouseReleaseEvent(QMouseEvent *event)
{
    if(!enabled)
        return;
    QTimer::singleShot(100,this,SLOT(resetClick()));

}
