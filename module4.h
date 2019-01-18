#ifndef MODULE4_H
#define MODULE4_H

#include <QWidget>
#include <qobject.h>
#include "qlabel.h"

#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"
#include "qcolor.h"
#include "qcombobox.h"

#include "circlebutton.h"

#include "qmath.h"
#include "QTime"
#include "qtextdocument.h"
#include "QAbstractTextDocumentLayout"
#include "qgraphicsscene.h"
#include "QGraphicsView"
#include "QGraphicsTextItem"
#include "QGraphicsItem"
#include "customslider.h"
#include "picbutton.h"
#include <QtSql>
#include <QSqlQuery>
#include "QLineEdit"




class lineEdit2: public QLineEdit
{
    Q_OBJECT
public:
    lineEdit2(QWidget *parent = nullptr):QLineEdit(parent)
    {installEventFilter(this);}

protected:
    /*  void focusInEvent(QFocusEvent *e)
    {

    }*/
    bool eventFilter(QObject *object, QEvent *event)
    {
        if (event->type() == QEvent::FocusIn)
        {
            emit clicked(this);
        }

        return false;
    }

signals:
    void clicked(lineEdit2*);
};







class com:public QObject
{
    Q_OBJECT
public:
    com(){}
    void sendMoved(int id,QPointF pos)
    {
        emit moved(id,pos);
    }
signals:
    void moved(int id,QPointF pos);
};

class bubbleItem : public QGraphicsItem
{

public:
    bubbleItem(QSize size,QString text="",int id=0,QColor color=Qt::red):text(text),id(id),color(color)
    {
        rank = -1;
        bounding = QRectF(-size.width()/2,-size.height()/2,size.width(),size.height());
        setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);

    }
    QRectF boundingRect() const override
    {

        return bounding;
    }
    void setBoundingRect(QSize size)
    {
        bounding = QRectF(-size.width()/2,-size.height()/2,size.width(),size.height());
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

    void setRank(int nurank)
    {
        rank = nurank;
        update();
    }
private:
    QString text;
    int id;
    QColor color;
    QRectF bounding;
    int rank;
public:
    com communicator;

protected:
    virtual QVariant    itemChange ( GraphicsItemChange change, const QVariant & value )
    {
        if (change == ItemPositionChange && scene()) {
            // value is the new position.
            QPointF newPos = value.toPointF();


            QRectF rect = scene()->sceneRect();
            if(newPos.x()+boundingRect().width()/2>rect.width())
                newPos.setX(rect.width()-boundingRect().width()/2);
            if(newPos.x()-boundingRect().width()/2<0)
                newPos.setX(boundingRect().width()/2);
            if(newPos.y()+boundingRect().height()/2>rect.height())
                newPos.setY(rect.height()-boundingRect().height()/2);
            if(newPos.y()-boundingRect().height()/2<0)
                newPos.setY(boundingRect().height()/2);

            communicator.sendMoved(id,newPos);
            return newPos;
        }


        return QGraphicsItem::itemChange(change, value);
    }

};

class skillTab:public QLabel
{
    Q_OBJECT
public:
    explicit skillTab(QLabel *parent, QString PATH="", int id=0):QLabel(parent),PATH(PATH),id(id)
    {}

private:
    QString PATH;
    int id;
public:
    void setText();
};

class module4 : public QLabel
{
    Q_OBJECT
public:
    explicit module4(QLabel *parent = nullptr, QString PATH="");
    void init();
private:
    QString PATH;
    void loadSkillList();
    QStringList skillList;
    std::vector<bubbleItem*> bubbles;
    std::vector<skillTab*> targets;
    std::vector<QColor> colors;
    std::vector<int> ranking;

    QGraphicsView *mainView;
    QGraphicsScene *mainScene;
    picButton *nextButton,*nextButton2;
    customSlider *cs;
    int activeQuestion;
    std::vector<QPixmap> bgPixS;
    std::vector<int> score2,score1;
    std::vector<picButton*> keyboard;
    void setupKeyboard();
    void initDb();
    void getData();
    void insertData();

    std::vector<lineEdit2*> inputs;
    lineEdit2* activeInput;

    std::vector<QRegularExpression> exps;
    picButton *submit;

    picButton *home;


private slots:
    void bubbleMoved(int id,QPointF pos);
    void goSecondPart();
    void showNextQuestion();
    void goKeyboard();
    void getKey(QString key);
    void selectInput(lineEdit2* ln);
    void submitInputs();

protected:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void goHome();
};

#endif
