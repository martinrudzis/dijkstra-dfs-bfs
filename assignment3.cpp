#include <iostream>
#include <climits>
#include <map>
#include <sstream>
#include <vector>
#include <cassert>

#include "graph.h"

////////////////////////////////////////////////////////////////////////////////
// This is 80 characters - Keep all lines under 80 characters                 //
////////////////////////////////////////////////////////////////////////////////

using namespace std;

// tests basic edge functions
void testEdgeClass() {
   std::cout << "Testing edge class:" << std::endl;
   Edge defConstruct;
   Edge testEdge("A", 10);
   assert(defConstruct.getEndVertex() == "");
   assert(defConstruct.getWeight() == 0);
   assert(testEdge.getEndVertex() == "A");
   assert(testEdge.getWeight() == 10);
   std::cout << "Passed edge class test" << std::endl;
}

// Tests connect
void testVertexConnect() {
   std::cout << "Testing vertex connect method:" << std::endl;
   Vertex testVertex("A");
   testVertex.connect("B", 5);
   testVertex.connect("C", 6);
   std::cout << "Passed test" << std::endl;
}

// Tests disconnect
void testVertexDisconnect() {
   std::cout << "Testing vertex disconnect method:" << std::endl;
   Vertex testVertex("X");
   testVertex.connect("Y", 1);
   testVertex.connect("Z", 3);
   assert(testVertex.getNumberOfNeighbors() == 2);
   assert(testVertex.disconnect("Y"));
   assert(testVertex.disconnect("Z"));
   assert(testVertex.getNumberOfNeighbors() == 0);
   std::cout << "Passed test" << std::endl;
}

// tests getEdgeWeight
void testVertexGetEdgeWeight() {
   std::cout << "Testing vertex getEdgeWeight method" << std::endl;
   Vertex testVertex("I");
   testVertex.connect("J", 5);
   testVertex.connect("K", 100);
   testVertex.connect("L", 0);
   assert(testVertex.getEdgeWeight("J") == 5);
   assert(testVertex.getEdgeWeight("K") == 100);
   assert(testVertex.getEdgeWeight("L") == 0);
   assert(testVertex.getEdgeWeight("M") == -1);
   std::cout << "Passed test" << std::endl;
}

// Does the it return correct number of neighbors?
void testVertexGetNumberOfNeighbors() {
   std::cout << "Testing vertex getNumberOfNeighbors method:" << std::endl;
   Vertex testVertex("1");
   testVertex.connect("2", 3);
   testVertex.connect("3", 3);
   testVertex.connect("4", 3);
   assert(testVertex.getNumberOfNeighbors() == 3);
   std::cout << "Passed test" << std::endl;
}

// tests get next neighbor
void testVertexGetNextNeighbor() {
   std::cout << "Testing vertex getNextNeighbor method:" << std::endl;
   Vertex testVertex("1");
   testVertex.connect("2", 3);
   testVertex.connect("3", 4);
   testVertex.connect("4", 5);
   std::string nextNeighbor = "";
   for (int i = 0; i < testVertex.getNumberOfNeighbors(); i++) {
      nextNeighbor = testVertex.getNextNeighbor();
      std::cout << nextNeighbor << std::endl;      
   }
   std::cout << "Passed test" << std::endl;
}

// Tests == operator
void testVertexEquivalencyOperator() {
   std::cout << "Testing vertex equivalency operator:" << std::endl;
   Vertex testVertex1("A");
   Vertex testVertex2("A");
   Vertex testVertex3("B");
   assert(testVertex1 == testVertex2);
   assert(!(testVertex2 == testVertex3));
   std::cout << "Passed test" << std::endl;
}

// Tests < operator
void testVertexLessThanOperator() {
   std::cout << "Testing vertex less than operator:" << std::endl;
   Vertex testVertex1("A");
   Vertex testVertex2("B");
   Vertex testVertex3("A");
   assert(testVertex1 < testVertex2);
   assert(!(testVertex2 < testVertex3));
   std::cout << "Passed test" << std::endl;
}

