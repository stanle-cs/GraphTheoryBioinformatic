#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::getline;
using std::pair;
using std::map;

/**
 *  Process the input from input into machine readable vector of edges
 * 
 * \param graph a vector of string obtained from the input containing
 *              entries in the form of "X -> Y,Z"
 * 
 * \return  a vector of edges containg a pair of two nodes in the form
 *          {X, Y}
 */
vector<pair<string, string>> inputprocessing(const vector<string>& graph) {
    
    vector<pair<string, string>> edges;
    
    for (int i =0; i < graph.size(); i++) {
        
        string temp = graph[i];
        string origin = "";
        string destination = "";
        
        //catch the origin node
        int origin_end = temp.find(" -> ",0);
        origin = temp.substr(0, origin_end);
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
            edges.push_back({origin, destination});
            //delete that part
            temp.erase(0, dest_end + 1);
        }
    }    

    return edges;
}

void identify_sink_source(vector<pair<string, string>> edges, string& sink, string& source) {
    
    map<string, int> inlets, outlets;

    for (int i = 0; i < edges.size(); i++) {
        //if it has been initialized, just increase it
        if (inlets.count(edges[i].first) != 0) {
            inlets[edges[i].first]++;
        }
        //if not, initialize it
        else {
            inlets[edges[i].first] = 0;
        }

        //if it has been initialized, just increase it
        if (outlets.count(edges[i].second) != 0) {
            outlets[edges[i].second]++;
        }
        //if not, initialize it
        else {
            outlets[edges[i].second] = 0;
        }

    }
}

/**
 * Finds an Eulerian path in the given graph
 *
 * \param graph     a vector of strings containing edges in the graph;
 *                  one string may correspond to multiple edges if it
 *                  is of the form "X -> Y,Z"
 *
 * \return a string containing an Eulerian path through the given graph
 */
string eulerian_path(const vector<string>& graph) {
    
    vector<pair<string, string>> edges;
    string sink, source;

    //input processing
    edges = inputprocessing(graph);
    
    //TODO: finish this
    identify_sink_source(edges, sink, source);

    //output
    cout << "\n";
    for (int i = 0; i < edges.size(); i++) {
        cout << "{" << edges[i].first << ", " << edges[i].second << "}\n";
    }


    return "";
}


int main(int argc, char* argv[]) {
    vector<string> graph;

    string line;
    while (getline(cin, line, '\n')) {
        graph.push_back(line);
    }

    cout << eulerian_path(graph) << endl;
}
