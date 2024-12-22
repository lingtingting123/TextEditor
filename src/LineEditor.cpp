#include <iostream>
#include <string.h>
#include "../include/util.h"
#include "../include/LineEditor.h"

using namespace std;

LineEditor::LineEditor()
{
    curLineIdx = 0;
    running = false;
}

void LineEditor::showTextBuffer(int n)
{
    textBuffer.show(n);
}

bool LineEditor::lineIdxChange(int idx)
{
    // 检查合法性
    if (idx < 1 || idx > textBuffer.linesNum())
    {
        cout << "超出有效位置区间，移动失败" << endl;
        return false;
    }
    curLineIdx = idx;
    return true;
}

void LineEditor::DelLineAt(int idx)
{
    textBuffer.DelElem(idx);
    // 特别地，删除最后一行后移动到第一行
    // curLineIdx = curLineIdx % textBuffer.linesNum();

    curLineIdx = idx;
    // 更易读：
    if (curLineIdx > textBuffer.linesNum())
        curLineIdx = 1;
}

void LineEditor::DelCurLine()
{
    DelLineAt(curLineIdx);
}

void LineEditor::InsertLineAt(int idx, CharString newLine)
{
    textBuffer.InsertElem(idx, newLine);
    curLineIdx = idx;
}

void LineEditor::FindSubstr(CharString needle)
{
    while (curLineIdx <= textBuffer.linesNum())
    {
        // cout << "当前是寻找缓冲区的目标串操作，显示缓冲区：" << endl;
        // textBuffer.show(textBuffer.linesNum());
        // cout << "显示目标子串：" << endl;
        // cout << needle << endl;
        // 取出当前行
        curLine = textBuffer.GetElem(curLineIdx);

        // 在当前行查找子串
        int index = Index(curLine, needle);

        if (index == -1)
        {
            // 未找到子串
            curLineIdx++;
        }
        else
        {
            // 找到子串，带标号输出其在当前行的位置
            cout << "(" << curLineIdx << ")" << curLine << endl;
            cout << ' ';
            for (int i = 0; i <= curLineIdx / 10; i++)
                cout << ' ';
            cout << ' ';
            for (int i = 0; i < index; i++)
                cout << ' ';
            for (int i = 1; i <= needle.len; i++)
                cout << '^';
            cout << endl;
            return;
        }
    }
    // 如果都未找到
    cout << "未找到" << needle << endl;
    curLineIdx--;
}

bool LineEditor::FindAndReplace_inline(CharString oldPart, CharString newPart)
{
    // 在当前行查找目标子串
    curLine = textBuffer.GetElem(curLineIdx);
    int index = Index(curLine, oldPart);

    // 未找到返回flase
    if (index == -1)
        return false;

    // 如果找到目标子串,将其替换成新子串
    CharString newLine = curLine;
    newLine.replaceSubstr(oldPart, newPart);
    textBuffer.SetElem(curLineIdx, newLine);
    return true;
}

void LineEditor::FindAndReplace_whole(CharString oldPart, CharString newPart)
{
    for (curLineIdx = 1; curLineIdx <= textBuffer.linesNum(); curLineIdx++)
    {
        FindAndReplace_inline(oldPart, newPart);
    }
    curLineIdx--;
}

void LineEditor::readFromFile(const string &filename, DblLinkList<CharString> &textBuffer)
{
    cout << "Reading content from input file " << "\"" << filename << "\"" << "..." << endl;

    ifstream file(filename);
    if (!file)
    {
        cerr << "Failed to open file for reading!" << endl;
        return;
    }

    char line[MAX_LINE_LENGTH]; // 定义一个字符数组存储每行内容
    int k = 1;                  // 行号计数

    while (file.getline(line, MAX_LINE_LENGTH))
    {                                      // 使用 file.getline 读取一行到 char 数组
        CharString charStr(line);          // 将读取的行转为 CharString
        textBuffer.InsertElem(k, charStr); // 插入链表
        k++;
    }
    cout << "Buffer content successfully read from " << filename << '!' << endl;

    //textBuffer.show(textBuffer.linesNum());

    curLineIdx = 1;

    file.close();
}

