#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include "mylayout.h"
#include "cards.h"

int main(int argc, char* argv[]){
    //Create application, central widget and layouts.
    QApplication app(argc, argv);
    QWidget *cw = new QWidget;
    QHBoxLayout *scoreLayout = new QHBoxLayout();
    QHBoxLayout *chanceLayout = new QHBoxLayout();
    QHBoxLayout *gameOverLayout = new QHBoxLayout();
    QHBoxLayout *firstLine = new QHBoxLayout();

    QVBoxLayout *cl = new QVBoxLayout(cw);
    QPushButton *ng = new QPushButton();

    ng->setText("New Game");
    cw->setWindowTitle("Pairs");

    //Create a MyLayout object.
    MyLayout *layout = new MyLayout();
    layout->setMyLayout();

    //Create two labels to display the score.
    QLabel *label = new QLabel;
    QLabel *scoreLabel = new QLabel;
    label->setText("Score: ");
    scoreLabel->setText(QString::number(layout->score));

    //Create two labels to display the remaining chances.
    QLabel *labelTwo = new QLabel;
    QLabel *chanceLabel = new QLabel;
    labelTwo->setText("No. of Tries\nRemaining");
    chanceLabel->setText(QString::number(layout->chancesRemain));

    //Create a game over label. Displays YOU WON or GAME OVER when the game is over.
    QLabel *gameOverLabel = new QLabel;
    gameOverLabel->setText("");
    gameOverLayout->addWidget(gameOverLabel);

    chanceLayout->addWidget(labelTwo);
    chanceLayout->addWidget(chanceLabel);
    chanceLayout->insertStretch(-1, 1);

    scoreLayout->addWidget(label);
    scoreLayout->addWidget(scoreLabel);
    scoreLayout->insertStretch(-1, 1);


    firstLine->addLayout(scoreLayout);
    firstLine->addLayout(chanceLayout);
    firstLine->addWidget(ng);

    cl->addLayout(firstLine);
    cl->addLayout(gameOverLayout);
    cl->addWidget(new QLabel(""));
    cl->addLayout(layout);

    //Connect the new game button to initializeGame slot.
    QObject::connect(ng, &QPushButton::clicked, layout, &MyLayout::initializeGame);

    //Connect all cards to buttonClicked slot.
    for (int i = 0; i < 30; ++i) {
        cards *card = qobject_cast<cards*>(layout->itemAt(i)->widget());
        QObject::connect(card, &cards::clicked, layout, &MyLayout::buttonClicked);
    }

    //If a scoreChanged signal is emited from the layout change the score label.
    QObject::connect(layout, &MyLayout::scoreChanged, [scoreLabel](int score){
        scoreLabel->setText(QString::number(score));
    });

    //If a chancesChanged signal is emited from the layout change the chance label.
    QObject::connect(layout, &MyLayout::chancesChanged, [chanceLabel](int chancesRemain){
        chanceLabel->setText(QString::number(chancesRemain));
    });

    //If a gameFinished signal is emited from the layout change the gameOver label.
    QObject::connect(layout, &MyLayout::gameFinished, [gameOverLabel](QString string){
        gameOverLabel->setText(string);
    });




    cw->show();

    return app.exec();
}


