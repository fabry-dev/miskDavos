#include <QApplication>
#include "qwindow.h"
#include "qscreen.h"
#include "qwidget.h"
#include "videowindow.h"
#include "qdebug.h"



#include "serialwatcher.h"

#define PATH_DEFAULT (QString)"/home/fred/Dropbox/Taf/Cassiopee/book/files/"


#define defaultSpeed (55)

int main(int argc, char *argv[])

{
    QApplication a(argc, argv);

    QCursor cursor(Qt::BlankCursor);
    a.setOverrideCursor(cursor);
    a.changeOverrideCursor(cursor);


    QString PATH;
    QStringList params = a.arguments();
    if(params.size()>1)
        PATH = params[1];
    else
        PATH=PATH_DEFAULT;


    int speed = defaultSpeed;


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

                else
                    qDebug()<<paramName<<" - "<<paramValue;
            }
        }
        file.close();

    }







    serialWatcher * serialwatch = new serialWatcher(NULL);



    qDebug()<<a.screens().size();


    std::vector<videoWindow*> vws;

    videoWindow *vw = new videoWindow(NULL,PATH,2);
    vws.push_back(vw);
    videoWindow *vw2 = new videoWindow(NULL,PATH,1);
    vws.push_back(vw2);


    for(int i = 0;i<vws.size();i++)
    {
        vws[i]->setGeometry(a.screens()[i]->geometry().x(),a.screens()[i]->geometry().y(),1920,1080);
        vws[i]->show();

        a.connect(serialwatch,SIGNAL(nuPage(int)),vws[i],SLOT(goToPage(int)));
        a.connect(vws[i],SIGNAL(updStatus(int,bool)),serialwatch,SLOT(getStatus(int,bool)));

    }




    return a.exec();

}
