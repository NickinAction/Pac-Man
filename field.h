#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QTimer>

class Field : public QWidget
{
    Q_OBJECT
public:
    explicit Field(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent * Event) override;


    int coinCount = 0;
    bool bufferOn = false;

    double counter = 0;

signals:


public slots:

    void move(int key);
    bool canMove(int key);
    void ifCoin();

private:
    QColor wall_color;
    QColor coin_color;
    QColor cherry_color;
    QColor blank_color;
    QColor player_color;
    QColor ghost_color;

    char fieldAscii[21][21]={{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
                             {'.','#','o','o','o','o','o','o','o','o','#','o','o','o','o','o','o','o','o','#','.'},
                             {'.','#','o','#','#','o','#','#','#','o','#','o','#','#','#','o','#','#','o','#','.'},
                             {'.','#','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','#','.'},
                             {'.','#','o','#','#','o','#','o','#','#','#','#','#','o','#','o','#','#','o','#','.'},
                             {'.','#','o','o','o','o','#','o','o','o','#','o','o','o','#','o','o','o','o','#','.'},
                             {'.','#','#','#','#','o','#','#','#','.','#','.','#','#','#','o','#','#','#','#','.'},
                             {'.','.','.','.','#','o','#','.','.','.','.','.','.','.','#','o','#','.','.','.','.'},
                             {'#','#','#','#','#','o','#','.','#','#','#','#','#','.','#','o','#','#','#','#','#'},
                             {'.','.','.','.','.','o','.','.','#','.','.','.','#','.','.','o','.','.','.','.','.'},
                             {'#','#','#','#','#','o','#','.','#','#','#','#','#','.','#','o','#','#','#','#','#'},
                             {'.','.','.','.','#','o','#','.','.','0','.','.','.','.','#','o','#','.','.','.','.'},
                             {'.','#','#','#','#','o','#','.','#','#','#','#','#','o','#','o','#','#','#','#','.'},
                             {'.','#','o','o','o','o','o','o','o','o','#','o','o','o','o','o','o','o','o','#','.'},
                             {'.','#','o','#','#','o','#','#','#','o','#','o','#','#','#','o','#','#','o','#','.'},
                             {'.','#','o','o','#','o','o','o','o','o','o','o','o','o','o','o','#','o','o','#','.'},
                             {'.','#','#','o','#','o','#','o','#','#','#','#','#','o','#','o','#','o','#','#','.'},
                             {'.','#','o','o','o','o','#','o','o','o','#','o','o','o','#','o','o','o','o','#','.'},
                             {'.','#','o','#','#','#','#','#','#','o','#','o','#','#','#','#','#','#','o','#','.'},
                             {'.','#','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','#','.'},
                             {'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'}};


    const int fieldHeight =21;
    const int fieldWidth = 21;

    int pm_x = 10;
    int pm_y = 15;



};

#endif // FIELD_H
