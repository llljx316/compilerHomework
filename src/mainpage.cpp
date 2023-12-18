#include <QApplication>
#include <fstream>

#include "lexicalanalyzer.h"
#include "mainpage.h"
#include "util.h"
#include "QuadExpression.h"


//string getOperation(char op){
//    switch(op){
//    case '@' :
//        return "入栈";
//        break;
//    case 'p':
//        return "归约";
//        break;
//    case 'b':
//        return "出栈";
//        break;
//    case '#':
//        return "终止";
//        break;
//    default:
//        throw ERROR;
//        break;
//    }
//}

//std::string getParseResult(){
//    std::fstream stack_file;
//    std::string tmp;
//    std::vector<int> id_stack;
//    std::vector<string> content_stack;
//    std::ostringstream output;
//    std::string first_token;
//    int line=0;
//    stack_file.open("stack.tmp",std::ios::in);
//    while(stack_file.eof()==false)
//    {

//        getline(stack_file,tmp);
//        if(tmp.size()==0)
//            break;
//        line++;
//        std::stringstream sstr(tmp);
//        char sign;
//        sstr>>sign;


//        if(sign=='p')
//        {
//            int i;
//            std::string str;

//            sstr>>i;
//            sstr>>str;
//            if(str.size()==0)
//                std::cout<<"@@!!!"<<std::endl;
//            id_stack.emplace_back(i);
//            content_stack.emplace_back(str);

//        }
//        else if(sign=='b')
//        {
//            id_stack.pop_back();
//            content_stack.pop_back();
//        }
//        else if(sign=='@')
//        {
//            sstr>>first_token;
//            // std::cout<<first_token<<"\n";
//        }

//        stringstream tmps;
////        QElapsedTimer time;
////        time.start();
////        while(time.elapsed() < 200)             //等待时间流逝5秒钟
////            QCoreApplication::processEvents();   //处理事件
////        painting_borad->update();




//        output  <<std::right <<setw(4)<<line << "# " << std::left << "操作：" << getOperation(sign) << " "<< "当前第一字符:" << setw(11) << first_token << "状态栈：" ;
//        for(int &x: id_stack){
//            tmps<< x << ' ';
//        }
//        output   << setw(75) << tmps.str();
//        tmps.clear();
//        tmps.str("");
//        output <<"符号栈：";
//        for(std::string &x: content_stack){
//            tmps << x << ' ';
//        }
//        output << tmps.str();
//        tmps.clear();
//        tmps.str("");
//        output << endl;

//    }

//    return output.str();

//}



