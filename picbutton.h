#ifndef PICBUTTON_H
#define PICBUTTON_H

#include "qobject.h"
#include "qlabel.h"
#include "qdebug.h"
#include "qpropertyanimation.h"
#include "QMouseEvent"
#include "qpalette.h"
#include "qtimer.h"

class picButton : public QLabel
{
    Q_OBJECT
public:
    picButton(QWidget *parent,uint w,QString file,QString clickFile="",QString msg="");

private:
    QPixmap pic,onpic;
    QString msg;
    bool enabled;
protected:
    void mousePressEvent( QMouseEvent* ev );
    void mouseReleaseEvent(QMouseEvent * event);
signals:
    void clicked(QString msg);
private slots:
    void resetClick();
public slots:
    void enable();
    void disable();
};

#endif // PICBUTTON_H
