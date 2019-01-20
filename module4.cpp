#include "module4.h"
#include "qdebug.h"
#include "qpushbutton.h"
#include "qstringlist.h"

QStringList keyboardStr = (QStringList)"@"<<"1"<<"2"<<"3"<<"4"<<"5"<<"6"<<"7"<<"8"<<"9"<<"0"<<"back"
                                         <<""<<"q"<<"w"<<"e"<<"r"<<"t"<<"y"<<"u"<<"i"<<"o"<<"p"
                                        <<""<<"a"<<"s"<<"d"<<"f"<<"g"<<"h"<<"j"<<"k"<<"l"
                                       <<""<<"z"<<"x"<<"c"<<"v"<<"b"<<"n"<<"m"<<"."<<""
                                      <<"-"<<"_"<<" "<<"_"<<"-"<<"enter";



int xspacing = 300;
int yspacing = 300;

int xs[]={0,1,2,3,4,3,2,1,2};
int ys[]={0,1,2,1,0,-1,-2,-1,-0};

#define SCENE_x0 0
#define SCENE_y0 400
#define SCENE_x1 width()
#define SCENE_y1 height()



module4::module4(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{

    qsrand(QTime::currentTime().msecsSinceStartOfDay());

    showFullScreen();
    resize(3840,2160);

    loadSkillList();

    colors.push_back(QColor::fromRgb(0x8E,0x31,0x8B));//pourpre
    colors.push_back(QColor::fromRgb(0x06,0x71,0xB7));//blue
    colors.push_back(QColor::fromRgb(0xF3,0x79,0x40));//orange
    colors.push_back(QColor::fromRgb(0xFB,0xA9,0x2B));//jaune



    vp = new mpvWidget(this);
    vp->resize(size());
    vp->setLoop(false);
    vp->setProperty("pause", false);
    vp->setProperty("keep-open",true);

    vp->hide();



    mainScene = new QGraphicsScene(this);
    mainScene->setSceneRect(QRect(0,0,SCENE_x1-SCENE_x0,SCENE_y1-SCENE_y0));

    mainView = new QGraphicsView(this);
    mainView->move(SCENE_x0,SCENE_y0);
    mainView->resize(SCENE_x1-SCENE_x0,SCENE_y1-SCENE_y0);

    mainView->setScene(mainScene);
    mainView->setStyleSheet("background-color: transparent;");
    mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    for(int i = 0;i<skillList.size();i++)
    {
        bubbleItem * bubble = new bubbleItem(QSize(400,400),skillList[i],i,colors[i%4]);
        mainScene->addItem(bubble);
        connect(&(bubble->communicator),SIGNAL(moved(int,QPointF)),this,SLOT(bubbleMoved(int,QPointF)));
        bubbles.push_back(bubble);

        skillTab *tab = new skillTab(this,PATH,i);
        tab->resize(QSize(400,400));
        tab->move(2500+(i%3)*400,450+i*150);
        targets.push_back(tab);
    }


    nextButton = new picButton(this,0,PATH+"next.png",PATH+"next2.png","next");
    nextButton->move(2500,2000);
    nextButton->raise();
    nextButton->hide();
    connect(nextButton,SIGNAL(clicked(QString)),this,SLOT(goSecondPart()));

    nextButton2 = new picButton(this,0,PATH+"next.png",PATH+"next2.png","next");
    nextButton2->move(3400,2000);
    nextButton2->raise();
    nextButton2->hide();
    connect(nextButton2,SIGNAL(clicked(QString)),this,SLOT(showNextQuestion()));

    nextButton3 = new picButton(vp,0,PATH+"next.png",PATH+"next2.png","next");
    nextButton3->move(width()-nextButton3->width()-10,height()-nextButton3->height()-10);
    nextButton3->raise();
    nextButton3->hide();
    connect(nextButton3,SIGNAL(clicked(QString)),this,SLOT(goKeyboard()));
    connect(vp,SIGNAL(videoOver()),nextButton3,SLOT(show()));
    connect(vp,SIGNAL(videoOver()),nextButton3,SLOT(raise()));

    cs = new customSlider(this,PATH,geometry());

    for(int i =0;i<5;i++)
        bgPixS.push_back(QPixmap(PATH+"scale"+QString::number(i+1)+".png"));


    home = new picButton(this,0,PATH+"home.png",PATH+"homeOn.png","");
    connect(home,SIGNAL(clicked(QString)),this,SIGNAL(goHome()));
    home->move(width()-50-home->width(),50);
    home->show();
    home->raise();


    //std::vector<lineEdit2*> inputs;

    for(int i = 0;i<4;i++)
    {
        inputs.push_back(new lineEdit2(this));
        inputs[i]->hide();
        inputs[i]->setFont(QFont("Arial",40));
        connect( inputs[i],SIGNAL(clicked(lineEdit2*)),this,SLOT(selectInput(lineEdit2*)));
    }

    inputs[0]->move(760,330);
    inputs[0]->resize(900,80);

    inputs[1]->move(760,650);
    inputs[1]->resize(900,85);

    inputs[2]->move(2730,330);
    inputs[2]->resize(150,80);

    inputs[3]->move(2875,650);
    inputs[3]->resize(800,85);



    exps.push_back(QRegularExpression(""));
    exps.push_back(QRegularExpression(""));
    exps.push_back(QRegularExpression("^[0-9]{2,3}$"));
    exps.push_back(QRegularExpression("^[0-9a-zA-Z]+([0-9a-zA-Z]*[-._+])*[0-9a-zA-Z]+@[0-9a-zA-Z]+([-.][0-9a-zA-Z]+)*([0-9a-zA-Z]*[.])[a-zA-Z]{2,6}$"));


    pdf = new QLabel(this);
    pdf->resize(size());
    pdf->hide();


    videoSlide = new QPropertyAnimation(vp,"pos");
    videoSlide->setDuration(500);
    videoSlide->setEasingCurve(QEasingCurve::InCurve);



    setupKeyboard();
    initDb();
    //  nextButton->show();
    init();





    sendEmail("fred","frederic.abry@gmail.com",33,4);
     /*getData();
     getData2();*/
}

void module4::init()
{

    for (auto k:keyboard)
        k->hide();

    score2.resize(5,0);
    score1.resize(9,-1);
    cs->hide();
    nextButton2->hide();

    setPixmap(QPixmap(PATH+"background.png").scaled(size()));

    ranking.resize(skillList.size(),-1);


    for(auto in:inputs)
        in->hide();


    for(int i = 0;i<skillList.size();i++)
    {
        bubbles[i]->show();
        bubbles[i]->setPos(550+xs[i]*xspacing,900+ys[i]*yspacing);

        targets[i]->show();
        targets[i]->setText();

    }

    mainView->show();
    mainView->raise();
    nextButton->raise();

}

void module4::goSecondPart()
{


    insertData();
    nextButton->hide();
    mainView->hide();
    for(int i = 0;i<skillList.size();i++)
    {
        bubbles[i]->hide();
        targets[i]->hide();

    }

    cs->reset();

    activeQuestion = -1;
    showNextQuestion();

}

void module4::goKeyboard()
{



    for(auto in:inputs)
    {
        in->clear();
        in->show();
    }
    activeInput = inputs[0];

    nextButton->hide();
    mainView->hide();
    for(int i = 0;i<skillList.size();i++)
    {
        bubbles[i]->hide();
        targets[i]->hide();

    }


    setPixmap(QPixmap(PATH+"registration.png").scaled(size()));

    cs->hide();
    nextButton2->hide();
    nextButton->hide();

    for (auto k:keyboard)
        k->show();

    vp->raise();
    hideVideo();

}

void module4::selectInput(lineEdit2* ln)
{
    ln->setFocus();
    activeInput = ln;
}

void module4::showNextQuestion()
{
    activeQuestion++;

    if(activeQuestion>0)
    {
        score2[activeQuestion-1]=cs->getValue();
    }

    if(activeQuestion>=5)
    {
        nextButton2->hide();
        int totalScore = 0;
        for(auto i:score2)
            totalScore += i/5;


        qDebug()<<"mongolo score: "<<totalScore;

        int id = 1+(totalScore/11);
        if(id<1) id=1;
        if(id>9) id=9;

        playVideo(id);

        //culture remixer ; climate changer ; contestant ; the authentic self ; global citizen ; startup artist ; transitionist ; eco maker ; speedrunner

        return;
    }
    nextButton2->show();
    nextButton2->disable();
    QTimer::singleShot(1000,nextButton2,SLOT(enable()));
    nextButton2->raise();
    cs->reset();
    cs->setCursorColor(activeQuestion);
    cs->show();
    setPixmap(bgPixS[activeQuestion]);
}

void module4::loadSkillList()
{

    QFile file(PATH+"skills.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no skills file";

    }
    else
    {

        QTextStream in(&file);

        QString  line;


        while(!in.atEnd()) {
            line = in.readLine();
            skillList.append(line);

        }
        file.close();

    }

}

void module4::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<ev->pos();
}

