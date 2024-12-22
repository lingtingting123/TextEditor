#include <iostream>
#include <fstream>
#include <stack>
#include "../src/CharString.cpp"
#include "DblLinkList.h"
#include "Defined.h"

using namespace std;

//这是文本编辑器类，可实现对缓冲区与文件的操作

//每步状态
struct status{
    DblLinkList<CharString> textBuffers;
    int lineIdx;
};

class LineEditor{
public:
    //---数据---//
    DblLinkList<CharString> textBuffer;
    int curLineIdx;
    CharString curLine;
    stack<status> historystatus; //用栈实现撤销操作
    char* currentFile;

    UserCommand userCommand;
    bool running;

    //---函数---//

    //查看缓冲区前n行内容
    void showTextBuffer(int n);

    //转到指定i行
    bool lineIdxChange(int idx);

    //删除指定i行
    void DelLineAt(int idx);

    //删除该行并移到下一行
    void DelCurLine();

    //插入单个新行，指定行号
    void InsertLineAt(int idx, CharString newLine);

    //从当前行开始查找子串
    void FindSubstr(CharString needle);

    //在当前行查找并替换目标子串
    bool FindAndReplace_inline(CharString oldPart, CharString newPart);

    //在全文查找并替换目标子串
    void FindAndReplace_whole(CharString oldPart, CharString newPart);

    //将指定文件内容输入到缓冲区中
    void readFromFile(const string& filename, DblLinkList<CharString>& textBuffer);

    //将缓冲区内容输出到指定文件中
    void writeToFile(const string& filename, const DblLinkList<CharString>& textBuffer);

    //撤销操作
    void Undo();

    //新建文本文件
    void createNewFile(const char* filePath);

    //记录当前文本路径
    void updateFile(const char* newFilePath);

    
// public:
    //初始化
    LineEditor();

    void run();
};