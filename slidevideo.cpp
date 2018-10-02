#include "slidevideo.h"
#include "qdebug.h"
#include "qtimer.h"

#define FPS 24

slidevideo::slidevideo(QObject *parent, QString PATH, QList<QWidget *> screenList,int x0,int w0,int totalWidth,QString filename,int deltax):QObject(parent),PATH(PATH),screenList(screenList),x0(x0),w0(w0),totalWidth(totalWidth),filename(filename),deltax(deltax)
{


    //bgImg.load(PATH+filename);
    //bgImg = bgImg.scaledToHeight(1080);
    closed = false;



    //resize(bgImg.size());
    videoPlayer *lbl;
    lbls.clear();
    for (int i = 0;i<screenList.size();i++)
    {
        lbl = new videoPlayer(screenList.at(i),filename);
       // lbl->resize(w0,1080);
      //  lbl->move(x0,0);
        lbls.push_back(lbl);


    }

  //  connect(lbl,SIGNAL(destroyed(QObject*)),this,SLOT(deleteLater()));



  redraw();

}



void slidevideo::goBackward()
{
    x0-=deltax;

    if(x0>totalWidth)
        x0 -= totalWidth;
    if(x0<0)
        x0 += totalWidth;

    redraw();
}

void slidevideo::goForward()
{

    x0+=deltax;

    if(x0>totalWidth)
        x0 -= totalWidth;
    if(x0<0)
        x0 += totalWidth;


    redraw();


}

void slidevideo::redraw()
{

    if(closed)
    {
     //   qDebug()<<"-----------------LOCKED---------------------------";
        return;
    }


    if(x0<1920) //start on first screen
    {
        if(x0+w0<1920)//fits on first screen
        {
            lbls[0]->move(x0,0);
            lbls[0]->show();
            lbls[1]->hide();
            lbls[2]->hide();
        }
        else if(x0+w0<2*1920) //shows on both screen
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
        if(x0+w0<1920*2)//fits on second screen
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
        if(x0+w0<1920*3)//fits on third screen
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
            if(x0+w0>totalWidth)//goes back to first screen
            {
                lbls[0]->move(x0-totalWidth,0);
                lbls[0]->show();


            }



        }

    }
    else//starts outside any screen
    {




        if(x0+w0>totalWidth)//goes back to first screen
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


void slidevideo::shutdown()
{
    closed = true;

    for(auto lbl:lbls)
    {
        lbl->closePlayer();
    }

    lbls.clear();

    deleteLater();
}



slidevideo::~slidevideo()
{
    //for(auto lbl:lbls)
    //    lbl->deleteLater();
    qDebug()<<"closed video window";

}


