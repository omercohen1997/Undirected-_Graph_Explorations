#include<iostream>
#include <list>
#include <math.h> 
#include <time.h>
#include<fstream>
#define Vertex 1000
using namespace std;


class Graph
{
	bool visited[Vertex];
	int distance[Vertex]; //d(v)	
	list<int> arrOfList[Vertex]; // adjacency list
public:
	Graph();
	int getVertex()const; 
	void addEdge(int, int);
	int *BFS(int startVertex); // BFS used for Connectivity and diam, its return Array of distances
	int isIsolated();
};

Graph::Graph() // initilize and Mark all the vertices as not visited
{
	for (int i = 0; i < Vertex; i++)
	{
		visited[i] = false;
		distance[i] = 0;
	}
}

int Graph::getVertex() const
{
	return Vertex;
}

void Graph::addEdge(int src, int dest) // add edge (there are 2 edges push_back because its undircted graph
{
	arrOfList[src].push_back(dest);
	arrOfList[dest].push_back(src);
}

int *Graph::BFS(const int startVertex)
{
	
	list<int> queue;    // Create a queue for BFS

	// Mark the current node as visited and enqueue it
	visited[startVertex] = true;
	queue.push_back(startVertex);
	list<int>::iterator i;      // 'i' will be used to get all adjacent vertices of a vertex

	while (!queue.empty()) 
	{
		int currVertex = queue.front();   // Dequeue a vertex from queue and print it
		queue.pop_front();

		/* Get all adjacent vertices of the dequeued
		 vertex currVertex. If a adjacent has not been visited,        
		then mark it visited and enqueue it
		*/

		for (i = arrOfList[currVertex].begin(); i != arrOfList[currVertex].end(); ++i) // This loop passes over the neighbors of the vertex in the list array we are on
		{
			int adjVertex = *i;    

			if (!visited[adjVertex])// check if the vertex visited already, if not than make it visited and enqueue into the queue
			{
				distance[adjVertex] = distance[currVertex] + 1;  //  the array distance 
				visited[adjVertex] = true;
				queue.push_back(adjVertex);
			}
		}
	}
	return distance;
}


int diameter(Graph &g) //  ---- Ex 2=> return diam of graph	
{
	int *arr, amountOfVertex, diam = 0, i, j;
	amountOfVertex = g.getVertex();

	if (amountOfVertex == 1) // if the graph has only one vertex than diam=0
		return 0;
	else
	{
		for (i = 0; i < amountOfVertex; i++)
		{
			arr = g.BFS(i); //BFS for every vertex in order to find the diam

			for (j = 0; j < amountOfVertex; j++)
			{
				if (i != j && arr[i] == 0 && arr[j] == 0) // checkin if the graph is connected. if not than no point to continue therefore no diam
					return -1;
				diam = (diam < arr[j]) ? diam = arr[j] : diam;  // change the diam to the bigger on if needed
			}
			j = 0;
		}
		return diam;
	}
}

int connectivity(Graph &g) // --> checking connectivity of graph. 1 for true, 0 for false.
{
	int *arr, amountOfVertex;

	amountOfVertex = g.getVertex();
	arr = g.BFS(0); // start the bfs from vertex 0 (it doesnt matter which one)
	if (amountOfVertex == 1) // if no.of.nill=1 graph is connect
		return 1;
	else
	{
		for (int i = 1; i < amountOfVertex; i++)
		{
			if (arr[i] == 0) // if there is another vertex except the one with started with the bfs that his distance = 0 => graph isn't connect
				return 0;
		}
	}
	return 1;
}

int Graph::isIsolated() // checking if we have at least one vertex without neighbors than return 1 , else return 0
{
	for (int i = 0; i < getVertex(); i++)
	{
		if (arrOfList[i].empty() == 1)
		{
			return 1;
		}
	}
	return 0;
}

Graph build_random_graph(int v, double p)// Ex1--> building random graph depends on random p
{
	int i, j;
	Graph gr;
	for (i = 0; i < v; i++)
	{
		for (j = i + 1; j < v; j++)
		{
			if ((double)rand() / RAND_MAX <= p)
				gr.addEdge(i, j); // Add edge
		}
	}
	return gr;
}


