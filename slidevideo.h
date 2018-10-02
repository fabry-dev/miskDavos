#ifndef slidevideo_H
#define slidevideo_H


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
#include "videoplayer.h"



class slidevideo:public QObject
{
    Q_OBJECT

public:
    slidevideo(QObject *parent = 0, QString PATH="", QList<QWidget*> screenList={}, int x0=0, int w0=1920, int totalWidth=1920, QString filename="",int deltax=1);
    ~slidevideo();
    void shutdown();


private:
    QString PATH;
   QList<QWidget*> screenList;
    std::vector<videoPlayer*> lbls;

    int x0,w0;
    int totalWidth;
    QString filename;
    int deltax;
    void redraw();
    bool closed;

private slots:
    void goBackward();
    void goForward();

};








#endif
