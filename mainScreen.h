#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "qdebug.h"
#include <QWidget>
#include <qobject.h>
#include "qlabel.h"
#include "module0.h"
#include "module2.h"
#include "module3.h"
#include "module4.h"

class mainScreen : public QLabel
{
    Q_OBJECT
public:
    explicit mainScreen(QLabel *parent = nullptr, QString PATH="",bool DEBUG=false);

private:
    QString PATH;
    bool DEBUG;
    module0 *md0;
    module2 *md2;
    module3 *md3;
    module4 *md4;

    QLabel *activeModule;

    QPropertyAnimation *goaway,*come;
    void initDb();


private slots:
    void goModule0();
    void goModule1();
    void goModule2();
    void goModule3();
    void goModule4();


};

#endif
