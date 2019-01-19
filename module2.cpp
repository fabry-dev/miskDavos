#include "module2.h"
#include "qdebug.h"
#include "qpushbutton.h"


const int bubbleX[4][5] = {
    {80,1180,1900,2700,3120},
    {350,870,1600,2450,3400},
    {100,1200,1675,2750,3450},
    {400,830,1980,2530,3150}
};

const int bubbleY[4][5] = {
    {870,1300,900,1300,1160},
    {950,1300,1000,950,980},
    {1370,900,1200,1000,1200},
    {1280,950,1140,1300,950}
};


module2::module2(QLabel *parent, QString PATH) : QLabel(parent),PATH(PATH)
{
    
    // showFullScreen();
    resize(3840,2160);
    
    
    
    vp = new mpvWidget(this);
    vp->resize(size());
    vp->setLoop(false);
    vp->setProperty("pause", false);
    vp->setProperty("keep-open",true);


    
    vp->hide();



    connect(vp,SIGNAL(clicked(QPoint)),this,SLOT(getVideoClicked(QPoint)));
    
    videoCaption = new QLabel(vp);

    videoCaption->setText(  "If you would like to know more, visit GYI.Miskglobalforum.com");
    videoCaption->setFont(QFont("Arial",25));
    videoCaption->adjustSize();
    videoCaption->move(vp->width()*0.5+20,vp->height()-videoCaption->height()-30);
    videoCaption->setStyleSheet("QLabel {color : black; }");
    videoCaption->show();
    videoCaption->raise();



    
    videoSlide = new QPropertyAnimation(vp,"pos");
    videoSlide->setDuration(500);
    videoSlide->setEasingCurve(QEasingCurve::InCurve);
    
    colors.push_back(QColor::fromRgb(0x8E,0x31,0x8B));//pourpre
    colors.push_back(QColor::fromRgb(0x06,0x71,0xB7));//blue
    colors.push_back(QColor::fromRgb(0xF3,0x79,0x40));//orange
    colors.push_back(QColor::fromRgb(0xFB,0xA9,0x2B));//jaune
    
    home = new picButton(this,0,PATH+"home.png",PATH+"homeOn.png","");
    connect(home,SIGNAL(clicked(QString)),this,SIGNAL(goHome()));
    home->move(width()-50-home->width(),50);
    home->show();
    home->raise();
    
    
    loadExploreCountries();
    loadCompareCountries();
    
    
    compareButton = new picButton(this,300,PATH+"compare.png",PATH+"compareOn.png","");
    connect(compareButton,SIGNAL(clicked(QString)),this,SLOT(goCompare()));
    compareButton->move(3300,0);
    
    exploreButton = new picButton(this,300,PATH+"explore.png",PATH+"exploreOn.png","");
    connect(exploreButton,SIGNAL(clicked(QString)),this,SLOT(goExplore()));
    exploreButton->move(3300,0);
    
    goStatsButton = new picButton(this,500,PATH+"gorank.png",PATH+"gorankOn.png","");
    connect(goStatsButton,SIGNAL(clicked(QString)),this,SLOT(goRank()));
    goStatsButton->move(2800,85);



    
    compareButton->hide();
    exploreButton->hide();
    
    
    vp->raise();//video player on top of everything
    
    for (auto b:exploreBubbles)
        b->hide();
    
    
    //  QStringList commands = { "Argentina", "Australia", "Brazil", "Canada","China","Denmark","Egypt","France","Germany","India","Indonesia","Italy","Japan","Jordan","Mexico","Nigeria","Pakistan","Russia","Saudi Arabi","South Africa","South Korea","Sweden","Turkey","United Kingdom","US"};
    combo = new QComboBox(this);
    combo->addItems(compareCountries);
    combo->move(130,435);
    combo->resize(400,100);
    combo->hide();
    combo->setFont(QFont("Arial",30));
    //combo->setStyleSheet("padding-left: 10px;border-radius: 50px;QComboBox::drop-down  {  color:white;};");


    rankLbl = new QLabel(this);
    rankLbl->resize(size());
    rankLbl->setPixmap(QPixmap(PATH+"arrayStats.png"));
    rankLbl->hide();

    goMapButton = new picButton(rankLbl,300,PATH+"gomap.png",PATH+"gomapOn.png","");
    connect(goMapButton,SIGNAL(clicked(QString)),this,SLOT(goMap()));
    goMapButton->move(width()-goMapButton->width()-100,85);
    goMapButton->show();


    rankAnim = new QPropertyAnimation(rankLbl,"pos");
    rankAnim->setDuration(500);
    rankAnim->setEasingCurve(QEasingCurve::InCurve);

    connect(combo,SIGNAL(activated(QString)),this,SLOT(addNewCountry(QString)));
    

    
    // goCompare();
}


