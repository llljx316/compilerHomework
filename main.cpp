#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>

#include "mainpage.h"



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LR1ParserWindow window;
    window.show();

    return app.exec();
}
