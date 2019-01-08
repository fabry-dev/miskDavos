#ifndef serialWatcher_H
#define serialWatcher_H

#include <QObject>
#include <QFile>
#include <QSocketNotifier>
#include "qdebug.h"
#include <QSerialPort>
class serialWatcher
      : public QObject
{
    Q_OBJECT



public:
    explicit serialWatcher(QObject *parent = nullptr,int pageTops=10);
    ~serialWatcher();
private:
    int pageTops;
    QSerialPort *port;
    int tops;
    int page;
    std::vector<bool> pageStatus;
    bool ready;

private slots:
    void readData();
    void getStatus(int,bool);

signals:
    void goForward();
    void goBackward();
    void nuPage(int);
};

#endif // serialWatcher_H
