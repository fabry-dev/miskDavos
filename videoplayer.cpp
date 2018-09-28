#include "videoplayer.h"
#include "qdebug.h"
#include "mainwindow.h"






void callbacks( const libvlc_event_t* event, void* ptr )
{
    videoPlayer *self = reinterpret_cast<videoPlayer*>( ptr );
    switch ( event->type )
    {
    case libvlc_MediaPlayerEndReached:
        emit (self->endReached());

        break;
    default:
        break;
    }


}



videoPlayer::videoPlayer( QWidget *parent , QString videoFile ):QWidget(parent)
{


    // Load the VLC engine
    videoInst = libvlc_new(0, NULL);

    // Create a new item


    //videoFile = PATH+"video.mov";
    videoM = libvlc_media_new_path (videoInst,videoFile.toStdString().c_str());
    //libvlc_media_add_option(videoM, "input-repeat=-1");
    libvlc_media_add_option(videoM, ":no-audio");
    libvlc_media_add_option(videoM,":avcodec-hw=dxva2");
    // Create a media player playing environement
    videoMp = libvlc_media_player_new_from_media (videoM);
    m_eventMgr = libvlc_media_player_event_manager( videoMp );
    libvlc_event_attach( m_eventMgr, libvlc_MediaPlayerEndReached,      callbacks, this );



    //    libvlc_media_player_event_manager()
    libvlc_video_set_mouse_input(videoMp, false);
    libvlc_media_player_set_xwindow(videoMp, this->winId());

    setWindowState(Qt::WindowFullScreen);
    // setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    //QWidget::show()
    this->show();
    this->raise();
    resize(QSize(1920,1080));
    libvlc_media_player_play (videoMp);

}



void videoPlayer::restart(void)
{
    qDebug()<<"restart";
    libvlc_media_player_set_media(videoMp,videoM );

    libvlc_media_player_play (videoMp);

}





videoPlayer::~videoPlayer()
{

    libvlc_media_release (videoM);
    libvlc_media_player_release (videoMp);
    libvlc_release (videoInst);
    qDebug()<<"video player stopped";


}
