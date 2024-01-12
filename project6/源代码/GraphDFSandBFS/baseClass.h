#ifndef BASECLASS_H
#define BASECLASS_H

#endif // BASECLASS_H

#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include <sstream>
#include<algorithm>
#include<queue>
#pragma once

struct Node
{
    int idx;
    int x, y;
    int childnum;
    std::string data;
    Node *firstchild, *nextsibling;
    Node():childnum(0), firstchild(nullptr), nextsibling(nullptr){}
    Node(int idx): idx(idx), childnum(0), firstchild(nullptr), nextsibling(nullptr){}
    Node(int idx, std::string data): idx(idx), childnum(0), data(data), firstchild(nullptr), nextsibling(nullptr){}
};

struct Edge{
    int i, j, w;
    Edge(int i, int j, int w): i(i), j(j), w(w){}
};

// 邻接多重表

struct AMLGraph
{
    struct EdgeNode{
        int ivex, jvex;
        int weight;
        EdgeNode* ilink, *jlink;
        EdgeNode(){}
        EdgeNode(int ivex, int jvex, int weight):ivex(ivex), jvex(jvex) ,weight(weight), ilink(nullptr), jlink(nullptr){}
    };

    void InitGraph(int vexNum, int edgeNum, std::vector<Edge>& edges)
    {
        vexnum = vexNum, edgenum = edgeNum;
        adjmulist.clear();
        adjmulist.resize(vexnum);
        for(auto& v: adjmulist) v = nullptr;
        for(Edge& e: edges)
        {
            InsertEdge(e.i, e.j, e.w);
        }
    }

    void InsertEdge(int ivex, int jvex, int weight)
    {
        EdgeNode* edge = new EdgeNode(ivex, jvex, weight);

        edge->ilink = adjmulist[ivex];
        adjmulist[ivex] = edge;

        edge->jlink = adjmulist[jvex];
        adjmulist[jvex] = edge;

    }

    std::vector<EdgeNode* > adjmulist;
    int vexnum;
    int edgenum;
};

// 邻接表
struct ALGraph{
    struct EdgeNode{
        int next;
        int weight;
        EdgeNode* nextedge;
        EdgeNode(int next, int weight): next(next), weight(weight){}
    };

    void InitGraph(int vexNum, int edgeNum, std::vector<Edge>& edges)
    {
        vexnum = vexNum, edgenum = edgeNum;
        vertices.clear();
        vertices.resize(vexnum);
        for(auto& v: vertices) v = nullptr;
        for(Edge& e: edges)
        {
            InsertEdge(e.i, e.j, e.w);
        }
    }

    void InsertEdge(int i, int j, int w)
    {
        EdgeNode* ei = new EdgeNode(i, w);
        EdgeNode* ej = new EdgeNode(j, w);
        ej->nextedge = vertices[i];
        vertices[i] = ej;
        ei->nextedge = vertices[j];
        vertices[j] = ei;
    }

    std::vector<EdgeNode*> vertices;
    int vexnum;
    int edgenum;
};

template <typename ElemType>
class Stack{
private:
    struct Node{
        ElemType e;
        Node* next;
        Node(ElemType e, Node* next = nullptr): e(e), next(next){}
    };

    int sz;
    Node* head;

public:
    Stack(): sz(0), head(nullptr){}

    void push(ElemType e)
    {
        Node* node = new Node(e, head);
        head = node;
        sz++;
    }

    void pop()
    {
        if(!sz)return;
        if(sz == 1)
        {
            delete head;
            head = nullptr;
            sz--;
        }
        else
        {
            Node* temp = head;
            head = head->next;
            delete temp;
            sz--;
        }
    }

    ElemType& top()
    {
        return head->e;
    }

    int size()
    {
        return size;
    }

    bool empty()
    {
        return sz == 0;
    }
};

template <typename ElemType>
class Queue{
private:
    struct Node{
        ElemType data;
        Node* next;
        Node(const ElemType &e, Node* next = nullptr): data(e), next(next){}
    };

    int size;
    Node* head;
    Node* tail;
public:
    Queue(): size(0), head(nullptr), tail(nullptr){}
    void push(const ElemType& e)
    {
        if(head)
        {
            tail->next = new Node(e);
            tail = tail->next;
        }
        else
        {
            head = tail = new Node(e);
        }
        size++;
    }

    void pop()
    {
        if(empty())
        {
            return;
        }
        if(head == tail)
        {
            delete head;
            head = tail = nullptr;
        }
        else
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }

        size--;
    }

    ElemType& front()
    {
        return head->data;
    }

    bool empty()
    {
        return size==0;
    }

    ~Queue()
    {
        while(head)
        {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};
