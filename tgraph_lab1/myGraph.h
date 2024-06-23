#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

const int Inf = 100000;

class myGraph
{
private:
	vector<vector<int>> matrixSmezh;
	vector<vector<int>> matrixSmezhNetwork;
	vector<vector<int>> matrixPosWeight;
	vector<vector<int>> matrixPosWeightNetwork;
	vector<vector<int>> matrixMixWeight;
	vector<vector<int>> matrixMixWeightNetwork;
	vector<vector<int>> matrixCosts;
	vector<vector<int>> matrixSmezhNotOrgr;
	vector<vector<int>> matrixPosWeightNotOrgr;
	vector<vector<int>> matrixMixWeightNotOrgr;
	vector<vector<int>> minOstov;
	vector<int> distrErlang();
	void addPosWeights(vector<vector<int>> matrixSm, vector<vector<int>> &matrix);
	void addMixWeights(vector<vector<int>> matrixSm, vector<vector<int>> &matrix);
	vector<vector<int>> shim_alg(vector<vector<int>> res, int minMax);
	vector<vector<int>> sumMatrix(vector<vector<int>> a, vector<vector<int>> b);
	vector<vector<int>> productMatrix(vector<vector<int>> a, vector<vector<int>> b);
	bool bfs(vector<vector<int>> bandwidth, vector<int>& preVer);
	vector<int> DijkstraWithNeg(int v1, int v2, vector<vector<int>> costs);
	int findDet(vector<vector<int>>& minor, int n);
	vector<vector<int>> PreuferDecode(vector<pair<int, int>> preufer);
	void findEulerCycle(vector<vector<int>> matrixEuler, vector<int> degreesVer);
	void findHamiltonianCycles(ofstream& file, vector<vector<int>> & matrixWeights, vector<int> &path, vector<int> &minPath, int lenght, int &minLenght) const;

public:
	int ver;
	myGraph(int n);
	void completeToNetwork();
	void makeNotOrgraph();
	void printMatrixSmezh();
	void printMatrix(vector<vector<int>> matrix, string str);
	void printMatrixMixWeights();
	void printMatrixPosWeights();
	void Shimbell_method(int e, int minMax);
	void MatrixDostizh(int v1, int v2);
	void Dijkstra(int v1, int v2);
	void Floyd_Warshall(int v1, int v2, int matrix);
	void findMaxWay(int v1, int v2, int matrix);
	int Ford_Fulkerson();
	void minCostFlow();
	void Kirchhoff();
	void Kraskal(int matrix);
	void Boruvka(int matrix);
	void PreuferCode();
	void Eulerian_graph();
	void Hamiltonian_graph(int matrix);
};

