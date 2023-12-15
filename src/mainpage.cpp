#include <QApplication>
#include "mainpage.h"

string getOperation(char op){
    switch(op){
    case '@' :
        return "入栈";
        break;
    case 'p':
        return "归约";
        break;
    case 'b':
        return "出栈";
        break;
    case '#':
        return "终止";
        break;
    default:
        throw ERROR;
        break;
    }
}

std::string getParseResult(){
    std::fstream stack_file;
    std::string tmp;
    std::vector<int> id_stack;
    std::vector<string> content_stack;
    std::ostringstream output;
    std::string first_token;
    int line=0;
    stack_file.open("stack.tmp",std::ios::in);
    while(stack_file.eof()==false)
    {

        getline(stack_file,tmp);
        if(tmp.size()==0)
            break;
        line++;
        std::stringstream sstr(tmp);
        char sign;
        sstr>>sign;


        if(sign=='p')
        {
            int i;
            std::string str;

            sstr>>i;
            sstr>>str;
            if(str.size()==0)
                std::cout<<"@@!!!"<<std::endl;
            id_stack.emplace_back(i);
            content_stack.emplace_back(str);

        }
        else if(sign=='b')
        {
            id_stack.pop_back();
            content_stack.pop_back();
        }
        else if(sign=='@')
        {
            sstr>>first_token;
            // std::cout<<first_token<<"\n";
        }

        stringstream tmps;
//        QElapsedTimer time;
//        time.start();
//        while(time.elapsed() < 200)             //等待时间流逝5秒钟
//            QCoreApplication::processEvents();   //处理事件
//        painting_borad->update();




        output  <<std::right <<setw(4)<<line << "# " << std::left << "操作：" << getOperation(sign) << " "<< "当前第一字符:" << setw(11) << first_token << "状态栈：" ;
        for(int &x: id_stack){
            tmps<< x << ' ';
        }
        output   << setw(75) << tmps.str();
        tmps.clear();
        tmps.str("");
        output <<"符号栈：";
        for(std::string &x: content_stack){
            tmps << x << ' ';
        }
        output << tmps.str();
        tmps.clear();
        tmps.str("");
        output << endl;

    }

    return output.str();

}



    //创建信号与槽
    void LR1ParserWindow::createConnections(){
        // 信号与槽
        connect(loadFileButton, &QPushButton::clicked, this, &LR1ParserWindow::loadFile);
        //        connect(startButton, &QPushButton::clicked, this, &)
        connect(lexicalAnalysisButton, &QPushButton::clicked, this, &LR1ParserWindow::lexicalAnalyse);
        connect(syntaxAnalysisButton, &QPushButton::clicked, this, &LR1ParserWindow::parseAnalyse);
        connect(actionGotoButton,&QPushButton::clicked, this, &LR1ParserWindow::showActionGoto);

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
//        auto *actionGotoLayout = new QVBoxLayout();

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
        thirdLayout->addWidget(actionGotoButton);

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
        disableButton(actionGotoButton);

        createConnections();



    }

    //处理文件的函数
    void LR1ParserWindow::loadFile() {
        QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "");
        if (!fileName.isEmpty()) {
            // 加载并处理文件
            //更改文件名称
            this->fileBox->setText(fileName);
            this->fileContent = readFileToString(fileName.toStdString());
            isFileLoad=1;
        }
    }

    void LR1ParserWindow::lexicalAnalyse(){

        string output;
        int preline=1,precol=1;
        string fullOutput;

        //如果没有读取文件
        if(!isFileLoad){
            fullOutput = string("需要读取文件！！！\n");
            lexicalOutputContent->setTextColor(QColor::fromString("red"));
        }

        if(lexer.analyze(fileContent,output, preline, precol)==RIGHT_STATUS){
            //重新定义输出
            fullOutput = readFileToString(lexicalOutputName);
            lexicalOutputContent->setTextColor(QColor::fromString("black"));
            enableButton(syntaxAnalysisButton);
        }
        else{
            fullOutput  = readFileToString(lexicalWrongFileName);
            fullOutput = string("分析错误，错误信息为：\n") + fullOutput;
            lexicalOutputContent->setTextColor(QColor::fromString("red"));
        }
        outputQ = QString::fromStdString(fullOutput);
        lexicalOutputContent->setText(outputQ);

    }

    int LR1ParserWindow::parseAnalyse(){
        string output;
#if 1
    #ifdef old
        Parser parser;
        parser.read_grammer(".\\grammer.syn");
        parser.get_all_symbol_first();
        parser.get_item_group_list();

        parser.get_LR1_table();
        parser.print_LR1_table();

        parser.print_DFA();
    #else
        mid_code_gen_parser parser;
        //下面是新的读的函数，但是会出问题
//        parser.read_grammer_Yacc(".\\ANSI_YACC_C99.txt");
        parser.read_grammer(".\\grammer.syn");
        parser.get_all_symbol_first();
        parser.get_item_group_list();

        parser.get_LR1_table();
        parser.print_LR1_table();

        parser.print_DFA();
    #endif
#endif



        //std::cout<<'\n'<<parser.check(".\token_result.txt");

#ifdef old
        int re=parser.check("token_result.txt");
        if(re==SYNTAX_SUCCESS){
            parseOutputContent->setTextColor("black");
            parseOutput();
            enableButton(actionGotoButton);
        }
        else{
            parseOutputContent->setTextColor("red");
            QString error = "分析失败！";
            parseOutputContent->setText(error);
        }
        return re;
#else
//        mid_code_gen_parser parser;
        auto re=parser.check("token_result.txt");
        auto tag=std::get<0>(re);
        auto message=std::get<1>(re);
        auto line=std::get<2>(re);
        auto col=std::get<3>(re);
        if(tag==true){
            parseOutputContent->setTextColor("black");
            parseOutput();
            enableButton(actionGotoButton);
        }
        else{
            parseOutputContent->setTextColor("red");
            QString error = (QString::fromStdString(message)+" ("+QString::number(line)+","+QString::number(col)+")\n");;
            parseOutputContent->setText(error);
        }
        return tag;
#endif

    }

    void LR1ParserWindow::parseOutput(){
        QString out = QString::fromStdString(getParseResult());
        parseOutputContent->setText(out);
    }

    void LR1ParserWindow::showActionGoto(){
        auto* newTable = new actionGotoWindow();
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
        disableButton(actionGotoButton);
        isFileLoad = 0;
    }

    void LR1ParserWindow::enableButton(QPushButton* button){
        button->setEnabled(1);
    }

    void LR1ParserWindow::disableButton(QPushButton* button){
        button->setEnabled(0);
    }


