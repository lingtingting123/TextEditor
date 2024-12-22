#pragma once
#include <iostream>
using namespace std;

//这是用于基于双向链表用于存放每一行元素的模板类，表示缓冲区
//模板类的声明和实现需放在一个文件里

template <class T>
struct Node{
    T data;
    Node* next;
    Node* pre;

    Node():next(nullptr),pre(nullptr){}
    Node(T data):data(data),next(nullptr),pre(nullptr){}
};

//模板类
template <class T>
class DblLinkList{
protected:
    Node<T>* head;
    int size;

public:
    //初始化
    DblLinkList();

    //析构函数
    ~DblLinkList();

    //取出元素（行）
    T GetElem(int idx)const;

    //插入元素（行）/ 尾插
    void InsertElem(int idx, T &data);

    // //指定位置元素进行部分查找并替换
    // void ModifyElem(int idx, T oldPart, T newPart);

    //覆盖元素
    void SetElem(int idx,T newElem);

    //删除元素（行）
    void DelElem(int idx);

    //输出缓冲区元素
    void show(int end);

    //返回行数
    int linesNum()const{return size;}

    //重载赋值函数，用于实现撤销操作
    DblLinkList<T>& operator=(const DblLinkList<T>& copy);

    DblLinkList(const DblLinkList<T>& other);

};

template <class T>
inline DblLinkList<T>::DblLinkList()
{
    head = new Node<T>();
    int size = 0;
}

template <class T>
inline DblLinkList<T>::~DblLinkList()
{
    Node<T>* temp = head->next;
    while(temp!=nullptr){
        Node<T>* current = temp;
        temp = temp->next;
        delete current;
    }
    delete head;
}

template <class T>
inline T DblLinkList<T>::GetElem(int idx)const
{
    //判断位置是否合法
    //cout<<"合法位置："<<"[1 - "<<size<<" ],";
    //cout<<"获取位置："<<idx;
    if(idx>size||idx<1){
        cout<<"不合法"<<endl;
        return head->data;
    }

    //获取合法位置的元素
    Node<T>* goal = head->next;
    for(int k = 1; k < idx; k++,goal = goal->next);
    T elem = goal->data;
    return elem;
}

template <class T>
inline void DblLinkList<T>::InsertElem(int idx, T &data)
{
    //判断是否合法
    // cout<<"合法范围：";
    // cout<<"[ 1 - "<<size+1<<" ],";
    // cout<<"设置元素 "<<data<<" 位置= "<<idx;
    if(idx>size+1||idx<1){
        cout<<" , 不合法"<<endl;
        return;
    }

    //进行指定位置的元素插入
    Node<T>* newNode = new Node<T>(data);
    Node<T>* prev = head;

    //head: 0 结点
    for(int k = 1; k < idx; prev=prev->next,k++);

    if(head->next == nullptr){
        //空链表插入第一个结点
        // cout<<"head->next is nullptr"<<endl;
        head->next = newNode;
        newNode->pre = head;

    }else if(idx <= size){ //在尾节点之前插入
        //在prev结点后插入新结点
        newNode->next = prev->next;
        prev->next->pre = newNode;
        prev->next = newNode;
        newNode->pre = prev;

    }else{
        //尾插
        prev->next = newNode;
        newNode->pre = prev;
    }
    size++;
}

// template <class T>
// inline void DblLinkList<T>::ModifyElem(int idx, T oldPart, T newPart)
// {
//     if(idx>size||idx<1){
//         cout<<"位置超出合法范围"<<endl;
//         return;
//     }

//     //修改合法位置的元素
//     Node<T>* goal = head->next;
//     for(int k = 1; k < idx; k++,goal = goal->next);
//     goal.
// }

template <class T>
inline void DblLinkList<T>::SetElem(int idx, T newElem)
{
    if(idx>size||idx<1){
        cout<<"位置超出合法范围"<<endl;
        return;
    }

    //修改合法位置的元素
    Node<T>* goal = head->next;
    for(int k = 1; k < idx; k++, goal = goal->next);
    goal->data = newElem;
}

template <class T>
inline void DblLinkList<T>::DelElem(int idx)
{
    //判断是否合法
    cout<<"合法范围：";
    cout<<"[ 1 - "<<size<<" ],";
    cout<<"删除元素位置= "<<idx;
    if(idx>size||idx<1){
        cout<<" , 不合法"<<endl;
        return;
    }
    cout<<endl;

    //在有效位置删除元素
    Node<T>* cur = head->next;//站在第一个结点
    for(int k = 1; k < idx; cur=cur->next,k++);
    //删除cur结点
    if(cur->pre && cur->next){
        //如果当前删除的元素前后有节点
        cur->pre->next = cur->next;//前指后
        cur->next->pre = cur->pre;//后指前
    }else if(!cur->next){
        cur->pre->next = cur->next;
    }
    delete cur;
    size--;
}

template <class T>
inline void DblLinkList<T>::show(int end)
{
    if(head->next == nullptr){
        cout<<"该缓冲区为空"<<endl;
        return;
    }
    Node<T>* temp = head->next;
    int idx = 1;
    for(int k = 1; k <= size && k <= end; k++){
        cout<<idx<<": ";
        cout<<temp->data<<endl;
        temp = temp->next;
        idx++;
    }
}

template <class T>
inline DblLinkList<T>::DblLinkList(const DblLinkList<T> &other)
{
    head = new Node<T>(); // 创建一个新的空头节点
    size = 0;

    Node<T>* otherNode = other.head->next;
    while (otherNode != nullptr) {
        T data = otherNode->data; // 获取数据
        InsertElem(size + 1, data); // 将数据插入到当前链表中
        otherNode = otherNode->next;
    }
}

template <class T>
inline DblLinkList<T> &DblLinkList<T>::operator=(const DblLinkList<T> &copy)
{
    if(this == &copy){
        return *this;
    }

    Node<T>* temp = head->next;
    while(temp!= nullptr){
        Node<T>* cur = temp;
        temp = temp->next;
        delete cur;
    }
    size = 0;

    Node<T>* copyNode = copy.head->next;
    while(copyNode != nullptr){
        T data = copyNode->data;
        InsertElem(size+1,data);
        copyNode = copyNode->next;
    }
    return *this;
}
