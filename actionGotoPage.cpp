#include"actionGotoPage.h"

void actionGotoWindow::showTable(){
    QString tmp = QString::fromStdString( readFileToString("./LR1_table.txt"));
    actionGotoTable->setText(tmp);
}

actionGotoWindow::actionGotoWindow(){
    // 设置新窗口的属性，比如大小、标题等
    setWindowTitle("ActionGoto Table");
    resize(WIDTH, HEIGHT);

    //读取文件

//    actionGotoTable->setText("test");
    actionGotoTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    actionGotoTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    actionGotoTable->setWordWrapMode(QTextOption::NoWrap);
    actionGotoTable->setReadOnly(1);
    //添加控件
//    auto *mainLayout = new QHBoxLayout(this);
    setCentralWidget(actionGotoTable);
    showTable();
}
