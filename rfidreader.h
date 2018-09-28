#ifndef RFIDReader_H
#define RFIDReader_H

#include <QObject>
#include <QFile>
#include <QSocketNotifier>
#include "qdebug.h"

class RFIDReader
      : public QObject
{
    Q_OBJECT

    QString fileName = "/dev/input/event0";
    QSocketNotifier *notifier;
    int fd;

public:
    explicit RFIDReader(QObject *parent = nullptr);
    ~RFIDReader();

public slots:
    void handle_readNotification(int socket);
};

#endif // RFIDReader_H