void module4::bubbleMoved(int id, QPointF pos)
{
    QPoint nucursor = QPoint(0,0);

    ranking[id] = -1;

    for(int i = 0;i<skillList.size();i++)
    {
        int x1 = pos.x()-targets[i]->x()-targets[i]->width()/2;
        int y1 = pos.y()-targets[i]->y()+targets[i]->height()/2;
        int distance = abs(x1)+abs(y1);
        if(distance<70)
        {
            //check if i is in ranking already
            if(!(std::find(ranking.begin(), ranking.end(), i) != ranking.end()))
            {
                ranking[id] = i;

                /*double nux = cursor().pos().x()-x1;
                double nuy = cursor().pos().y()-y1;*/
                double nux = targets[i]->x()+targets[i]->width()/2;
                double nuy = targets[i]->y()-targets[i]->height()/2;

                //cursor().setPos(QPoint(nux,nuy));
                nucursor.setX(nux);
                nucursor.setY(nuy);
                break;
            }

        }
    }


    bubbles[id]->setRank(ranking[id],nucursor);




    for(auto i:ranking)
        if(i == -1)
        {
            nextButton->hide();
            return;
        }


    for(int i = 0;i<9;i++)
    {

        score1[i] = ranking[i];
    }


    //all ranked
    nextButton->show();

}

