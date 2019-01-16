#ifndef MODULE2_H
#define MODULE2_H

#include <QWidget>
#include <qobject.h>
#include "qlabel.h"

#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"
#include "qcolor.h"


#include "circlebutton.h"








class module2 : public QLabel
{
    Q_OBJECT
public:
    explicit module2(QLabel *parent = nullptr, QString PATH="");

private:
    QString PATH;
    std::vector<circleButton*> bubbles;
    void loadCountries();
    std::vector<QString> countryNames;
    std::vector<QColor> colors;
    mpvWidget *vp;
    QPropertyAnimation *videoSlide;

private slots:
    void showVideo(int countryId);
    void getVideoClicked(QPoint pos);
    void hideVideo();

protected:
    void mousePressEvent(QMouseEvent *ev);


};

#endif
