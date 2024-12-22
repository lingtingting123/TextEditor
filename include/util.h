#include <iostream>
#include <ctype.h> // toupper() 和 isspace() 函数
#include <string.h> // strlen() 函数
#include "Defined.h"
using namespace std;

//这是一个工具类

// 去除字符串中的空格和换行符，并将字母转换为大写
void processInput(char *input, char *output) {
    int i = 0, j = 0;

    // 遍历输入字符串
    while (input[i] != '\0') {
        if (!isspace(input[i])) {  // 忽略空格和换行符
            output[j] = toupper(input[i]);  // 转换为大写并写入 output
            j++;
        }
        i++;
    }
    output[j] = '\0';  // 末尾添加字符串结束符 '\0'
}


//用户输入转换成userCommand
UserCommand StrToCommand(const char *command)
{
    //成员不能直接访问，必须通过枚举名限定
    if(!strcmp(command,"DC")) return UserCommand::DC;
    if(!strcmp(command,"DA")) return UserCommand::DA;
    if(!strcmp(command,"N")) return UserCommand::N;
    if(!strcmp(command,"P")) return UserCommand::P;
    if(!strcmp(command,"B")) return UserCommand::B;
    if(!strcmp(command,"E")) return UserCommand::E;
    if(!strcmp(command,"I")) return UserCommand::I;
    if(!strcmp(command,"Q")) return UserCommand::Q;
    if(!strcmp(command,"V")) return UserCommand::V;
    if(!strcmp(command,"HEAD")) return UserCommand::HEAD;
    if(!strcmp(command,"F")) return UserCommand::F;
    if(!strcmp(command,"C")) return UserCommand::C;
    if(!strcmp(command,"RW")) return UserCommand::RW;
    if(!strcmp(command,"H")) return UserCommand::H;
    if(!strcmp(command,"R")) return UserCommand::R;
    if(!strcmp(command,"W")) return UserCommand::W;
    if(!strcmp(command,"Z")) return UserCommand::Z;

    return UserCommand::NONE;
}