void module4::setupKeyboard()
{

    int kwdefault = 180;
    int kspace = 0;
    int kw;
    int ky=kwdefault+kspace;
    int posX = 0;
    int posY0 = 600;
    int posY = posY0;
    for(int i = 0;i<49;i++)
    {

        kw = kwdefault;


        if(i==11)
        {
            kw = 1.8*kwdefault;
        }
        else if (i==12)
            kw = 1.5*kwdefault;
        else if (i==23)
            kw = 2*kwdefault;
        else if (i==33)
            kw = 2.55*kwdefault;
        else if (i==42)
            kw = 2.2*kwdefault;
        else if ((i==43)||(i==44)||(i==46)||(i==47))
            kw = 1.6*kwdefault;
        else if (i==45)
            kw = 6.3*kwdefault;
        else if (i==48)
        {
            kw = 1.8* kwdefault;
            posX = 1980;
            posY = 780;
        }
        picButton *pb = new picButton(this,kw,PATH+"off/"+QString::number(i)+".png",PATH+"on/"+QString::number(i)+".png",keyboardStr[i]);

        connect(pb,SIGNAL(clicked(QString)),this,SLOT(getKey(QString)));
        pb->move(posX,posY);
        //pb->setStyleSheet("border: 1px solid black");
        posX+=kw+kspace;


        if((i==11)||(i==22)||(i==32)||(i==42))
        {
            posX=0;
            posY+=ky;
        }

        pb->show();
        keyboard.push_back(pb);



    }




    int maxX = 0;

    for(auto p:keyboard)
    {
        if(p->geometry().left()>maxX)
            maxX = p->geometry().right();
    }

    for(auto p:keyboard)
        p->move(p->x()+(width()-maxX)/2,p->y()+400);

    submit = new picButton(this,0,PATH+"submit.png",PATH+"submitOn.png","");
    submit->move(width()-submit->width()-50,height()-submit->height()-50);
    connect(submit,SIGNAL(clicked(QString)),this,SLOT(submitInputs()));

    keyboard.push_back(submit);


    for(auto p:keyboard)
        p->hide();
}

