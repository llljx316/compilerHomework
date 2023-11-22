#include "LexicalAnalyzer.h"
#include <iostream>
#include <cassert>

int main() {
    // �����ʷ�������ʵ��
    LexicalAnalyzer lexer;

    // �����ַ������������ִʷ�Ԫ��
    std::string input_code = R"(
        // This is a single line comment
        /* This is a
        multiline comment */
        int main() {
            int number = 42; 
            float pi = 3.14; 
            if (number == 42) {
                printf("Hello, World!");
            }
            return 0;
        }
    )";

    // ����Ԥ�ڵ���Ϊ�����磬��������Ԥ������ע�ͱ��Ƴ�
    std::string expected_output = R"(
        int main() {
            int number = 42; 
            float pi = 3.14; 
            if (number == 42) {
                printf("Hello, World!");
            }
            return 0;
        }
    )";

    // ���������ַ���
    std::string actual_output;
    int pre_line = 1; // ���������ַ����ӵ�һ�п�ʼ
    int pre_col = 1; // ���������ַ����ӵ�һ�п�ʼ
    int result = lexer.analyze(input_code, actual_output, pre_line, pre_col);
    cout << actual_output << endl;
    cout << expected_output << endl;
    return 0;
}
