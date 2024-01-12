#include "graph.h"

Graph::Graph(): vexnum(0), edgenum(0), dfsRoot(nullptr), bfsRoot(nullptr){}

void Graph::InitMembers()
{
    visitVexList.clear();
    visitEdgeList.clear();
    visited.clear();
    visited.resize(vexnum);
    visited.assign(vexnum,false);
}

Node* Graph::GetDFSRoot()
{
    return dfsRoot;
}
Node* Graph::GetBFSRoot()
{
    return bfsRoot;
}

void Graph::InitGraph(std::string message)
{
    std::istringstream iss(message);
    std::string v1, v2;
    int w;
    while(iss >> v1 >> v2 >> w)
    {
        if(!idx.count(v1))
        {
            idx.emplace(v1, vexnum);
            vex.emplace(vexnum, v1);
            vexnum++;
        }
        if(!idx.count(v2))
        {
            idx.emplace(v2, vexnum);
            vex.emplace(vexnum, v2);
            vexnum++;
        }
        edges.emplace_back(idx[v1], idx[v2], w);
    }

    algraph.InitGraph(vexnum, edgenum, edges);
    amlgraph.InitGraph(vexnum, edgenum, edges);
}

bool Graph::DFSSequence(std::string start)
{
    if(!idx.count(start)) return false;
    InitMembers();

    int s = idx[start];     // 起始点下标
    Stack<int> vstk;        // 顶点下标栈
    Stack<Edge> estk;       // 边栈，和顶点栈同步出入，存储需要访问的边
    vstk.push(s);           // 顶点入栈

    while(!vstk.empty())
    {
        int now = vstk.top();
        vstk.pop();         // 顶点出栈

        if(visited[now])    // 访问过
        {
            if(!estk.empty()) estk.pop();   // 和顶点同步的边同时出栈
            continue;
        }
        visited[now] = true;
        visitVexList.emplace_back(now);     // 加入访问过的顶点序列
        if(!estk.empty())   // 判断边栈是否为空（避免 now == 顶点时候，边栈为空的情况）
        {
            visitEdgeList.emplace_back(estk.top());
            estk.pop();
        }
        AMLGraph::EdgeNode* edge = amlgraph.adjmulist[now]; // 该顶点的邻接顶点链表

        while(edge)
        {
            int next = (now == edge->ivex) ? edge->jvex:edge->ivex;
            if(!visited[next])      // 未访问过，加入栈中
            {
                vstk.push(next);
                estk.push(Edge(edge->ivex, edge->jvex, edge->weight));
            }
            edge = (now == edge->ivex) ? edge->ilink : edge->jlink;
        }
    }


    return true;
}
bool Graph::BFSSequence(std::string start)
{
    if(!idx.count(start)) return false;
    InitMembers();
    int s = idx[start];
    Queue<int> vque;
    Queue<Edge> eque;
    vque.push(s);
    visited[s] = true;

    while(!vque.empty())
    {
        int now = vque.front();
        vque.pop();
        visitVexList.emplace_back(now);

        if(!eque.empty())
        {
            visitEdgeList.emplace_back(eque.front());
            eque.pop();
        }

        AMLGraph::EdgeNode* edge = amlgraph.adjmulist[now];
        while(edge)
        {
            int next = (now == edge->ivex) ? edge->jvex : edge->ivex;
            if(!visited[next])
            {
                vque.push(next);
                eque.push(Edge(edge->ivex, edge->jvex, edge->weight));
                visited[next] = true;
            }
            edge = (now == edge->ivex) ? edge->ilink : edge->jlink;
        }
    }

    return true;

}
bool Graph::BuildDFSTree(std::string start)
{
    if(!idx.count(start)) return false;
    InitMembers();
    int s = idx[start];
    Stack<int>vstk;
    Stack<Node*>nstk;       // 与顶点栈同步的结点栈
    dfsRoot = nullptr;
    vstk.push(s);

    while(!vstk.empty())
    {
        int now = vstk.top();
        vstk.pop();

        if(visited[now]) continue;
        visited[now] = true;

        Node* node = new Node(now, vex[now]);
        if(now == s)
        {
            dfsRoot = node;
        }
        else
        {
            Node* father = nstk.top();
            nstk.pop();
            if(father->firstchild == nullptr)
            {
                father->firstchild = node;
            }
            else
            {
                Node* child = father->firstchild;
                while(child->nextsibling) child = child->nextsibling;
                child->nextsibling = node;
            }
        }

        ALGraph::EdgeNode* edge = algraph.vertices[now];


        while(edge)
        {
            if(!visited[edge->next])
            {
                vstk.push(edge->next);
                nstk.push(node);
            }
            edge = edge->nextedge;
        }
    }


    return true;

}
bool Graph::BuildBFSTree(std::string start)
{
    if(!idx.count(start)) return false;
    InitMembers();

    int s = idx[start];
    Queue<int> vque;
    Queue<Node*> nque;
    bfsRoot = new Node(s, vex[s]);
    visited[s] = true;
    vque.push(s), nque.push(bfsRoot);

    while(!vque.empty())
    {
        int now = vque.front();
        Node* root = nque.front();
        vque.pop(), nque.pop();
        ALGraph::EdgeNode* edge = algraph.vertices[now];

        Node* child  = nullptr;
        while(edge)
        {
            if(!visited[edge->next])
            {
                if(child == nullptr)
                {
                    child = new Node(edge->next, vex[edge->next]);
                    root->firstchild = child;
                }
                else
                {
                    child->nextsibling = new Node(edge->next, vex[edge->next]);
                    child = child->nextsibling;
                }
                vque.push(edge->next);
                nque.push(child);
                visited[edge->next] = true;
            }
            edge = edge->nextedge;
        }
    }

    return true;
}


std::string Graph::GetDFSSequence()
{
    std::string out = "深度优先遍历序列: \n";
    for(int i: visitVexList)
    {
        out += vex[i]+" ";
    }
    out += "\n\n";
    out += "深度优先遍历生成树的边集: \n";
    for(Edge& e: visitEdgeList)
    {
        out += vex[e.i] + " " + vex[e.j] + " " + std::to_string(e.w) + "\n";
    }

    return out;
}

std::string Graph::GetBFSSequence()
{
    std::string out = "广度优先遍历序列: \n";
    for(int i: visitVexList)
    {
        out += vex[i]+" ";
    }
    out += "\n\n";
    out += "广度优先遍历生成树的边集: \n";
    for(Edge& e: visitEdgeList)
    {
        out += vex[e.i] + " " + vex[e.j] + " " + std::to_string(e.w) + "\n";
    }
    return out;
}
