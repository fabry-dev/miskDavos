#include "videoplayer.h"
#include "qdebug.h"
#include "mainwindow.h"
#include "qtimer.h"
#define HW true





static inline void check_error(int status)
{
    if (status < 0) {
        qDebug()<<"mpv API error: "<<mpv_error_string(status);
        //  exit(1);
    }
}


/*


static void wakeup(void *ctx)
{
    // This callback is invoked from any mpv thread (but possibly also
    // recursively from a thread that is calling the mpv API). Just notify
    // the Qt GUI thread to wake up (so that it can process events with
    // mpv_wait_event()), and return as quickly as possible.

    videoPlayer *vp = (videoPlayer *)ctx;
    emit vp->mpv_events();
}



*/


videoPlayer::videoPlayer( QWidget *parent , QString videoFile ):QWidget(parent)
{
    deleteLock.unlock();

    this->show();
    this->raise();

    setAttribute(Qt::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WA_NativeWindow);
    //resize(QSize(1920,1080));


    std::setlocale(LC_NUMERIC, "C");
    mpv = mpv_create();
    if (!mpv) {
        qDebug()<<"failed creating context";
        return;
    }
    mpv_request_log_messages(mpv,"no");


    int64_t wid = this->winId();
    check_error(mpv_set_option(mpv, "wid", MPV_FORMAT_INT64, &wid));
    //check_error(mpv_set_option_string(mpv, "input-default-bindings", "yes"));
    check_error(mpv_set_option_string(mpv, "input-vo-keyboard", "no"));

    if(HW)
    {
        check_error(mpv_set_option_string(mpv, "vo", "opengl"));
        check_error(mpv_set_option_string(mpv, "hwdec", "auto"));
    }
    int val = 0;
    check_error(mpv_set_option(mpv, "osc", MPV_FORMAT_FLAG, &val));
    mpv_set_option_string( mpv, "loop", "inf");


    /* connect(this, SIGNAL(mpv_events()), this, SLOT(on_mpv_events()),
            Qt::QueuedConnection);


    mpv_set_wakeup_callback(mpv, wakeup, this);

*/
    if(mpv_initialize(mpv)<0)
        qDebug()<<"could not initialize MPV";
    else
        qDebug()<<"mpv initialized";

    QByteArray ba = videoFile.toLatin1();
    const char *videoFile2 = ba.data();
    const char *cmd[] = {"loadfile", videoFile2, NULL};

    qDebug()<<"load "<<mpv_command(mpv, cmd);


}






void videoPlayer::handle_mpv_event(mpv_event *event)
{

    switch (event->event_id) {

    case MPV_EVENT_SHUTDOWN:
    {
        qDebug() << "MPV player closed";

        //deleteLock.lock();
        //disconnect(this);
        //mpv_detach_destroy(mpv);
        //mpv=NULL;

        // QTimer::singleShot(3000,this,SLOT(deleteLater()));
        qDebug()<<"MPV player destroyed";
        break;
    }
    default: ;
        // Ignore uninteresting or unknown events.
    }
}



void videoPlayer::on_mpv_events()
{

    // Process all events, until the event queue is empty.
    while (mpv) {

        deleteLock.lock();
        mpv_event *event = mpv_wait_event(mpv, 0);
        deleteLock.unlock();
        if (event->event_id == MPV_EVENT_NONE)
            break;

        handle_mpv_event(event);

    }

}




void videoPlayer::closePlayer()
{


    qDebug()<<"Starting to delete video player";
    const char *cmd[] = {"quit", "1", NULL};
    //check_error(mpv_command(mpv, cmd));
    mpv_terminate_destroy(mpv);

    this->deleteLater();
}






videoPlayer::~videoPlayer()
{
    qDebug()<<"video player deleted";

}
