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

    QLabel target;
    void showTarget(uint choice);


protected:
    void resizeEvent(QResizeEvent *);


public slots:
    void showQuestion(int nuid);

private slots:
    void showChoice(int);
    void provideResults();

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
    picButton *home;
    std::vector<int> answers;
    void initDb();
    void getData();
    void insertData();
private slots:
    void getResult(int questionId,int answer);
signals:
    void goHome();

};

#endif // videoWindow_H
