#include <iostream>
#include <string.h>
#include "../include/CharString.h"
#include "../include/Defined.h"

using namespace std;

CharString::CharString(char *c)
{
    if(c == nullptr) {
        len = MAX_LINE_LENGTH;
        return;
    }
    len = strlen(c);
    cstring = new char[len + 1];  // +1 是为了存储字符串结束符 '\0'
    strcpy(cstring, c);
}

CharString::CharString()
{
    cstring = new char[MAX_LINE_LENGTH];
    len = MAX_LINE_LENGTH;
}

// CharString::~CharString()
// {
//     delete[] cstring;
// }

CharString &CharString::operator=(const CharString &copy)
{
    if(&copy != this){
        delete[] cstring;
        len = copy.len;
        cstring = new char[len+1];
        strcpy(cstring,copy.cstring);
    }
    return *this;
}

void CharString::replaceSubstr(CharString str1, CharString str2)
{
    int index = Index(cstring,str1);
    //操作不合法
    if(index == -1){
        cout<<"Didn't find "<<str1.cstring<<" in "<<cstring<<" , can't replace "<<str1.cstring<<" with "<<str2.cstring<<endl;
        return;
    }
    //替换
    for(int i = index, k = 0; i <= index + str1.len-1; i++,k++){
        cstring[i] = str2.cstring[k];
    }
}

void CharString::showCharString()
{
    cout<<cstring;
    // cout<<"len = "<<len<<endl;
}

// CharString Read(istream &in)
// {
//     char* temp = new char[100];
//     in >> temp;
//     return CharString(temp);
// }

CharString Read(istream &in)
{
    char* temp = new char[MAX_LINE_LENGTH]; 

    // 使用 cin.getline 读取包含空格的整行
    in.getline(temp, MAX_LINE_LENGTH);  // 读取最多 99 个字符，最后一位保留给 '\0'

    // 返回一个 CharString 对象
    return CharString(temp);
}



int Index(CharString haystack, CharString needle)
{
    char* haystr = haystack.cstring;
    char* neestr = needle.cstring;
    char* result = strstr(haystr,neestr);

    //如果result为空指针，证明没有在haystack里面发现needle，返回-1
    if(result == nullptr) return -1;

    //如果result不为空指针，needle在haystack里面出现的位置为指针差值
    return (result - haystr);
}