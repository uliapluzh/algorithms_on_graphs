#define _CRT_SECURE_NO_WARNINGS
#include "myGraph.h"
#include <locale.h>
#include "helpfunc.h"


int main()
{
	setlocale(LC_ALL, "Rus");

	int choice, q, v1, v2, minMax, chooseMatrix;
	myGraph gr = createGraph();

	do
	{
		menu();
		choice = getinteger();

		switch (choice)
		{
		case 1:
			minMax = getMinMax();
			q = getE(gr.ver);
			gr.Shimbell_method(q, minMax);
			break;
		case 2:
			v1 = getVer(gr.ver, 1);
			v2 = getVer(gr.ver, 2);
			gr.MatrixDostizh(v1, v2);
			break;
		case 3:
			gr.printMatrixSmezh();
			break;
		case 4:
			gr.printMatrixPosWeights();
			break;
		case 5:
			gr.printMatrixMixWeights();
			break;
		case 6:
			v1 = getVer(gr.ver, 1);
			v2 = getVer(gr.ver, 2);
			gr.printMatrixPosWeights();
			gr.Dijkstra(v1 - 1, v2 - 1);
			break;
		case 7:
			chooseMatrix = getMatrix();
			v1 = getVer(gr.ver, 1);
			v2 = getVer(gr.ver, 2);
			if(chooseMatrix) gr.printMatrixMixWeights();
			else gr.printMatrixPosWeights();
			gr.Floyd_Warshall(v1, v2, chooseMatrix);
			break;
		case 8:
			chooseMatrix = getMatrix();
			v1 = getVer(gr.ver, 1);
			v2 = getVer(gr.ver, 2);
			if (chooseMatrix) gr.printMatrixMixWeights();
			else gr.printMatrixPosWeights();
			gr.findMaxWay(v1 - 1, v2 - 1, chooseMatrix);
			break;
		case 9:
			gr.completeToNetwork();
			gr.minCostFlow();
			break;
		case 10:
			gr.Kirchhoff();
			break;
		case 11:
			chooseMatrix = getMatrix();
			gr.Kraskal(chooseMatrix);
			break;
		case 12:
			chooseMatrix = getMatrix();
			gr.Boruvka(chooseMatrix);
			break;
		case 13:
			gr.PreuferCode();
			break;
		case 14:
			gr.Eulerian_graph();
			break;
		case 15:
			chooseMatrix = getMatrix();
			gr.Hamiltonian_graph(chooseMatrix);
			break;
		case 16:
			gr = createGraph();
			break;

		default:
			break;
		}

		if (choice != 17)
		{
			cout << endl;
			system("pause");
		}

	} while (choice != 17);


	return 0;
}
