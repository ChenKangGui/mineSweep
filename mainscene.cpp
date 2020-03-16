#include "mainscene.h"
#include "config.h"
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <ctime>
#include <QAction>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
{
    Init_Scene();
    Restart();
}

MainScene::~MainScene()
{

}

void MainScene::Init_Scene()
{
    setWindowTitle("扫雷");
    setFixedSize(GRID_X*20,GRID_Y*20+40);


    connect( &m_timer,SIGNAL(timeout()), this, SLOT(updateTime()) );
}

void MainScene::Restart()
{
    for(int j=0; j<GRID_X; j++)
    {
        for(int i=0; i<GRID_Y; i++)
        {
            m_mine.map[j][i] = 0;
        }
    }

    qsrand(time(NULL));
    for(int i=0; i<MINE_NUM; i++)   //布雷
    {
        int x = rand() % GRID_X;
        int y = rand() % GRID_Y;
        if(m_mine.map[x][y] != 99)
        {
            m_mine.map[x][y] = 99;
        }
        else
        {
            i--;
        }
    }

    for(int x=0; x<GRID_X; x++)
    {
        for(int y=0; y<GRID_Y; y++)
        {
            if(m_mine.map[x][y] == 99)
            {
                for(int i=-1; i<=1; i++)
                {
                    for(int j=-1; j<=1; j++)
                    {
                        if(x+i<0 || x+i>=GRID_X || y+j<0 || y+j>=GRID_Y || m_mine.map[x+i][y+j]==99)
                            continue;
                        m_mine.map[x+i][y+j]++;
                    }
                }
            }
        }
    }

    m_mine.nMine = 50;
    rtime = 0;
    m_face = 1;
    counter = 0;
    end = false;
    m_timer.start(10);
    update();
}

void MainScene::paintEvent(QPaintEvent *)
{
    QPixmap num(":/res/item1.bmp");
    QPixmap mine(":/res/item2.bmp");
    QPixmap top(":/res/item3.bmp");
    QPixmap face(":/res/item4.bmp");

    QPainter painters(this);

    //顶部框架
    painters.drawPixmap(0, 0, top, 0, 0, 70, 40);
    painters.drawPixmap(180, 0, top, 80, 0, 40, 40);
    painters.drawPixmap(330, 0, top, 130, 0, 70, 40);
    painters.drawPixmap(70, 0, 110, 40, top, 70, 0, 10, 40);
    painters.drawPixmap(220, 0, 110, 40, top, 70, 0, 10, 40);
    painters.drawPixmap(180+7, 7, face, m_face * 24, 0, 24, 24);

    //剩余雷数目
    painters.drawPixmap(27, 7, num, m_mine.nMine/10 * 20, 0, 20, 27);
    painters.drawPixmap(7, 7, num, 0, 0, 20, 27);
    painters.drawPixmap(47, 7, num, m_mine.nMine%10 * 20, 0, 20, 27);

    //时间
    painters.drawPixmap(337, 7, num, rtime/100 *20, 0, 20, 27);
    painters.drawPixmap(357, 7, num, rtime%100/10 *20, 0, 20, 27);
    painters.drawPixmap(377, 7, num, rtime%10 *20, 0, 20, 27);

    //扫雷区
    for(int x=0; x<GRID_X; x++)
    {
        for(int y=0; y<GRID_Y; y++)
        {
            if(m_mine.map[x][y] >= 0 && m_mine.map[x][y]<=8 || m_mine.map[x][y] == 99)
            {
                painters.drawPixmap(x*20, y*20+40, mine, 10 *20, 0, 20, 20);
            }
            if(m_mine.map[x][y] >=100 && m_mine.map[x][y] <=108)
            {
                painters.drawPixmap(x*20, y*20+40, mine, m_mine.map[x][y]%100*20, 0, 20, 20);
            }
            else if(m_mine.map[x][y] == 199)
            {
                painters.drawPixmap(x*20, y*20+40, mine, 9*20, 0, 20, 20);
            }
            else if(m_mine.map[x][y] >= 50 && m_mine.map[x][y] <= 58 || m_mine.map[x][y] == 145)
            {
                painters.drawPixmap(x*20, y*20+40, mine, 11*20, 0, 20, 20);
            }
            else if(m_mine.map[x][y] == 245)
            {
                painters.drawPixmap(x*20, y*20+40, mine, 12*20, 0, 20, 20);
            }
            else if(m_mine.map[x][y] >= 200 && m_mine.map[x][y]<=208)
            {
                painters.drawPixmap(x*20, y*20+40, mine, (m_mine.map[x][y]-100)%100*20, 0, 20, 20);
            }
        }
    }
}


