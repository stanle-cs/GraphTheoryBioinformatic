#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

struct graph {
    vector<pair<string, string>> edges;
    vector<string> nodelist;
    map<string, int> outlet;
    map<string, int> inlet;
    int N;
};

string output_process(struct graph graph) {
    string output;
    for (int i = 0; i < graph.edges.size(); i++) {
        output += graph.edges[i].first + "->" + graph.edges[i].second + "\n";
    }
    cout << "inlets: \n";
    for (auto const &i: graph.inlet) {
        cout << i.first << ": " << i.second << "\n";
    }
    cout << "outlets: \n";
    for (auto const &i: graph.outlet) {
        cout << i.first << ": " << i.second << "\n";
    }
    return output;
}

struct graph input_process(vector<string> graph_input) {
    struct graph graph;

    for (int i =0; i < graph_input.size(); i++) {
        
        //define temporary stuff to be used during processing
        string temp = graph_input[i];
        string origin = "";
        string destination = "";
        
        //catch the origin node
        int origin_end = temp.find(" -> ",0);
        origin = temp.substr(0, origin_end);
        //append it to the nodelist        
        graph.nodelist.push_back(origin);
        //check if origin is a registered node, if not initialize it
        if (graph.outlet.count(origin) != 1) {
            graph.outlet[origin] = 0;
        }
        //delete it from the temp string
        temp.erase(0, origin_end + 4);
        //add "," for uniformity
        temp += ",";
        //while there are ","s remained in the temp string
        while (temp.find(",", 0) != -1) {
            //catch this destination node
            int dest_end = temp.find(",", 0);
            destination = temp.substr(0, dest_end);
            //finalize the edge and store it into the edge list
            graph.edges.push_back({origin, destination});
            //increase the outlet count of origin
            graph.outlet[origin]++;
            //check if destination is a registered node, if not then initialize it
            if (graph.inlet.count(destination) == 0) {
                graph.inlet[destination] = 1;
            }
            //else we increase its inlet by one
            else {
                graph.inlet[destination]++;
            }
            //delete that part
            temp.erase(0, dest_end + 1);
        }
    }
    //define the size of graph
    graph.N = graph.nodelist.size();
    return graph;  
}

int main () {
    vector<string> graph_input;
    struct graph graph;
    string line;
    while (getline(cin, line, '\n')) {
        graph_input.push_back(line);
    }
    graph = input_process(graph_input);
    cout << output_process(graph) << endl;
    cout << graph.N;
    return 0;
}