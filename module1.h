#ifndef MODULE1_H
#define MODULE1_H

#include <QWidget>
#include <qobject.h>
#include "qlabel.h"

#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"
#include "qcolor.h"
#include "qcombobox.h"

#include "circlebutton.h"
#include "picbutton.h"
#include "qmath.h"
#include "qlineedit.h"
#include "qstylefactory.h"
#include "qpropertyanimation.h"
#include "QGraphicsOpacityEffect"
#include "picbutton2.h"

class module1 : public QLabel
{
    Q_OBJECT
public:
    explicit module1(QLabel *parent = nullptr, QString PATH="");
    void init();
private:
    QString PATH;
    mpvWidget *vp,*vp2;
    picButton *home;
    QPropertyAnimation *videoSlide;
    picButton2 *b1,*b2;
    picButton *skip,*skip2;
    QPropertyAnimation *animVp;
        QTimer *goHomeTimer;
private slots:
    void showVideo(QString video);
    void hideVideo();
protected:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void goHome();

};

#endif