void testGraphConstructor() {
   std::cout << "Testing Graph constructor:" << std::endl;
   Graph testGraph;
   std::cout << "Passed test" << std::endl;
}

// Tests add(), getNumEdges(), getNumVertices()
void testGraphAdd() {
   std::cout << "Testing Graph add method:" << std::endl;
   Graph testGraph;
   assert(testGraph.add("A", "B", 10));
   assert(testGraph.getNumVertices() == 2);
   assert(testGraph.add("B", "C", 5));
   assert(testGraph.getNumVertices() == 3);
   assert(testGraph.getNumEdges() == 2);
   // Attempt self-connection
   assert(!testGraph.add("A", "A", 3));
   assert(!testGraph.add("D", "D", 5));
   // Attempt multiple edges to same vertex
   assert(!testGraph.add("A", "B", 15));
   std::cout << "Passed test" << std::endl;
}

// Tests various graph functions
void testGraphGetEdgeWeight() {
   std::cout << "Testing Graph getEdgeWeight method:" << std::endl;
   Graph testGraph;
   testGraph.add("1", "2", 5);
   testGraph.add("1", "3", 10);
   testGraph.add("2", "3", 15);
   testGraph.add("3", "4", 20);
   // Attempt to get edge weight of connected, existing edges
   assert(testGraph.getEdgeWeight("1", "2") == 5);
   assert(testGraph.getEdgeWeight("1", "3") == 10);
   assert(testGraph.getEdgeWeight("2", "3") == 15);
   assert(testGraph.getEdgeWeight("3", "4") == 20);
   // Attempt to get edge weight of unconnected, existing edges
   assert(testGraph.getEdgeWeight("1", "4") == INT_MAX);
   assert(testGraph.getEdgeWeight("2", "4") == INT_MAX);
   // Attempt to get edge weight of nonexistant edges
   assert(testGraph.getEdgeWeight("6", "4") == INT_MAX);
   assert(testGraph.getEdgeWeight("15", "9") == INT_MAX);
   assert(testGraph.getEdgeWeight("3", "7") == INT_MAX);
   std::cout << "Passed test" << std::endl;
}

// Test ability to read from file
void testGraphReadFile() {
   std::cout << "Testing Graph readFile method" << std::endl;
   Graph testGraph1;
   Graph testGraph2;
   Graph testGraph3;
   testGraph1.readFile("graph0.txt");
   testGraph2.readFile("graph1.txt");
   testGraph3.readFile("graph2.txt");

   assert(testGraph1.getNumEdges() == 3);
   assert(testGraph2.getNumEdges() == 9);
   assert(testGraph3.getNumEdges() == 24);
   std::cout << "Passed test" << std::endl;
}

// global variable - need better method, but works for testing
// used by graphVisitor
ostringstream graphOut;

// global variable for Djikstra
map<string, int> weight;
map<string, string> previous;

// OK or ERR for tests
template <typename T>
string isOK(const T& got, const T& expected) {
    if (got == expected) {
        return "OK: ";
    } else {
        cout << "    Got   " << got << "\n expected " << expected << endl;
        return "ERR: ";
    }
}

// visitor function - print out the vertex label
void display(const string& anItem) {
    cout << anItem << " ";
}

// visitor function - add the string to global variable graphOut
void graphVisitor(const string& anItem) {
    graphOut << anItem << " ";
}

// add the path to get to this vertex to global variable global variable
// [A B]
// previous is a map of vertexLabel prevVertexLabel
// need to process it in reverse to get the path
void graphCostDisplayPath(string vertex) {
    vector<string> v;
    auto prevIt = previous.find(vertex);
    while (prevIt != previous.end()) {
        v.push_back(prevIt->second);
        prevIt = previous.find(prevIt->second);
    }
    // v now has the path
    // oh, avoiding the joy of subtracting from unsigned int!
    int size = static_cast<int>(v.size());
    // return if we did we not go through any other vertex
    if (size <= 1) {
        return;
    }
    // add space between entries
    graphOut << "via [";
    // all entries except the first and last one in reverse
    // v[size-1] is the starting vertex, skipping it
    for (int i {size - 2}; i > 0; --i)
        graphOut << v[i] << " ";
    // add the second to last entry without space after it
    // last entry is the destination vertex, not needed
    if (size > 0)
        graphOut << v[0];
    graphOut << "] ";
}

