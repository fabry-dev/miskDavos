#include "module3.h"
#include "qdebug.h"



module3::module3(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{

    showFullScreen();
    resize(3840,2160);
    initDb();

    q = new question(this,PATH);
    connect(q,SIGNAL(result(int,int)),this,SLOT(getResult(int,int)));
    q->resize(size());
    q->hide();

    home = new picButton(this,0,PATH+"home.png",PATH+"homeOn.png","");
    connect(home,SIGNAL(clicked(QString)),this,SIGNAL(goHome()));
    home->move(width()-50-home->width(),50);
    home->show();
    home->raise();


}


void module3::init()
{
 answers.clear();
 showQuestion(1);

}




void  module3::initDb()
{

    const QString DRIVER("QSQLITE");
    QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
    db.setHostName("hostname");
    db.setDatabaseName(PATH+"module3");
    db.setUserName("user");
    db.setPassword("password");

    if(!db.open())
        qWarning() << "ERROR: " << db.lastError();

    qDebug()<<db.tables();


    QSqlQuery query("CREATE TABLE players (id INTEGER PRIMARY KEY AUTOINCREMENT, q1 INTEGER, q2 INTEGER, q3 INTEGER, q4 INTEGER, q5 INTEGER, q6 INTEGER, q7 INTEGER, q8 INTEGER, q9 INTEGER, date TIMESTAMP)");

    bool success = query.exec();

    if(!success)
    {
        qDebug() << query.lastError();
    }
    else qDebug() << "table created";

}

void module3::insertData()
{
    QDateTime timestamp = QDateTime::currentDateTime();

    QSqlQuery query;
    query.prepare("INSERT INTO players (q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14, date) VALUES (:q1,:q2,:q3,:q4,:q5,:q6,:q7,:q8,:q9,:q10,:q11,:q12,:q13,:q14,:datetime)");

    for(int i = 0;i<answers.size();i++)
        query.bindValue(":q"+QString::number(i+1), answers.at(i));

    query.bindValue(":datetime", timestamp.toString("yyyy-MM-dd hh:mm:ss"));


    bool success = query.exec();
    if(!success)
    {
        qDebug() << query.lastError();
    }


   getData();
}

void module3::getData()
{

    QSqlQuery query;
    query.prepare("SELECT q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14,date FROM players");
    query.exec();

    std::vector<int> buf;
    buf.resize(14,-1);

    while (query.next()) {
        for(int i = 0;i<14;i++)
        buf[i] = query.value(i).toInt();

        QDateTime date = query.value(9).toDateTime();
        qDebug()<<date<<buf;
    }
}




void module3::getResult(int questionId, int answer)
{
    qDebug()<<questionId<<answer;
    answers.push_back(answer);
    q->hide();
    q->showQuestion(1);
}




void module3::showQuestion(int id)
{
    q->showQuestion(id);
}




question::question(QLabel *parent, QString PATH):QLabel(parent),PATH(PATH)
{

    vp = new mpvWidget(this);
    vp->resize(size());
    vp->setLoop(false);
    vp->setProperty("pause", false);
    vp->setProperty("keep-open",true);
    vp->show();

    int yb = 1731;
    int yh = 1129;

    int y0;
    int d0 = 220;

    circleButton * cb0 = new circleButton(this,QColor::fromRgb(0xFA,0xA9,0x2A),0);
    buttons.push_back(cb0);
    int x0 = 491;y0 = yb;
    cb0->move(x0-d0/2,y0-d0/2);

    circleButton * cb1 = new circleButton(this,QColor::fromRgb(0xF4,0x79,0x41),1);
    buttons.push_back(cb1);
    x0 = 979;y0 = yh;
    cb1->move(x0-d0/2,y0-d0/2);

    circleButton * cb2 = new circleButton(this,QColor::fromRgb(0x06,0x71,0xB8),2);
    buttons.push_back(cb2);
    x0 = 1440;y0 = yb;
    cb2->move(x0-d0/2,y0-d0/2);


    circleButton * cb3 = new circleButton(this,QColor::fromRgb(0xFC,0x48,0x61),3);
    buttons.push_back(cb3);
    x0 = 1924;y0 = yh;
    cb3->move(x0-d0/2,y0-d0/2);

    circleButton * cb4 = new circleButton(this,QColor::fromRgb(0xB7,0x47,0xB5),4);
    buttons.push_back(cb4);
    x0 = 2408; y0 = yb;
    cb4->move(x0-d0/2,y0-d0/2);


    circleButton * cb5 = new circleButton(this,QColor::fromRgb(0x8E,0x32,0x8B),5);
    buttons.push_back(cb5);
    x0 = 2894; y0 = yh;
    cb5->move(x0-d0/2,y0-d0/2);

    circleButton * cb6 = new circleButton(this,QColor::fromRgb(0x89,0x8E,0x91),6);
    buttons.push_back(cb6);
    x0 = 3373; y0 = yb;
    cb6->move(x0-d0/2,y0-d0/2);


    for (auto b:buttons)
    {
        b->resize(d0,d0);
        b->hide();

        connect(b,SIGNAL(clicked(int)),this,SLOT(showChoice(int)));
    }



    target.setParent(this);
    target.hide();


    show();





}

void question::showChoice(int choice)
{
    answerId = choice;
    disconnect(vp,SIGNAL(videoOver()),0,0);
    for (auto b:buttons)
        b->hide();

    connect(vp,SIGNAL(videoOver()),this,SLOT(provideResults()));
    vp->loadFile(PATH+"question"+QString::number(id)+"stats.mp4");

    showTarget(choice);
}

void question::showTarget(uint choice)
{
    if((choice<0)||(choice>=buttons.size()))
        return;

    int d0 = buttons[choice]->width();

    target.resize(2.3*d0,2.3*d0);

    //compute corresponding circle center
    int x0 = buttons[choice]->x()+buttons[choice]->width()/2;
    int y0 = buttons[choice]->y()+buttons[choice]->height()/2;

    target.move(x0-target.width()/2,y0-target.height()/2);

    target.setPixmap(QPixmap(PATH+"target.png").scaledToHeight(target.height()));

    target.show();
}

void question::showQuestion(int nuid)
{

    target.hide();
    id = nuid;

    disconnect(vp,SIGNAL(videoOver()),0,0);

    for(auto b:buttons)
    {
        b->hide();
        connect(vp,SIGNAL(videoOver()),b,SLOT(show()));
    }
    vp->loadFile(PATH+"question"+QString::number(id)+".mp4");

     show();
}

void question::provideResults()
{
    emit result(id,answerId);
}

void question::resizeEvent(QResizeEvent *)
{
    vp->resize(size());
}

