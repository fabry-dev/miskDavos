#include "slidewindow.h"
#include "qdebug.h"
#include "qtimer.h"

#define FPS 24

slideWindow::slideWindow(QObject *parent, QString PATH, QList<QWidget *> screenList, int x0, int totalWidth, QString filename, int deltax, int id):QObject(parent),PATH(PATH),screenList(screenList),x0(x0),totalWidth(totalWidth),filename(filename),deltax(deltax),id(id)
{


    bgImg.load(PATH+filename);
    bgImg = bgImg.scaledToHeight(1080);


    x0=0;

    //resize(bgImg.size());
    QLabel *lbl;
    for (int i = 0;i<screenList.size();i++)
    {
        lbl = new QLabel(screenList.at(i));
        lbl->resize(bgImg.size());
        lbl->setPixmap(bgImg);
        lbl->move(x0,0);
        //  lbl->setStyleSheet("border: 1px solid black");
        lbls.append(lbl);


    }

    qDebug()<<bgImg.width();

    redraw();

}






void slideWindow::goForward()
{

    x0+=deltax;
    if(x0>totalWidth)
        x0 -=totalWidth;
    if(x0<0)
        x0 += totalWidth;


    redraw();

}



void slideWindow::goBackward()
{

    x0-=deltax;
    if(x0>totalWidth)
        x0 -=totalWidth;
    if(x0<0)
        x0 += totalWidth;


    redraw();

}


void slideWindow::redraw()
{



    if(x0<1920) //start on first screen
    {

        if(x0+bgImg.width()<1920)//fits on first screen
        {
            lbls[0]->move(x0,0);
            lbls[0]->show();
            lbls[1]->hide();
            lbls[2]->hide();




        }
        else if(x0+bgImg.width()<2*1920) //shows on both screen
        {



            lbls[0]->move(x0,0);

            lbls[1]->move(x0-1920,0);
            lbls[0]->show();
            lbls[1]->show();
            lbls[2]->hide();


        }
        else
        {



            lbls[0]->move(x0,0);

            lbls[1]->move(x0-1920,0);
            lbls[2]->move(x0-2*1920,0);
            lbls[0]->show();
            lbls[1]->show();
            lbls[2]->show();


        }

    }
    else if(x0<2*1920) //start on second screen
    {
        if(x0+bgImg.width()<1920*2)//fits on second screen
        {
            lbls[1]->move(x0-1920,0);
            lbls[1]->show();
            lbls[0]->hide();
            lbls[2]->hide();




        }
        else //shows on both screen
        {
            lbls[0]->hide();
            lbls[1]->move(x0-1920,0);
            lbls[1]->show();
            lbls[2]->move(x0-2*1920,0);
            lbls[2]->show();


        }

    }
    else if(x0<3*1920) //starts on third screen
    {
        if(x0+bgImg.width()<1920*3)//fits on third screen
        {
            lbls[2]->move(x0-2*1920,0);
            lbls[2]->show();
            lbls[1]->hide();


        }
        else //shows on both screen
        {
            lbls[1]->hide();
            lbls[2]->move(x0-2*1920,0);
            lbls[2]->show();
            if(x0+bgImg.width()>totalWidth)//goes back to first screen
            {
                lbls[0]->move(x0-totalWidth,0);
                lbls[0]->show();


            }


        }

    }


    else//starts outside any screen
    {




        if(x0+bgImg.width()>totalWidth)//goes back to first screen
        {

            if(x0-totalWidth<2*1920) //both screens
            {
                lbls[0]->move(x0-totalWidth,0);
                lbls[0]->show();
                lbls[1]->move(x0-totalWidth-1920,0);
                lbls[1]->show();

            }

            else if(x0-totalWidth<1920) //fits on first screen
            {
                lbls[1]->hide();
                lbls[2]->hide();
                lbls[0]->move(x0-totalWidth,0);
                lbls[0]->show();

            }



        }

    }


}







slideWindow::~slideWindow()
{
    qDebug()<<"closed showindow";
    for(auto lbl:lbls)
        lbl->deleteLater();



}


