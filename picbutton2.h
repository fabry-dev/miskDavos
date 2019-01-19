#ifndef picButton2_H
#define picButton2_H

#include "qobject.h"
#include "qlabel.h"
#include "qdebug.h"
#include "qpropertyanimation.h"
#include "QMouseEvent"
#include "qpalette.h"
#include "qtimer.h"
#include "qpropertyanimation.h"






class picButton2 : public QLabel
{
    Q_OBJECT
public:
    picButton2(QWidget *parent, uint w, QString file, QString clickFile="", QString msg="");

private:
    QPixmap pic,onpic;
    QString msg;
    bool enabled;
      QLabel *button;
     QPropertyAnimation *anim1,*anim2;
     bool on;

protected:
    void mousePressEvent( QMouseEvent* ev );
    void mouseReleaseEvent(QMouseEvent * event);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
signals:
    void clicked(QString msg);
private slots:
    void resetClick();

public slots:
    void enable();
    void disable();
};

#endif // picButton2_H
