#include "circlebutton.h"

circleButton::circleButton(QWidget *parent,QColor color,int id) : QWidget(parent),color(color),id(id)
{

    isClicked = false;

    trans = new QPropertyAnimation(this, "geometry");
    trans->setDuration(80);
    trans->setEasingCurve(QEasingCurve::InCurve);
}



void circleButton::resizeEvent(QResizeEvent *event)
{


}

void circleButton::paintEvent(QPaintEvent *event)
{



    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);


    painter.setPen(QPen(QColor(color), 0));

    painter.setBrush(QColor(color));
    painter.drawEllipse(QPointF(width()/2,height()/2),0.9*width()/2,0.9*height()/2);


}



void circleButton::mousePressEvent(QMouseEvent *ev)
{
    if(isClicked)
        return;

    disconnect(trans,0,0,0);

    isClicked = true;
    //circle center
    float x0 =(float) x()+width()/2;
    float y0 =(float) y()+height()/2;
    //new width
    float w1 =(float) width()*1.5;
    float h1 =(float) height()*1.5;
    //new coordinates
    float x1 =(float) x0-w1/2;
    float y1 =(float) y0-h1/2;

    initialGeometry = geometry();

    trans->setStartValue(QRect(geometry()));
    trans->setEndValue(QRect(x1,y1,w1,h1));
    trans->start();



}


void circleButton::mouseReleaseEvent(QMouseEvent *ev)
{
    if(!isClicked)
        return;

    disconnect(trans,0,0,0);
    trans->stop();
    trans->setStartValue(QRect(geometry()));
    trans->setEndValue(initialGeometry);
    trans->start();
    connect(trans,SIGNAL(finished()),this,SLOT(resetClick()));//when transformation is done, click is sent

}

void circleButton::resetClick()
{
    if(!isClicked)
        return;

    isClicked = false;

    emit clicked(id);
}
