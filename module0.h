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
    void init();
private:
    QString PATH;
    picButton *b1,*b2,*b3,*b4;
    QPropertyAnimation * goawayTransition;


private slots:
    void disableButtons();

signals:
    void goModule1();
    void goModule2();
    void goModule3();
    void goModule4();

protected:
    void mousePressEvent(QMouseEvent *ev);


};

#endif
