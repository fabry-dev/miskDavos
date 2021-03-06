#ifndef MODULE0_H
#define MODULE0_H

#include <QWidget>
#include <qobject.h>
#include "qlabel.h"

#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"
#include "qcolor.h"
#include "qcombobox.h"
#include "picbutton2.h"
#include "circlebutton.h"
#include "picbutton.h"
#include "qmath.h"
#include "qlineedit.h"
#include "qstylefactory.h"

class module0 : public QLabel
{
    Q_OBJECT
public:
    explicit module0(QLabel *parent = nullptr, QString PATH="");

private:
    QString PATH;
    picButton2 *b2,*b3,*b4;
    picButton2 *b1;
    QPropertyAnimation * goawayTransition;


public slots:
    void disableButtons();
        void init();
signals:
    void goModule1();
    void goModule2();
    void goModule3();
    void goModule4();

protected:
    void mousePressEvent(QMouseEvent *ev);


};

#endif
