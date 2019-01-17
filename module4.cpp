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


    cs = new customSlider(this,PATH,geometry());

    for(int i =0;i<5;i++)
        bgPixS.push_back(QPixmap(PATH+"scale"+QString::number(i+1)+".png"));

    setupKeyboard();

    //  nextButton->show();
    init();
    goSecondPart();
    goKeyboard();

}



void module4::init()
{
    score2.resize(5,0);
    score1.resize(9,-1);
    cs->hide();
    nextButton2->hide();

    setPixmap(QPixmap(PATH+"background.png").scaled(size()));

    ranking.resize(skillList.size(),-1);


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

    qDebug()<<"score1 "<<score1;
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
    setPixmap(QPixmap(PATH+"keyboardBg.png").scaled(size()));

    cs->hide();
    nextButton2->hide();
    nextButton->hide();

    for (auto k:keyboard)
        k->show();
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
        qDebug()<<"score2 "<<score2;
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
                break;
            }
        }
    }
    bubbles[id]->setRank(ranking[id]);




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
        p->move(p->x()+(width()-maxX)/2,p->y());


    for(auto p:keyboard)
        p->hide();
}


void module4::getKey(QString key)
{

    qDebug()<<key;
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

