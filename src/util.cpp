#include"util.h"
#include "LexicalAnalyzer.h"
#include "scanner.h"
#include <sstream>
#include <fstream>


//读取文件为字符串
std::string readFileToString(const std::string& filePath) {
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        throw std::runtime_error("无法打开文件: " + filePath);
    }

    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    fileStream.close();
    return buffer.str();
}
\
void Preprocess(const std::string& filename)
{
    //输出文件重定向
    ofstream fout;
    fout.open("test_preprocessing.txt", ios::out);
    if (fout.is_open() == 0)
    {
        cout << "output open failed" << endl;
        exit(-1);
    }

    //输入文件重定向
    ifstream infile;
    infile.open(filename, ios::in);
    if (infile.is_open() == 0)
    {
        cout << "input open failed" << endl;
    }

    std::string line;

    std::string inn;
    while (getline(infile, line))
    {
        inn += line + '\n';
    }

    cout << "source program:" << endl << inn << endl;
    Scanner scanner;
    inn = scanner.Preprocessing(inn, inn.size());

    fout << inn;

    cout << "after program preprocessing:" << endl << inn << endl;

//    QString qstrinn = QString::fromStdString(inn);

//    ui->textEdit->setText(qstrinn);

    fout.close();
    infile.close();
}