//创建信号与槽
void LR1ParserWindow::createConnections(){
    // 信号与槽
    connect(loadFileButton, &QPushButton::clicked, this, &LR1ParserWindow::loadFile);
    //        connect(startButton, &QPushButton::clicked, this, &)
    connect(lexicalAnalysisButton, &QPushButton::clicked, this, &LR1ParserWindow::lexicalAnalyse);
    connect(syntaxAnalysisButton, &QPushButton::clicked, this, &LR1ParserWindow::parseAnalyse);
    connect(QuadExpressionButton,&QPushButton::clicked, this, &LR1ParserWindow::showActionGoto);

    connect(resetButton, &QPushButton::clicked, this, &LR1ParserWindow::reset);
}
LR1ParserWindow::LR1ParserWindow(QWidget *parent) : QWidget(parent) {
    // 布局
    auto *mainLayout = new QHBoxLayout(this);
    auto *leftLayout = new QVBoxLayout();
    auto *lexicalOutputLayout = new QVBoxLayout();
    auto *parseOutputLayout = new QVBoxLayout();

    auto *secondLayout = new QHBoxLayout();
    auto *thirdLayout = new QHBoxLayout();
    auto *fileLayout = new QVBoxLayout();
//        auto *QuadExpressionLayout = new QVBoxLayout();

    this->setMinimumSize(WIDTH,HEIGHT);

    // 创建输出框和文件路径标签
    introduction->setText("这是一个词法和语法分析器，同时还有一些奇奇怪怪的功能");
    fileBox->setText("文件路径：当前未选择文件");

//        fileBox->setReadOnly(true);

    // 创建输出的标签以及内容
    lexicalOutputContent->setReadOnly(1);
    parseOutputContent->setReadOnly(1);

    // 添加组件到布局
    fileLayout->addWidget(introduction);
    fileLayout->addWidget(fileBox);
    fileLayout->addWidget(loadFileButton);

    secondLayout->addWidget(lexicalAnalysisButton);
    secondLayout->addWidget(syntaxAnalysisButton);
//        secondLayout->addWidget(viewSyntaxButton);
    secondLayout->addWidget(resetButton);
    thirdLayout->addWidget(QuadExpressionButton);

    lexicalOutputLayout->addWidget(lexicalOutputArea);
    lexicalOutputLayout->addWidget(lexicalOutputContent);

    parseOutputLayout->addWidget(parseOutputArea);
    parseOutputLayout->addWidget(parseOutputContent);
    //增加滑动条关闭自动换行
    parseOutputContent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    parseOutputContent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    parseOutputContent->setWordWrapMode(QTextOption::NoWrap);

    leftLayout->addLayout(fileLayout);
    leftLayout->addLayout(secondLayout);
    leftLayout->addLayout(thirdLayout);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(lexicalOutputLayout);
    mainLayout->addLayout(parseOutputLayout);

    QFont mfont("Consolas", 10);
    parseOutputContent->setFont(mfont);


    disableButton(syntaxAnalysisButton);
    disableButton(QuadExpressionButton);

    createConnections();



}

//处理文件的函数
void LR1ParserWindow::loadFile() {
    fileName = QFileDialog::getOpenFileName(this, "打开文件", "");
    if (!fileName.isEmpty()) {
        // 加载并处理文件
        //更改文件名称
        this->fileBox->setText(fileName);
        this->fileContent = readFileToString(fileName.toStdString());
        isFileLoad=1;
    }
    //预处理
//    inn = scanner.Preprocessing(inn, inn.size());
    Preprocess(fileName.toStdString());
}

//出错处理的问题
void LR1ParserWindow::lexicalAnalyse(){

    Scanner scanner;
    string fullOutput;

    if(!isFileLoad){
        fullOutput = string("需要读取文件！！！\n");
        lexicalOutputContent->setTextColor(QColor::fromString("red"));
        outputQ = QString::fromStdString(fullOutput);
        lexicalOutputContent->setText(outputQ);
        return;
    }

    auto tokens = scanner.getTokens("test_preprocessing.txt");//可能出问题

    //输出文件重定向
    ofstream fout;
    fout.open("Lexical_Results.txt", ios::out);
    if (fout.is_open() == 0)
    {
        fullOutput = string("输出文件Lexical_Results.txt打开失败\n");
        lexicalOutputContent->setTextColor(QColor::fromString("red"));
        outputQ = QString::fromStdString(fullOutput);
        lexicalOutputContent->setText(outputQ);
        return;
    }
    //cout << "Lexical results:" << endl;
    for (auto i : tokens)
        fout << i.get_name() <<" " << i.get_type_output() << " " << i.get_line() << endl;
    fout.close();

    QFile file( "Lexical_Results.txt" );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        fullOutput = string("输出文件Lexical_Results.txt打开失败\n");
        lexicalOutputContent->setTextColor(QColor::fromString("red"));
        outputQ = QString::fromStdString(fullOutput);
        lexicalOutputContent->setText(outputQ);
        return;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    lexicalOutputContent->setTextColor(QColor::fromString("black"));
    lexicalOutputContent->setText(line);
    enableButton(syntaxAnalysisButton);
//    ui->textEdit->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
    return;




}

