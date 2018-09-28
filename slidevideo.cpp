#include "slidevideo.h"
#include "qdebug.h"
#include "qtimer.h"
#include "videoplayer.h"
#define FPS 24

slidevideo::slidevideo(QObject *parent, QString PATH, QList<QWidget *> screenList,int x0,int w0,int totalWidth,QString filename,int deltax):QObject(parent),PATH(PATH),screenList(screenList),x0(x0),w0(w0),totalWidth(totalWidth),filename(filename),deltax(deltax)
{


    //bgImg.load(PATH+filename);
    //bgImg = bgImg.scaledToHeight(1080);


    x0=0;

    //resize(bgImg.size());
    videoPlayer *lbl;
    for (int i = 0;i<screenList.size();i++)
    {
        lbl = new videoPlayer(screenList.at(i),filename);
        lbl->resize(w0,1080);
        lbl->move(x0,0);
        lbls.append(lbl);


        connect(lbls[0],SIGNAL(endReached()),lbl,SLOT(restart()));


    }



    mvTimer = new QTimer(this);
    connect(mvTimer,SIGNAL(timeout()),this,SLOT(mvPic()));
    mvTimer->start(1000/36);

    mv();

}






void slidevideo::mv()
{

    x0+=deltax;
    if(x0>totalWidth)
        x0 = 0;
    if(x0<0)
        x0 = totalWidth;



    if(lbls.size()==2)//2screens
    {

        if(x0<1920) //start on first screen
        {
            if(x0+w0<1920)//fits on first screen
            {
                lbls[0]->move(x0,0);
                lbls[0]->show();
                lbls[1]->hide();
            }
            else //shows on both screen
            {
                lbls[0]->move(x0,0);
                lbls[0]->show();
                lbls[1]->move(x0-1920,0);
                lbls[1]->show();
            }

        }
        else if(x0<2*1920) //starts on second screen
        {
            if(x0+w0<1920*2)//fits on second screen
            {
                lbls[1]->move(x0-1920,0);
                lbls[1]->show();
                lbls[0]->hide();
            }
            else //shows on both screen
            {
                lbls[1]->move(x0-1920,0);
                lbls[1]->show();
                if(x0+w0>totalWidth)//goes back to first screen
                {
                    lbls[0]->move(x0-totalWidth,0);
                    lbls[0]->show();

                }
            }

        }
        else//starts outside any screen
        {

            lbls[1]->hide();
            if(x0+w0>totalWidth)//goes back to first screen
            {
                lbls[0]->move(x0-totalWidth,0);
                lbls[0]->show();

            }

        }
    }
    else  if(lbls.size()==3)//3screens
    {


        if(x0<1920) //start on first screen
        {
            if(x0+w0<1920)//fits on first screen
            {
                lbls[0]->move(x0,0);
                lbls[0]->show();
                lbls[1]->hide();
            }
            else //shows on both screen
            {
                lbls[0]->move(x0,0);
                lbls[0]->show();
                lbls[1]->move(x0-1920,0);
                lbls[1]->show();
            }

        }
        else if(x0<2*1920) //start on first screen
        {
            if(x0+w0<1920*2)//fits on second screen
            {
                lbls[1]->move(x0-1920,0);
                lbls[1]->show();
                lbls[0]->hide();
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

            lbls[1]->hide();
            lbls[2]->hide();
            if(x0+w0>totalWidth)//goes back to first screen
            {
                lbls[0]->move(x0-totalWidth,0);
                lbls[0]->show();

            }

        }


    }


}






slidevideo::~slidevideo()
{
    qDebug()<<"closed showindow";

}


