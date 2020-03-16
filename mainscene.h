#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include <mine.h>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = 0);
    ~MainScene();

    QTimer m_timer;
    Mine m_mine;
    int rtime;
    int m_face;
    int counter;
    bool end;

    void LeftPress(int x, int y);
    void rightPress(int x,  int y);
    void Restart();
    void paintEvent(QPaintEvent *);
    void Init_Scene();
    void mousePressEvent(QMouseEvent *event);
    void gameOver();

public slots:
    void updateTime();
};

#endif // MAINSCENE_H
