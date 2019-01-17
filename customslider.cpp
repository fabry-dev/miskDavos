#include "customslider.h"

#define xMin  515
#define xMax  3174



#define FONTSIZE 20

customSlider::customSlider(QWidget *parent, QString PATH,QRect geo)  :QGraphicsView(parent),PATH(PATH)
{

    setGeometry(geo);
    scene = new QGraphicsScene();
    scene->setSceneRect(QRect(0,0,geo.width(),geo.height()));

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);




    setStyleSheet("background-color: transparent;");

    for(int i = 0;i<5;i++)
    {

        cursorPixS.push_back(QPixmap(PATH+"cursor"+QString::number(i+1)+".png"));

    }

    cursorPix.load(PATH+"cursor1.png");
    cursorPix = cursorPix.scaledToHeight(150);
    int w = cursorPix.width();
    int h = cursorPix.height();

    item = new movingCursor(NULL);


    scene->addItem(item);//
    setScene(scene);
    connect(item,SIGNAL(valueChanged(double)),this,SIGNAL(valueChanged(double)));


    item->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemSendsScenePositionChanges);
    item->setY0((height()-h)/2+80);
    item->setPos((width()-w)/2,item->y());
    setCursorColor(1);


}


void customSlider::reset()
{


    question = -1;
    item->setPos((width()-item->pixmap().width())/2,item->y());

}


void customSlider::setCursorColor(int i)
{
if(i<0)
    return;
if(i>=5)
    return;


    item->setPixmap(cursorPixS[i]);

}


double customSlider::getValue()
{
    double val = (double)(item->x()-xMin)/(xMax-xMin);
    return (int)100*val;
}








movingCursor::movingCursor(QGraphicsItem * parent):QGraphicsPixmapItem(parent)
{

    y0 = y();




}

void movingCursor::setY0(int nuy0)
{
    y0 = nuy0;

}


QVariant movingCursor::itemChange(GraphicsItemChange change, const QVariant &value)
{

    if (change == ItemPositionChange)
    {

        QPointF newPos = value.toPointF();
        if(newPos.x()<xMin)
            newPos.setX(xMin);
        else if(newPos.x()>xMax)
            newPos.setX(xMax);


        newPos.setY(y0);


        //compute %:
        double val = (double)(newPos.x()-xMin)/(xMax-xMin);

        emit valueChanged(val);

        return QGraphicsItem::itemChange(change, newPos);

    }

    return QGraphicsItem::itemChange(change, value);
}