// unreachable nodes should not even be in weight
// but if they are,
// skip them as they should have weight of INT_MAX
// C(8) via [A B]
// getting to C has a cost of 8, we can get to C via A->B->C
void graphCostDisplay() {
    graphOut.str("");
    for (auto it: weight) {
        int cost = it.second;
        if (cost == INT_MAX)
            continue;
        string vertex = it.first;
        graphOut << vertex << "(" << cost << ") ";
        graphCostDisplayPath(vertex);
    }
}

void testGraph0() {
    cout << "testGraph0" << endl;
    Graph g;
    g.readFile("graph0.txt");
    cout << isOK(g.getNumVertices(), 3) << "3 vertices" << endl;
    cout << isOK(g.getNumEdges(), 3) << "3 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C "s) << "DFS" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C "s) << "BFS" << endl;
    g.djikstraCostToAllVertices("A", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(), "B(1) C(4) via [B] "s)
         << "Djisktra" << endl;
}

void testGraph1() {
    cout << "testGraph1" << endl;
    Graph g;
    g.readFile("graph1.txt");
    cout << isOK(g.getNumVertices(), 10) << "10 vertices" << endl;
    cout << isOK(g.getNumEdges(), 9) << "9 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C D E F G H "s) << "DFS" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B H C G D E F "s) << "BFS" << endl;

    g.djikstraCostToAllVertices("A", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 "B(1) C(2) via [B] "s +
                 "D(3) via [B C] E(4) via [B C D] "s +
                 "F(5) via [B C D E] " +
                 "G(4) via [H] "s +
                 "H(3) "s)
         << "Djisktra" << endl;
}

void testGraph2() {
    std::cout << "testGraph2" << std::endl;
    Graph g;

    g.readFile("graph2.txt");
    cout << isOK(g.getNumVertices(), 21) << "21 vertices" << endl;
    cout << isOK(g.getNumEdges(), 24) << "24 edges" << endl;

    graphOut.str("");
    g.depthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B E F J C G K L D H M I N "s)
         << "DFS from A" << endl;

    graphOut.str("");
    g.depthFirstTraversal("O", graphVisitor);
    cout << isOK(graphOut.str(), "O P R S T U Q "s)
         << "DFS from O" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("A", graphVisitor);
    cout << isOK(graphOut.str(), "A B C D E F G H I J K L M N "s)
         << "BFS from A" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("D", graphVisitor);
    cout << isOK(graphOut.str(), "D H I M N "s)
         << "BFS from D" << endl;

    graphOut.str("");
    g.depthFirstTraversal("U", graphVisitor);
    cout << isOK(graphOut.str(), "U "s)
         << "DFS from U" << endl;

    graphOut.str("");
    g.breadthFirstTraversal("U", graphVisitor);
    cout << isOK(graphOut.str(), "U "s)
         << "BFS from U" << endl;

    g.djikstraCostToAllVertices("O", weight, previous);
    graphCostDisplay();
    cout << isOK(graphOut.str(),
                 "P(5) Q(2) R(3) via [Q] S(6) via [Q R] " +
                 "T(8) via [Q R S] U(9) via [Q R S] "s)
         << "Djisktra O" << endl;
}

int main() {
   // My test functions
   testEdgeClass();
   testVertexConnect();
   testVertexDisconnect();
   testVertexGetEdgeWeight();
   testVertexGetNumberOfNeighbors();
   testVertexGetNextNeighbor();
   testVertexEquivalencyOperator();
   testVertexLessThanOperator();

   testGraphConstructor();
   testGraphAdd();
   testGraphGetEdgeWeight();
   testGraphReadFile();

// Provided
    testGraph0();
    testGraph1();
    testGraph2();
    return 0;
}
