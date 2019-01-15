#include <QApplication>
#include "qwindow.h"
#include "qscreen.h"
#include "qwidget.h"
#include "videowindow.h"
#include "qdebug.h"



#include "serialwatcher.h"

#define PATH_DEFAULT (QString)"/home/fred/Dropbox/Taf/Cassiopee/book/files/"
#define TIMEOUT 15000

#define defaultSpeed (10)

int main(int argc, char *argv[])

{
    QApplication a(argc, argv);




    QString PATH;
    QStringList params = a.arguments();
    if(params.size()>1)
        PATH = params[1];
    else
        PATH=PATH_DEFAULT;

    bool HIDE_CURSOR=false;
    int speed = defaultSpeed;
    int timeout = TIMEOUT;


    QFile file(PATH+"config.cfg");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"no config file";

    }
    else
    {

        QTextStream in(&file);

        QString  line;
        QString paramName,paramValue;
        QStringList params;

        while(!in.atEnd()) {
            line = in.readLine();
            line = (line.split("#"))[0];
            params = line.split("=");
            if(params.size()>=2)
            {
                paramName = params[0];
                paramValue = params[1];

                if (paramName=="SPEED")
                {
                    bool test;
                    speed = paramValue.toInt(&test);
                    if(!test)
                        speed = defaultSpeed;
                }
                else if (paramName=="TIMEOUT")
                {
                    bool test;
                    timeout = paramValue.toInt(&test);
                    if(!test)
                        speed = TIMEOUT;
                }
                else if (paramName.mid(0,6)=="CURSOR")
                    HIDE_CURSOR = (paramValue=="NO");

                else
                    qDebug()<<paramName<<" - "<<paramValue;
            }
        }
        file.close();

    }




    if (HIDE_CURSOR)
    {
        QCursor cursor(Qt::BlankCursor);
        a.setOverrideCursor(cursor);
        a.changeOverrideCursor(cursor);
    }


    serialWatcher * serialwatch = new serialWatcher(NULL,speed,timeout);



    qDebug()<<"Screens count: "<<a.screens().size();


    std::vector<videoWindow*> vws;

    videoWindow *vw = new videoWindow(NULL,PATH,2);
    vws.push_back(vw);
    videoWindow *vw2 = new videoWindow(NULL,PATH,1);
    vws.push_back(vw2);


    for(int i = 0;(i<vws.size())&&(i<a.screens().size());i++)
    {

        vws[i]->setGeometry(a.screens()[i]->geometry().x(),a.screens()[i]->geometry().y(),1920,1080);
        vws[i]->show();

        a.connect(serialwatch,SIGNAL(nuPage(int)),vws[i],SLOT(goToPage(int)));
        a.connect(vws[i],SIGNAL(updStatus(int,bool)),serialwatch,SLOT(getStatus(int,bool)));
        a.connect(serialwatch,SIGNAL(gotTimeOut()),vws[i],SLOT(goSaving()));
        a.connect(serialwatch,SIGNAL(exitTimeOut()),vws[i],SLOT(exitSaving()));
    }




    return a.exec();

}
