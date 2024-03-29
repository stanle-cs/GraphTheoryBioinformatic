#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <chrono>

using namespace std;
using namespace std::chrono;

typedef struct graph {
    multiset<pair<string, string>> edges;
    set<string> nodelist;
    map<string, int> outlet;
    map<string, int> inlet;
    map<string, multiset<string>> deBruijin;
    int N;
    void addedge(string origin, string destination) {
        edges.insert({origin, destination});
        deBruijin[origin].insert(destination);
        outlet[origin]++;
        inlet[destination]++;
    }
    void deleteedge(string origin, string destination) {
        multiset<pair<string, string>>::iterator edge_it = edges.find({origin, destination});
        edges.erase(edge_it);
        multiset<string>::iterator deBruijin_it = deBruijin[origin].find(destination);
        deBruijin[origin].erase(deBruijin_it);
        outlet[origin]--;
        inlet[destination]--;
    }
    string toString() {
        string output = "";
        output += "nodelist:\n";
        for (auto const& node: nodelist) {
            output += node + "\n";
        }
        output += "edges:\n";
        for (auto const& edge: edges) {
            output += edge.first + "->" + edge.second + "\n";
        }
        output += "deBruijin:\n";
        for (auto const& node: deBruijin) {
            for (auto const& dest: node.second) {
                output += node.first + "->" + dest + "\n";
            }
        }
        return output;
    }
} graph;

/** Process the input from graph into printable string
 * 
 * \param graph a struct graph containing the graph that needed to be output
 * 
 * \return  an output string containing the edgelist, inlet and outlet count
 *          of each node
 */
string output_process(vector<pair<string, string>> eulerian_path) {
    string output = "";
    output += eulerian_path.front().first + eulerian_path.front().second.substr(eulerian_path.front().second.length() -1, 1);
    for (int i = 1; i < eulerian_path.size(); i++) {
        output += eulerian_path[i].second.substr(eulerian_path[i].second.length() - 1, 1);
    }

    return output;
}

/** Retravel a Eulerian path and get to the designated position start
 * 
 * \param path		a vector of pair string containing the edge of the path
 * \param start		where the new start of the path should be
 * 
 * \return a new path that started at the start position designated
 */
  
vector<pair<string, string>> retravelpath(vector<pair<string,string>> path, string start_node) {
	vector<pair<string, string>> output = path;
	while (output.back().second != start_node) {
		output.insert(output.begin(),output.back());
		output.erase(output.end());
	}
	return output;
}

/** Find the sink and source of the given graph
 * 
 * \param graph     a struct graph that has nodelist, edgelist, inlet
 *                  and outlet counts
 * \param &sink     the sink to be returned to
 * \param &source   the source to be returned to
 */
void findsinksource(struct graph graph, string &sink, string &source) {
    //cout << graph.toString();
    for (auto const& node: graph.nodelist) {
        if (graph.inlet[node] < graph.outlet[node]) {
            source = node;
        }
        if (graph.inlet[node] > graph.outlet[node]) {
            sink = node;
        }
    }
    //cout << "source: " << source << " sink: " << sink << "\n";
}

/** construct a Eulerian Cycle using the input graph
 * \param eulerian_graph    a graph that contain a cycle
 * 
 * \return a eulerian cycle in the form of a string
 */
vector<pair<string, string>> eulerian_cycle(struct graph eulerian_graph, string start) {

    vector<pair<string, string>> eulerian_path;
    set<string> cyclenodes;
    int total_egde = eulerian_graph.edges.size();

    //while there are still unaccounted edge
    while (eulerian_path.size() < total_egde) {
        //while there are available outlet
        while (eulerian_graph.outlet[start] > 0) {
            //cycle through the deBruijin set of start
            for (auto const& node: eulerian_graph.deBruijin[start]) {
                //add this edge to the path
                eulerian_path.push_back({start,node});
                cyclenodes.insert(start);
                //delete it so we won't do it twice
                eulerian_graph.deleteedge(start, node);
                //now start at the new node, loop again
                start = node;
                //break the for loop to go back to the while loop
                break;
                
            }
        }
        //if we are here, we are stuck since no outlet available
        //to find new start, search in our cyclenodes list
        for (auto const& node: cyclenodes) {
            //if there exist a node with outlet count other than 0
            if (eulerian_graph.outlet[node] != 0) {
                //set new start at that node
                start = node;
                //rearrange the path to get to the new start again
                eulerian_path = retravelpath(eulerian_path, node);
                //break out of for loop
                break;
            }
        }
        //here is the end of the algorithm, everything should work!
    }
    //output
    return eulerian_path;
}

/**
 * 
 * 
 * 
 */
