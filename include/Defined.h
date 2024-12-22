#pragma once
//---定义常量---

//一行最多80个字符
const int MAX_LINE_LENGTH = 80;

//缓冲区最多1024行
const int MAX_LINES_NUM = 1024;

//---枚举UserCommand---
enum class UserCommand {
    NONE,
    DC, //delete current line 删除当前行并移动到下一行
    DA, //delete line at index of 删除指定行并移动到下一行
    N, //转到下一行
    P, //转到前一行
    B, //转到开始一行
    E, //转到最后一行
    I, //插入单个新行
    Q, //退出文本编辑器的运行
    V, //显示缓冲区所有内容
    HEAD, //显示缓冲区前五行内容
    F, //选择从第1/cur行开始查找目标串第一次出现的位置，返回第一个出现的行号
    C, //查找目标串出现的位置并进行替换（1次/行）：该行/全缓冲区
    RW, //用新行替代当前行
    H, //查看帮助
    R, //打开文本文件（1）/新建文本文件（0）
    W, //保存当前文件（1）/另存为...（0）
    Z, //撤销，返回上一步（状态）
};
