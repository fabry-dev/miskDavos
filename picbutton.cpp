#include "picbutton.h"

picButton::picButton(QWidget *parent, uint w, QString file, QString clickFile, QString msg,bool animate):QLabel(parent),msg(msg),animate(animate)
{
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

    resize(pic.size());
    setPixmap(pic);

    enable();


}


void picButton::moveEvent(QMoveEvent *event)
{

}


void picButton::animationOver()
{

}




void picButton::enable()
{
    enabled = true;

}

void picButton::disable()
{

    enabled = false;
}

void picButton::resetClick()
{
    if(!enabled)
        return;
    setPixmap(pic);
    emit clicked(msg);
}


void picButton::mousePressEvent(QMouseEvent *ev)
{

    if(!enabled)
        return;
    setPixmap(onpic);
}



void picButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(!enabled)
        return;
    QTimer::singleShot(100,this,SLOT(resetClick()));

}
