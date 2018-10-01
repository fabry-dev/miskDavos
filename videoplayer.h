#ifndef videoPlayer_H
#define videoPlayer_H

#include "qwidget.h"
#include <vlc/vlc.h>
#include "qevent.h"
#include <QObject>
#include <mpv/client.h>

class videoPlayer : public QWidget
{
    Q_OBJECT
public:
    videoPlayer( QWidget *parent = 0, QString videFile="");
    ~videoPlayer() ;


private:

    mpv_handle *mpv;




};



#endif // videoPlayer_H
