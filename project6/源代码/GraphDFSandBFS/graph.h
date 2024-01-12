#ifndef GRAPH_H
#define GRAPH_H

#include<baseClass.h>

class Graph
{
public:
    Graph();
    void InitMembers();
    void InitGraph(std::string message);
    bool DFSSequence(std::string start);
    bool BFSSequence(std::string start);
    bool BuildDFSTree(std::string start);
    bool BuildBFSTree(std::string start);
    std::string GetDFSSequence();
    std::string GetBFSSequence();
    Node* GetDFSRoot();
    Node* GetBFSRoot();


private:
    int vexnum;
    int edgenum;
    Node* dfsRoot;
    Node* bfsRoot;
    ALGraph algraph;
    AMLGraph amlgraph;
    std::vector<bool> visited;
    std::vector<int> visitVexList;
    std::vector<Edge> visitEdgeList;
    std::vector<Edge> edges;  // 暂存 边的信息，方便计算图中顶点数目
    std::unordered_map<std::string , int > idx;
    std::unordered_map<int, std::string> vex;
};

#endif // GRAPH_H
