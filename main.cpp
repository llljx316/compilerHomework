#include "LexicalAnalyzer.h"
#include <iostream>
#include <cassert>

int main() {
    // 创建词法分析器实例
    LexicalAnalyzer lexer;

    // 输入字符串，包含各种词法元素
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

    // 定义预期的行为，例如，这里我们预期所有注释被移除
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

    // 分析输入字符串
    std::string actual_output;
    int pre_line = 1; // 假设输入字符串从第一行开始
    int pre_col = 1; // 假设输入字符串从第一列开始
    int result = lexer.analyze(input_code, actual_output, pre_line, pre_col);
    cout << actual_output << endl;
    cout << expected_output << endl;
    return 0;
}
