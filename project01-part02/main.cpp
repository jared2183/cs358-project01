/* main.cpp */

//
// Parallelizes a generic "work graph" where work is randomly
// distributed across the vertices in the graph. Naive 
// parallelization works, but doesn't scale. A much more 
// dynamic solution is needed.
// 
// Usage:
//   work [-?] [-t NumThreads]
//
// Author:
//   Jared Yang
//   Northwestern University
// 
// Initial template:
//   Prof. Joe Hummel
//   Northwestern University
//

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <chrono>
#include <random>
#include <sys/sysinfo.h>
#include <omp.h>

#include "workgraph.h"

using namespace std;


//
// Globals:
//
static int _numThreads = 1;  // default to sequential execution

//
// Function prototypes:
//
static void ProcessCmdLineArgs(int argc, char* argv[]);


//
// main:
//
int main(int argc, char *argv[])
{
	cout << "** Work Graph Application **" << endl;
	cout << endl;

	//
	// Set defaults, process environment & cmd-line args:
	//
	ProcessCmdLineArgs(argc, argv);

	WorkGraph wg;  // NOTE: wg MUST be created in sequential code

	cout << "Graph size:   " << wg.num_vertices() << " vertices" << endl;
	cout << "Start vertex: " << wg.start_vertex() << endl;
	cout << "# of threads: " << _numThreads << endl;
	cout << endl;

	cout << "working";
	cout.flush();

  auto start = chrono::high_resolution_clock::now();


	//
	// TODO: solve all the vertices in the graph. This code just
	// solves the start vertex.
	//
	queue<int> q;
	q.push(wg.start_vertex());

	unordered_map<int, bool> seen;
	seen[wg.start_vertex()] = true;
	
	// bfs
	while (!q.empty()) {
		// pops from queue
		int vtx = q.front();
		q.pop();

		// visit
		wg.do_work(vtx);

		// add neighbors to queue if not seen
		for (int nbor : wg.neighbors(vtx)) {
			if (seen.find(nbor) == seen.end()) {
				seen[nbor] = true;  // Mark as seen immediately after pushing
				q.push(nbor);
			}
		}
	}

	auto stop = chrono::high_resolution_clock::now();
	auto diff = stop - start;
	auto duration = chrono::duration_cast<chrono::milliseconds>(diff);

	cout << endl;
	cout << endl;

	cout << endl;
	cout << "** Done!  Time: " << duration.count() / 1000.0 << " secs" << endl;
	cout << "** Execution complete **" << endl;
  	cout << endl;

	return 0;
}


//
// processCmdLineArgs:
//
static void ProcessCmdLineArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{

		if (strcmp(argv[i], "-?") == 0)  // help:
		{
			cout << "**Usage: work [-?] [-t NumThreads]" << endl << endl;
			exit(0);
		}
		else if ((strcmp(argv[i], "-t") == 0) && (i+1 < argc))  // # of threads:
		{
			i++;
			_numThreads = atoi(argv[i]);
		}
		else  // error: unknown arg
		{
			cout << "**Unknown argument: '" << argv[i] << "'" << endl;
			cout << "**Usage: work [-?] [-t NumThreads]" << endl << endl;
			exit(0);
		}

	}//for
}
