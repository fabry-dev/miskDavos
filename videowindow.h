#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <qobject.h>
#include "qlabel.h"
#include "picbutton.h"
#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"

class videoWindow : public QLabel
{
    Q_OBJECT
public:
    explicit videoWindow(QLabel *parent = nullptr,QString PATH="");


    mpvWidget *vp;
private:
    QString PATH;
    QTimer *saverTimer;
    int range;
    bool stopped;

signals:
    void donePlaying();

private slots:
    void goSaverVideo();
    void startQuestionVideo(uint question);
    void playingOver();
    void unblockPlayer();
    void startIntro();
    void startVideo(QString videoUrl);
};

#endif // videoWindow_H
