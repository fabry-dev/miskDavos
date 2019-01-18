#include "module3.h"
#include "qdebug.h"


QStringList answersTxt = QStringList() << "Strongly disagree" << "Somewhat disagree"<<"Neither disagree nor agree"<<"Somewhat agree"<<"Strongly agree"<<"Don't know"<<"N/A" ;

module3::module3(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{

    showFullScreen();
    resize(3840,2160);
    initDb();

    loadStats();


    q = new question(this,PATH);
    connect(q,SIGNAL(result(int,int)),this,SLOT(getResult(int,int)));
    q->resize(size());
    q->hide();

    home = new picButton(this,0,PATH+"home.png",PATH+"homeOn.png","");
    connect(home,SIGNAL(clicked(QString)),this,SIGNAL(goHome()));
    home->move(width()-50-home->width(),50);
    home->show();
    home->raise();

    results = new QLabel(this);
    results->resize(size());
    results->hide();
    results->raise();

}


void module3::init()
{

    results->hide();

    answers.clear();

    showQuestion(1);
}




void  module3::initDb()
{


    QSqlQuery query("CREATE TABLE module3 (id INTEGER PRIMARY KEY AUTOINCREMENT, q1 INTEGER, q2 INTEGER, q3 INTEGER, q4 INTEGER, q5 INTEGER, q6 INTEGER, q7 INTEGER, q8 INTEGER, q9 INTEGER, q10 INTEGER, q11 INTEGER, q12 INTEGER, q13 INTEGER, q14 INTEGER,date TIMESTAMP)");

    bool success = query.exec();

    if(!success)
    {
        qDebug() << query.lastError();
    }
    else qDebug() << "table created";

}

void module3::insertData()
{
    qDebug()<<"insert";
    QDateTime timestamp = QDateTime::currentDateTime();

    QSqlQuery query;
    query.prepare("INSERT INTO module3 (q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14, date) VALUES (:q1,:q2,:q3,:q4,:q5,:q6,:q7,:q8,:q9,:q10,:q11,:q12,:q13,:q14,:datetime)");

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
    qDebug()<<"get";
    QSqlQuery query;
    query.prepare("SELECT q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14,date FROM module3");
    query.exec();

    std::vector<int> buf;
    buf.resize(14,-1);

    while (query.next()) {
        for(int i = 0;i<14;i++)
            buf[i] = query.value(i).toInt();

        QDateTime date = query.value(14).toDateTime();
        qDebug()<<date<<buf;
    }
}

void module3::getResult(int questionId, int answer)
{
    qDebug()<<questionId<<answer;
    answers.push_back(answer);
    q->hide();

    if(questionId<14)
        q->showQuestion(questionId+1);
    else
    {
        qDebug()<<"questionnaire over";
        insertData();
        displayResults();

    }
}




void module3::showQuestion(int id)
{
    q->showQuestion(id);
}



void module3::loadStats()
{

    QString questionName;
    std::vector<int> stats;

    bool test;
    QFile file(PATH+"answers.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no config file";

    }
    else
    {

        QTextStream in(&file);

        QString  line;
        QStringList params;

        while(!in.atEnd()) {
            stats.clear();
            line = in.readLine();
            params = (line.split(","));

            if(params.size()<2)
                break;

            questionName = params[0];

            for(int i = 1;i<params.size();i++)
            {
                stats.push_back(params[i].toInt(&test));
            }


            questionStats.push_back(stats);
            questionNames.push_back(questionName);
        }
        file.close();

    }


}


void module3::displayResults()
{


    QPixmap pix = QPixmap(PATH+"results.png");

    QPainter painter(&pix);
    QFont font = QFont("Myriad",40);
    painter.setFont(font);
    painter.setPen(QPen(Qt::black, 2));

    QFontMetrics fm(font);


    int W;
    for(int i = 0;i<14;i++)
    {
        QString percent = QString::number(questionStats[i][answers[i]])+"%";
        W = fm.width(answersTxt[answers[i]]);
        painter.drawText(QRectF(2160-W/2,255+i*129,width(),height()),answersTxt[answers[i]]);
        W = fm.width(percent);
        painter.drawText(QRectF(3050-W/2,255+i*129,width(),height()),percent);
    }

    results->setPixmap(pix);

    results->show();


}




void module3::mousePressEvent(QMouseEvent *ev)
{

    qDebug()<<ev->pos();
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

