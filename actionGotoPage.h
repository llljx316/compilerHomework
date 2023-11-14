#ifndef ACTIONGOTOPAGE_H
#define ACTIONGOTOPAGE_H

#endif // ACTIONGOTOPAGE_H
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

class actionGotoWindow : public QMainWindow {
private:
    QTextEdit* actionGotoTable = new QTextEdit();
    void showTable();
public:
    actionGotoWindow();
};