void  module4::initDb()
{

    QSqlQuery query("CREATE TABLE module4 (id INTEGER PRIMARY KEY AUTOINCREMENT, q1 INTEGER, q2 INTEGER, q3 INTEGER, q4 INTEGER, q5 INTEGER, q6 INTEGER, q7 INTEGER, q8 INTEGER, q9 INTEGER, date TIMESTAMP)");

    bool success = query.exec();

    if(!success)
    {
        qDebug() << query.lastError();
    }
    else qDebug() << "table created";



    QSqlQuery query2("CREATE TABLE module42 (id INTEGER PRIMARY KEY AUTOINCREMENT, firstname TEXT, lastname TEXT, email TEXT, age INTEGER, archetype INTEGER, date TIMESTAMP)");

    success = query2.exec();

    if(!success)
    {
        qDebug() << query2.lastError();
    }
    else qDebug() << "table2 created";
}

void module4::insertData()
{

    qDebug()<<"score1 "<<score1;

    QDateTime timestamp = QDateTime::currentDateTime();

    QSqlQuery query;
    query.prepare("INSERT INTO module4 (q1,q2,q3,q4,q5,q6,q7,q8,q9, date) VALUES (:q1,:q2,:q3,:q4,:q5,:q6,:q7,:q8,:q9,:datetime)");

    for(int i = 0;i<score1.size();i++)
        query.bindValue(":q"+QString::number(i+1), score1.at(i));

    query.bindValue(":datetime", timestamp.toString("yyyy-MM-dd hh:mm:ss"));


    bool success = query.exec();
    if(!success)
    {
        qDebug() << query.lastError();
    }


    // getData();
}


void module4::insertData2(QString firstName, QString lastName, QString email, int age,int archetype)
{



    QDateTime timestamp = QDateTime::currentDateTime();

    QSqlQuery query;
    query.prepare("INSERT INTO module42 (firstname,lastname,email, age,archetype,date) VALUES (:firstname,:lastname,:email,:age,:archetype,:datetime)");


    query.bindValue(":firstname", firstName);
    query.bindValue(":lastname", lastName);
    query.bindValue(":email", email);
    query.bindValue(":age", age);
    query.bindValue(":archetype", archetype);
    query.bindValue(":datetime", timestamp.toString("yyyy-MM-dd hh:mm:ss"));


    bool success = query.exec();
    if(!success)
    {
        qDebug() << query.lastError();
    }



}




void module4::getData2()
{

    QSqlQuery query;
    query.prepare("SELECT id,firstname,lastname,email,age,archetype, date FROM module42");
    query.exec();

    std::vector<int> buf;
    buf.resize(9,-1);

    int id,archetype;QDateTime date;int age;QString firstName,lastName,email;

    while (query.next()) {

        id = query.value(0).toInt();
        firstName = query.value(1).toString();
        lastName = query.value(2).toString();
        email = query.value(3).toString();
        age = query.value(4).toInt();
        archetype = query.value(5).toInt();
        date = query.value(6).toDateTime();

        qDebug()<<id<<date<<firstName<<lastName<<email<<age<<archetype;
    }
}


void module4::getData()
{

    QSqlQuery query;
    query.prepare("SELECT q1,q2,q3,q4,q5,q6,q7,q8,q9, date FROM module4");
    query.exec();

    std::vector<int> buf;
    buf.resize(9,-1);

    while (query.next()) {
        for(int i = 0;i<9;i++)
            buf[i] = query.value(i).toInt();

        QDateTime date = query.value(9).toDateTime();
        qDebug()<<date<<buf;
    }
}










void module4::getKey(QString txt)
{
    QRegularExpression exp;

    if(txt=="enter")
    {
        /* if((nameEdit->text()>3)&&(emailEdit->text()>3))
        {
            validateData();
        }*/

    }
    else if(txt=="back")
    {
        if(activeInput->text().size()>0)
            activeInput->setText((activeInput->text()).remove((activeInput->text()).size()-1,1));

    }
    else
    {
        // activeInput->setText();
        QString buf = activeInput->text()+txt;


        for(int i = 0;i<inputs.size();i++)
        {
            if(inputs[i]==activeInput)
            {
                QRegularExpressionMatch match = exps[i].match(buf, 0, QRegularExpression::PartialPreferCompleteMatch);
                if( ((match.hasPartialMatch())||(match.hasMatch())))
                    activeInput->setText(buf);
                break;
            }
        }







    }
    activeInput->setCursorPosition( activeInput->text().size());

}