void MainScene::mousePressEvent(QMouseEvent *event)
{
    if(event->x()< 0 || event->x() > 400 || event->y()<0 || event->y()>440)
        return ;

    int x = event->x();
    int y = event->y();

    if(event->button() == Qt::LeftButton)
    {
        if(x>=187 && x<=211 && y>0 && y<=24)
            Restart();
        else if(y>24)
        {
            if(end)
                return ;
            x /= 20;
            y = (y-40)/20;
            if(m_mine.map[x][y] == 99)
            {
                gameOver();
            }
            else
            {
                if(m_mine.map[x][y]<=8)
                {
                    LeftPress(x, y);
                }
            }
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        if(y>24 && end != true)
        {
            x /= 20;
            y = (y-40)/20;

            rightPress(x, y);
        }
    }

}

void MainScene::rightPress(int x, int y)
{
    if(m_mine.map[x][y] >=0 && m_mine.map[x][y] <= 8)
    {
        m_mine.nMine--;
        m_mine.map[x][y] += 50;
    }
    else if(m_mine.map[x][y] >= 50 && m_mine.map[x][y] <= 58)
    {
        m_mine.nMine++;
        m_mine.map[x][y] -= 50;
    }
    else if(m_mine.map[x][y] == 99)
    {
        m_mine.nMine--;
        m_mine.map[x][y] = 145;
    }
    else if(m_mine.map[x][y] == 145)
    {
        if(m_mine.nMine <50)
        {
            m_mine.nMine++;
        }
        m_mine.map[x][y] = 99;
    }
    else
    {
        qDebug() << m_mine.map[x][y];
    }

    if(m_mine.nMine==0)
    {
        int num =0;
        for(int i; i<GRID_X; i++)
        {
            for(int j=0; j<GRID_Y; j++)
            {
                if(m_mine.map[i][j] == 145)
                {
                    num++;
                }
                m_mine.map[i][j] += 100;
            }
        }

        if(num < 50)
        {
            m_face -= 1;
            QMessageBox::information(NULL, "提示", "完了，你被炸死了！！", QMessageBox::Yes, QMessageBox::Yes);
        }
        else
        {
            QMessageBox::information(NULL, "提示", "恭喜，你赢了！！", QMessageBox::Yes, QMessageBox::Yes);
        }

        m_timer.stop();
        update();
        end = true;
    }
    update();
}


void MainScene::LeftPress(int x, int y)
{
    for(int m=x-1; m<=x+1; m++)
    {
        for(int n=y-1; n<=y+1; n++)
        {
            if(m<0 || n<0 || m>=GRID_X || n>=GRID_Y)
                continue;
            if(m_mine.map[m][n]<10)
            {
                m_mine.map[m][n] += 100;
                if(m_mine.map[m][n] == 100)
                    LeftPress(m, n);
            }
        }
    }
    int num = 0;
    for(int i=0; i<GRID_X; i++)
    {
        for(int j=0; j<GRID_Y; j++)
        {
            if(m_mine.map[i][j]>=100 && m_mine.map[i][j]<=108)
            {
                num++;
                if(num>=250)
                {
                    for(int x=0; x<GRID_X; x++)
                    {
                        for(int y=0; y<GRID_Y; y++)
                        {
                            if(m_mine.map[x][y] == 99)
                            {
                                m_mine.map[x][y] += 100+55;
                            }
                            else
                            {
                                m_mine.map[x][y] += 100;
                            }
                        }
                    }
                    m_mine.nMine = 0;
                    m_timer.stop();
                    update();
                    end = true;
                    QMessageBox::information(NULL, "提示", "恭喜，你赢了！！", QMessageBox::Yes, QMessageBox::Yes);
                    break;
                }
            }
        }
    }
    qDebug() << num;
}

void MainScene::gameOver()
{
    for(int x=0; x<GRID_X; x++)
    {
        for(int y=0; y<GRID_Y; y++)
        {
            if(m_mine.map[x][y] == 99 || m_mine.map[x][y] == 145)
            {
                m_mine.map[x][y] += 100;
            }
        }
    }
    m_face = 0;
    m_timer.stop();
    update();
    QMessageBox::information(NULL, "提示", "完了，你被炸死了！！", QMessageBox::Yes, QMessageBox::Yes);
}

void MainScene::updateTime()
{
    counter++;
    if(counter >= 100)
    {
        counter = 0;
        rtime++;
    }
    if(rtime >= 999)  //超时
    {
        gameOver();
    }
    update();
}
