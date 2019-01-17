#ifndef MODULE2_H
#define MODULE2_H

#include <QWidget>
#include <qobject.h>
#include "qlabel.h"

#include "mpvwidget.h"
#include "qtimer.h"
#include "QDirIterator"
#include "qcolor.h"
#include "qcombobox.h"

#include "circlebutton.h"

#include "qmath.h"
#include "qlineedit.h"
#include "qstylefactory.h"

class countryIcon:public QLabel
{

    Q_OBJECT
public:
    explicit countryIcon(QLabel *parent, QString PATH="", QString name="", QColor color=Qt::red);

private:
    QString PATH,name;
    QColor color;
public:
    QString getName() {return name;}


protected:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void deleteClicked();
};


class countryBubble : public QLabel
{
    Q_OBJECT
public:
    explicit countryBubble(QWidget *parent = nullptr, QColor color=Qt::red, uint score=50, QString name="");

private:
    QColor color;
    uint score;
    QString name;

};


class module2 : public QLabel
{
    Q_OBJECT
public:
    explicit module2(QLabel *parent = nullptr, QString PATH="");
    void init();
private:
    QString PATH;
    std::vector<circleButton*> exploreBubbles;
    void loadExploreCountries();
    std::vector<QString> countryNames;
    std::vector<QColor> colors;
    mpvWidget *vp;
    QPropertyAnimation *videoSlide;
    QComboBox* combo;
  //  std::vector<countryIcon*> activeCountries;
std::vector<QString> activeCountries;
    std::vector<std::vector<uint>> compareScores;
    QStringList compareCountries;

    void loadCompareCountries();
    void organiseCountries();
    std::vector<countryBubble*>countryBubbles;
    std::vector<countryIcon*>countryIcons;

private slots:
    void showVideo(int countryId);
    void getVideoClicked(QPoint pos);
    void hideVideo();
    void goCompare();
    void goExplore();

    void addNewCountry(QString country);
    void removeCountry();

protected:
    void mousePressEvent(QMouseEvent *ev);


};

#endif
