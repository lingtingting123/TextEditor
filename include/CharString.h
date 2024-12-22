#pragma once
#include <iostream>
using namespace std;

//这是用C语言风格实现的字符串的类
//可以代表一行元素，也可以代表目标字符串

class CharString{

public:
    char* cstring;
    int len;

    CharString(char* c);
    CharString();

    // ~CharString(){
    //     delete cstring;
    // }

    //读入的字符串成为一个实例
    //可实现行的重写
    CharString& operator=(const CharString &copy);  //const非常重要！

    //替换：将该字符串(行)中出现的str1（旧子串）替换为str2（目标子串）
    void replaceSubstr(CharString str1, CharString str2);

    void showCharString();

};

//从用户输入读入字符串
CharString Read(istream &in);

//查找：返回第二个字符串在第一个字符串中出现的下标，若未出现，返回-1
int Index(CharString haystack, CharString needle);

//重载输出函数
ostream& operator<<(ostream& out, const CharString &a){
    out<<a.cstring;
    return out;
}