void LineEditor::writeToFile(const string &filename, const DblLinkList<CharString> &textBuffer)
{
    cout << "Writing buffer content to output file " << "\"" << filename << "\"" << "..." << endl;

    ofstream file(filename); // 打开输出文件
    if (!file)
    {
        cerr << "Failed to open file for writing!" << endl;
        return;
    }

    // 遍历缓冲区的每一行
    for (int i = 1; i <= textBuffer.linesNum(); i++)
    {
        CharString line = textBuffer.GetElem(i); // 获取第 i 行
        file << line.cstring << endl;            // 写入文件并换行
    }

    file.close(); // 关闭文件
    cout << "Buffer content successfully written to " << filename << '!' << endl;
}

void LineEditor::Undo()
{
    if (historystatus.empty() || historystatus.size() == 1)
    {
        cout << "没有可撤销的操作！" << endl;
        return;
    }
    historystatus.pop();
    status preStatus = historystatus.top();
    textBuffer = preStatus.textBuffers;
    curLineIdx = preStatus.lineIdx;
    cout << "成功返回上一步状态" << endl;
}

void LineEditor::createNewFile(const char* filePath)
{
    ofstream newFile(filePath); // 打开文件，如果不存在则创建
    if (!newFile)
    {
        cerr << "无法创建文件: " << filePath << endl;
        return;
    }
    newFile.close(); // 立即关闭，文件已创建
    cout << "文件 " << filePath << " 已成功创建。" << endl;

    updateFile(filePath);

    cout<<"当前文件路径："<<currentFile<<endl;

    cout << "温馨提示：进行编写后，请点击保存(W)才会把最终内容保存至新建文件中" << endl;
}

void LineEditor::updateFile(const char *newFilePath)
{
    currentFile = new char[MAX_LINE_LENGTH];
    strcpy(currentFile,newFilePath);
}

