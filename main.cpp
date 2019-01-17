#include <QApplication>
#include "qwindow.h"
#include "qscreen.h"
#include "qwidget.h"
#include "module0.h"
#include "module4.h"
#include "module3.h"
#include "module2.h"
#include "qdebug.h"
#include "mainScreen.h"
#include "QPushButton"



#define PATH_DEFAULT (QString)"/home/fred/Dropbox/Taf/Cassiopee/miskDavos/files/"
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
    bool DEBUG=false;



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
                if (paramName.mid(0,6)=="CURSOR")
                    HIDE_CURSOR = (paramValue=="NO");
                else if (paramName.mid(0,5)=="DEBUG")
                    DEBUG = (paramValue=="YES");


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






    qDebug()<<"Screens count: "<<a.screens().size();

    mainScreen *ms = new mainScreen(NULL,PATH,DEBUG);
    ms->setGeometry(a.screens()[0]->geometry().x(),a.screens()[0]->geometry().y(),3840,2160);
    ms->show();




    return a.exec();

}
