#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

class EditorDialog : public QDialog
{
public:
    EditorDialog();
    int blackChessLeft[7] = {1,2,2,2,2,2,5};
    int redChessLeft[7] = {1,2,2,2,2,2,5};
    int radius = 33;
    int chessChose = 0;


protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);

};

#endif // EDITORDIALOG_H
