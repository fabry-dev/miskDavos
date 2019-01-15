#ifndef serialWatcher_H
#define serialWatcher_H

#include <QObject>
#include <QFile>
#include <QSocketNotifier>
#include "qdebug.h"
#include <QSerialPort>
#include "qtimer.h"

class serialWatcher
      : public QObject
{
    Q_OBJECT



public:
    explicit serialWatcher(QObject *parent = nullptr, int pageTops=10, int timeoutValue=10000);
    ~serialWatcher();
private:
    int pageTops;
    int timeoutValue;
    QSerialPort *port;
    int tops;
    int page;
    std::vector<bool> pageStatus;
    bool ready;
    bool saving;
    QTimer *savingTimer;

private slots:
    void readData();
    void getStatus(int,bool);
    void timeOut();

signals:
    void exitTimeOut();
    void gotTimeOut();
    void goForward();
    void goBackward();
    void nuPage(int);
};

#endif // serialWatcher_H
