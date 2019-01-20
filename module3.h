#ifndef MODULE3_H
#define MODULE3_H

#include <QWidget>
#include <qobject.h>
#include "qlabel.h"
#include "picbutton.h"
#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"
#include "qcolor.h"
#include "circlebutton.h"
#include <QtSql>
#include <QSqlQuery>
#include "qstringlist.h"

class question:public QLabel
{

    Q_OBJECT
public:
    explicit question(QLabel *parent = nullptr,QString PATH="");

private:
    int id,answerId;
    QString PATH;
    mpvWidget *vp;
    std::vector<circleButton*> buttons;

    QLabel target,target2;
    void showTarget(uint choice);

    circleButton * cb0,*cb1,*cb2,*cb3,*cb4,*cb5,*cb6,*cb7;

    std::vector<QColor> colors;
    picButton *skipButton;
    QTimer *goHomeTimer;
protected:

    void resizeEvent(QResizeEvent *);


public slots:
    void showQuestion(int nuid);


private slots:
    void showChoice(int);
    void preProvideResults();
    void provideResults();
    void skip();

signals:
    void result(int questionid,int result);
};







class module3 : public QLabel
{
    Q_OBJECT
public:
    explicit module3(QLabel *parent = nullptr, QString PATH="");
    void init();
    mpvWidget *vp;
    void showQuestion(int id);
private:
    QString PATH;
    question * q;
    picButton *home,*home2;
    std::vector<int> answers;
    void initDb();
    void getData();
    void insertData();
    void loadStats();
    void displayResults();
    std::vector<QString> questionNames;
    std::vector<std::vector<int>> questionStats;
    QLabel *results;
    QTimer *goHomeTimer;


private slots:
    void getResult(int questionId,int answer);
signals:
    void goHome();
protected:
    void mousePressEvent(QMouseEvent *ev);

};

#endif // videoWindow_H
