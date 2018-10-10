#ifndef videoPlayer_H
#define videoPlayer_H

#include "qwidget.h"

#include "qevent.h"
#include <QObject>
#include <mpv/client.h>
#include "qmutex.h"
#include "qopengl.h"


class videoPlayer : public QWidget
{
    Q_OBJECT
public:
    videoPlayer( QWidget *parent = 0, QString videFile="");
    ~videoPlayer() ;
    void closePlayer();

private:
    void handle_mpv_event(mpv_event *event);
    mpv_handle *mpv;
    QMutex deleteLock;

private slots:
    void on_mpv_events();

signals:
    void mpv_events(void);

};



#endif // videoPlayer_H
