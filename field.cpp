#include "field.h"
#include <QTime>
#include <QTimer>
#include <QtWidgets>
#include "mainwindow.h"
#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <QtDebug>

using namespace std;

//emit testSignal();

Field::Field(QWidget *parent) : QWidget(parent)
{

    wall_color = QColor(0, 0, 255, 255);
    coin_color = QColor(223, 201, 56, 255);
    cherry_color = QColor(255, 0, 0, 255);
    blank_color = QColor(0, 0, 0, 255);
    player_color = QColor(255, 255, 0, 255);
    ghost_color = QColor(255, 102, 255, 255);
    restart();

}

void Field::restart() {
    static char new_field[21][21] = {{'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'},
                                 {'.','#','o','o','o','o','o','o','o','o','#','o','o','o','o','o','o','o','o','#','.'},
                                 {'.','#','o','#','#','o','#','#','#','o','#','o','#','#','#','o','#','#','o','#','.'},
                                 {'.','#','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','#','.'},
                                 {'.','#','o','#','#','o','#','o','#','#','#','#','#','o','#','o','#','#','o','#','.'},
                                 {'.','#','o','o','o','o','#','o','o','o','#','o','o','o','#','o','o','o','o','#','.'},
                                 {'.','#','#','#','#','o','#','#','#','.','#','.','#','#','#','o','#','#','#','#','.'},
                                 {'.','.','.','.','#','o','#','.','.','.','.','.','.','.','#','o','#','.','.','.','.'},
                                 {'#','#','#','#','#','o','#','.','#','#','.','#','#','.','#','o','#','#','#','#','#'},
                                 {'.','.','.','.','.','o','.','.','#','.','.','.','#','.','.','o','.','.','.','.','.'},
                                 {'#','#','#','#','#','o','#','.','#','#','#','#','#','.','#','o','#','#','#','#','#'},
                                 {'.','.','.','.','#','o','#','.','.','.','0','.','.','.','#','o','#','.','.','.','.'},
                                 {'.','#','#','#','#','o','#','.','#','#','#','#','#','o','#','o','#','#','#','#','.'},
                                 {'.','#','o','o','o','o','o','o','o','o','#','o','o','o','o','o','o','o','o','#','.'},
                                 {'.','#','o','#','#','o','#','#','#','o','#','o','#','#','#','o','#','#','o','#','.'},
                                 {'.','#','o','o','#','o','o','o','o','o','o','o','o','o','o','o','#','o','o','#','.'},
                                 {'.','#','#','o','#','o','#','o','#','#','#','#','#','o','#','o','#','o','#','#','.'},
                                 {'.','#','o','o','o','o','#','o','o','o','#','o','o','o','#','o','o','o','o','#','.'},
                                 {'.','#','o','#','#','#','#','#','#','o','#','o','#','#','#','#','#','#','o','#','.'},
                                 {'.','#','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','o','#','.'},
                                 {'.','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','.'}};

    fieldAscii = reinterpret_cast<char*> (new_field);
    fieldWidth = 21;
    fieldHeight = 21;
    pm_x = 10;
    pm_y = 15;
    g_x = 10;
    g_y = 9;
    scoreCount = 0;
    weaknessOn = false;
    powerUpTimer = 0;
    gameOver = false;
    gamePaused = false;
}

void Field::paintEvent(QPaintEvent * Event) {

    QPainter painter(this);
    painter.fillRect(0, 0, 630, 630, wall_color);


    for (int i = 0; i < fieldWidth; ++i) {
        for (int j = 0; j < fieldHeight; ++j) {
            switch(/*fieldAscii[j][i]*/ *(fieldAscii + fieldWidth*j + i)) {
                case '#':

                    painter.fillRect(i*42, j*42, 42, 42, wall_color);
                break;

                case 'o':

                    painter.fillRect(i*42, j*42, 42, 42, blank_color);
                    painter.setBrush(coin_color);
                    painter.drawEllipse(i*42 + 12, j*42 + 12, 18, 18);

                break;

                case '.':

                    painter.fillRect(i*42, j*42, 42, 42, blank_color);

                break;

                case '0':

                    painter.fillRect(i*42, j*42, 42, 42, blank_color);
                    painter.setBrush(cherry_color);
                    painter.drawEllipse(i*42 + 10, j*42 + 10, 22, 22);

            }
        }
    }


    if (gameOver) {
       QFont font("Arial", 30);
       painter.setFont(font);
       //font.setPointSize(48);

       QRectF rect{0, 0, double(fieldSize), double(fieldSize)};

       painter.setPen(Qt::white);

       painter.drawText(rect, Qt::AlignHCenter | Qt::AlignVCenter, "Game Over");
    }

    if (int(powerUpTimer*10) % 2 != 0){
        painter.fillRect(pm_x*42, pm_y*42, 42, 42, Qt::white);
    }
    else {
        painter.fillRect(pm_x*42, pm_y*42, 42, 42, player_color);
    }

    painter.fillRect(g_x*42, g_y*42, 42, 42, blank_color);
    painter.setBrush(ghost_color);
    painter.drawEllipse(g_x*42 + 6, g_y*42 + 6, 30, 30);


}