void LineEditor::run()
{
    running = true;

    while (running)
    {
        // cout<<"当前缓冲区内容:"<<endl;
        // textBuffer.show(textBuffer.linesNum());
        // // 显示当前行
        if (curLineIdx)
        {
            cout << "当前位置：" << endl;
            curLine = textBuffer.GetElem(curLineIdx);
            cout << "(" << curLineIdx << ") " << curLine << endl;
        }
        else
        {
            cout << "文件缓存为空" << endl;
        }

        // 显示输入提示字符
        cout << "?";

        // 获取用户指令
        char *input = new char[MAX_LINE_LENGTH];
        char *command = new char[MAX_LINE_LENGTH];

        cin >> input;

        // 去除字符串中的空格和换行符，并将字母转换为大写
        processInput(input, command);

        // 将输入的字符串指令转换为usercommand类型
        userCommand = StrToCommand(command);

        // Usercommand类型匹配并执行操作
        if (userCommand == UserCommand::DC)
        {
            cout << "删除当前行" << endl;
            DelCurLine();
        }
        else if (userCommand == UserCommand::DA)
        {
            cout << "删除指定行" << endl;
            int lineNum = 1;
            cout << "请输入行数：";
            cin >> lineNum;
            DelLineAt(lineNum);
        }
        else if (userCommand == UserCommand::N)
        {
            cout << "转到下一行" << endl;
            lineIdxChange(curLineIdx + 1);
        }
        else if (userCommand == UserCommand::P)
        {
            cout << "转到上一行" << endl;
            lineIdxChange(curLineIdx - 1);
        }
        else if (userCommand == UserCommand::B)
        {
            cout << "转到第一行" << endl;
            lineIdxChange(1);
        }
        else if (userCommand == UserCommand::E)
        {
            cout << "转到最后一行" << endl;
            lineIdxChange(textBuffer.linesNum());
        }
        else if (userCommand == UserCommand::I)
        {
            cout << "插入单个新行" << endl;
            int lineNum = 1;
            CharString newLine;

            cout << "请输入行号：";
            cin >> lineNum;
            cin.get(); // 消除缓冲区的换行字符

            cout << "请输入新行：";
            newLine = Read(cin);

            InsertLineAt(lineNum, newLine);
        }
        else if (userCommand == UserCommand::Q)
        {
            cout << "退出编辑器" << endl;
            running = false;
        }
        else if (userCommand == UserCommand::V)
        {
            cout << "查看缓存区所有内容" << endl;
            showTextBuffer(textBuffer.linesNum());
        }
        else if (userCommand == UserCommand::HEAD)
        {
            cout << "查看缓存区前五行内容" << endl;
            showTextBuffer(5);
        }
        else if (userCommand == UserCommand::F)
        {
            cout << "查找目标子串" << endl;
            int idx;
            cout << "请选择开始查找的行（第一行 = 1，当前行 = 0）：";

            cin >> idx;

            // 更新curLineIdx
            if (idx)
                curLineIdx = 1;
            cout << "请输出要查找的子串：";
            cin.get();
            CharString needle = Read(cin);

            // 查找子串
            FindSubstr(needle);
        }
        else if (userCommand == UserCommand::C)
        {
            cout << "查找并替换目标子串" << endl;
            int InCurLine;
            cout << "请选择该行有效或全文有效（该行 = 1，全文 = 0）：";
            cin >> InCurLine;
            cout << "请输出要被替换的子串：";
            cin.get();
            CharString oldPart = Read(cin);
            cout << "请输出要替换成的子串：";
            CharString newPart = Read(cin);

            if (InCurLine == 1)
                FindAndReplace_inline(oldPart, newPart);
            else if(InCurLine == 0)
                FindAndReplace_whole(oldPart, newPart);
        }
        else if (userCommand == UserCommand::RW)
        {
            cout << "重写当前行" << endl;
            cout << "请重写：" << endl;
            cin.get();
            CharString newLine = Read(cin);
            textBuffer.SetElem(curLineIdx, newLine);
        }
        else if (userCommand == UserCommand::H)
        {
            cout << "查看帮助：" << endl;
            cout << "有效命令：b(egin) c(hange) d(el)c(cur) d(el)a(t) e(nd)" << endl;
            cout << "         f(ind) g(o) h(elp) i(nsert) p(rior) q(uit) r(ead)" << endl;
            cout << "         z(撤销) v(iew) w(rite) rw(rewrite) head" << endl;
        }
        else if (userCommand == UserCommand::R)
        {
            int openFile;
            cout << "打开文本文件（1）/新建文本文件（0）" << endl;
            cin >> openFile;
            if (openFile == 1)
            {
                cout << "请输入要读取的文本文件路径: ";
                char *inputFile = new char[MAX_LINE_LENGTH];
                cin >> inputFile;
                readFromFile(inputFile, textBuffer);

                updateFile(inputFile);
            }
            else if (openFile == 0)
            {
                char filePath[MAX_LINE_LENGTH];
                cout << "请输入新建文本文件路径：" << endl;
                cin >> filePath;

                createNewFile(filePath);

                updateFile(filePath);

                DblLinkList<CharString> newTextBuffer;
                textBuffer = newTextBuffer;
                curLineIdx = 0;
            }
        }
        else if (userCommand == UserCommand::W)
        {
            int storeInCurrentFile;
            cout << "保存(1)/另存为(0):";
            cin >> storeInCurrentFile;
            if(storeInCurrentFile == 1){
                writeToFile(currentFile, textBuffer);
            }else if(storeInCurrentFile == 0){
                cout << "请输入要保存到的文本文件路径: ";
                char *outputFile = new char[MAX_LINE_LENGTH];
                cin >> outputFile;
                writeToFile(outputFile, textBuffer);
            }
        }
        else if (userCommand == UserCommand::Z)
        {
            cout << "撤销，返回上一步（状态）" << endl;
            Undo();
        }

        // if (historystatus.empty())
        // {
        //     cout << "上一步操作为空" << endl;
        // }
        // else
        // {
        //     // cout << "上一步记录：" << endl;
        //     // status pre = historystatus.top();
        //     // pre.textBuffers.show(pre.textBuffers.linesNum());
        //     // cout << "previous lineNum = " << pre.lineIdx << endl;
        // }

        // 压入当前状态
        if (userCommand != UserCommand::Z )
            historystatus.push({textBuffer, curLineIdx});

        cout << endl;
    }

    // //操作结束，显示当前行
    // cout<<"当前位置："<<endl;
    // curLine = textBuffer.GetElem(curLineIdx);
    // cout<<"("<<curLineIdx<<") "<<curLine<<endl;
}

// bool LineEditor::NextLine()
// {
//     lineIdxChange(curLineIdx + 1);
// }

// bool LineEditor::LastLine()
// {
//     lineIdxChange(curLineIdx - 1);
// }