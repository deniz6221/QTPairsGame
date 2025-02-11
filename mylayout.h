#ifndef MYLAYOUT_H
#define MYLAYOUT_H

#include <QGridLayout>
#include <algorithm>

class MyLayout : public QGridLayout
{
private:
    Q_OBJECT
public:
    MyLayout();
    int score;
    int chancesRemain;
    void setMyLayout();
    bool activeSlot;
    bool gameOver;
    signals:
        void scoreChanged(int score);
        void chancesChanged(int chancesRemain);
        void gameFinished(QString string);
public slots:
    void initializeGame();
    void buttonClicked();
    void activateSlot();
};


#endif // MYLAYOUT_H
