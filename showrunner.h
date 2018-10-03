#ifndef RFIDReader_H
#define RFIDReader_H

#include <QObject>
#include <QFile>
#include <QSocketNotifier>
#include "qdebug.h"
#include "qtimer.h"
#include "qwidget.h"
#include "slidevideo.h"
#include "slidewindow.h"
#include "serialwatcher.h"












class showRunner
        : public QObject
{
    Q_OBJECT

    QString fileName = "/dev/input/by-id/usb-SpringCard_ProxNRoll_RFID_Scanner_517F095D-event-kbd";
    QSocketNotifier *notifier;
    int fd;
    QTimer *RFIDtimeout;
    QList <QWidget*> widgetList;
    QString PATH;
    int speed;
    serialWatcher *serialwatch;
    std::vector<slidevideo*> videos;
    std::vector<slideWindow*> photos;
    std::vector<QLabel *> bgLbls;
    QPixmap bgImg;

    std::vector<uchar> code0,code1,code2,codeBuf;

       videoPlayer *testvp;
       std::vector<videoPlayer *> testvps;
       slidevideo *testslide;

private:
    int activeShow;
    int getVideoWidth(QString name);

public:
    explicit showRunner(QObject *parent = nullptr, QList <QWidget*> widgetList={}, QString PATH="", int speed=10, serialWatcher *serialwatch=NULL);
    ~showRunner();

public slots:
    void handle_readNotification(int socket);


private slots:
    void onTimeout(void);
    void startShow(int show);
    void stopShow();




};

#endif // RFIDReader_H
