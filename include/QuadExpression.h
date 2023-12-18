#ifndef QUADEXPRESSION_H
#define QUADEXPRESSION_H

#endif // QUADEXPRESSION_H
// 假设这是你的新窗口类
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include "util.h"
#include <QMainWindow>
#include <QPushButton>

class QuadExpressionWindow : public QMainWindow {
private:
    QTextEdit* QuadExpressionTable = new QTextEdit();
    void showTable();
public:
    QuadExpressionWindow();
};
