#include"QuadExpression.h"

void QuadExpressionWindow::showTable(){
    QString tmp = QString::fromStdString( readFileToString("./LR1_table.txt"));
    QuadExpressionTable->setText(tmp);
}

QuadExpressionWindow::QuadExpressionWindow(){
    // 设置新窗口的属性，比如大小、标题等
    setWindowTitle("ActionGoto Table");
    resize(WIDTH, HEIGHT);

    //读取文件

//    QuadExpressionTable->setText("test");
    QuadExpressionTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QuadExpressionTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QuadExpressionTable->setWordWrapMode(QTextOption::NoWrap);
    QuadExpressionTable->setReadOnly(1);
    //添加控件
//    auto *mainLayout = new QHBoxLayout(this);
    setCentralWidget(QuadExpressionTable);
    showTable();
}
