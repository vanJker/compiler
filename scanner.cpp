#include "scanner.h"

// C++ 关键字
const QSet<QString> KEYWORD_SET = {
    "asm", "else", "new", "this", "auto", "enum",
    "operator", "throw", "bool", "explicit", "private",
    "true", "break", "export", "protected", "try",
    "case", "extern", "public", "typedef", "catch",
    "false", "register", "typeid", "char", "float",
    "reinterpret_cast", "typename", "class", "for",
    "return", "union", "const", "friend", "short",
    "unsigned", "const_cast", "goto", "signed", "using",
    "continue", "if", "sizeof", "virtual", "default",
    "inline", "static", "void", "delete", "int",
    "static_cast", "volatile", "do",  "long", "struct",
    "wchar_t", "double", "mutable", "switch", "while",
    "dynamic_cast", "namespace", "template", "include"
};

// C++ 标点符号
const QSet<char> PUNCTUATION_SET = {
    '{', '}', ';', '#'
};

// 判断当前字符是否为数字字符
bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

// 判断当前字符是否为字母字符
bool isLetter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// 判断当前字符是否为标点符号
bool isPunctuation(char c)
{
    return PUNCTUATION_SET.contains(c);
}

// 判断当前字符是否为空白字符
bool isBlank(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

// 判断当前字符是否为文件结束符
bool isEOF(char c)
{
    return c == '\0' || c == EOF;
}

// 判断当前单词是否为关键字
bool isKeyword(QString word)
{
    return KEYWORD_SET.contains(word);
}


// 单词属性表
QMap<QString, QString> word_map;

char str[1024];	// 输入
int line = 0;  	// 行号
int i = 0;	// 当前位置

// 扫描一个单词
char *doScan(char *str)
{
    int state = 0;	// 状态
    QString word;	// 当前单词

    char c = *str++;
    while (true) {
        switch (state) {
        case 0:
            // state 0 只负责根据已有字符进行状态转发，不读取新字符
            if (isBlank(c)) {
                if (isEOF(c)) return --str;
                c = *str++;
                state = 0;
            } else if (isLetter(c) || c == '_') {
                state = 1;
            } else if (isDigit(c)) {
                state = 2;
            } else if (c == '<') {
                state = 3;
                word += c;
                c = *str++;
            } else if (c == '>') {
                state = 5;
                word += c;
                c = *str++;
            } else if (c == '=') {
                state = 7;
                word += c;
                c = *str++;
            } else if (c == '+') {
                state = 8;
                word += c;
                c = *str++;
            } else if (c == '-') {
                state = 9;
                word += c;
                c = *str++;
            } else if (c == '*') {
                state = 11;
                word += c;
                c = *str++;
            } else if (c == '.') {
                state = 12;
                word += c;
                c = *str++;
            } else if (c == '!') {
                state = 13;
                word += c;
                c = *str++;
            } else if (c == '%') {
                state = 14;
                word += c;
                c = *str++;
            } else if (c == '&') {
                state = 15;
                word += c;
                c = *str++;
            } else if (c == '^') {
                state = 16;
                word += c;
                c = *str++;
            } else if (c == '|') {
                state = 17;
                word += c;
                c = *str++;
            } else if (c == ':') {
                state = 18;
                word += c;
                c = *str++;
            } else if (c == '/') {
                state = 19;
                word += c;
                c = *str++;
            } else if (c == '[' || c == ']') {
                state = 23;
            } else if (c == '(' || c == ')') {
                state = 24;
            } else if (c == '~') {
                state = 25;
            } else if (c == ',') {
                state = 26;
            } else if (c == '\'') {
                state = 27;
                word += c;
                c = *str++;
            } else if (c == '\"') {
                state = 28;
                word += c;
                c = *str++;
            } else if (isPunctuation(c)) {
                state = 29;
            }
            break;
        case 1:
            // state 1 - 关键字 / 标识符
            if (isLetter(c) || isDigit(c) || c == '_') {
                state = 1;
                word += c;
                c = *str++;
            } else {
                // 碰到分隔符
                if (isKeyword(word) && !word_map.contains(word)) {
                    // 该关键字且之前从未保存过
                    word_map.insert(word, "关键字");
                } else {
                    // 标识符
                    word_map.insert(word, "标识符");
                }
                return --str;
            }
            break;
        case 2:
            // state 2 - 数字
            if (isDigit(c)) {
                state = 2;
                word += c;
                c = *str++;
            } else {
                // 碰到分隔符
                if (!word_map.contains(word)) {
                    // 该数字之前从未保存过
                    word_map.insert(word, "数字");
                }
                return --str;
            }
            break;
        case 3:
            // state 3 - 运算符 <=, <
            if (c == '<') {
                state = 4;
                word += c;
                c = *str++;
            } else if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return --str;
            }
            break;
        case 4:
            // state 4 - 运算符 <<=, <<
            if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "位运算符");
                }
                return --str;
            }
            break;
        case 5:
            // state 5 - 运算符 >=, >
            if (c == '>') {
                state = 6;
                word += c;
                c = *str++;
            } else if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return --str;
            }
            break;
        case 6:
            // state 6 - 运算符 >>=, >>
            if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "位运算符");
                }
                return --str;
            }
            break;
        case 7:
            // state 7 - 运算符 ==, =
            if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "赋值运算符");
                }
                return --str;
            }
            break;
        case 8:
            // state 8 - 运算符 ++, +=, +
            if (c == '+') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "自增运算符");
                }
                return str;
            } else if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "加号/正号");
                }
                return --str;
            }
            break;
        case 9:
            // state 9 - 运算符 --, -=, -
            if (c == '-') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "自减运算符");
                }
                return str;
            } else if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else if (c == '>') {
                state = 10;
                word += c;
                c = *str++;
            } else {
                 if (!word_map.contains(word)) {
                    word_map.insert(word, "减号/负号");
                }
                return --str;
            }
            break;
        case 10:
            // state 10 - 运算符 ->*, ->, -
            if (c == '*') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "成员指针运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "指向成员运算符");
                }
                return --str;
            }
            break;
        case 11:
            // state 11 - 运算符 *=, *
            if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "乘号/地址访问运算符");
                }
                return --str;
            }
            break;
        case 12:
            // state 12 - 运算符 .*, .
            if (c == '*') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "成员指针运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "成员访问运算符");
                }
                return --str;
            }
            break;
        case 13:
            // state 13 - 运算符 !=, !
            if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return --str;
            }
            break;
        case 14:
            // state 14 - 运算符 %=, %
            if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "取余");
                }
                return --str;
            }
            break;
        case 15:
            // state 15 - 运算符 &&, &=, &
            if (c == '&') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return str;
            } else if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "取地址/按位与");
                }
                return --str;
            }
            break;
        case 16:
            // state 16 - 运算符 ^=, ^
            if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "按位异或");
                }
                return --str;
            }
            break;
        case 17:
            // state 17 - 运算符 ||, |=, |
            if (c == '|') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逻辑运算符");
                }
                return str;
            } else if (c == '=') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "复合赋值运算符");
                }
                return str;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "按位或");
                }
                return --str;
            }
            break;
        case 18:
            // state 18 - 运算符 ::
            if (c == ':') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "作用域运算符");
                }
                return str;
            } else {
                return --str;
            }
            break;
        case 19:
            // state 19 - 运算符 /
            if (c == '*') {
                state = 20;
                word += c;
                c = *str++;
            } else if (c == '/') {
                state = 22;
                word += c;
                c = *str++;
            } else {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "除号");
                }
                return --str;
            }
            break;
        case 20:
            // state 20 - 运算符 /*
            if (c == '*') {
                state = 21;
            }
            word += c;
            c = *str++;
            break;
        case 21:
            // state 21 - 运算符 */
            if (c == '/') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "多行注释");
                }
                return str;
            } else {
                state = 20;
                word += c;
                c = *str++;
            }
            break;
        case 22:
            // state 22 - 运算符 //
            if (c == '\n') {
                if (!word_map.contains(word)) {
                    word_map.insert(word, "单行注释");
                }
                return str;
            } else {
                word += c;
                c = *str++;
            }
            break;
        case 23:
            // state 23 - 运算符 [, ]
            if (c == '[' || c == ']') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "下标运算符");
                }
                return str;
            }
            break;
        case 24:
            // state 24 - 运算符 (, )
            if (c == '(' || c == ')') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "括号");
                }
                return str;
            }
            break;
        case 25:
            // state 25 - 运算符 ~
            if (c == '~') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "按位取反");
                }
                return str;
            }
            break;
        case 26:
            // state 26 - 运算符 ,
            if (c == ',') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "逗号运算符");
                }
                return str;
            }
            break;
        case 27:
            // state 27 - 运算符 '
            if (c == '\'') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "字符");
                }
                return str;
            } else {
                word += c;
                c = *str++;
            }
            break;
        case 28:
            // state 28 - 运算符 "
            if (c == '\"') {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "字符串");
                }
                return str;
            } else {
                word += c;
                c = *str++;
            }
        case 29:
            // state 29 - 标点符号 {, }, #, ;
            if (isPunctuation(c)) {
                word += c;
                if (!word_map.contains(word)) {
                    word_map.insert(word, "标点符号");
                }
                return str;
            }
        default:
            break;
        }
    }
}