void WriteIntoCsv(int attribute, double *ArrProbability, int *ArrAttribute)	 // Write into the csv the simulations results
{
	int i;
	ofstream myFile;
	myFile.open("Graphs simulations.csv", ios::app); // open and giving the file name.  app mode means it won't run over the details we already have
	switch (attribute) // switch for the attribute that sent to the function and print his probbabilities in the csv
	{
	case 1:
		myFile << "Connectivity p:,";
		for (i = 0; i < 10; i++)
			myFile << ArrProbability[i] << ",";
		myFile << endl;
		break;

	case 2:
		myFile << "Diameter p:,";
		for (i = 0; i < 10; i++)
			myFile << ArrProbability[i] << ",";
		myFile << endl;
		break;

	case 3:
		myFile << "isIsolated p:,";
		for (i = 0; i < 10; i++)
			myFile << ArrProbability[i] << ",";
		myFile << endl;
		break;
	}

	i = 0;
	myFile << "Assessment of the probability:,"; 
	for (i = 0; i < 10; i++)   // print into the csv the results/500 
		myFile << ArrAttribute[i] / 500.0 << ",";
	myFile << endl << endl;
	myFile.close();
}

int main()
{
	srand((unsigned int)time(NULL));
	int  i, j;
	Graph graph;

	// creathe three arrays that contains 5 numbers above the threshold and 5 under it.
	double arrT1[10] = { 0.002,0.003,0.004,0.005,0.006,0.007,0.008,0.009,0.01,0.02 }; // Threshold1 = 0.006907755279

	double arrT2[10] = { 0.010,0.023 ,0.045 ,0.070 ,0.095 ,0.113 ,0.13 ,0.15 ,0.21 ,0.30 }; // Threshold2 = 0.1175394

	double arrT3[10] = { 0.002,0.0035,0.0045,0.0055,0.0065,0.0075,0.0085,0.0095,0.02,0.03 }; // Threshold2 =  0.006907755279

	cout << "------------------- Start of arrtitube 1 :Connectivity-------------------" << endl;
	int connect, arr_countT1[10] = { 0 }, countTotalConnect = 0;
	for (i = 0; i < 10; i++)// ----- For arrtitube 1: Connectivity
	{
		for (j = 0; j < 500; j++)
		{
			graph = build_random_graph(Vertex, arrT1[i]);
			connect = connectivity(graph);
			arr_countT1[i] += connect; // array that count for each p how much are connected 
		}
		cout << "P: " << arrT1[i] << " - " << arr_countT1[i] << " out of 500 are connected" << endl;
		countTotalConnect += arr_countT1[i]; // count total connected of 5000

	}
	cout << "Total amount of connected Graphs out of 5000: " << countTotalConnect << endl;
	cout << "------------------- End Connectivity-------------------" << endl;
	cout << endl;
	WriteIntoCsv(1, arrT1, arr_countT1);


	cout << "------------------- Start of arrtitube 2 :Diameter-------------------" << endl;
	int diam = 0, arr_countT2[10] = { 0 }, countTotalDiam = 0, sumDiam = 0;
	for (i = 0; i < 10; i++) // ---- ------ arrtitube 2 : Diameter
	{
		for (j = 0; j < 500; j++)
		{
			graph = build_random_graph(Vertex, arrT2[i]);
			if (connectivity(graph) == 1) // before checking the diam this if checking if that graph is even connect
			{
				if (diameter(graph) <= 2)
					arr_countT2[i]++;  //array that count for each p how much diameter <=2
			}
		}
		cout << "P: " << arrT2[i] << " - " << arr_countT2[i] << " out of 500 are <= 2" << endl;
		countTotalDiam += arr_countT2[i];  // count total diameters  of 5000 that <=2

	}
	cout << "Total amount of diamater out of 5000: " << countTotalDiam << endl;
	cout << "------------------- End Diameter-------------------" << endl;
	cout << endl;
	WriteIntoCsv(2, arrT2, arr_countT2);


	cout << "------------------- Start of arrtitube 3 :isIsolated-------------------" << endl;
	int arr_countT3[10] = { 0 }, countTotalisIsolated = 0, sumisIsolated = 0;
	for (i = 0; i < 10; i++) // ---- ------ arrtitube 3 : IsIsolated
	{
		for (j = 0; j < 500; j++)
		{
			graph = build_random_graph(Vertex, arrT3[i]);
			sumisIsolated = graph.isIsolated();
			if (sumisIsolated == 1)
				arr_countT3[i] += sumisIsolated;  // array that count for each p how much are is isolated 
		}

		cout << "p: " << arrT3[i] << " - " << arr_countT3[i] << " out of 500 are Isolated" << endl;
		countTotalisIsolated += arr_countT3[i]; // count total is isolated of 5000 
	}

	cout << "Total amount of Isolated Graphs out of 5000: " << countTotalisIsolated << endl;
	cout << "------------------- End isIsolated-------------------" << endl;
	WriteIntoCsv(3, arrT3, arr_countT3);

}



