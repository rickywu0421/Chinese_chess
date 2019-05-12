#include "editordialog.h"

EditorDialog::EditorDialog()
{

}
void EditorDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this); //Dialog 長:500,寬:300
    for (int i = 0;i < 7;++i) { //Black
        if(blackChessLeft[i]){
            qreal x = i*68;
            qreal y = 10;

            QString pixmapSorce = ":/img/image/b" + QString::number(i+1) + ".png";
            painter.drawPixmap(QPointF(x,y),QPixmap(pixmapSorce));
        }
    }
    for (int i = 0;i < 7;++i) { //Red
        if(redChessLeft[i]){
            qreal x = i*68;
            qreal y = 100;
            QString pixmapSorce = ":/img/image/r" + QString::number(i+1) + ".png";
            painter.drawPixmap(QPointF(x,y),QPixmap(pixmapSorce));
        }
    }
}

void EditorDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->pos().x() >= 0 && event->pos().x() <= 500 && event->pos().y() >= 0 && event->pos().y() <= 300){
        qreal centerX;
        qreal centerY;
        if(event->pos().y() >= 10 && event->pos().y() <= 10 + 2*radius){ //black
            for (int i = 0;i < 7;++i) {
                centerX = i*68 + radius;
                centerY = 10 + radius;
                if(event->pos().x() - centerX >= -28 && event->pos().x() - centerX <= 28){
                    if(blackChessLeft[i] > 0){
                        chessChose = i + 1;
                    }
                }
            }
        }
        else if(event->pos().y() >= 100 && event->pos().y() <= 100 + 2*radius){ //red
            for (int i = 0;i < 7;++i) {
                centerX = i*68 + radius;
                centerY = 100 + radius;
                if(event->pos().x() - centerX >= -28 && event->pos().x() - centerX <= 28){
                    if(redChessLeft[i] > 0){
                        chessChose = - i - 1;
                    }
                }
            }
        }
    }
}
