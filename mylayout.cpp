#include "mylayout.h"
#include <cards.h>
#include <iostream>
#include <QLabel>
#include <QEventLoop>
#include <QTimer>
#include <QApplication>

//Creating an object with this constructor basically creates a QGridLayout object.
MyLayout::MyLayout():QGridLayout()
{

}


//This slot is for initalizing the game. If a user clicks the new game button this function will be triggered.
void MyLayout::initializeGame(){
    chancesRemain = 50;
    gameOver = false;
    activeSlot = true;
    score = 0;
    emit chancesChanged(50);
    emit gameFinished("");
    emit scoreChanged(0);

    //Card texts
    QString strings[30] = {"tree", "tree", "flower", "flower",
                           "river", "river", "mountain", "mountain",
                           "sun", "sun", "moon", "moon",
                           "star", "star", "ocean", "ocean", "bird", "bird"
                           , "fish", "fish", "butterfly", "butterfly", "cloud"
                           , "cloud", "rain", "rain", "snow", "snow", "wind","wind"};

    //Shuffle the above array for random cards each time.
    std::srand(std::time(0));
    std::random_shuffle(&strings[0], &strings[29]);
    for (int i = 0; i < 30; ++i) {
        cards* card = qobject_cast<cards*>(this->itemAt(i)->widget());
        card->setText("?");
        card->name = strings[i];
    }

}

//This function is trigerred initially to create cards and set texts, names.
void MyLayout::setMyLayout(){
    chancesRemain = 50;
    gameOver = false;
    activeSlot = true;
    score = 0;

    //Card texts
    QString strings[30] = {"tree", "tree", "flower", "flower",
                           "river", "river", "mountain", "mountain",
                           "sun", "sun", "moon", "moon",
                           "star", "star", "ocean", "ocean", "bird", "bird"
                           , "fish", "fish", "butterfly", "butterfly", "cloud"
                           , "cloud", "rain", "rain", "snow", "snow", "wind","wind"};

    //Shuffle the above array for random cards each time.
    std::srand(std::time(0));
    std::random_shuffle(&strings[0], &strings[29]);
    int count = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            cards *card = new cards();
            card->name = strings[count];
            card->setText("?");
            count++;
            this->addWidget(card, i, j, 1, 1);
        }
    }
}

//Slot to control button clicks. Compares cards and takes appropriate actions if there were an open card before the button.
void MyLayout::buttonClicked(){
    int otherIndex = -2;
    cards* card = qobject_cast<cards*>(sender());

    //If the card is already open, game is over or there are two cards open currently do nothing.
    if(QString::compare(card->text(), "?") != 0 || !activeSlot || gameOver){
        return;
    }

    //Find the other open card if it exists.
    for (int i = 0; i < 30; ++i) {
        cards* searchedCard = qobject_cast<cards*>(this->itemAt(i)->widget());
        if(QString::compare("?", searchedCard->text()) != 0 && QString::compare("", searchedCard->name) != 0){
            otherIndex = i;
            break;
        }
    }
    //If there isn't another card simply open the current card.
    if(otherIndex == -2){
        card->setText(card->name);
    }
    else{
        //Decrease remaining chances and emit a signal to inform the main widget.
        chancesRemain--;
        emit chancesChanged(chancesRemain);
        //Set gameOver to true if no chances remain.
        if(chancesRemain == 0){
            gameOver = true;
        }


        cards* revealedCard = qobject_cast<cards*>(this->itemAt(otherIndex)->widget());
        card->setText(card->name);

        //Deactivate this slot for 1 second to show both cards to the user. The user can't click cards in this interval.
        activeSlot = false;
        QTimer::singleShot(1000, this, &MyLayout::activateSlot);
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, &QEventLoop::quit);
        loop.exec();

        //Compare both cards. If they are the same set the text of these cards to empty.
        if(QString::compare(card->name, revealedCard->name) == 0){
            score++;
            card->name = "";
            revealedCard->name = "";
            card->setText("");
            revealedCard->setText("");
            //If the score is 15 emit a signal to inform main widget. The user wins when this is the case.
            if(score == 15){
                gameOver = true;
                emit MyLayout::scoreChanged(score);
                emit gameFinished("YOU WON!");
            }
            //Emit a signal to change the score label. Emit a gameFinished signal if no chances remain.
            else{
                emit MyLayout::scoreChanged(score);
                if(gameOver){
                    emit gameFinished("GAME OVER");
                }
            }

        }
        //If the cards are not equal, set both card names to "?". Check if there are chances left, if not emit a game over signal.
        else{
            revealedCard->setText("?");
            card->setText("?");
            if(gameOver){
                emit gameFinished("GAME OVER");
            }

        }
    }
}

//Slot to re-activate the buttonClicked slot after two buttons are opened.
void MyLayout::activateSlot(){
    activeSlot = true;
}
