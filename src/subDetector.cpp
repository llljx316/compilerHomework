#include "common.h"
#include <vector>
#include"subDetector.h"

extern std::string Delimiter[DELIMITER_NUM]; // �����������ط�����ķָ�������
extern std::string ReserveWord[RESERVED_WORD_NUM]; // �����ⲿ����ı���������
extern std::set<std::string> Digit; // �����������ط���������ּ���
extern std::string Operator[OPERATOR_NUM]; // �����������ط���������������
extern std::set<std::string> Identifier; // �����ⲿ����ı�ʶ������



// ==================================================================================================================================================
//                                                    DelimiterDetector
// �ڷָ��������в��Ҹ������ַ���
// ch: Ҫ���ҵ��ַ���
int DelimiterDetector::findDelimiter(std::string ch)
{
    // �����ָ�������
    for (int index = 0; index < DELIMITER_NUM; index++) {
        // ����ҵ�ƥ��ķָ������������������е�����
        if (ch.find(Delimiter[index]) != -1)
            return index;
    }
    // ���û���ҵ�ƥ��ķָ���������NOT
    return NOT;
}

// �жϸ����ַ�����ָ��λ���Ƿ���һ����Ч�ķָ���
// str: �������ַ�����start: ��ʼλ�õ����ã�end: ����λ�õ�����
// line: �кŵ����ã�col: �кŵ�����
int DelimiterDetector::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    uint currentPosition = start; // ��ǰ�����λ��

    // ����findDelimiter�������ҵ�ǰλ�õ��ַ��Ƿ��Ƿָ���
    if (findDelimiter(&str[currentPosition]) == NOT) {
        // ������Ƿָ���������NOT
        return NOT;
    }
    else {
        // ����Ƿָ���������start��endλ�ã������кż�һ
        start = end = currentPosition;
        col++;
        // ����SUCCESS��ʾ�ɹ��ҵ��ָ���
        return SUCCESS;
    }
}
// ==================================================================================================================================================




// ==================================================================================================================================================
//                                                    KeywordDetector
// ���캯������ʼ��������ӳ���
KeywordDetector::KeywordDetector()
{
    // �������������飬��������������������ӳ���ϵ
    for (unsigned int i = 0; i < RESERVED_WORD_NUM; i++)
        ReservedWord_Map[ReserveWord[i]] = i;
}

// �жϸ����ַ�����ָ�������Ƿ�Ϊ������
// str: �������ַ�����start: ��ʼλ�õ����ã�end: ����λ�õ�����
// line: �кŵ����ã�col: �кŵ�����
int KeywordDetector::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    uint currentPosition = start; // ��ǰ�����λ��
    uint currentColumn = col; // ��ǰ�к�
    std::string foundWord = ""; // ���ڴ洢ʶ����ĵ���

    // ѭ�������ַ����е��ַ�
    while (1) {
        // ����ַ�����ĸ��������ӵ�word��
        if (isAlphaOrUnderscore(str[currentPosition])) {
            foundWord += str[currentPosition];
            currentPosition++;
            currentColumn++;
        }
        else {
            break; // ��������ĸ�ַ�ʱֹͣѭ��
        }
    }

    // �ڱ�����ӳ����в���ʶ����ĵ���
    auto iter = ReservedWord_Map.find(foundWord);
    if (iter != ReservedWord_Map.end()) { // ����ҵ�
        end = currentPosition - 1; // ���ý���λ��
        col = currentColumn; // �����к�
        return iter->second; // ���ر�������ӳ����е�λ��
    }
    else {
        return NOT; // ���δ�ҵ�������NOT
    }
}
// ==================================================================================================================================================




// ==================================================================================================================================================
//                                                    NumericSequenceValidator
// ���캯������ʼ��״̬ΪSTATE_START
NumericSequenceValidator::NumericSequenceValidator()
{
    state = this->STATE_START;
}

// ����״̬ΪSTATE_START
void NumericSequenceValidator::resetState()
{
    state = STATE_START;
}