//if there is a coin, pick it up and add to coin counter.
void Field::ifCoin() {
    if ( *(fieldAscii + fieldWidth*pm_y + pm_x) == 'o') {
        *(fieldAscii + fieldWidth*pm_y + pm_x) = '.';
        scoreCount++;
    }
    else if (*(fieldAscii + fieldWidth*pm_y + pm_x) == '0'){

        *(fieldAscii + fieldWidth*pm_y + pm_x) = '.';
        this->weaknessOn = true;
    }
}

//returns true if it is possible to move in that direction, starts moving if possible; otherwise false.
bool Field::pm_move(int key) {
    //testSignal();
    int x, y;
    switch (key)
      {
     case Qt::Key_Up:
        y = (pm_y - 1 + fieldHeight) % fieldHeight;
        x = pm_x;
        if (*(fieldAscii + fieldWidth*y + x) != '#') {

            pm_y = (pm_y - 1 + fieldHeight) % fieldHeight;

            return true;
        }

       break;

     case  Qt::Key_Down:
        y = (pm_y + 1) % fieldHeight;
        x = pm_x;
        if (*(fieldAscii + fieldWidth*y + x) != '#') {

            pm_y = (pm_y + 1) % fieldHeight;

            return true;
        }
       break;

     case  Qt::Key_Left:
        y = pm_y;
        x = (pm_x - 1 + fieldWidth) % fieldWidth;
        if (*(fieldAscii + fieldWidth*y + x) != '#') {

            pm_x = (pm_x - 1 + fieldWidth) % fieldWidth;

            return true;
        }
       break;

     case Qt::Key_Right:
        y = pm_y;
        x = (pm_x + 1) % fieldWidth;
        if (*(fieldAscii + fieldWidth*y + x) != '#') {

            pm_x = (pm_x + 1) % fieldWidth;

            return true;
        }
       break;
       }
    return false;
}

void Field::g_move () {
    // Ghost seeks shortest path to Pacman and moves 1 cell along this path

        int x = g_x;
        int y = g_y;

        int cur_x;
        int cur_y;

        queue <pair <int, int>> order;

        int matrix[fieldHeight][fieldWidth];

            for (int i = 0; i < fieldHeight; i++) {
                for (int j = 0; j < fieldWidth; j++) {
                    matrix[i][j] = -1;
                }
            }
        matrix[y][x] = 0;

        while (1) {
            ///UP
            cur_y = (y - 1 + fieldHeight) % fieldHeight;
            cur_x = x;
            if (*(fieldAscii + fieldWidth*cur_y + cur_x) != '#'
                && matrix[cur_y][cur_x] == -1) {

                matrix[cur_y][cur_x] = matrix[y][x] + 1;

                order.push(make_pair(cur_y,cur_x));

            }

            cur_y = (y + 1) % fieldHeight;
            cur_x = x;
            ///////////////////////////////////////////////////////////////
            ///DOWN
            if (*(fieldAscii + fieldWidth*cur_y + cur_x) != '#'
                && matrix[cur_y][cur_x] == -1) {

                matrix[cur_y][cur_x] = matrix[y][x] + 1;

                order.push(make_pair(cur_y,cur_x));

            }
            ///////////////////////////////////////////////////////////////
            cur_y = y;
            cur_x = (x + 1) % fieldWidth;
            ///RIGHT
            if (*(fieldAscii + fieldWidth*cur_y + cur_x) != '#'
                && matrix[cur_y][cur_x] == -1) {

                matrix[cur_y][cur_x] = matrix[y][x] + 1;

                order.push(make_pair(cur_y,cur_x));

            }
            ////////////////////////////////////////////////////////////////////
            cur_y = y;
            cur_x = (x - 1 + fieldWidth) % fieldWidth;
            ///LEFT
            if (*(fieldAscii + fieldWidth*cur_y + cur_x) != '#'
                && matrix[cur_y][cur_x] == -1) {

                matrix[cur_y][cur_x] = matrix[y][x] + 1;

                order.push(make_pair(cur_y,cur_x));

            }

            y = order.front().first;
            x = order.front().second;

            order.pop();

            if (x == pm_x && y == pm_y) {
                //matrix[cur_y][cur_x] = matrix[y][x] + 1;
                break;

            }
        }

    while (matrix[y][x] != 1) { ////////////////////////////////////////////////////////////////////
            ///UP
            cur_y = (y - 1 + fieldHeight) % fieldHeight;
            cur_x = x;
            if (matrix[cur_y][cur_x] == matrix[y][x] - 1) {
                x = cur_x;
                y = cur_y;

                continue;
            }
            ///////////////////////////////////////////////////////////////
            cur_y = (y + 1) % fieldHeight;
            cur_x = x;
            ///DOWN
            if (matrix[cur_y][cur_x] == matrix[y][x] - 1) {
                x = cur_x;
                y = cur_y;

                continue;
            }
            ///////////////////////////////////////////////////////////////
            cur_y = y;
            cur_x = (x + 1) % fieldWidth;
            ///RIGHT
            if (matrix[cur_y][cur_x] == matrix[y][x] - 1) {
                x = cur_x;
                y = cur_y;

                continue;
            }
            ////////////////////////////////////////////////////////////////////
            cur_y = y;
            cur_x = (x - 1 + fieldWidth) % fieldWidth;
            ///LEFT
            if (matrix[cur_y][cur_x] == matrix[y][x] - 1) {
                x = cur_x;
                y = cur_y;

                continue;
            }
        }

        g_y = y;
        g_x = x;

}