//std::string getParseResult(){
//    std::fstream stack_file;
//    std::string tmp;
//    std::vector<int> id_stack;
//    std::vector<string> content_stack;
//    std::ostringstream output;
//    std::string first_token;
//    int line=0;
//    stack_file.open("stack.txt",std::ios::in);
//    while(stack_file.eof()==false)
//    {

//        getline(stack_file,tmp);
//        if(tmp.size()==0)
//            break;
//        line++;
//        std::stringstream sstr(tmp);
//        char sign;
//        sstr>>sign;


//        if(sign=='p')
//        {
//            int i;
//            std::string str;

//            sstr>>i;
//            sstr>>str;
//            if(str.size()==0)
//                std::cout<<"@@!!!"<<std::endl;
//            id_stack.emplace_back(i);
//            content_stack.emplace_back(str);

//        }
//        else if(sign=='b')
//        {
//            id_stack.pop_back();
//            content_stack.pop_back();
//        }
//        else if(sign=='@')
//        {
//            sstr>>first_token;
//            // std::cout<<first_token<<"\n";
//        }

//        stringstream tmps;
//        //        QElapsedTimer time;
//        //        time.start();
//        //        while(time.elapsed() < 200)             //等待时间流逝5秒钟
//        //            QCoreApplication::processEvents();   //处理事件
//        //        painting_borad->update();




//        output  <<std::right <<setw(4)<<line << "# " << std::left << "操作：" << getOperation(sign) << " "<< "当前第一字符:" << setw(11) << first_token << "状态栈：" ;
//            for(int &x: id_stack){
//            tmps<< x << ' ';
//        }
//        output   << setw(75) << tmps.str();
//        tmps.clear();
//        tmps.str("");
//        output <<"符号栈：";
//        for(std::string &x: content_stack){
//            tmps << x << ' ';
//        }
//        output << tmps.str();
//        tmps.clear();
//        tmps.str("");
//        output << endl;

//    }

//    return output.str();

//}

//#define old
int LR1ParserWindow::parseAnalyse(){
    Scanner scanner;
    auto tokens = scanner.getTokens("test_preprocessing.txt");
    MidCodeParser midcodeparser;
    midcodeparser.open_file("grammar.txt");
    midcodeparser.build();
    int tag = midcodeparser.analyse(tokens);

    if(tag==0){
        parseOutputContent->setTextColor("black");
//        parseOutput();
        enableButton(QuadExpressionButton);
    }
    else{
        parseOutputContent->setTextColor("red");
//        QString error = (QString::fromStdString(message)+" ("+QString::number(line)+","+QString::number(col)+")\n");;
//        parseOutputContent->setText(error);
    }

    QFile file( "stack.txt" );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"no read";
        return tag;
    }
    qDebug()<<"Yes read";
    QTextStream in(&file);
    QString line = in.readAll();//读取所有
    parseOutputContent->setText(line);
    //设置文本框只读
    //ui->textEdit->setReadOnly(true);
    file.close();
    return tag;
}

//void LR1ParserWindow::parseOutput(){
//    QString out = QString::fromStdString(getParseResult());
//    parseOutputContent->setText(out);
//}

void LR1ParserWindow::showActionGoto(){
    auto* newTable = new QuadExpressionWindow();
    newTable->show();
}


//重置函数
void LR1ParserWindow::reset(){
    fileBox->setText("文件路径：当前未选择文件");
    fileContent = "";
    outputQ.clear();
    lexicalOutputContent->setTextColor(QColor::fromString("black"));
    lexicalOutputContent->clear();
    parseOutputContent->clear();
    disableButton(syntaxAnalysisButton);
    disableButton(QuadExpressionButton);
    isFileLoad = 0;
}

void LR1ParserWindow::enableButton(QPushButton* button){
    button->setEnabled(1);
}

void LR1ParserWindow::disableButton(QPushButton* button){
    button->setEnabled(0);
}