string eulerian_path(graph eulerian_graph) {
    // the eulerian path in the vector of string pairs form
    vector<pair<string, string>> eulerian_path;
    // the sink and source where we start and finish
    string sink, source;
    // the placeholder for output
    string output = "";

    //find sink and source
    sink = "";
    source = "";
    findsinksource(eulerian_graph, sink, source);

    //in case no sink or source can be found, fall back into eulerian_cycle
    if ((sink =="") || (source =="")) {
        set<string>::iterator cycle_it = eulerian_graph.nodelist.begin();
        //cout << "\nNode chosen as start: " << *cycle_it << "\n";
        eulerian_path = eulerian_cycle(eulerian_graph, *cycle_it);
        //eulerian_path.pop_back();
        output = output_process(eulerian_path);
        return output;
    }
    //add an edge between sink and source
    eulerian_graph.addedge(sink, source);
    eulerian_path = eulerian_cycle(eulerian_graph, source);
    //output
    //rotate to the extra edge we added
    while ((eulerian_path.back().first != sink) || (eulerian_path.back().second != source)) {
            eulerian_path.insert(eulerian_path.begin(), eulerian_path.back());
            eulerian_path.pop_back();
        }
    //get rid of it
    eulerian_path.pop_back();

    //output
    output = output_process(eulerian_path);

    return output;
}

/**
 * Creates a de Bruijn graph from a colletion of k-mers
 *
 * \param patterns      a vector of strings containing the k-mer colleciton
 *                      to be made into a de Bruijn graph
 *
 * \return a string containing an adjacency list representation of the
 * de Bruijn graph as described in the problem specification
 */
graph inputProcess(const vector<string>& patterns) {
    vector<string> output;
    map<string, vector<string>> deBruijinGraph;
    graph eulerian_graph;

    //construct map
    //for each line of input
    for (int i = 0; i < patterns.size(); i++) {
        //edge = a substring of the input string minus the last char
        string edge = patterns[i].substr(0, patterns[i].length() - 1);
        //add the substring of the input string minus the first char as the outlet
        deBruijinGraph[edge].push_back(patterns[i].substr(1, patterns[i].length() - 1));
    }


    // loop through the deBruijin graph
    for (auto const& node: deBruijinGraph) {
        for (int i = 0; i < node.second.size(); i++) {
            //do actual work here
            string origin = node.first, destination = node.second[i];

            // append origin to the nodelist
            if (eulerian_graph.nodelist.count(origin) != 1) {
                eulerian_graph.nodelist.insert(origin);
            }

            // append destination to the nodelist
            if (eulerian_graph.nodelist.count(destination) != 1) {
                eulerian_graph.nodelist.insert(destination);
            }
            // finalize the edge and store it into the edge list
            eulerian_graph.edges.insert({origin, destination});
            // add the edge to the deBruijin Graph
            eulerian_graph.deBruijin[origin].insert(destination);
            // increase the outlet count of origin
            eulerian_graph.outlet[origin]++;
            // check if destination is a registered node, if not then initialize it
            if (eulerian_graph.inlet.count(destination) == 0) {
                eulerian_graph.inlet[destination] = 1;
            }
            // else we increase its inlet by one
            else {
                eulerian_graph.inlet[destination]++;
            }

            /*------CHECKS - TO BE DELETED------*/
            //cout << node.first << "->" << node.second[i] << "\n";
        }
    }
    
    // fill in the zeros in inlet list
    for (auto const& node: eulerian_graph.nodelist) {
        if (eulerian_graph.inlet.count(node) == 0) {
            eulerian_graph.inlet[node] = 0;
        }
    }
    // fill in the zeros in outlet list
    for (auto const& node: eulerian_graph.nodelist) {
        if (eulerian_graph.outlet.count(node) == 0) {
            eulerian_graph.outlet[node] = 0;
        }
    }
    // define the size of graph
    eulerian_graph.N = eulerian_graph.nodelist.size();


    return eulerian_graph;
}


/*
 * Reconstructs a string from its k-mer composition
 *
 * \param k         the length of the k-mers in the composition
 * \param patterns  a vector of strings containing the k-mer composition
 *
 * \return a string text with k-mer composition equal to patterns
 */


string reconstruct(int k, const vector<string>& patterns) {
    // TODO: your code here
    string output = "";
    output = eulerian_path(inputProcess(patterns));

    return output;
}

int main(int argc, char* argv[]) {
    int k;
    cin >> k;

    vector<string> patterns;
    string pattern;

    while (cin >> pattern) {
        patterns.push_back(pattern);
    }

    auto start = high_resolution_clock::now(); 
    cout << reconstruct(k, patterns) << endl;
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<seconds>(stop - start); 
    if (duration.count() > 0) cout << duration.count() << endl;

}
