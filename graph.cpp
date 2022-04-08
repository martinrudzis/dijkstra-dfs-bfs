#include <queue>
#include <climits>
#include <set>
#include <iostream>
#include <fstream>
#include <map>
#include <functional>

#include "graph.h"

/**
 * A graph is made up of vertices and edges
 * A vertex can be connected to other vertices via weighted, directed edge
 */


////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////


/** constructor, empty graph */
Graph::Graph() {
    numberOfVertices = 0;
    numberOfEdges = 0;
}

/** destructor, delete all vertices and edges
    only vertices stored in map
    no pointers to edges created by graph */
Graph::~Graph() {
    for(auto i = vertices.begin(); i != vertices.end(); ++i) {
        delete i->second;
        i->second = nullptr;
    }
}

/** return number of vertices */
int Graph::getNumVertices() const { 
    return numberOfVertices; 
}

/** return number of vertices */
int Graph::getNumEdges() const { 
    return numberOfEdges; 
}

/** add a new edge between start and end vertex
    if the vertices do not exist, create them
    calls Vertex::connect
    a vertex cannot connect to itself
    or have multiple edges to another vertex */
bool Graph::add(std::string start, std::string end, int edgeWeight) {
    if (start != end) {
        // Create the vertices if they don't exist
        auto* startPtr = findOrCreateVertex(start);
        auto* endPtr = findOrCreateVertex(end);
        bool canConnect = startPtr->connect(end, edgeWeight);
        if (canConnect) { numberOfEdges++; }
        // Add the vertices to vertices map if they don't exist
        if (canConnect && vertices.count(start) == 0) {
            vertices[start] = startPtr;
            numberOfVertices++;
        }
        if (canConnect && vertices.count(end) == 0) {
            vertices[end] = endPtr;
            numberOfVertices++;
        }
        return canConnect;
    }
    return false;
}

/** return weight of the edge between start and end
    returns INT_MAX if not connected or vertices don't exist */
int Graph::getEdgeWeight(std::string start, std::string end) const { 
    // Try to find the start vertex
    auto* startVertex = findVertex(start);
    // If found, getEdgeWeight returns the weight value
    if (startVertex != nullptr) {
        // Vertex::getEdgeWeight returns -1 if the end vertex is not found
        int edgeWeight = startVertex->getEdgeWeight(end);
        // return edgeWeight if edgeWeight is not -1
        if (edgeWeight != -1) { return edgeWeight; }
    }
    // Otherwise, retrun INT_MAX
    return INT_MAX; 
}

/** read edges from file
    the first line of the file is an integer, indicating number of edges
    each edge line is in the form of "string string int"
    fromVertex  toVertex    edgeWeight */
void Graph::readFile(std::string filename) {
    std::ifstream inputFile;
    inputFile.open(filename);
    if (inputFile.is_open()) {
        std::string line;
        int numLines;
        inputFile >> numLines;
        for (int i = 0; i < numLines; i++) {
            std::string start, end;
            int weight;
            inputFile >> start;
            inputFile >> end;
            inputFile >> weight;
            add(start, end, weight);
            getline(inputFile, line);
        }
    }
}

/** depth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::depthFirstTraversal(std::string startLabel, 
void visit(const std::string&)) {
    unvisitVertices();
    auto* startVertex = findVertex(startLabel);
    depthFirstTraversalHelper(startVertex, visit);
}

/** breadth-first traversal starting from startLabel
    call the function visit on each vertex label */
void Graph::breadthFirstTraversal(std::string startLabel,
void visit(const std::string&)) {
    unvisitVertices();
    std::queue<std::string> q;
    q.push(startLabel);
    auto* startVertex = findVertex(startLabel);
    visit(startVertex->getLabel());
    startVertex->visit();
    while (!q.empty()) {
        std::string wLabel = q.front();
        q.pop();
        auto* w = findVertex(wLabel);
        for (int i = 0; i <= w->getNumberOfNeighbors(); i++) {
            std::string uLabel = w->getNextNeighbor();
            auto* u = findVertex(uLabel);
            if (!u->isVisited()) {
                visit(u->getLabel());
                u->visit();
                q.push(uLabel);
            } 
        }
    }
}

