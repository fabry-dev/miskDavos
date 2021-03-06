#ifndef CIRCLEBUTTON_H
#define CIRCLEBUTTON_H

#include <QWidget>
#include "qpainter.h"
#include "qdebug.h"
#include "qcolor.h"
#include "qtimer.h"
#include "qpropertyanimation.h"


class circleButton : public QWidget
{
    Q_OBJECT
public:
    explicit circleButton(QWidget *parent = nullptr, QColor color=Qt::red, int id=0, QString txt="", bool animate=false);
    void setColor(QColor nuColor)
    {
        color = nuColor;
        update();
    }

private:
    QColor color;
    int id;
    QString txt;
    QPropertyAnimation *trans;
    QRectF initialGeometry;
    bool isClicked;
     QPropertyAnimation *anim1,*anim2;

public slots:

protected:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent( QMouseEvent* ev );
    void mouseReleaseEvent(QMouseEvent * event);

signals:
    void clicked(int);

private slots:
    void resetClick();
};

#endif // CIRCLEBUTTON_H