void module4::submitInputs()
{

    for(int i = 0;i<inputs.size();i++)
    {
        QRegularExpressionMatch match = exps[i].match(inputs[i]->text(), 0, QRegularExpression::PartialPreferCompleteMatch);
        if( !(match.hasMatch()))
            return;
    }


    for(auto input:inputs)
        qDebug()<<input->text();

    QString firstname = inputs[0]->text();
    QString lastname = inputs[1]->text();
    insertData2(firstname,lastname,inputs[3]->text(),inputs[2]->text().toInt(), archetypeId);

    firstname.replace(0, 1, firstname[0].toUpper());
    lastname.replace(0, 1, lastname[0].toUpper());

    sendEmail(firstname+" "+lastname,inputs[3]->text(), inputs[2]->text().toInt(), archetypeId);

    emit goHome();
}


void module4::buildPdf(QString name,int age_i,int mongoloId)
{

    for(auto p:keyboard)
        p->hide();

    double W,H;

    if((mongoloId<1)||(mongoloId>9))
        return;
    std::vector<QColor> cs;



    cs.push_back(QColor::fromRgb(159,158,204));
    cs.push_back(QColor::fromRgb(0,184,156));
    cs.push_back(QColor::fromRgb(249,189,50));
    cs.push_back(QColor::fromRgb(142,208,237));
    cs.push_back(QColor::fromRgb(211,109,107));
    cs.push_back(QColor::fromRgb(211,109,107));
    cs.push_back(QColor::fromRgb(0,184,156));
    cs.push_back(QColor::fromRgb(173,204,61));
    cs.push_back(QColor::fromRgb(159,158,204));

    QString age = QString::number(age_i+11);
    QString skillset = name+"'s skillset";

    QPixmap pix=QPixmap(PATH+"A"+QString::number(mongoloId)+".png");



    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, true);




    QFont font = QFont("Arial",30);



    W = (QFontMetrics(font)).width(name);
    int W0 = 300;
    if(W>W0)
    {
        font.setPointSizeF(font.pointSizeF()*W0/W);
        W = W0;
    }
    H = (QFontMetrics(font)).height();




    painter.setPen(QPen(cs[mongoloId-1], 0));
    painter.setFont(font);
    painter.drawText(1260-W/2,230-H/2,name);



    font =  QFont("Arial",25);
    W = (QFontMetrics(font)).width(skillset);
    W0 = 375;
    if(W>W0)
    {
        font.setPointSizeF(font.pointSizeF()*W0/W);
        W = W0;
    }
    painter.setFont(font);
    painter.drawText(1220-W/2,550,skillset);


    font =  QFont("Arial",25,QFont::Bold);
    painter.setFont(font);
    painter.setPen(QPen((Qt::black), 0));
    painter.drawText(1327,252,age);
    /*pdf->setPixmap(pix);
    pdf->show();
    pdf->raise();*/


    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(PATH+"archetype.pdf");
    printer.setFullPage(true);

    double xscale = printer.pageRect().width()/double(pix.width());
    double yscale = printer.pageRect().height()/double(pix.height());

    double scale = qMin(xscale, yscale);

    QPainter painter2(&printer);

    painter2.scale(scale, scale);

    painter2.drawPixmap(0,0,pix.width(),pix.height(),pix);

}


/*
#define FROM "youth.misk@gmail.com"
#define SMTPSERVER  "smtp.gmail.com"
#define SMTPPORT    465
#define SMTPUSER    "youth.misk@gmail.com"
#define SMTPPASS    "MGF_2019"

*/


#define FROM "research@miskglobalforum.com"
#define SMTPSERVER  "smtp.gmail.com"
#define SMTPPORT    465
#define SMTPUSER    "research@miskglobalforum.com"
#define SMTPPASS    "gvlysqnzsoabzepj"