/** find the lowest cost from startLabel to all vertices that can be reached
    using Djikstra's shortest-path algorithm
    record costs in the given map weight
    weight["F"] = 10 indicates the cost to get to "F" is 10
    record the shortest path to each vertex using given map previous
    previous["F"] = "C" indicates get to "F" via "C"

    cpplint gives warning to use pointer instead of a non-const map
    which I am ignoring for readability */
void Graph::djikstraCostToAllVertices(
    std::string startLabel,
    std::map<std::string, int>& weight,
    std::map<std::string, std::string>& previous) {
        weight.clear();
        previous.clear();
        std::priority_queue<std::pair<int, std::string>> pq;
        std::set<std::string> vertexSet;

        auto* start = findVertex(startLabel);
        for (int i = 0; i < start->getNumberOfNeighbors(); i++) {
            std::string uLabel = start->getNextNeighbor();
            int weightU = start->getEdgeWeight(uLabel);
            weight[uLabel] = weightU;
            previous[uLabel] = startLabel;
            pq.push(std::make_pair(-weightU, uLabel));
        }
        vertexSet.insert(startLabel);
        while (!pq.empty()) {
            std::string vLabel = pq.top().second;
            pq.pop();
            if (vertexSet.count(vLabel) == 0) {
                vertexSet.insert(vLabel);
                auto* v = findVertex(vLabel);
                int weightV = weight.find(vLabel)->second;
                for (int i = 0; i < v->getNumberOfNeighbors(); i++) {
                    std::string uLabel = v->getNextNeighbor();
                    int v2ucost = v->getEdgeWeight(uLabel);
                    if (weight[uLabel] == 0) {
                        weight[uLabel] = weight[vLabel] + v2ucost;
                        previous[uLabel] = vLabel; 
                        pq.push(std::make_pair(-(weightV + v2ucost), uLabel));
                    }
                    else {
                        if (weight[uLabel] > (weight[vLabel] + v2ucost)) {
                            weight[uLabel] = weight[vLabel] + v2ucost;
                            previous[uLabel] = vLabel; 
                            pq.push(std::make_pair(-(weightV + v2ucost), 
                            uLabel));
                        }
                    }
                }
            }
        }
    }


/** helper for depthFirstTraversal */
void Graph::depthFirstTraversalHelper(Vertex* startVertex,
void visit(const std::string&)) {
    visit(startVertex->getLabel());
    startVertex->visit();
    for (int i = 0; i < startVertex->getNumberOfNeighbors(); i++) {
        std::string nLabel = startVertex->getNextNeighbor();
        auto* n = findVertex(nLabel);
        if (!n->isVisited()) {
            depthFirstTraversalHelper(n, visit);
        }
    }
}

/** helper for breadthFirstTraversal */
// void Graph::breadthFirstTraversalHelper(Vertex*startVertex,
//                                         void visit(const std::string&)) {}

/** mark all verticies as unvisited */
void Graph::unvisitVertices() {
    for (auto i = vertices.begin(); i != vertices.end(); ++i) {
        i->second->unvisit();
    }
}

/** find a vertex, if it does not exist return nullptr */
Vertex* Graph::findVertex(const std::string& vertexLabel) const { 
    if (vertices.count(vertexLabel) != 0) {
        auto* vertexPtr = vertices.at(vertexLabel);
        return vertexPtr;
    }
    return nullptr; 
}

/** find a vertex, if it does not exist create it and return it */
Vertex* Graph::findOrCreateVertex(const std::string& vertexLabel) { 
    auto* vertexPtr = findVertex(vertexLabel);
    if (vertexPtr == nullptr) {
        vertexPtr = new Vertex(vertexLabel);
    }
    return vertexPtr; 
}