// ����ַ����Ƿ�����Ч����������
// str: ��Ҫ�����ַ�����start: ��ʼλ�õ����ã�end: ����λ�õ�����
// line: �кŵ����ã�col: �кŵ�����
int NumericSequenceValidator::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    // �ǳ�ʼ״̬ʱ����NOT
    if (state != STATE_START)
        return NOT;

    uint currentPosition = start; // ��ǰ����λ��
    for (;; currentPosition++) {
        // �����ַ�����Χʱ����NOT
        if (currentPosition > str.size()) {
            return NOT;
        }

        // ���ݵ�ǰ״̬���ַ�����״̬ת��
        switch (state) {
        case STATE_START:
            if (isdigit(str[currentPosition])) {
                // �����ַ�������STATE_START״̬
            }
            else if (str[currentPosition] == '.') {
                // ����С�������STATE_1״̬
                state = STATE_1;
            }
            else if (str[currentPosition] == 'e') {
                // ����e����STATE_3״̬
                state = STATE_3;
            }
            else {
                // �����ַ�������ֹ״̬
                state = STATE_TERMINATE;
            }
            break;

        case STATE_1:
            if (isdigit(str[currentPosition])) {
                // �����ַ�����STATE_2״̬
                state = STATE_2;
            }
            else if (str[currentPosition] == 'e') {
                // ����e����STATE_3״̬
                state = STATE_3;
            }
            else if (str[currentPosition] == '.') {
                // �ٴ�����С����������״̬
                state = STATE_ERROR;
            }
            else {
                // �����ַ�������ֹ״̬
                state = STATE_TERMINATE;
            }
            break;

        case STATE_2:
            if (isdigit(str[currentPosition])) {
                // �����ַ�������STATE_2״̬
            }
            else if (str[currentPosition] == 'e') {
                // ����e����STATE_3״̬
                state = STATE_3;
            }
            else if (str[currentPosition] == '.') {
                // ����С����������״̬
                state = STATE_ERROR;
            }
            else {
                // �����ַ�������ֹ״̬
                state = STATE_TERMINATE;
            }
            break;

        case STATE_3:
            if (isdigit(str[currentPosition])) {
                // �����ַ�����STATE_4״̬
                state = STATE_4;
            }
            else if (str[currentPosition] == '.' || str[currentPosition] == 'e') {
                // ����С�����e�������״̬
                state = STATE_ERROR;
            }
            else {
                // �����ַ�������ֹ״̬
                state = STATE_TERMINATE;
            }
            break;

        case STATE_4:
            if (isdigit(str[currentPosition])) {
                // �����ַ�������STATE_4״̬
            }
            else if (str[currentPosition] == '.' || str[currentPosition] == 'e') {
                // ����С�����e�������״̬
                state = STATE_ERROR;
            }
            else {
                // �����ַ�������ֹ״̬
                state = STATE_TERMINATE;
            }
            break;

        default:
            // Ĭ������½������״̬
            state = STATE_ERROR;
            break;
        }

        // ���������ֹ״̬�����״̬���˳�ѭ��
        if (state == STATE_TERMINATE || state == STATE_ERROR)
            break;
    }

    // ���������ֹ״̬������ʶ�������������
    if (state == STATE_TERMINATE) {
        end = currentPosition - 1; // ���ý���λ��
        state = STATE_START; // ����״̬
        col = col + (currentPosition - start); // �����к�
        // ��ʶ������������м���Digit����
        std::string temp = str.substr(start, end - start + 1);
        Digit.insert(temp);
        return SUCCESS;
    }
    else {
        state = STATE_START; // ����״̬
        return NOT; // ����NOT��ʾδʶ�����Ч��������
    }
}
// ==================================================================================================================================================



// ==================================================================================================================================================
//                                                    OperatorDetector
// ���캯������ʼ�������ӳ���
OperatorDetector::OperatorDetector()
{
    // �������������ӵ�ӳ����У�ӳ�����ǵ�����
    for (unsigned int i = 0; i < OPERATOR_NUM; i++)
        Operator_Map[Operator[i]] = i;
}

// �жϸ����ַ�����ָ��λ���Ƿ���һ����Ч�������
// str: �������ַ�����start: ��ʼλ�õ����ã�end: ����λ�õ�����
// line: �кŵ����ã�col: �кŵ�����
int OperatorDetector::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col) {
    char currentChar = str[start]; // ��ǰ������ַ�

    // �����ַ��ж����������
    switch (currentChar) {
        // ���ַ��������ֱ������endΪstart
    case ',':
    case '.':
    case '?':
    case ':':
        end = start;
        break;

        // ������˫�ַ���������������Ҫ�����һ���ַ�
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '=':
    case '!':
    case '~':
    case '^':
    case '>':
    case '<':
    case '&':
    case '|':
        // �����һ���ַ��� '=' ������ͬ�ַ��������� '>' �� '<'��������˫�ַ������
        end = (str[start + 1] == '=' || (currentChar == str[start + 1] && currentChar != '>' && currentChar != '<')) ? start + 1 : start;
        break;

        // �������������������������
    default:
        return NOT;
    }

    // �����к�
    col += (end - start) + 1;
    // ����SUCCESS��ʾ�ɹ�ʶ��������
    return SUCCESS;
}
// ==================================================================================================================================================



// ==================================================================================================================================================
//                                                    SymbolRecognizer
// ��һ���ַ������뵽��ʶ��������
// ch: Ҫ������ַ���
void SymbolRecognizer::addToken(std::string ch)
{
    Identifier.insert(ch); // ���ַ������뵽��ʶ������
}

// �жϸ����ַ�����ָ�������Ƿ�Ϊ��Ч�ı�ʶ��
// str: �������ַ�����start: ��ʼλ�õ����ã�end: ����λ�õ�����
// line: �кŵ����ã�col: �кŵ�����
int SymbolRecognizer::isAccepted(const std::string& str, unsigned int& start, unsigned int& end, unsigned int& line, unsigned int& col)
{
    uint currentPosition = start; // ��ǰ�����λ��
    uint currentColumn = col; // ��ǰ�к�
    std::string identifierStr = ""; // ���ڴ洢ʶ����ı�ʶ��

    // ѭ�������ַ����е��ַ�
    while (1) {
        // ����ַ�����ĸ�����֣�������ӵ�word��
        if (isAlphaOrUnderscore(str[currentPosition]) || isNumericDigit(str[currentPosition])) {
            identifierStr += str[currentPosition];
            currentPosition++;
            currentColumn++;
        }
        else {
            break; // ��������ĸ�������ַ�ʱֹͣѭ��
        }
    }

    end = currentPosition - 1; // ���ñ�ʶ���Ľ���λ��
    col = currentColumn; // �����к�

    // ���δʶ�����ʶ��������NOT
    if (identifierStr == "")
        return NOT;

    // �ɹ�ʶ���ʶ����������ӵ���ʶ�����ϣ�������SUCCESS
    addToken(identifierStr);
    return SUCCESS;
}
// ==================================================================================================================================================