void module4::sendEmail(QString name,QString email, int age, int mongoloCode)
{

    buildPdf(name,age,mongoloCode);


    qDebug()<<"sending email to: "<<email;




    quickmail_initialize();
    quickmail mailobj = quickmail_create(FROM, "Misk Global Forum");
    quickmail_add_to(mailobj, email.toStdString().c_str());

    quickmail_add_header(mailobj, "Importance: Low");
    quickmail_add_header(mailobj, "X-Priority: 5");
    quickmail_add_header(mailobj, "X-MSMail-Priority: Low");


    QString body =  "";

    QFile f(PATH+"email.txt");
    if (f.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&f);
        body=in.readAll();
    }
    f.close();


    qDebug()<<body;


    QByteArray body1 = body.toLatin1();
    char *body2 = body1.data();
    quickmail_add_body_memory(mailobj, "text/html", body2, body.size(), 0);

    QString at = PATH+"archetype.pdf";
    QByteArray at1 = at.toLatin1();
    char *at2  = at1.data();
    quickmail_add_attachment_file(mailobj, at2, NULL);


    const char* errmsg;
    quickmail_set_debug_log(mailobj, stderr);
    qDebug()<<"test send";

    if ((errmsg = quickmail_send_secure(mailobj, SMTPSERVER, SMTPPORT, SMTPUSER, SMTPPASS)) != NULL)
    {

        qDebug()<< "Error sending e-mail: "<<stderr ;

    }
    else
    {
        qDebug()<<"Email sent.";

    }

    quickmail_destroy(mailobj);
    quickmail_cleanup();

    qDebug()<<"email cleaned up";
}



void module4::playVideo(int videoId)
{


    archetypeId = videoId;


    disconnect(videoSlide,0,0,0);



    QString videoName = PATH+"video"+QString::number(videoId)+".mp4";

    vp->move(width(),0);
    vp->show();
    vp->raise();
    vp->loadFile(videoName);

    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(0,0));

    QTimer::singleShot(200,videoSlide,SLOT(start()));
    //videoSlide->start();

}


void module4::hideVideo()
{

    disconnect(videoSlide,0,0,0);
    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(width(),0));
    connect(videoSlide,SIGNAL(finished()),vp,SLOT(hide()));
    videoSlide->start();

}

//skill bubbles
void bubbleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    double w = boundingRect().width();
    double h = boundingRect().height();
    QTextDocument td;

    QString html = "<body style=\"font-family:'Arial';font-size:20pt;color:white;\">";
    QAbstractTextDocumentLayout *layout = td.documentLayout();
    td.setHtml(html+text+"</body>");

    double W = layout->documentSize().width();
    double H = layout->documentSize().height();


    //
    double r = qMin(w,h);





    painter->setRenderHint(QPainter::Antialiasing, true);






    painter->setPen(QPen(QColor(color), 0));
    painter->setBrush(QColor(color));
    painter->translate(- boundingRect().width()/2,- boundingRect().height()/2);
    painter->drawEllipse(QPointF(w/2,h/2),r/2,r/2);

    painter->setPen(QPen(QColor(Qt::white), 0));



    if(rank>=0)
    {
        QFont font = QFont("Arial",40);
        painter->setFont(font);
        QFontMetrics fm(font);
        double W1 = fm.width(QString::number(rank+1));


        painter->drawText(QRectF(boundingRect().width()/2-W1/2,boundingRect().height()/2+100,boundingRect().width(),boundingRect().height()),QString::number(rank+1));
    }

    painter->translate((boundingRect().width()-W)/2,(boundingRect().height()-H)/2);

    td.drawContents(painter);

}


//skill tabs

void skillTab::setText()
{

    QString text = QString::number(id+1);

    QPixmap pix(size());
    pix.fill(Qt::transparent); // add this!
    QFont font = QFont("Arial",35) ;
    QFontMetrics fm(font);
    double W = fm.width(text);
    double H = fm.height();
    float a = W/(width()-200);

    QPainter painter(&pix);
    resize(pix.size());

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 0));
    painter.setFont(font);

    painter.setBrush(QColor(Qt::white));
    painter.drawEllipse(QPointF(width()/2,height()/2),width()/2,height()/2);

    painter.drawText(QRectF(width()/2-W/2,(height())/2-H/2,width(),height()),text);


    if(W>width()-200)
        font.setPointSizeF((double)font.pointSize()/a);




    setPixmap(pix);
}