void module2::init()
{
    compareButton->hide();
    exploreButton->hide();
    goCompare();
}


void module2::goCompare()
{
    compareButton->hide();
    exploreButton->show();
    for (auto b:exploreBubbles)
        b->hide();
    
    setPixmap(QPixmap(PATH+"compareBackground.png").scaled(width(),height()));
    
    combo->show();
}

void module2::goExplore()
{
    exploreButton->hide();
    compareButton->show();
    setPixmap(QPixmap(PATH+"exploreBackground.png").scaled(width(),height()));
    for (auto b:exploreBubbles)
        b->show();
    
    combo->hide();
    for(auto b:countryBubbles)
        b->deleteLater();
    countryBubbles.clear();
    
    
    for(auto i:countryIcons)
        i->deleteLater();
    countryIcons.clear();
    
    activeCountries.clear();
}

void module2::goRank()
{
    rankLbl->move(-width(),0);
    rankLbl->show();


    disconnect(rankAnim,0,0,0);


    rankAnim->setStartValue(rankLbl->pos());
    rankAnim->setEndValue(pos());
    rankAnim->start();
    // connect(rankAnim,SIGNAL(finished()),md->,SLOT(hide()));



}

void module2::goMap()
{



    disconnect(rankAnim,0,0,0);


    rankAnim->setStartValue(rankLbl->pos());
    rankAnim->setEndValue(QPoint(-width(),0));
    connect(rankAnim,SIGNAL(finished()),rankLbl,SLOT(hide()));
    rankAnim->start();


}



//Explore

void module2::loadExploreCountries()
{
    uint count = 0;
    
    QString countryName;
    int countrySize,countryValue,posX,posY,colorCode;
    bool test;
    QFile file(PATH+"countries.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no config file";
        
    }
    else
    {
        
        QTextStream in(&file);
        
        QString  line;
        QStringList params;
        
        while(!in.atEnd()) {
            line = in.readLine();
            params = (line.split(","));
            
            if(params.size()!=6)
                break;
            
            countryName = params[0];
            
            countryValue = params[1].toInt(&test);
            if(!test)
                break;
            
            countrySize = params[2].toInt(&test);
            if(!test)
                break;
            
            posX = params[3].toInt(&test);
            if(!test)
                break;
            
            posY = params[4].toInt(&test);
            if(!test)
                break;
            
            colorCode= params[5].toInt(&test);
            if((!test)&&(colorCode>=colors.size()))
                break;
            
            
            
            circleButton *b = new circleButton(this,colors[colorCode],count,QString::number(countryValue)+" "+countryName);
            exploreBubbles.push_back(b);
            countryNames.push_back(countryName);
            connect(b,SIGNAL(clicked(int)),this,SLOT(showVideo(int)));
            b->resize(QSize(countrySize,countrySize));
            b->move(posX,posY);
            b->show();
            count ++;
        }
        file.close();
        
    }
    
}

void module2::showVideo(int countryId)

{
    if(countryId>=countryNames.size())
        return;
    
    
    disconnect(videoSlide,0,0,0);
    
    QString videoName = PATH+countryNames[countryId]+".mp4";
    
    qDebug()<<videoName;
    vp->move(width(),0);
    vp->show();
    vp->loadFile(videoName);
    
    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(0,0));
    
    QTimer::singleShot(200,videoSlide,SLOT(start()));
    //videoSlide->start();
    
}

void module2::hideVideo()
{
    disconnect(videoSlide,0,0,0);
    videoSlide->setStartValue(vp->pos());
    videoSlide->setEndValue(QPoint(width(),0));
    connect(videoSlide,SIGNAL(finished()),vp,SLOT(hide()));
    videoSlide->start();
    
}

void module2::getVideoClicked(QPoint pos)
{
    if((pos.x()>160)&&(pos.y()>120)&&(pos.x()<400)&&(pos.y()<300))
    {
        hideVideo();
    }
    
    //    qDebug()<<pos;
}






//Compare

void module2::loadCompareCountries()
{
    QString countryName;
    std::vector<uint>score;
    score.resize(5);
    
    bool test;
    QFile file(PATH+"compare.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no config file";
        
    }
    else
    {
        
        QTextStream in(&file);
        
        QString  line;
        QStringList params;
        
        while(!in.atEnd()) {
            line = in.readLine();
            params = (line.split(","));
            
            if(params.size()!=6)
                break;
            
            countryName = params[0];
            
            
            score[0]=(params[1].toInt(&test));
            if(!test)
                break;
            
            score[1]=(params[2].toInt(&test));
            if(!test)
                break;
            
            score[2]=(params[3].toInt(&test));
            if(!test)
                break;
            
            score[3]=(params[4].toInt(&test));
            if(!test)
                break;
            
            score[4]=(params[5].toInt(&test));
            if(!test)
                break;
            
            
            compareCountries.push_back(countryName);
            compareScores.push_back(score);
            
        }
        file.close();
        
    }
    
}

