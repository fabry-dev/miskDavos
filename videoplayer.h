#ifndef videoPlayer_H
#define videoPlayer_H

#include "qwidget.h"
#include <vlc/vlc.h>
#include "qevent.h"
#include <QObject>


class videoPlayer : public QWidget
   {
       Q_OBJECT
   public:
       videoPlayer( QWidget *parent = 0, QString videFile="");
       ~videoPlayer() ;

       // event manager
       libvlc_event_manager_t *m_eventMgr;

       // event handler callback
       static void vlcEvent(const libvlc_event_t* event, void* vlcvideo_object);

   private:
       libvlc_instance_t * videoInst;
       libvlc_media_player_t *videoMp;
       libvlc_media_t *videoM;

    signals:
        void endReached();
    private slots:
        void restart();


   };



#endif // videoPlayer_H
