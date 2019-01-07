#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <qobject.h>
#include "qlabel.h"

#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"

class videoWindow : public QLabel
{
    Q_OBJECT
public:
    explicit videoWindow(QLabel *parent = nullptr, QString PATH="", int id=1);
    mpvWidget *vp;
    int id;
    void startVideo(QString videoUrl);
private:
    QString PATH;
    int page;


signals:
    void donePlaying();
    void updStatus(int id,bool status);

private slots:
    void playingOver();
    void initVideo();
    void goToPage(int page);

protected:
    void resizeEvent(QResizeEvent *event);

};

#endif // videoWindow_H
