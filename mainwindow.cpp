#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gameOver = true;

    this->setFixedSize(1000,800);
    this->setWindowTitle("李維兵長的象棋遊戲");

    QPalette palette;
    palette.setBrush(QPalette::Background,QPixmap(":/img/image/board.png"));
    this->setPalette(palette);

    font.setPixelSize(30);
    font.setFamily("Microsoft YaHei");

    scene = new QGraphicsScene(700,0,300,800);

    text = scene->addText("星爆氣流盤",font);
    text->setPos(780,40);
    text->setDefaultTextColor(Qt::black);

    ui->graphicsView->setFrameStyle(0);
    ui->graphicsView->setScene(scene);

    onePersonGame = new QPushButton("單人模式",this);
    onePersonGame->setGeometry(790,200,150,40);
    onePersonGame->setStyleSheet("font-size:18px;"
                             "font-family:DFKai-SB;"
                             "background-color:#FFA07A;");

    multiPeopleGame = new QPushButton("連線對戰",this);
    multiPeopleGame->setGeometry(790,300,150,40);
    multiPeopleGame->setStyleSheet("font-size:18px;"
                             "font-family:DFKai-SB;"
                             "background-color:#FFA07A;");

    editorGame = new QPushButton("編輯模式",this);
    editorGame->setGeometry(790,400,150,40);
    editorGame->setStyleSheet("font-size:18px;"
              "font-family:DFKai-SB;"
              "background-color:#FFA07A;");

    exitGame = new QPushButton("離開遊戲",this);
    exitGame->setGeometry(790,500,150,40);
    exitGame->setStyleSheet("font-size:18px;"
              "font-family:DFKai-SB;"
              "background-color:#FFA07A;");

    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/sounds/music/Aldnoah.Zero_OST_MKAlieZ.mp3"));
    music->play();


    allAddress = QNetworkInterface::allAddresses();
    foreach(QHostAddress addr,allAddress){
        if(addr.protocol() == QAbstractSocket::IPv4Protocol &&
                addr != QHostAddress(QHostAddress::LocalHost)){
            qDebug() << QString("IP:%1").arg(addr.toString());
        }
    }

    connect(onePersonGame,SIGNAL(clicked(bool)),this,SLOT(onePersonGameSlots()));
    connect(multiPeopleGame,SIGNAL(clicked(bool)),this,SLOT(multiPeopleGameSlots()));
    connect(editorGame,SIGNAL(clicked(bool)),this,SLOT(editorGameSlots()));
    connect(exitGame,SIGNAL(clicked(bool)),this,SLOT(exitGameSlots()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::iniChess(){
    //set situation
    memset(situation,0,sizeof(situation));
    //黑棋
    situation[4][0] = 1;
    situation[3][0] = 2;
    situation[5][0] = 2;
    situation[2][0] = 3;
    situation[6][0] = 3;
    situation[0][0] = 4;
    situation[8][0] = 4;
    situation[1][0] = 5;
    situation[7][0] = 5;
    situation[1][2] = 6;
    situation[7][2] = 6;
    for(int i = 0;i < 5;++i) situation[i * 2][3] = 7;
    //紅棋
    situation[4][9] = -1;
    situation[3][9] = -2;
    situation[5][9] = -2;
    situation[2][9] = -3;
    situation[6][9] = -3;
    situation[0][9] = -4;
    situation[8][9] = -4;
    situation[1][9] = -5;
    situation[7][9] = -5;
    situation[1][7] = -6;
    situation[7][7] = -6;
    for(int i = 0;i < 5;++i) situation[i * 2][6] = -7;
    iniCanMove();
}
void MainWindow::iniCanMove()
{
    for(int i = 0;i < 9;++i){
        for(int j = 0;j < 10;++j){
            canMove[i][j] = false;
        }
    }
}
void MainWindow::moveRule(int i,int j,int value){
    //所有棋子的移動規則
    bool bossToBoss,isSkip;
    switch(value){
    case 1: //黑將
        bossToBoss = false;
        if(i-1>=3 && situation[i-1][j] <= 0){
            for (int y=j+1;y < 10;++y) {
                if(situation[i-1][y] != 0){
                    if(situation[i-1][y] != -1){
                        break;
                    }
                    else {
                        bossToBoss = true;
                        break;
                    }
                }
            }
            if(bossToBoss == false){
                canMove[i-1][j] = true;
            }
        }
        bossToBoss = false;
        if(i+1<=5 && situation[i+1][j] <= 0){
            for (int y=j+1;y < 10;++y) {
                if(situation[i+1][y] != 0){
                    if(situation[i+1][y] != -1){
                        break;
                    }
                    else {
                        bossToBoss = true;
                        break;
                    }
                }
            }
            if(bossToBoss == false){
                canMove[i+1][j] = true;
            }
        }
        if(j-1>=0 && situation[i][j-1] <= 0) canMove[i][j-1] = true;
        if(j+1<=2 && situation[i][j+1] <= 0) canMove[i][j+1] = true;
        break;
    case 2://黑仕
        if(i-1>=3 && j-1>=0 && situation[i-1][j-1] <= 0) canMove[i-1][j-1] = true;
        if(i-1>=3 && j+1<=2 && situation[i-1][j+1] <= 0) canMove[i-1][j+1] = true;
        if(i+1<=5 && j-1>=0 && situation[i+1][j-1] <= 0) canMove[i+1][j-1] = true;
        if(i+1<=5 && j+1<=2 && situation[i+1][j+1] <= 0) canMove[i+1][j+1] = true;
        break;
    case 3://黑象
        if(i-2>=0 && j-2>=0 && situation[i-2][j-2] <= 0) canMove[i-2][j-2] = true;
        if(i-2>=0 && j+2<=4 && situation[i-2][j+2] <= 0) canMove[i-2][j+2] = true;
        if(i+2<=8 && j-2>=0 && situation[i+2][j-2] <= 0) canMove[i+2][j-2] = true;
        if(i+2<=8 && j+2<=4 && situation[i+2][j+2] <= 0) canMove[i+2][j+2] = true;
        break;
    case 4://黑車
        for(int x = 1;x <=8;++x){ //橫向右
            if(i+x <=8 && situation[i+x][j] ==0){
                canMove[i+x][j] = true;
            }
            else if(i+x <=8 && situation[i+x][j] <0){
                canMove[i+x][j] = true;
                break;
            }
            else break;
        }
        for(int x = 1;x <=8;++x){ //橫向左
            if(i-x >=0 && situation[i-x][j] ==0){
                canMove[i-x][j] = true;
            }
            else if(i-x >=0 && situation[i-x][j] <0){
                canMove[i-x][j] = true;
                break;
            }
            else break;
        }
        for(int y = 1;y <=9;++y){ //橫向下
            if(j+y<=9 && situation[i][j+y] == 0){
                canMove[i][j+y] = true;
            }
            else if(j+y<=9 && situation[i][j+y] < 0){
                canMove[i][j+y] = true;
                break;
            }
            else break;
        }
        for(int y = 1;y <=9;++y){ //橫向上
            if(j-y>=0 && situation[i][j-y] == 0){
                canMove[i][j-y] = true;
            }
            else if(j-y>=0 && situation[i][j-y] < 0){
                canMove[i][j-y] = true;
                break;
            }
            else break;
        }
        break;
    case 5://黑馬
        if(situation[i+1][j] == 0){
            if(i+2<=8 && j-1>=0 && situation[i+2][j-1] <= 0) canMove[i+2][j-1] = true;
            if(i+2<=8 && j+1<=9 && situation[i+2][j+1] <=0) canMove[i+2][j+1] = true;
        }
        if(situation[i][j-1] == 0){
            if(i-1>=0 && j-2>=0 && situation[i-1][j-2] <=0) canMove[i-1][j-2] = true;
            if(i+1<=8 && j-2>=0 && situation[i+1][j-2] <=0) canMove[i+1][j-2] = true;
        }
        if(situation[i-1][j] == 0){
            if(i-2>=0 && j-1>=0 && situation[i-2][j-1] <=0) canMove[i-2][j-1] = true;
            if(i-2>=0 && j+1<=9 && situation[i-2][j+1] <=0) canMove[i-2][j+1] = true;
        }
        if(situation[i][j+1] == 0){
            if(i-1>=0 && j+2<=9 && situation[i-1][j+2] <=0) canMove[i-1][j+2] = true;
            if(i+1<=8 && j+2<=9 && situation[i+1][j+2] <=0) canMove[i+1][j+2] = true;
        }
        break;
    case 6://黑炮
        isSkip = false;
        for(int x = 1;x <=8;++x){ //橫向右
            if(i+x <=8 && situation[i+x][j] == 0 && isSkip == false) canMove[i+x][j] = true;
            else if(i+x <=8 && situation[i+x][j] != 0 && isSkip == false){
                isSkip = true;
            }
            else if(i+x <=8 && situation[i+x][j] < 0 && isSkip == true){
                canMove[i+x][j] = true;
                break;
            }
        }
        isSkip = false;
        for(int x = 1;x <=8;++x){ //橫向左
            if(i-x >=0 && situation[i-x][j] == 0 && isSkip == false) canMove[i-x][j] = true;
            else if(i-x >=0 && situation[i-x][j] != 0 && isSkip == false){
                isSkip = true;
            }
            else if(i-x >=0 && situation[i-x][j] < 0 && isSkip == true){
                canMove[i-x][j] = true;
                break;
            }
        }
        isSkip = false;
        for(int y = 1;y <=9;++y){ //直向上
            if(j+y <=9 && situation[i][j+y] == 0 && isSkip == false) canMove[i][j+y] = true;
            else if(j+y <=9 && situation[i][j+y] != 0 && isSkip == false){
                isSkip = true;
            }
            else if(j+y <= 9 && situation[i][j+y] < 0 && isSkip == true){
                canMove[i][j+y] = true;
                break;
            }
        }
        isSkip = false;
        for(int y = 1;y <=9;++y){ //直向下
            if(j-y >=0 && situation[i][j-y] == 0 && isSkip == false) canMove[i][j-y] = true;
            else if(j-y >=0 && situation[i][j-y] != 0 && isSkip == false){
                isSkip = true;
            }
            else if(j-y>=0 && situation[i][j-y] < 0 && isSkip == true){
                canMove[i][j-y] = true;
                break;
            }
        }
        break;
    case 7: //黑卒
        if(j < 5){
            if(situation[i][j+1] <= 0) canMove[i][j+1] = true;
        }
        else{
            if(i-1>=0 && situation[i-1][j] <= 0) canMove[i-1][j] = true;
            if(i+1<=8 && situation[i+1][j] <= 0) canMove[i+1][j] = true;
            if(j+1<=9 && situation[i][j+1] <= 0) canMove[i][j+1] = true;
        }
        break;
    case -1: //紅帥
        bossToBoss = false;
        if(i-1>=3 && situation[i-1][j] >= 0){
            for (int y=j-1;y >= 0 ;--y) {
                if(situation[i-1][y] != 0){
                    if(situation[i-1][y] != 1){
                        break;
                    }
                    else {
                        bossToBoss = true;
                        break;
                    }
                }
            }
            if(!bossToBoss){
                canMove[i-1][j] = true;
            }
        }
        bossToBoss = false;
        if(i+1<=5 && situation[i+1][j] >= 0){
            for (int y=j-1;y >= 0;--y) {
                if(situation[i+1][y] != 0){
                    if(situation[i+1][y] != 1){
                        break;
                    }
                    else {
                        bossToBoss = true;
                        break;
                    }
                }
            }
            if(!bossToBoss){
                canMove[i+1][j] = true;
            }

        }
        if(j-1>=7 && situation[i][j-1] >= 0) canMove[i][j-1] = true;
        if(j+1<=9 && situation[i][j+1] >= 0) canMove[i][j+1] = true;
        break;
    case -2: //紅士
        if(i-1>=3 && j-1>=7 && situation[i-1][j-1] >= 0) canMove[i-1][j-1] = true;
        if(i-1>=3 && j+1<=9 && situation[i-1][j+1] >= 0) canMove[i-1][j+1] = true;
        if(i+1<=5 && j-1>=7 && situation[i+1][j-1] >= 0) canMove[i+1][j-1] = true;
        if(i+1<=5 && j+1<=9 && situation[i+1][j+1] >= 0) canMove[i+1][j+1] = true;
        break;
    case -3: //紅相
        if(i-2>=0 && j-2>=5 && situation[i-2][j-2] >= 0) canMove[i-2][j-2] = true;
        if(i-2>=0 && j+2<=9 && situation[i-2][j+2] >= 0) canMove[i-2][j+2] = true;
        if(i+2<=8 && j-2>=5 && situation[i+2][j-2] >= 0) canMove[i+2][j-2] = true;
        if(i+2<=8 && j+2<=9 && situation[i+2][j+2] >= 0) canMove[i+2][j+2] = true;
        break;
    case -4: //紅車
        for(int x = 1;x <=8;++x){ //橫向右
            if(i+x <=8 && situation[i+x][j] == 0){
                canMove[i+x][j] = true;
            }
            else if(i+x <=8 && situation[i+x][j] > 0){
                canMove[i+x][j] = true;
                break;
            }
            else break;
        }
        for(int x = 1;x <=8;++x){ //橫向左
            if(i-x >=0 && situation[i-x][j] ==0){
                canMove[i-x][j] = true;
            }
            else if(i-x >=0 && situation[i-x][j] > 0){
                canMove[i-x][j] = true;
                break;
            }
            else break;
        }
        for(int y = 1;y <=9;++y){ //橫向下
            if(j+y<=9 && situation[i][j+y] == 0){
                canMove[i][j+y] = true;
            }
            else if(j+y<=9 && situation[i][j+y] > 0){
                canMove[i][j+y] = true;
                break;
            }
            else break;
        }
        for(int y = 1;y <=9;++y){ //橫向上
            if(j-y>=0 && situation[i][j-y] == 0){
                canMove[i][j-y] = true;
            }
            else if(j-y>=0 && situation[i][j-y] > 0){
                canMove[i][j-y] = true;
                break;
            }
            else break;
        }
        break;
    case -5: //紅馬
        if(situation[i+1][j] == 0){
            if(i+2<=8 && j-1>=0 && situation[i+2][j-1] >=0) canMove[i+2][j-1] = true;
            if(i+2<=8 && j+1<=9 && situation[i+2][j+1] >=0) canMove[i+2][j+1] = true;
        }
        if(situation[i][j-1] == 0){
            if(i-1>=0 && j-2>=0 && situation[i-1][j-2] >=0) canMove[i-1][j-2] = true;
            if(i+1<=8 && j-2>=0 && situation[i+1][j-2] >=0) canMove[i+1][j-2] = true;
        }
        if(situation[i-1][j] == 0){
            if(i-2>=0 && j-1>=0 && situation[i-2][j-1] >=0) canMove[i-2][j-1] = true;
            if(i-2>=0 && j+1<=9 && situation[i-2][j+1] >=0) canMove[i-2][j+1] = true;
        }
        if(situation[i][j+1] == 0){
            if(i-1>=0 && j+2<=9 && situation[i-1][j+2] >=0) canMove[i-1][j+2] = true;
            if(i+1<=8 && j+2<=9 && situation[i+1][j+2] >=0) canMove[i+1][j+2] = true;
        }
        break;
    case -6: //紅炮
        isSkip = false;
        for(int x = 1;x <=8;++x){ //橫向右
            if(i+x <=8 && situation[i+x][j] == 0 && isSkip == false) canMove[i+x][j] = true;
            else if(i+x <=8 && situation[i+x][j] != 0 && isSkip == false){
                isSkip = true;
            }
            else if(i+x <=8 && situation[i+x][j] > 0 && isSkip == true){
                canMove[i+x][j] = true;
                break;
            }
        }
        isSkip = false;
        for(int x = 1;x <=8;++x){ //橫向左
            if(i-x >=0 && situation[i-x][j] == 0 && isSkip == false) canMove[i-x][j] = true;
            else if(i-x >=0 && situation[i-x][j] != 0 && isSkip == false){
                isSkip = true;
            }
            else if(i-x >=0 && situation[i-x][j] > 0 && isSkip == true){
                canMove[i-x][j] = true;
                break;
            }
        }
        isSkip = false;
        for(int y = 1;y <=9;++y){ //直向上
            if(j+y <=9 && situation[i][j+y] == 0 && isSkip == false) canMove[i][j+y] = true;
            else if(j+y <=9 && situation[i][j+y] != 0 && isSkip == false){
                isSkip = true;
            }
            else if(j+y <= 9 && situation[i][j+y] > 0 && isSkip == true){
                canMove[i][j+y] = true;
                break;
            }
        }
        isSkip = false;
        for(int y = 1;y <=9;++y){ //直向下
            if(j-y >=0 && situation[i][j-y] == 0 && isSkip == false) canMove[i][j-y] = true;
            else if(j-y >=0 && situation[i][j-y] != 0 && isSkip == false){
                isSkip = true;
            }
            else if(j-y>=0 && situation[i][j-y] > 0 && isSkip == true){
                canMove[i][j-y] = true;
                break;
            }
        }
        break;
    case -7: //紅兵
        if(j > 4){
            if(situation[i][j-1] >= 0) canMove[i][j-1] = true;
        }
        else{
            if(i-1>=0 && situation[i-1][j] >= 0) canMove[i-1][j] = true;
            if(i+1<=8 && situation[i+1][j] >= 0) canMove[i+1][j] = true;
            if(j-1>=0 && situation[i][j-1] >= 0) canMove[i][j-1] = true;
        }
        break;
    }
}

void MainWindow::moveOrEat(int i,int j,int value,int eaten_i,int eaten_j,int eaten_value){
    ++step;
    if(eaten_value == 0){
        situation[i][j] = 0;
        situation[eaten_i][eaten_j] = value;
    }
    else{
        situation[i][j] = 0;
        situation[eaten_i][eaten_j] = value;
    }
    if(!waitFromSocket && hostIsServer){
        char buf[90];
        for (int i = 0;i < 9;++i) {
            for(int j = 0;j < 10;++j){
                buf[10*i+j] = static_cast<char>(situation[i][j]);
            }
        }
        clientConnection->write(buf,sizeof(buf));
        waitFromSocket = true;
    }
    else if(!waitFromSocket && hostIsClient){
        char buf[90];
        for (int i = 0;i < 9;++i) {
            for(int j = 0;j < 10;++j){
                buf[10*i+j] = static_cast<char>(situation[i][j]);
            }
        }
        client->write(buf,sizeof(buf));
        waitFromSocket = true;
    }
    iniCanMove();
    winOrNot();
}

void MainWindow::winOrNot()
{
    int blackBossPlace[2] = {0,0};
    int redBossPlace[2] = {0,0};
    while(1){
        gameOver = true;
        for(int i = 3;i <= 5;++i){ //判斷將是否死了
            for(int j = 0;j <= 2;++j){
                if(situation[i][j] == 1){
                    gameOver = false;
                    blackBossPlace[0] = i;
                    blackBossPlace[1] = j;
                }
            }
        }

        if(gameOver == true) break;

        gameOver = true;
        for(int i = 3;i <= 5;++i){ //判斷帥是否死了
            for(int j = 7;j <= 9;++j){
                if(situation[i][j] == -1){
                    gameOver = false;
                    redBossPlace[0] = i;
                    redBossPlace[1] = j;
                }
            }
        }

        if(gameOver == true) break;

        if(blackBossPlace[0] == redBossPlace[0]){ //判斷王是否見王
            gameOver = true;
            for(int j = blackBossPlace[1] + 1;j < redBossPlace[1];++j){
                if(situation[blackBossPlace[0]][j] != 0){
                    gameOver = false;
                    break;
                }
            }
            if(gameOver == true){
                ++step;
            }
        }
        break;
    }
    if(gameOver == true){
        scene->removeItem(text);
        text = scene->addText("遊戲結束!",font);
        QString winner = (step % 2 == 0? "紅棋":"黑棋");
        if(!hostIsServer && !hostIsClient){
            QMessageBox::information(this,"對戰結果",winner + "獲勝!",QMessageBox::Yes);
        }
        else{
            if(hostIsServer){
                if(winner == "紅棋"){
                    QMessageBox::information(this,"對戰結果","戰敗!再接再厲!",QMessageBox::Yes);
                }
                else if(winner == "黑棋"){
                    QMessageBox::information(this,"對戰結果","恭喜您獲勝!",QMessageBox::Yes);
                }
            }
            else{
                if(winner == "紅棋"){
                    QMessageBox::information(this,"對戰結果","恭喜您獲勝!",QMessageBox::Yes);
                }
                else if(winner == "黑棋"){
                    QMessageBox::information(this,"對戰結果","戰敗!再接再厲!",QMessageBox::Yes);
                }
            }
        }
    }
}



bool MainWindow::checkCanSet(int i,int j,int value){
    if(situation[i][j]){
        return false;
    }
    switch(value){
    case 1:
        if(i >= 3 && i <= 5 && j >= 0 && j <= 2){
            for (int y = j+1;y <= 9;++y) {
                if(situation[i][y] != 0 && situation[i][y] != -1) return true;
                else if(situation[i][y] == -1) return false;
            }
            return true;
        }
        break;
    case 2:
        if(i >= 3 && i <= 5 && j >= 0 && j <= 2) return true;
        break;
    case 3:
        if(j <= 4) return true;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        return true;
    case -1:
        if(i >= 3 && i <= 5 && j >= 7 && j <= 9){
            for (int y = j-1;y >= 0;--y) {
                if(situation[i][y] != 0 && situation[i][y] != 1) return true;
                else if(situation[i][y] == 1) return false;
            }
            return true;
        }
        break;
    case -2:
        if(i >= 3 && i <= 5 && j >= 7 && j <= 9) return true;
        break;
    case -3:
        if(j >= 5) return true;
        break;
    case -4:
    case -5:
    case -6:
    case -7:
        return true;
    }
    return false;
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    if(event->pos().x() >= 0 && event->pos().x() <= 700
            && event->pos().y() >= 0 && event->pos().y() <=800
                && gameOver == false && waitFromSocket == false){
        if(!editorMode){
            for(int i = 0;i < 9;++i){
                qreal centerX,centerY;
                centerX = 64.28 + 72.18*i;
                for(int j = 0;j < 10;++j){
                    if(j < 5){
                        centerY = 47.3 + 78.23*j;
                    }
                    else{
                        centerY = 47.3 + 79.56 + 78.23*(j - 1);
                    }
                    if(event->pos().x() - centerX <= 28 && event->pos().x() - centerX >= -28
                            && event->pos().y() - centerY <= 28 && event->pos().y() - centerY >= -28){
                        if(canMove[i][j]){ //準備要吃
                            moveOrEat(choseX,choseY,situation[choseX][choseY],i,j,situation[i][j]);
                        }
                        else if(situation[i][j] == 0){ //按到不能走的地方
                            iniCanMove();
                            return;
                        }
                        else if(step % 2 == 0 && situation[i][j] > 0){ //黑棋回合且選到黑棋
                            iniCanMove();
                            moveRule(i,j,situation[i][j]);
                        }
                        else if(step % 2 == 1 && situation[i][j] < 0){ //紅棋回合且選到紅棋
                            iniCanMove();
                            moveRule(i,j,situation[i][j]);
                        }
                        else{
                            iniCanMove();
                            return;
                        }
                        choseX = i;
                        choseY = j;
                        repaint();
                    }
                }
            }
        }
        else if(editorMode){
            if (dialog->chessChose) { //put chess
                for(int i = 0;i < 9;++i){
                    qreal centerX,centerY;
                    centerX = 64.28 + 72.18*i;
                    for(int j = 0;j < 10;++j){
                        if(j < 5){
                            centerY = 47.3 + 78.23*j;
                        }
                        else{
                            centerY = 47.3 + 79.56 + 78.23*(j - 1);
                        }
                        if(event->pos().x() - centerX <= 28 && event->pos().x() - centerX >= -28
                                && event->pos().y() - centerY <= 28 && event->pos().y() - centerY >= -28){
                            if(checkCanSet(i,j,dialog->chessChose)){
                                ++chessInBoard;
                                situation[i][j] = dialog->chessChose;

                                if(dialog->chessChose > 0) dialog->blackChessLeft[dialog->chessChose - 1]--;
                                else if(dialog->chessChose < 0) dialog->redChessLeft[-(dialog->chessChose) -1]--;

                                repaint();
                                dialog->repaint();
                                dialog->chessChose = 0;
                            }
                            else{
                                dialog->chessChose = 0;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}
void MainWindow::paintEvent(QPaintEvent *){        
    QPainter painter(this);
    painter.setFont(font);

    if(!editorMode){
        if(!hostIsServer && !hostIsClient){
            if(step % 2 == 0){ //黑方進攻
                scene->removeItem(text);
                text = scene->addText("黑方進攻",font);
                text->setPos(790,40);
                text->setDefaultTextColor(Qt::black);
            }
            else if(step % 2 == 1 && step > 0){ //紅方進攻
                scene->removeItem(text);
                text = scene->addText("紅方進攻",font);
                text->setPos(790,40);
                text->setDefaultTextColor(Qt::red);
            }
        }
        else{
            if(hostIsServer){
                if(step % 2 == 0){ //黑方進攻
                    scene->removeItem(text);
                    text = scene->addText("我方(黑)進攻",font);
                    text->setPos(740,40);
                    text->setDefaultTextColor(Qt::black);
                }
                else if(step % 2 == 1 && step > 0){ //紅方進攻
                    scene->removeItem(text);
                    text = scene->addText("等待對方(紅)進攻",font);
                    text->setPos(740,40);
                    text->setDefaultTextColor(Qt::red);
                }
            }

            else{
                if(step % 2 == 0){ //黑方進攻
                    scene->removeItem(text);
                    text = scene->addText("等待對方(黑)進攻",font);
                    text->setPos(740,40);
                    text->setDefaultTextColor(Qt::black);
                }
                else if(step % 2 == 1 && step > 0){ //紅方進攻
                    scene->removeItem(text);
                    text = scene->addText("我方(紅)進攻",font);
                    text->setPos(740,40);
                    text->setDefaultTextColor(Qt::red);
                }
            }
        }
    }

    for(int i = 0;i < 9;++i){
        qreal x,y;
        x = 64.28 + 72.18*i - RADIUS;
        for(int j = 0;j < 10;++j){
            if(j < 5)
                y = 47.3 + 78.23*j - RADIUS;

            else
                y = 47.3 + 79.56 + 78.23*(j - 1) - RADIUS;

            switch(situation[i][j]){
            case 1 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/b1.png")); break;
            case 2 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/b2.png")); break;
            case 3 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/b3.png")); break;
            case 4 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/b4.png")); break;
            case 5 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/b5.png")); break;
            case 6 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/b6.png")); break;
            case 7 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/b7.png")); break;
            case -1 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/r1.png")); break;
            case -2 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/r2.png")); break;
            case -3 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/r3.png")); break;
            case -4 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/r4.png")); break;
            case -5 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/r5.png")); break;
            case -6 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/r6.png")); break;
            case -7 : painter.drawPixmap(QPointF(x,y),QPixmap(":/img/image/r7.png")); break;
            }
            if(canMove[i][j]){
                painter.setPen(QPen(Qt::white,7));
                painter.drawPoint(QPointF(x + RADIUS,y + RADIUS));
            }

        }
    }
}

void MainWindow::onePersonGameSlots()
{
    if(exitFromSingle){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開始新的單人遊戲?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        exitFromSingle = 0;
    }
    else if(exitFromServer){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開始新的單人遊戲?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        hostIsServer = 0;
        exitFromServer = 0;

        delete wait;
        delete server;
    }
    else if(exitFromClient){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開始新的單人遊戲?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        hostIsClient = 0;
        exitFromClient = 0;

        delete client;
    }
    else if(exitFromEditor){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開啟新的單人遊戲?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        if(editorMode){
            finishEdit->close();
            dialog->close();
        }
        editorMode = 0;
        exitFromEditor = 0;
    }


    choseX = 0;
    choseY = 0;
    gameOver = false;
    step = 0;
    waitFromSocket = false;
    exitFromSingle = 1;

    iniChess();
    repaint();
}

void MainWindow::multiPeopleGameSlots()
{
    if(exitFromSingle){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開始新的連線遊戲?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        exitFromSingle = 0;
    }
    else if(exitFromServer){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開始新的連線遊戲?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        hostIsServer = 0;
        exitFromServer = 0;

        delete wait;
        delete server;
    }
    else if(exitFromClient){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開始新的連線遊戲?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        hostIsClient = 0;
        exitFromClient = 0;

        delete client;
    }
    else if(exitFromEditor){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開啟新的連線遊戲?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        if(editorMode){
            finishEdit->close();
            dialog->close();
        }
        editorMode = 0;
        exitFromEditor = 0;
    }


    QPushButton *server = new QPushButton;
    server->setText("我想當房主");
    QPushButton *client = new QPushButton;
    client->setText("我要連到別人房間");

    QDialog *dialog = new QDialog;
    dialog->setWindowTitle("連線對戰");
    dialog->setMinimumWidth(200);
    dialog->setMinimumHeight(50);

    QFormLayout *layout = new QFormLayout(dialog);
    layout->addWidget(server);
    layout->addWidget(client);

    connect(server,SIGNAL(clicked()),dialog,SLOT(close()));
    connect(server,SIGNAL(clicked()),this,SLOT(tcpServerSlots()));
    connect(client,SIGNAL(clicked()),dialog,SLOT(close()));
    connect(client,SIGNAL(clicked()),this,SLOT(tcpClientSlots()));

    dialog->exec();
}


void MainWindow::exitGameSlots()
{
    QMessageBox verify(QMessageBox::NoIcon,"確認視窗","真的要退出遊戲嗎?",QMessageBox::Yes | QMessageBox::No,this);
    if(verify.exec() == QMessageBox::Yes){
        exit(0);
    }
}

void MainWindow::tcpServerSlots()
{
    QDialog *dialog = new QDialog;
    QFormLayout *layout = new QFormLayout(dialog);

    QLabel *addrLabel = new QLabel("IP address:");
    QLabel *portLabel = new QLabel("Port number:");
    QLineEdit *addrEdit = new QLineEdit;
    QLineEdit *portEdit = new QLineEdit;
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout->addRow(addrLabel,addrEdit);
    layout->addRow(portLabel,portEdit);
    layout->addRow(buttonBox);

    dialog->setLayout(layout);
    dialog->setWindowTitle("您是Server");

    connect(buttonBox,SIGNAL(accepted()),dialog,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),dialog,SLOT(reject()));
    if(dialog->exec() == QDialog::Accepted){
        QString address = addrEdit->text();
        int port = portEdit->text().toInt();

        server = new QTcpServer;
        server->listen(static_cast<QHostAddress>(address),static_cast<uint16_t>(port));

        wait = new QLabel(this);
        wait->setText("等待玩家連線...");
        wait->setGeometry(350,325,300,150);
        wait->setAlignment(Qt::AlignCenter);
        wait->setStyleSheet("background-color:#FFA07A;"
                            "font-size:30px;"
                            "font-family:'Segoe UI';");
        exitFromServer = 1;
        connect(server,SIGNAL(newConnection()),wait,SLOT(close()));
        connect(server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

        wait->show();
    }
    else{
        return;
    }
}

void MainWindow::acceptConnection()
{
    clientConnection = server->nextPendingConnection();

    choseX = 0;
    choseY = 0;
    gameOver = false;
    step = 0;
    waitFromSocket = false;
    hostIsServer = true;

    iniChess();
    repaint();

    connect(clientConnection,SIGNAL(readyRead()),this,SLOT(readFromSocket()));
}

void MainWindow::tcpClientSlots()
{
    QDialog *dialog = new QDialog;
    QFormLayout *layout = new QFormLayout(dialog);

    QLabel *addrLabel = new QLabel("server's IP address:");
    QLabel *portLabel = new QLabel("server's Port number:");
    QLineEdit *addrEdit = new QLineEdit;
    QLineEdit *portEdit = new QLineEdit;
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    layout->addRow(addrLabel,addrEdit);
    layout->addRow(portLabel,portEdit);
    layout->addRow(buttonBox);

    dialog->setLayout(layout);
    dialog->setWindowTitle("您是Client");

    connect(buttonBox,SIGNAL(accepted()),dialog,SLOT(accept()));
    connect(buttonBox,SIGNAL(rejected()),dialog,SLOT(reject()));
    while(1){
        if(dialog->exec() == QDialog::Accepted){
            QString serverAddr = addrEdit->text();
            int serverPort = portEdit->text().toInt();

            client = new QTcpSocket;
            client->connectToHost(static_cast<QHostAddress>(serverAddr),static_cast<uint16_t>(serverPort));
            exitFromClient = 1;
            if(client->waitForConnected(3000)){
                connectedToServer();
                break;
            }
            else{
                if(QMessageBox::warning(this,"消息框","沒有連線到您指定的server,是否要重新輸入?",QMessageBox::Yes | QMessageBox::No) ==
                        QMessageBox::Yes){
                    continue;
                }
                else{
                    return;
                }
            }
        }
        else{
            return;
        }
    }
}
void MainWindow::connectedToServer()
{
    choseX = 0;
    choseY = 0;
    gameOver = false;
    step = 0;
    waitFromSocket = true;
    hostIsClient = true;

    iniChess();
    repaint();

    connect(client,SIGNAL(readyRead()),this,SLOT(readFromSocket()));
}

void MainWindow::readFromSocket()
{
    if(hostIsServer){
        char buf[90];
        clientConnection->read(buf,90);
        for (int i = 0;i < 9;++i) {
            for (int j = 0;j < 10;++j) {
                situation[i][j] = static_cast<int>(buf[10 * i + j]);
            }
        }
        ++step;
        waitFromSocket = false;
    }
    else if(hostIsClient){
        char buf[90];
        client->read(buf,90);
        for (int i = 0;i < 9;++i) {
            for (int j = 0;j < 10;++j) {
                situation[i][j] = static_cast<int>(buf[10 * i + j]);
            }
        }
        ++step;
        waitFromSocket = false;
    }
    repaint();
    winOrNot();
}
void MainWindow::editorGameSlots()
{
    if(exitFromSingle){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開啟編輯模式?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        exitFromSingle = 0;
    }
    else if(exitFromServer){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開啟編輯模式?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        hostIsServer = 0;
        exitFromServer = 0;

        delete wait;
        delete server;
    }
    else if(exitFromClient){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開啟編輯模式?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        hostIsClient = 0;
        exitFromClient = 0;

        delete client;
    }
    else if(exitFromEditor){
        if(QMessageBox::information(this,"確認視窗","確定退出當前遊戲,並開啟編輯模式?",QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
            return;
        }
        if(editorMode){
            finishEdit->close();
            dialog->close();
        }
        editorMode = 0;
        exitFromEditor = 0;
    }

    gameOver = false;
    waitFromSocket = false;
    editorMode = 1;
    exitFromEditor = 1;


    memset(situation,0,90 * sizeof(int));
    this->repaint();
    dialog = new EditorDialog;
    dialog->setGeometry(250,250,500,200);
    dialog->show();
    dialog->repaint();

    finishEdit = new QPushButton(this);
    finishEdit->setGeometry(820,600,100,50);
    finishEdit->setText("設置完畢");
    finishEdit->show();

    connect(finishEdit,SIGNAL(clicked()),dialog,SLOT(close()));
    connect(finishEdit,SIGNAL(clicked()),finishEdit,SLOT(close()));
    connect(finishEdit,SIGNAL(clicked()),this,SLOT(editorGameStartSlots()));
}
void MainWindow::editorGameStartSlots()
{
    if(!chessInBoard){
        if(QMessageBox::information(this,"提醒視窗","您沒放棋子啊!",QMessageBox::Yes) == QMessageBox::Yes){
            dialog->show();
            finishEdit->show();
            return;
        }
    }
    else if((dialog->blackChessLeft[0] == 1) || (dialog->redChessLeft[0] == 1)){
        if(QMessageBox::information(this,"提醒視窗","您有王沒放!",QMessageBox::Yes) == QMessageBox::Yes){
            dialog->show();
            finishEdit->show();
            return;
        }
    }

    choseX = 0;
    choseY = 0;
    step = 0;
    editorMode = 0;
    iniCanMove();
    repaint();
}
