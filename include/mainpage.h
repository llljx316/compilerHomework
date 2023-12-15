#ifndef MAINPAGE_H
#define MAINPAGE_H

#endif // MAINPAGE_H
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>

//调用的接口
#include "LexicalAnalyzer.h"
#include "parser.h"
#include "mid_code_gen.h"
#include "util.h"
#include "actionGotoPage.h"
#include <string>
#include <iostream>
#include <cassert>
#include <sstream>
#include <iomanip>


class LR1ParserWindow : public QWidget {
private:
    QLabel *filePathLabel;
    // 创建按钮
    QPushButton *lexicalAnalysisButton = new QPushButton("词法分析", this);
    QPushButton *syntaxAnalysisButton = new QPushButton("语法分析", this);
    QPushButton *resetButton = new QPushButton("重置", this);
    QPushButton *actionGotoButton = new QPushButton("Action/Goto表", this);

    // 创建文件路径标签
    QLabel *introduction = new QLabel(this);
    QLabel *fileBox = new QLabel(this);
    QPushButton *loadFileButton = new QPushButton("加载文件");

    //输出区域
    QLabel *lexicalOutputArea = new QLabel("词法分析输出区");
    QTextEdit *lexicalOutputContent = new QTextEdit(this);

    QLabel *parseOutputArea = new QLabel("语法分析输出区");
    QTextEdit *parseOutputContent = new QTextEdit(this);

    string fileContent;
    LexicalAnalyzer lexer;
    Parser parser;
    QString outputQ;

    bool isFileLoad = 0;

    //创建信号与槽
    void createConnections();
public:
    LR1ParserWindow(QWidget *parent = nullptr);
private slots:
    //处理文件的函数
    void loadFile();

    void lexicalAnalyse();

    int parseAnalyse();

    void parseOutput();

    void showActionGoto();

    void enableButton(QPushButton* button);

    void disableButton(QPushButton* button);

    //重置函数
    void reset();


};