void module2::addNewCountry(QString country)
{
    
    if(activeCountries.size()>= 4)
        return;
    
    for(auto activeCountry:activeCountries)
        if(activeCountry==country)
            return;
    
    
    activeCountries.push_back(country);
    organiseCountries();
    
}

void module2::removeCountry()
{
    countryIcon* icon = (countryIcon*)(QObject::sender() );
    activeCountries.erase(std::remove(activeCountries.begin(), activeCountries.end(), icon->getName()), activeCountries.end());
    
    organiseCountries();
}


void module2::organiseCountries()
{
    int spacing = 50;
    
    for(auto b:countryBubbles)
        b->deleteLater();
    countryBubbles.clear();
    
    
    for(auto i:countryIcons)
        i->deleteLater();
    countryIcons.clear();
    
    
    
    if(activeCountries.size()<=0)
        return;
    
    countryIcon* icon = new countryIcon(this,PATH,activeCountries[0],colors[0]);
    icon->move(600,440);
    connect(icon,SIGNAL(deleteClicked()),this,SLOT(removeCountry()));
    icon->show();
    countryIcons.push_back(icon);
    
    
    
    
    for(int i = 1;i<activeCountries.size();i++)
    {
        
        countryIcon *previous = countryIcons.at(i-1);
        icon = new countryIcon(this,PATH,activeCountries[i],colors[i]);
        connect(icon,SIGNAL(deleteClicked()),this,SLOT(removeCountry()));
        icon->show();
        icon->move(previous->geometry().bottomRight().x()+spacing,440);
        countryIcons.push_back(icon);
    }
    
    
    
    
    for(int i = 0;i<activeCountries.size();i++)
    {
        int n = compareCountries.indexOf(activeCountries[i]);
        std::vector<uint> score;
        
        score = compareScores[n];
        
        for (int j=0;j<score.size();j++)
        {
            countryBubble *b = new countryBubble(this,colors[i],score[j],activeCountries[i]);
            b->move(bubbleX[i][j],bubbleY[i][j]);
            countryBubbles.push_back(b);
            b->show();
        }
    }
    
    
    
    combo->setEnabled(activeCountries.size()<4);
    
}


countryIcon::countryIcon(QLabel *parent, QString PATH,QString name, QColor color):QLabel(parent),PATH(PATH),name(name),color(color)
{
    
    //this->setStyleSheet("border: 1px solid black");
    
    
    QFont font = QFont("Arial",30) ;
    
    
    QFontMetrics fm(font);
    
    double W = fm.width(name);
    double H = fm.height();
    
    double W1 = W+220;
    if(W1<150) W1=150;
    resize(W1,100);
    QPixmap pix = QPixmap(PATH+"icon.png").scaled(size());
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 0));
    painter.setFont(font);
    painter.drawText(QRectF(95,(height()-H)/2,width(),height()),name);
    painter.drawText(QRectF((width()-75),(height()-H)/2,width(),height()),"X");
    int r = 35;
    painter.setBrush(QColor(color));
    painter.setPen(QPen(color, 0));
    painter.drawEllipse(QRectF(40,(height()-r)/2,r,r));
    setPixmap(pix.scaledToWidth(width()));
    
}



countryBubble::countryBubble(QWidget *parent, QColor color, uint score,QString name): QLabel(parent),color(color),score(score),name(name)
{
    
    double r = (double)qSqrt(2000*score);
    
    
    if(r<50)
        r=50;
    resize(r,r);
    
    QPixmap pix(r,r);
    pix.fill(Qt::transparent);
    
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    
    painter.setPen(QPen(QColor(color), 0));
    painter.setBrush(QColor(color));
    painter.drawEllipse(QPointF(width()/2,height()/2),0.95*width()/2,0.95*height()/2);
    painter.setPen(QPen(QColor(Qt::white), 0));
    
    QFont font = QFont("Arial",20+(r/5)) ;
    
    QFontMetrics fm(font);
    double W = fm.width(QString::number(score));
    double H = fm.height();
    painter.setFont(font);
    painter.drawText(QRectF((width()-W)/2,(height()-H)/3,width(),height()),QString::number(score));
    
    font = QFont("Arial",(20+(r/5))/4) ;
    painter.setFont(font);
    QFontMetrics fm2(font);
    W = fm2.width(name);
    H = fm2.height();
    painter.drawText(QRectF((width()-W)/2,2*(height()-H)/3,width(),height()),name);
    setPixmap(pix);
    
}



void countryIcon::mousePressEvent(QMouseEvent *ev)
{
    if(ev->pos().x()>width()*7/10)
        emit deleteClicked();
}



void module2::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<ev->pos();
    
}
