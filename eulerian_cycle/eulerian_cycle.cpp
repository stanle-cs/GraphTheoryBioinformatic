#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::size_t;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::getline;
using std::map;
using std::pair;

/**
 * Retravel a Eulerian path and get to the designated position start
 * 
 * \param path		a vector of pair string containing the edge of the path
 * \param start		where the new start of the path should be
 * 
 * \return a new path that started at the start position designated
 */
  
vector<pair<string, string>> retravelpath(vector<pair<string,string>> path, int start) {
	vector<pair<string, string>> output;
	for (int i = path.size() - 1; i >= start; i--) {
		path.insert(path.begin(),path.back());
		path.erase(path.end());
	}

	return path;;
}

/**
 * Finds an Eulerian cycle in the given graph
 *
 * \param graph     a vector of strings containing edges in the graph;
 *                  one string may correspond to multiple edges if it
 *                  is of the form "X -> Y,Z"
 *
 * \return a string containing an Eulerian cycle through the given graph
 */
string eulerian_cycle(const vector<string>& graph) {
    
    
    map<string, vector<string>>     deMap;
    map<string, int>                outlets;
    map<string, map<string, bool>>  walked;
    vector<pair<string, string>>    eulerianpath;
	vector<string>                  keys;
    string                          output = "";
	string							original_start = "";
    int                             c = 0;


	//String processing function - a mess but it works!
    for (int i = 0; i < graph.size(); i++) {

		//placeholder string used for input processing
		string temp = graph[i];

		//add "," at the end of input for uniform
		temp += ",";
		
		//begin input processing
		size_t found = temp.find(" -> ", 0);
		string node = temp.substr(0, found);
		outlets[node] = 0; 
		temp.erase(0, found + 4);
		while (1) {
			size_t comma = temp.find(",", 0);
			if (comma == -1) break;
			deMap[node].push_back(temp.substr(0, comma));
			temp.erase(0, comma + 1);
			outlets[node]++;
		}
    }

    //initiate the walked map: no path has been walked
	//also initiate a keys string vector to keep track of nodes
    
    for (auto const& dict: deMap) {
		for (int i = 0; i < dict.second.size(); i++) {
			walked[dict.first][dict.second[i]] = false;
			c++;
			keys.push_back(dict.first);
		}
    }

	string start = keys[0];
	original_start = start;

    /** How the algorithm is going to work:
	 * - start from keys[0], choose its first available edge
	 * - walk to its destination. the starting node has less outlet now because 1 is used.
	 * - now we repeat until we see a destination with no outlet
	 * - then we rotate the path to get to a node that has free outlet
	 * - repeat until we hit all the edge, that is, c = 0
	 * */
    //while there are edges not accounted for in the graph
	while (c > 0) {
        
		//while there are outlets at start, meaning not stuck
		while (outlets[start] > 0) {

			//here we cycle through all outlets connected to start
			for (int i = 0; i < deMap[start].size(); i++) {

				//the first outlet that has not been touched will be used
				if (walked[start][deMap[start][i]] == false) {	
                    
                    string destination = deMap[start][i];
					
                    //here we reduce the amount of outlets available at start node by 1 since we used it
                    outlets[start]--;

                    //and decrease the total amount of edges availalbe by one
					c--;

                    //here we mark the edge we used so we won't walk it again
                    walked[start][destination] = true;

					//herer we add the new edge to the path
                    eulerianpath.push_back({start, destination});

                    //here we do everything again but now with the new destination as the start
					start = destination;

                    //break the cycling of outlets since we found our next node already
					break;
				}
			}
		}
        
        //here we are stuck, we finished one cycle

		//now we look for that node in our current eulerianpath that still have an outlet
		int i = 0;
        while ((outlets[start] == 0) && (i < eulerianpath.size())) {
            //we cycle through them all until we find one that actually have an unused outlet
            start = eulerianpath[i].first;
			i++;
        }
		
		//if we can't find it anywhere, then just give up, we are done! (hopefully)
		if (i > eulerianpath.size()) {
			break;
		}
		//else, that means that we found a good new start instead
		else {
			//here we retravel the path to get to that good new start again
			eulerianpath = retravelpath(eulerianpath, i - 1);
		}
        //here we restart the algorithm until we touched all edges
	}

	//finish!, now we rotate the path to the original start, this is a purely cosmetic and personal choice
	for (int i = 0; i < eulerianpath.size(); i++) {
			if (eulerianpath[i].first == original_start) {
					eulerianpath = retravelpath(eulerianpath, i);
					break;
			}
	}

	//output
	for (int i = 0; i < eulerianpath.size(); i++) {
		output += eulerianpath[i].first + "->";
	}
	output += eulerianpath.back().second;

    return output;
}


int main(int argc, char* argv[]) {
    vector<string> graph;

    string line;
    while (getline(cin, line, '\n')) {
        graph.push_back(line);
    }

    cout << eulerian_cycle(graph) << endl;
	return 0;
}


