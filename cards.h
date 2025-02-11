#ifndef CARDS_H
#define CARDS_H

#include <QPushButton>

class cards : public QPushButton
{
private:
    Q_OBJECT
public:
    cards();
    QString name;

};


#endif // CARDS_H
