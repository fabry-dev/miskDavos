#include "videoplayer.h"
#include "qdebug.h"
#include "mainwindow.h"








static inline void check_error(int status)
{
    if (status < 0) {
        qDebug()<<"mpv API error: "<<mpv_error_string(status);
        exit(1);
    }
}

videoPlayer::videoPlayer( QWidget *parent , QString videoFile ):QWidget(parent)
{

    this->show();
    this->raise();
    resize(QSize(1920,1080));


    std::setlocale(LC_NUMERIC, "C");
    mpv_handle *mpv = mpv_create();
    if (!mpv) {
       qDebug()<<"failed creating context";
    return;
    }



    int64_t wid = this->winId();
    check_error(mpv_set_option(mpv, "wid", MPV_FORMAT_INT64, &wid));
    check_error(mpv_set_option_string(mpv, "input-default-bindings", "yes"));
    check_error(mpv_set_option_string(mpv, "input-vo-keyboard", "no"));
    int val = 0;
    check_error(mpv_set_option(mpv, "osc", MPV_FORMAT_FLAG, &val));
    mpv_set_option_string( mpv, "loop", "inf");
    check_error(mpv_initialize(mpv));

    QByteArray ba = videoFile.toLatin1();
    const char *videoFile2 = ba.data();
    const char *cmd[] = {"loadfile", videoFile2, NULL};

    check_error(mpv_command(mpv, cmd));


}











videoPlayer::~videoPlayer()
{
 mpv_terminate_destroy(mpv);



}
