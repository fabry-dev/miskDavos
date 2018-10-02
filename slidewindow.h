#ifndef slidewindow_H
#define slidewindow_H


#include "qobject.h"
#include "qpainter.h"
#include "qlabel.h"
#include "qthread.h"
#include "qevent.h"
#include <QMouseEvent>
#include "qstringlist.h"
#include "mainwindow.h"
#include "QTime"
#include "qwindow.h"
#include "qscreen.h"
#include "qwidget.h"

class slideWindow:public QObject
{
    Q_OBJECT

public:
    slideWindow(QObject *parent = 0, QString PATH="", QList<QWidget*> screenList={}, int x0=0, int totalWidth=1920, QString filename="", int deltax=1,int id=0);
    ~slideWindow();


private:
    QString PATH;
    QPixmap bgImg;
    QList<QWidget*> screenList;
    QList<QLabel*> lbls;

    int x0;
    int totalWidth;
    QString filename;
    int deltax;
    int id;
    void redraw();
private slots:
    void goBackward();
    void goForward();
};








#endif
