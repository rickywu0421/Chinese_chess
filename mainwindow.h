#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtNetwork>
#include <QMediaPlayer>
#include <QMainWindow>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPixmap>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QPalette>
#include <QFont>
#include <QMessageBox>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include <QString>
#include <QList>
#include <QDebug>
#include "editordialog.h"
#define NUM 7
#define RADIUS 33


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public:
    void iniChess();
    void iniCanMove();
    void moveRule(int i,int j,int value);
    void moveOrEat(int i,int j,int value,int eaten_i,int eaten_j,int eaten_value);
    void winOrNot();
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    bool checkCanSet(int i,int j,int value);
    void connectedToServer();

public slots:
    void onePersonGameSlots();
    void multiPeopleGameSlots();
    void editorGameSlots();
    void editorGameStartSlots();
    void exitGameSlots();
    void tcpServerSlots();
    void tcpClientSlots();
    void acceptConnection();
    void readFromSocket();

private:
    QTcpServer *server;
    QTcpSocket *clientConnection;
    QTcpSocket *client;
    QList<QHostAddress> allAddress;
    QGraphicsScene *scene;
    QGraphicsTextItem *text;
    QFont font;
    QMediaPlayer *music;
    QPushButton *onePersonGame;
    QPushButton *multiPeopleGame;
    QPushButton *editorGame;
    QPushButton *finishEdit;
    QPushButton *exitGame;
    QLabel *wait;
    EditorDialog *dialog;

    int situation[9][10] = {0};
    bool canMove[9][10] = {false};
    int step = -1;
    int choseX,choseY;
    bool gameOver;
    bool waitFromSocket;
    bool hostIsServer = false;
    bool hostIsClient = false;
    int exitFromSingle = 0;
    int exitFromServer = 0;
    int exitFromClient = 0;
    int exitFromEditor = 0;
    int editorMode = 0;
    int chessInBoard = 0;
    int blackLeft[NUM] = {1,2,2,2,2,2,5};
    int redLeft[NUM] = {1,2,2,2,2,2,5};
};

#endif // MAINWINDOW_H
