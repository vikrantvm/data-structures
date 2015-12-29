//standard includes
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <ctime>
#include <vector>

//graphviz includes
#include "SimpleGraph.h"

using namespace std;

const double kPi =  3.14159265358979323;
const double kAttract = 3e-3;
const double kRepel = 3e-3;

void Welcome();
string getLine();
int getPositiveInteger();
string getGraphFile();
Node initializeNodes();
int getSimulationTime();
SimpleGraph loadGraph();
double getElapsedTime(time_t startTime);
void thisIsWhereMagicHappens(SimpleGraph& graph);
vector<Node> initializeDeltaNode(SimpleGraph& graph);
void calculateAttractiveForces(SimpleGraph& graph, vector<Node>& deltaNodeForces);

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

string getLine(){
    string line;
    getline(cin, line);
    return line;
}

int getPositiveInteger(){
    while(true){
        stringstream convertInt;
        convertInt << getLine();
        int resultInt;
        if(convertInt >> resultInt){
            char remaining;
            if(convertInt >>remaining)
                cout<<"Unexpected character";
            else
                return resultInt;
        }
        else if(resultInt < 0)
            cout<<"Please enter a positive integer"<<endl;
        else
            cout<<"Please enter an integer"<<endl;

        cout<<"Retry: ";

    }
}

string getGraphFile(){
    while(true){
        cout<<"Enter name of the graph file: ";
        string input = getLine();
        ifstream graphFile(input.c_str());
        if(graphFile.is_open()){
            graphFile.close();
            return input;
        }
        else
            cout<<"Can't find the graph file"<<input<<endl;
    }
}

Node initializeNodes(size_t currentNode, size_t maximumNodes){
    Node node;
    node.x = cos(2 * kPi * static_cast<double>(currentNode) / static_cast<double>(maximumNodes));
    node.y = sin(2 * kPi * static_cast<double>(currentNode) / static_cast<double>(maximumNodes));
    return node;
}

int getSimulationTime(){
    cout<<"Enter simulation time: ";
    return getPositiveInteger();
}

SimpleGraph loadGraph(){
    SimpleGraph graph;
    ifstream graphFile(getGraphFile().c_str());
    size_t numberOfNodes;
    graphFile >> numberOfNodes;

    for(size_t i = 0; i < numberOfNodes; ++i){
        Node initialNodes = initializeNodes(i, numberOfNodes);
        graph.nodes.push_back(initialNodes);
    }

    size_t start, end;

    while(graphFile >> start >> end){
        Edge edges;
        edges.start = start;
        edges.end = end;
        graph.edges.push_back(edges);
    }

    return graph;
}

double getElapsedTime(time_t startTime){
    return difftime(time(NULL), startTime);
}

vector<Node> initializeDeltaNode(SimpleGraph& graph){
    vector<Node> nodeVector;
    for(size_t i = 0; i < graph.nodes.size(); ++i){
        Node node;
        node.x = 0;
        node.y = 0;
        nodeVector.push_back(node);
    }
    return nodeVector;
}

double calculateAngleRadians(double x0, double x1, double y0, double y1){
    return atan2(y1 - y0, x1 - x0);
}
double calculateFAttract(double x0, double x1, double y0, double y1){
    return kAttract * pow(x1 - x0, 2.0) * pow(y1- y0, 2.0);
}

double calculateFRepel(double x0, double x1, double y0, double y1){
    return kRepel / (sqrt( pow(y1 - y0, 2.0) + pow(x1 - x0, 2.0) ) );
}

double calculateForceInX(double force, double thetaRadians){
    return force * cos(thetaRadians);
}

double calculateForceInY(double force, double thetaRadians){
    return force * sin(thetaRadians);
}

void calculateAttractiveForces(SimpleGraph& graph, vector<Node>& deltaNodeForces){
    for(size_t i = 0; i < graph.edges.size(); ++i){
        Node node0 = graph.nodes[graph.edges[i].start];
        Node node1 = graph.nodes[graph.edges[i].end];

        double x0 = node0.x;
        double y0 = node0.y;
        double x1 = node1.x;
        double y1 = node1.y;

        double fAttract = calculateFAttract(x0,x1, y0, y1);
        double theta = calculateAngleRadians(x0,x1, y0, y1);

        deltaNodeForces[graph.edges[i].start].x += calculateForceInX(fAttract,theta);
        deltaNodeForces[graph.edges[i].start].y += calculateForceInY(fAttract,theta);
        deltaNodeForces[graph.edges[i].end].x -= calculateForceInX(fAttract,theta);
        deltaNodeForces[graph.edges[i].end].y -= calculateForceInY(fAttract,theta);
    }
}

void calculateRepulsiveForces(SimpleGraph& graph, vector<Node>& deltaNodeForces){
    for(size_t i = 0; i < graph.nodes.size() - 1; ++i){
        for(size_t j = i + 1; j < graph.nodes.size(); ++j){
            Node node0 = graph.nodes[i];
            Node node1 = graph.nodes[j];

            double x0 = node0.x;
            double y0 = node0.y;
            double x1 = node1.x;
            double y1 = node1.y;

            double fRepel = calculateFRepel(x0,x1, y0, y1);
            double theta = calculateAngleRadians(x0,x1, y0, y1);

            deltaNodeForces[i].x -= calculateForceInX(fRepel,theta);
            deltaNodeForces[i].y -= calculateForceInY(fRepel,theta);
            deltaNodeForces[j].x += calculateForceInX(fRepel,theta);
            deltaNodeForces[j].y += calculateForceInY(fRepel,theta);
        }
    }

}

void updateNodePositions(SimpleGraph& graph, vector<Node>& deltaNodeForces){
    for(size_t i = 0; i < graph.nodes.size() - 1; ++i){
        graph.nodes[i].x += deltaNodeForces[i].x;
        graph.nodes[i].y += deltaNodeForces[i].y;

        deltaNodeForces[i].x = 0;
        deltaNodeForces[i].y = 0;
    }
}

void thisIsWhereMagicHappens(SimpleGraph& graph){
    vector<Node> nodeUpdates = initializeDeltaNode(graph);
    calculateRepulsiveForces(graph, nodeUpdates);
    calculateAttractiveForces(graph, nodeUpdates);
    updateNodePositions(graph, nodeUpdates);
}

// Main method
int main() {
    Welcome();
    SimpleGraph graph = loadGraph();
    InitGraphVisualizer(graph);
    DrawGraph(graph);

    int simTime = getSimulationTime();
    time_t start = time(NULL);
    while(true){
        thisIsWhereMagicHappens(graph);
        DrawGraph(graph);
        if(getElapsedTime(start) > simTime) break;
    }

    return 0;
}






