#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include "qobject.h"
#include "qgraphicsscene.h"
#include <QtWidgets>
#include <QtCore/qmath.h>
#include <QtCore/qstate.h>
#include "QGraphicsView"
#include "QGraphicsPixmapItem"





class movingCursor : public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    movingCursor(QGraphicsItem * parent = 0);
    void setY0(int nuy0);

protected:
        QVariant itemChange ( GraphicsItemChange change, const QVariant & value);
private:
        int y0;

signals:
    void valueChanged(double nuValue);
};


class customSlider : public QGraphicsView
{
    Q_OBJECT
public:
   customSlider(QWidget *parent = 0, QString PATH="",QRect geo=QRect(0,0,100,100)) ;
     void reset();
     double getValue();
     void setCursorColor(int color);
private:
   QFont font;
   QString PATH;
   QPixmap cursorPix;
   std::vector<QPixmap> cursorPixS;
   QGraphicsScene *scene;   
   movingCursor  *item;
   int question;

private slots:

signals:
    void valueChanged(double nuValue);


};




#endif // CUSTOMSLIDER_H
