#include <QApplication>
#include "qwindow.h"
#include "qscreen.h"
#include "qwidget.h"
#include "module4.h"
#include "module3.h"
#include "module2.h"
#include "qdebug.h"

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




    module3 *md3 = new module3(NULL,PATH+"module3/");
    md3->setGeometry(a.screens()[0]->geometry().x(),a.screens()[0]->geometry().y(),3840,2160);
    md3->hide();

    module2 *md2 = new module2(NULL,PATH+"module2/");
    md2->setGeometry(a.screens()[0]->geometry().x(),a.screens()[0]->geometry().y(),3840,2160);
    md2->hide();

    module4 *md4 = new module4(NULL,PATH+"module4/");
    md4->setGeometry(a.screens()[0]->geometry().x(),a.screens()[0]->geometry().y(),3840,2160);
    md4->show();

    if(DEBUG)
    {

    QPushButton *pb = new QPushButton(md2);
    pb->move(0,0);
    pb->resize(200,200);
    pb->setText("close app");
    a.connect(pb,SIGNAL(clicked(bool)),&a,SLOT(closeAllWindows()));
    pb->show();
    pb->raise();



    QPushButton *pb2 = new QPushButton(md3);
    pb2->move(0,0);
    pb2->resize(200,200);
    pb2->setText("close app");
    a.connect(pb2,SIGNAL(clicked(bool)),&a,SLOT(closeAllWindows()));
    pb2->show();
    pb2->raise();



    QPushButton *pb4 = new QPushButton(md4);
    pb4->move(0,0);
    pb4->resize(200,200);
    pb4->setText("close app");
    a.connect(pb4,SIGNAL(clicked(bool)),&a,SLOT(closeAllWindows()));
    pb4->show();
    pb4->raise();


    }
    return a.exec();

}
