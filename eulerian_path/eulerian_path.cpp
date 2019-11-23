#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <set>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::getline;
using std::pair;
using std::map;
using std::unordered_set;
using std::set;

struct graph {
    set<pair<string, string>> edges;
    unordered_set<string> nodelist;
    map<string, int> outlet;
    map<string, int> inlet;
    int N;
};

/**
 *  Process the input from graph into printable string
 * 
 * \param graph a struct graph containing the graph that needed to be output
 * 
 * \return  an output string containing the edgelist, inlet and outlet count
 *          of each node
 */
string output_process(struct graph graph) {
    string output = "";
    //output node list
    cout << "node list:\n";
    for (auto const& i: graph.nodelist) {
        cout << i << "\n";
    }
    
    //output edge list
    cout << "edgelist:\n";
    for (auto const& edge: graph.edges) {
        cout << edge.first << "->" << edge.second << "\n";
    }
    //output inlet counts
    cout << "inlets:\n";
    for (auto const &i: graph.inlet) {
        cout << i.first << ": " << i.second << "\n";
    }
    //output outlet counts
    cout << "outlets:\n";
    for (auto const &i: graph.outlet) {
        cout << i.first << ": " << i.second << "\n";
    }

    return output;
}

/**
 *  Process the input from input into machine readable struct graph
 * 
 * \param graph_input   a vector of string obtained from the input containing
 *                      entries in the form of "X -> Y,Z"
 * 
 * \return  a struct graph that contain edge list, node list, size, and inlet,
 *          outlet count
 */
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
            //append origin to the nodelist
            if (graph.nodelist.count(origin) != 1) {
                graph.nodelist.insert(origin);
            }
            //append destination to the nodelist
            if (graph.nodelist.count(destination) != 1) {
                graph.nodelist.insert(destination);
            }
            //finalize the edge and store it into the edge list
            graph.edges.insert({origin, destination});
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
    //fill in the zeros in inlet list
    for (auto const& node: graph.nodelist) {
        if (graph.inlet.count(node) == 0) {
            graph.inlet[node] = 0;
        }
    }
    //fill in the zeros in outlet list
    for (auto const& node: graph.nodelist) {
        if (graph.outlet.count(node) == 0) {
            graph.outlet[node] = 0;
        }
    }

    //define the size of graph
    graph.N = graph.nodelist.size();
    return graph;  
}

/**
 * find the sink and source of the given graph
 * 
 * \param graph     a struct graph that has nodelist, edgelist, inlet
 *                  and outlet counts
 * \param &sink     the sink to be returned to
 * \param &source   the source to be returned to
 */
void findsinksource(struct graph graph, string &sink, string &source) {
    for (auto const& node: graph.nodelist) {
        if (graph.inlet[node] < graph.outlet[node]) {
            source = node;
        }
        if (graph.inlet[node] > graph.outlet[node]) {
            sink = node;
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
    
    struct graph eulerian_graph;
    string sink, source;
    string output;
    //input processing
    eulerian_graph = input_process(graph);
    
    //find sink and source
    sink = "";
    source = "";
    findsinksource(eulerian_graph, sink, source);
    cout << "\n" << "Sink: " << sink << ", Source: " << source << "\n";
    
    //add an edge between sink and source


    
    //output
    output = output_process(eulerian_graph);


    return output;
}


int main(int argc, char* argv[]) {
    vector<string> graph;

    string line;
    while (getline(cin, line, '\n')) {
        graph.push_back(line);
    }

    cout << eulerian_path(graph) << endl;
}
