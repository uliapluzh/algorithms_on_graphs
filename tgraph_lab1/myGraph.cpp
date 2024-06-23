#include "myGraph.h"
#include <random>
#include <math.h>
#include <algorithm>
#include <iomanip> 
#include <fstream> 

myGraph::myGraph(int n)
{
	ver = n;
    vector<int> degrees = distrErlang();
    matrixSmezh.resize(ver, vector<int>(ver, 0));

    auto zero_degree = find(degrees.begin(), degrees.end(), 0);
    auto max_degree = max_element(degrees.begin(), degrees.end());
    double max = *max_degree;


    if (max > ver - 1 || zero_degree != degrees.end()) //нормализация
    {
        if (ver > 2) max = max / (ver - 2);

        for (int k = 0; k < degrees.size(); k++)
        {
            degrees[k] = static_cast<int>(degrees[k] / max + 1);
            //cout << degrees[k] << " ";
        }
    }

    for (int i = 0; i < ver; i++)
    {
        int available = ver - i - 1;
        vector<int> indices;

        for (int j = i + 1; j < ver; j++)
        {
            indices.push_back(j);
        }

        random_shuffle(indices.begin(), indices.end());

        for (int j = 0; j < degrees[i] && j < available; ++j)
        {
            int col = indices[j];
            matrixSmezh[i][col] = 1;
        }
    }

    addPosWeights(matrixSmezh, matrixPosWeight);
    addMixWeights(matrixSmezh, matrixMixWeight);
    printMatrix(matrixSmezh, "Матрица смежности");
    makeNotOrgraph();
}

void myGraph::completeToNetwork() 
{
    matrixSmezhNetwork = matrixSmezh;

    for (int i = 1; i < ver; i++) 
    {
        bool verConnected = false; 
        // проверяем, связана ли вершина i с предыдущими вершинами
        for (int j = 0; j < ver; j++) 
        {
            if (matrixSmezh[j][i] == 1) 
            {
                verConnected = true;
                break;
            }
        }
        // если вершина i не связана с предыдущими вершинами, добавляем ребро из предыдущей вершины
        if (!verConnected) 
        {
            matrixSmezhNetwork[i - 1][i] = 1;
        }
    }

    printMatrix(matrixSmezhNetwork, "Достроили до сети. Матрица смежности.");
    addPosWeights(matrixSmezhNetwork, matrixPosWeightNetwork);
    addMixWeights(matrixSmezhNetwork, matrixMixWeightNetwork);
    printMatrix(matrixPosWeightNetwork, "Матрица пропускных способностей");

}

void myGraph::makeNotOrgraph()
{
    matrixSmezhNotOrgr = matrixSmezh;

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (matrixSmezh[i][j] == 1)
                matrixSmezhNotOrgr[j][i] = 1;
        }
    }

    matrixPosWeightNotOrgr.resize(ver, vector<int>(ver, 0));

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (matrixSmezh[i][j] == 1)
            {
                int random = rand() % 15 + 1;
                matrixPosWeightNotOrgr[i][j] = random;
                matrixPosWeightNotOrgr[j][i] = random;
            }
        }
    }

    matrixMixWeightNotOrgr.resize(ver, vector<int>(ver, 0));

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (matrixSmezh[i][j] == 1)
            {
                int randomWeight;
                do
                {
                    randomWeight = rand() % 30 - 15;
                } while (randomWeight == 0);
                matrixMixWeightNotOrgr[i][j] = randomWeight;
                matrixMixWeightNotOrgr[j][i] = randomWeight;
            }
        }
    }
}


vector<int> myGraph::distrErlang()
{
    double m = 5.0; //параметр формы (порядок распределения)
    double lymbda = 0.5; //параметр масштаба
    //vector<int> r(100, 0);

    random_device rd;
    mt19937 gen(rd());
    vector<int> degrees;

    gamma_distribution<double> distribution(m, lymbda);
    mt19937 generator(random_device{}());

    for (int i = 0; i < ver; ++i)
    {
        degrees.push_back(static_cast<int>(distribution(generator)));
        //r[degrees[i]]++;
    }

   /* cout << "Степени вершин" << endl;
    for (int i = 0; i < degrees.size(); i++)
    {
        cout << degrees[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < r.size(); i++)
        cout << i << ": " << r[i] << endl;*/


    return degrees;
}


void myGraph::printMatrixSmezh()
{
    printf("\n");
    cout << "Матрица смежности" << endl;
    cout << "-----------------" << endl;
    cout << setw(4);
    for (int j = 0; j < ver; ++j)
    {
        cout << "| " << setw(2) << j + 1;
    }
    cout << "|" << endl;

    for (int i = 0; i < ver; ++i)
    {
        cout << setw(2) << i + 1 << "|";
        for (int j = 0; j < ver; ++j)
        {
            cout << setw(3) << matrixSmezh[i][j] << " ";
        }
        cout << endl;
    }
}

void myGraph::addPosWeights(vector<vector<int>> matrixSm, vector<vector<int>> &matrix)
{
    matrix.resize(ver, vector<int>(ver, 0));

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (matrixSm[i][j] == 1)
            {
                matrix[i][j] = rand() % 15 + 1;
            }
        }
    }
}

void myGraph::addMixWeights(vector<vector<int>> matrixSm, vector<vector<int>> &matrix)
{
    matrix.resize(ver, vector<int>(ver, 0));

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (matrixSm[i][j] == 1)
            {
                int randomWeight;
                do 
                {
                    randomWeight = rand() % 30 - 15;
                } while (randomWeight == 0);
                matrix[i][j] = randomWeight;
            }
        }
    }

    matrixCosts = matrix;

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (matrix[i][j] == 0)
                matrixCosts[i][j] = Inf;
        }
    }
}


void myGraph:: printMatrix(vector<vector<int>> matrix, string str) 
{
    cout << endl;
    cout << str << endl;
    cout << "------------------------------------" << endl;
    cout << setw(4);
    int v = matrix.size();
    for (int j = 0; j < v; ++j) 
    {
        cout << "| " << setw(2) << j + 1;
    }
    cout << "|" << endl;

    for (int i = 0; i < v; ++i) 
    {
        cout << setw(2) << i + 1 << "|";
        for (int j = 0; j < v; ++j)
        {
            cout << setw(3) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

void myGraph::printMatrixPosWeights()
{
    cout << "Матрица положительных весов" << endl;
    cout << "---------------------------" << endl;

    cout << setw(4);
    for (int j = 0; j < ver; ++j)
    {
        cout << "| " << setw(2) << j + 1;
    }
    cout << "|" << endl;

    for (int i = 0; i < ver; ++i)
    {
        cout << setw(2) << i + 1 << "|";
        for (int j = 0; j < ver; ++j)
        {
            cout << setw(3) << matrixPosWeight[i][j] << " ";
        }
        cout << endl;
    }
}

void myGraph::printMatrixMixWeights()
{
    cout << "Матрица c отрицательными весами" << endl;
    cout << "-------------------------------" << endl;

    cout << setw(4);
    for (int j = 0; j < ver; ++j)
    {
        cout << "| " << setw(2) << j + 1;
    }
    cout << "|" << endl;

    for (int i = 0; i < ver; ++i)
    {
        cout << setw(2) << i + 1 << "|";
        for (int j = 0; j < ver; ++j)
        {
            cout << setw(3) << matrixMixWeight[i][j] << " ";
        }
        cout << endl;
    }
}


vector<vector<int>> myGraph::shim_alg(vector<vector<int>> res, int minMax)
{
    vector<vector<int>> shimMatrix(ver, vector<int>(ver, 0));

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (minMax) shimMatrix[i][j] = 0;
            else shimMatrix[i][j] = 1000000;
            bool flag = 0;

            for (int k = 0; k < ver; k++)
            {
                int sum = 0;
                if (res[i][k] != 0 && matrixPosWeight[k][j] != 0)
                {
                    sum = res[i][k] + matrixPosWeight[k][j];
                    flag = 1;

                    if (minMax) shimMatrix[i][j] = max(shimMatrix[i][j], sum);
                    else shimMatrix[i][j] = min(shimMatrix[i][j], sum);
                }
            }
            if (flag == 0)
                shimMatrix[i][j] = 0;
        }
    }

    return shimMatrix;
}


void myGraph::Shimbell_method(int e, int minMax)
{
    vector<vector<int>> res = matrixPosWeight;
    printMatrix(matrixPosWeight, "Матрица положительных весов");
    
    if (e > 1)
    {
        for (int i = 0; i < e - 1; i++)
        {
            res = shim_alg(res, minMax);
        }
    }

    printMatrix(res, "Матрица Шимбелла");
}

vector<vector<int>> myGraph:: sumMatrix(vector<vector<int>> a, vector<vector<int>> b)
{
    vector<vector<int>> res(ver, vector<int>(ver, 0));

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            res[i][j] = a[i][j] + b[i][j];
        }
    }

    return res;
}

vector<vector<int>> myGraph::productMatrix(vector<vector<int>> a, vector<vector<int>> b)
{
    vector<vector<int>> res(ver, vector<int>(ver, 0));

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            for (int k = 0; k < ver; k++)
            {
                res[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return res;
}

void myGraph::MatrixDostizh(int v1, int v2)
{
    vector<vector<int>> matrixDost = matrixSmezh;
    vector<vector<int>> promMatrix = matrixSmezh;


    for (int i = 0; i < ver - 2; i++)
    {
        promMatrix = productMatrix(promMatrix, matrixSmezh);
        matrixDost = sumMatrix(matrixDost, promMatrix);
    }

    for (int i = 0; i < ver; i++)
    {
        matrixDost[i][i] = 1;
    }

    printMatrix(matrixDost, "Матрица достижимости");

    if (matrixDost[v1 - 1][v2 - 1] == 0) cout << "Маршрута между данными вершинами не существует" << endl;
    else cout << "Кол-во маршрутов между вершинами " << v1 << " и " << v2 << " равно " << matrixDost[v1 - 1][v2- 1] << endl;

}

void myGraph::Dijkstra(int v1, int v2)
{ 
    int count = 0;
    vector<int> d(ver, Inf);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    vector<int> preVer(ver, -1);

    d[v1] = 0;
    q.push(make_pair(0, v1));

    while (!q.empty())
    {
        int distance = q.top().first;
        int v = q.top().second;
        q.pop();

        if (distance > d[v]) continue;

        for (int i = 0; i < ver; i++)
        {
            if (matrixPosWeight[v][i] != 0)
            {
                if (d[i] > d[v] + matrixPosWeight[v][i])
                {
                    d[i] = d[v] + matrixPosWeight[v][i];
                    preVer[i] = v;
                    q.push(make_pair(d[i], i));
                }
            }
            count++;
        }
    }
    cout << endl;

    if (d[v2] == Inf) cout << "Данного пути не существует" << endl;
    else
    {
        cout << "Кратчайшее расстояние между вершинами " << v1 + 1 << " и " << v2 + 1 << " равно " << d[v2] << endl;

        cout << "Кратчайший путь: ";
        vector<int> s;

        for (int i = v2; i != -1; i = preVer[i])
        {
            s.push_back(i);
        }
        for (int i = s.size() - 1; i >= 0; i--)
        {
            cout << s[i] + 1;
            if (i != 0) cout << "->";
        }
        cout << endl;
    }

    cout << "Количество итераций: " << count << endl;
}

void myGraph::Floyd_Warshall(int v1, int v2, int matrix) //0 - полож, 1 - смеш
{
    int count = 0;
    vector<vector<int>> pred(ver, vector<int>(ver, -1));
    vector<vector<int>> dist;
    if (matrix) dist = matrixMixWeight;
    else dist = matrixPosWeight;

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (dist[i][j] == 0 && i != j) dist[i][j] = Inf;

            if (dist[i][j] != Inf && dist[i][j] != 0) pred[i][j] = i;
        }
    }

    for (int i = 0; i < ver; i++)
    {
        for (int a = 0; a < ver; a++)
        {
            for (int b = 0; b < ver; b++)
            {
                if (dist[a][i] != Inf && dist[i][b] != Inf && dist[a][b] > dist[a][i] + dist[i][b])
                {
                    dist[a][b] = dist[a][i] + dist[i][b];
                    pred[a][b] = i;
                }
                count++;
            }
        }
    }

    printf("\n");

    cout << "Матрица предшествующих вершин " << endl;
    cout << "-----------------------------" << endl;
    cout << setw(4);
    for (int j = 0; j < ver; ++j)
    {
        cout << "| " << setw(2) << j + 1;
    }
    cout << "|" << endl;

    for (int i = 0; i < ver; ++i)
    {
        cout << setw(2) << i + 1 << "|";
        for (int j = 0; j < ver; ++j)
        {
            cout << setw(3) << pred[i][j] + 1 << " ";
        }
        cout << endl;
    }

    printf("\n");

    cout << "Матрица расстояний" << endl;
    cout << "------------------" << endl;
    cout << setw(4);
    for (int j = 0; j < ver; ++j)
    {
        cout << "| " << setw(2) << j + 1;
    }
    cout << "|" << endl;

    for (int i = 0; i < ver; ++i)
    {
        cout << setw(2) << i + 1 << "|";
        for (int j = 0; j < ver; ++j)
        {
            if (dist[i][j] == Inf) cout << setw(3) << 0 << " ";
            else cout << setw(3) << dist[i][j] << " ";
        }
        cout << endl;
    }
    printf("\n");

    if (dist[v1 - 1][v2 - 1] == Inf) cout << "Данного пути не существует" << endl;
    else
    {
        cout << "Кратчайшее расстояние между вершинами " << v1 << " и " << v2 << " равно " << dist[v1 - 1][v2 - 1] << endl;

        cout << "Кратчайший путь: ";
        vector<int> s;
        int u = v1 - 1;
        int v = v2 - 1;

        while (u != v)
        {
            s.push_back(v);
            v = pred[u][v];
        }
        s.push_back(v);
        for (int i = s.size() - 1; i >= 0; i--)
        {
            cout << s[i]+ 1;
            if (i != 0) cout << "->";
        }
        cout << endl;

    }
    
    cout << "Количество итераций: " << count << endl;
}

void myGraph::findMaxWay(int v1, int v2, int matrix)
{
    vector<vector<int>> matrixWeights;
    int count = 0;
    vector<int> d(ver, -Inf);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    vector<int> preVer(ver, -1);

    if (matrix) matrixWeights = matrixMixWeight;
    else matrixWeights = matrixPosWeight;

    d[v1] = 0;
    q.push(make_pair(0, v1));

    while (!q.empty())
    {
        int distance = q.top().first;
        int v = q.top().second;
        q.pop();

        if (distance < d[v]) continue;

        for (int i = 0; i < ver; i++)
        {
            if (matrixWeights[v][i] != 0)
            {
                if (d[i] < d[v] + matrixWeights[v][i])
                {
                    d[i] = d[v] + matrixWeights[v][i];
                    preVer[i] = v;
                    q.push(make_pair(d[i], i));
                }
            }
            count++;
        }
    }
    cout << endl;

    if (d[v2] == -Inf) cout << "Данного пути не существует" << endl;
    else
    {
        cout << "Максимальное расстояние между вершинами " << v1 + 1 << " и " << v2 + 1 << " равно " << d[v2] << endl;

        cout << "Максимальный путь: ";
        vector<int> s;

        for (int i = v2; i != -1; i = preVer[i])
        {
            s.push_back(i);
        }
        for (int i = s.size() - 1; i >= 0; i--)
        {
            cout << s[i] + 1;
            if (i != 0) cout << "->";
        }
        cout << endl;
    }

    cout << "Количество итераций: " << count << endl;
}

bool myGraph::bfs(vector<vector<int>> bandwidth, vector<int> &preVer)
{
    vector<bool> isVisited(ver, false);
    queue<int> q;

    q.push(0);
    isVisited[0] = true;
    preVer[0] = -1;
  
    while (!q.empty()) 
    {
        int u = q.front();
        q.pop();

        for (int v = 0; v < ver; v++) 
        {
            if (isVisited[v] == false && bandwidth[u][v] > 0)
            {
                if (v == ver - 1) 
                {
                    preVer[v] = u;
                    return true;
                }
                q.push(v);
                preVer[v] = u;
                isVisited[v] = true;
            }
        }
    }

    return false;
}

int myGraph::Ford_Fulkerson()
{
    vector<vector<int>> bandwidth = matrixPosWeightNetwork;
    vector<int> preVer(ver, -1);
    int max_flow = 0;
    int u;

    while (bfs(bandwidth, preVer)) 
    {
        // находим минимальную остаточную пропускную способность ребер вдоль пути, найденного BFS
        int path_flow = Inf;

        for (int v = ver - 1; v != 0; v = preVer[v])
        {
            u = preVer[v];
            path_flow = min(path_flow, bandwidth[u][v]);
        }

        // обновляем остаточные пропускные способности ребер
        for (int v = ver - 1; v != 0; v = preVer[v])
        {
            u = preVer[v];
            bandwidth[u][v] -= path_flow;
            bandwidth[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    printMatrix(bandwidth, "Матрица максимального потока");

    cout << endl << "Максимальный поток в графе равен " << max_flow << endl;

    return max_flow;

}

void myGraph::minCostFlow()
{
    printf("\n");
    cout << "Матрица стоимости " << endl;
    cout << "-----------------" << endl;
    cout << setw(4);
    for (int j = 0; j < ver; ++j)
    {
        cout << "| " << setw(2) << j + 1;
    }
    cout << "|" << endl;

    for (int i = 0; i < ver; ++i)
    {
        cout << setw(2) << i + 1 << "|";
        for (int j = 0; j < ver; ++j)
        {
            if (matrixCosts[i][j] == Inf)
                cout << setw(3) << "Inf ";
            else
                cout << setw(3) << matrixCosts[i][j] << " ";
        }
        cout << endl;
    }

    vector<vector<int>> Matrcosts = matrixCosts;
    vector<vector<int>> bandwidth = matrixPosWeightNetwork;
    int max_flow = Ford_Fulkerson();
    int flow;
    if (max_flow != 1) flow = 2 * max_flow / 3;
    else flow = 1;
    
    int minCostFlow = 0;
    cout  << "Целевой поток равен " << flow << endl;

    while (flow != 0)
    {
        vector<int> s = DijkstraWithNeg(0, ver - 1, Matrcosts);
        sort(s.begin(), s.end());

        int bottleNeck = Inf; 
        int cost = 0; 

        for (auto it = s.begin(); it != s.end() - 1; it++) 
        {
            if (bandwidth[*it][*(it + 1)] < bottleNeck)
                bottleNeck = bandwidth[*it][*(it + 1)]; 
            cout << *it + 1 << "->"; 
        }

        if (!s.empty()) 
        {
            cout << s.back() + 1 << " "; 
        }

        bottleNeck = min(bottleNeck, flow);
        cout << "На этом пути пускаем поток " << bottleNeck;

        for (auto it = s.begin(); it != s.end() - 1; it++)
        {
            bandwidth[*it][*(it + 1)] -= bottleNeck;
            cost += Matrcosts[*it][*(it + 1)];
            if (bandwidth[*it][*(it + 1)] == 0) //если пропускная способность на ребре равна 0, то деактив его
                Matrcosts[*it][*(it + 1)] = Inf;
        }
       
        cout << " со стоимостью за единицу потока " << cost << endl; //вывод стоимости потока на пути
        minCostFlow += cost;
        cout << "Итоговая стоимость: " << cost * bottleNeck << "\n"; 

        flow -= bottleNeck;
    }

    cout << endl << "Минимальная стоимость потока равна " << minCostFlow << endl;

}

vector<int> myGraph::DijkstraWithNeg(int v1, int v2, vector<vector<int>> costs)
{
    vector<int> d(ver, Inf);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
    vector<int> preVer(ver, -1);

    d[v1] = 0;
    q.push(make_pair(0, v1));

    while (!q.empty())
    {
        int distance = q.top().first;
        int v = q.top().second;
        q.pop();

        if (distance > d[v]) continue;

        for (int i = 0; i < ver; i++)
        {
            if (costs[v][i] != Inf)
            {
                if (d[i] > d[v] + costs[v][i])
                {
                    d[i] = d[v] + costs[v][i];
                    preVer[i] = v;
                    q.push(make_pair(d[i], i));
                }
            }
        }
    }
    vector<int> s;
    for (int i = v2; i != -1; i = preVer[i])
    {
        s.push_back(i);
    }

    return s;
}

int myGraph::findDet(vector<vector<int>>& minor, int n)
{
    if (n == 1)
        return minor[0][0];

    else if (n == 2)
        return minor[0][0] * minor[1][1] - minor[1][0] * minor[0][1];

    else
    {
        int det = 0;

        for (int k = 0; k < n; k++)
        {
            vector<vector<int>> m(n - 1, vector<int>(n - 1, 0));

            for (int i = 1; i < n; i++)
            {
                int t = 0;

                for (int j = 0; j < n; j++)
                {
                    if (j == k)
                        continue;

                    m[i - 1][t] = minor[i][j];
                    t++;
                }
            }
            det += pow(-1, k) * minor[0][k] * findDet(m, n - 1);
        }

        return det;
    }
}


void myGraph::Kirchhoff()
{
    printMatrix(matrixSmezhNotOrgr, "Неориентированный граф. Матрица смежности.");
    vector<vector<int>> matrixKirch(ver, vector<int>(ver, 0));
    int deg;

    for (int i = 0; i < ver; i++)
    {
        deg = 0;
        for (int j = 0; j < ver; j++)
        {
            if (matrixSmezhNotOrgr[i][j] == 1)
            {
                deg++;
                matrixKirch[i][j] = -1;
            }
        }
        matrixKirch[i][i] = deg;
    }

    printMatrix(matrixKirch, "Матрица Кирхгофа");

    vector<vector<int>> minor(ver - 1, vector<int>(ver - 1, 0));
    for (int i = 0; i < ver - 1; i++)
    {
        for (int j = 0; j < ver - 1; j++)
        {
            minor[i][j] = matrixKirch[i + 1][j + 1];
        }
    }

    printMatrix(minor, "Минор[0][0]");

    cout << endl << "Кол-во остовных деревьев в графе равно " << findDet(minor, ver - 1) << endl;
}


// функция для поиска корня компоненты
int find(int vertex, vector<int>& parent) 
{
    if (parent[vertex] == vertex)
        return vertex;
    return parent[vertex] = find(parent[vertex], parent);
}

// функция для объединения двух компонент
void merge(int u, int v, vector<int>& parent) 
{
    u = find(u, parent);
    v = find(v, parent);

    if (u != v)
        parent[u] = v;
}

void myGraph::Kraskal(int matrix) 
{
    vector<vector<int>> matrixWeights;
    minOstov.clear();
    minOstov.resize(ver, vector<int>(ver, 0));
    vector<pair<int, pair<int, int>>> edgesVer;
    int count = 0;
    int minWeight = 0;

    if (matrix) 
    {
        matrixWeights = matrixMixWeightNotOrgr;
        printMatrix(matrixWeights, "Матрица с отрицательными весами");
    }
    else 
    {
        matrixWeights = matrixPosWeightNotOrgr;
        printMatrix(matrixWeights, "Матрица положительных весов");
    }

    for (int i = 0; i < ver; i++) 
    {
        for (int j = i + 1; j < ver; j++) 
        {
            if (matrixWeights[i][j] != 0) 
            {
                edgesVer.push_back(make_pair(matrixWeights[i][j], make_pair(i, j)));
            }
        }
    }

    sort(edgesVer.begin(), edgesVer.end());

    vector<int> parent(ver);
    for (int i = 0; i < ver; i++) parent[i] = i;

    for (int i = 0; i < edgesVer.size(); i++) 
    {
        int u = edgesVer[i].second.first;
        int v = edgesVer[i].second.second;

        if (find(u, parent) != find(v, parent)) 
        {
            minOstov[u][v] = edgesVer[i].first;
            minOstov[v][u] = edgesVer[i].first;
            merge(u, v, parent);
            minWeight += edgesVer[i].first;
        }

        count++;
    }

    printMatrix(minOstov, "Минимальный остов");
    cout << endl << "Вес минимального остова равен " << minWeight << endl;
    cout << "Количество итераций: " << count << endl;
}


void myGraph::Boruvka(int matrix)
{
    vector<vector<int>> matrixWeights;
    minOstov.clear();
    minOstov.resize(ver, vector<int>(ver, 0));
    int count = 0;
    int minWeight = 0;
    int total_comp = ver;

    if (matrix)
    {
        matrixWeights = matrixMixWeightNotOrgr;
        printMatrix(matrixWeights, "Матрица с отрицательными весами");
    }
    else
    {
        matrixWeights = matrixPosWeightNotOrgr;
        printMatrix(matrixWeights, "Матрица положительных весов");
    }

    vector<int> component(ver);
    for (int i = 0; i < ver; i++) component[i] = i;

    while (total_comp > 1) 
    {
        // выбираем минимальное ребро из каждой компоненты
        vector<pair<int, pair<int, int>>> minEdges(ver, make_pair(INT_MAX, make_pair(-1, -1)));

        for (int i = 0; i < ver; i++)
        {
            for (int j = 0; j < ver; j++)
            {
                if (matrixWeights[i][j] != 0)
                {
                    int comp1 = component[i];
                    int comp2 = component[j];
                    if (comp1 != comp2 && matrixWeights[i][j] < minEdges[comp1].first)
                    {
                        minEdges[comp1] = make_pair(matrixWeights[i][j], make_pair(i, j));
                    }
                }
            }
            count++;
        }

        // объединяем компоненты по найденным миним рёбрам
        for (int i = 0; i < ver; i++)
        {
            if (minEdges[i].first != INT_MAX)
            {
                int comp1 = component[minEdges[i].second.first];
                int comp2 = component[minEdges[i].second.second];
                if (comp1 != comp2)
                {
                    total_comp--;
                    minOstov[minEdges[i].second.first][minEdges[i].second.second] = minEdges[i].first;
                    minOstov[minEdges[i].second.second][minEdges[i].second.first] = minEdges[i].first;
                    minWeight += minEdges[i].first;

                    for (int j = 0; j < ver; j++)
                    {
                        if (component[j] == comp2)
                            component[j] = comp1;
                    }
                }
                count++;
            }
        }
    }

    printMatrix(minOstov, "Минимальный остов");
    cout << endl << "Вес минимального остова равен " << minWeight << endl;
    cout << "Количество итераций: " << count << endl; 
}

void compareMatrix(vector<vector<int>> matr1, vector<vector<int>> matr2)
{
    bool same = true;

    for (int i = 0; i < matr1.size(); i++)
    {
        for (int j = 0; j < matr1.size(); j++)
        {
            if (matr1[i][j] != matr2[i][j])
            {
                same = false;
                break;
            }
        }
    }

    if (same)
        cout << endl << "Остовы совпадают" << endl;
    else
        cout << endl << "Остовы не совпадают" << endl;

}

vector<vector<int>> myGraph::PreuferDecode(vector<pair<int, int>> preufer)
{
    vector<vector<int>> decodeOstov(ver, vector<int>(ver, 0));
    vector<int> notUsedVer;

    for (int i = 0; i < ver; i++)
    {
        auto it = find_if(preufer.begin(), preufer.end(), [i](const pair<int, int>& p) { return p.first == i; });

        if (it == preufer.end())
        {
            notUsedVer.push_back(i);
        }
    }

    while(!preufer.empty())
    {
        sort(notUsedVer.begin(), notUsedVer.end(), greater<int>());
        int e = preufer[0].first;

        decodeOstov[preufer[0].first][notUsedVer.back()] = preufer[0].second;
        decodeOstov[notUsedVer.back()][preufer[0].first] = preufer[0].second;

        notUsedVer.pop_back();
        preufer.erase(preufer.begin());
        
        auto it = find_if(preufer.begin(), preufer.end(), [e](const pair<int, int>& p) { return p.first == e; });

        if (it == preufer.end())
        {
            notUsedVer.push_back(e);
        }

    }

    return decodeOstov;
}


void myGraph::PreuferCode()
{
    vector<pair<int, int>> preufer;
    vector<vector<int>> decodedOstov;
    int nearVer;

    if (minOstov.size() == 0)
    {
        cout << "Остов не был сгенерирован" << endl;
        return;
    }

    vector<vector<int>> ostovCopy = minOstov;

    for (int i = 0; i < ver; i++)
    {
        int smezhVer = 0;

        for (int j = 0; j < ver; j++)
        {
            if (ostovCopy[i][j] != 0)
                smezhVer++; //считаем кол-во смежных вершин с вершиной i
        }

        if (smezhVer == 1)
        {
            for (int k = 0; k < ver; k++)
            {
                if (ostovCopy[i][k] != 0)
                    nearVer = k;
            }

            preufer.push_back(make_pair(nearVer, ostovCopy[i][nearVer]));

            ostovCopy[i][nearVer] = 0;
            ostovCopy[nearVer][i] = 0;

            i = 0;                       
        }
    }

    cout << "Код Прюфера (в скобках веса):" << endl;
    for (int i = 0; i < preufer.size(); i++)
        cout << preufer[i].first + 1 << "(" << preufer[i].second << ")" << endl;

    decodedOstov = PreuferDecode(preufer);
    printMatrix(decodedOstov, "После декодирования");
    printMatrix(minOstov, "Исходный остов");
    compareMatrix(minOstov, decodedOstov);

}

void myGraph::findEulerCycle(vector<vector<int>> matrixEuler, vector<int> degreesVer)
{
    stack<int> vertexes;
    vector<int> eulerianCycle;

    vertexes.push(0);

    while (!vertexes.empty())
    {
        int curVer = vertexes.top();
        
        if (degreesVer[curVer] == 0)
        {
            vertexes.pop();
            eulerianCycle.push_back(curVer + 1);
        }
        else
        {
            for (int i = 0; i < ver; i++)
            {
                if (matrixEuler[curVer][i] != 0)
                {
                    vertexes.push(i);
                    degreesVer[i]--;
                    degreesVer[curVer]--;
                    matrixEuler[curVer][i] = 0;
                    matrixEuler[i][curVer] = 0;
                    break;
                }
            }
        } 
    }

    cout << endl << "Эйлеров цикл: ";
    for (int i = 0; i < eulerianCycle.size(); i++)
    {
        cout << eulerianCycle[i];
        if (i != eulerianCycle.size() - 1)
            cout << " ->";
    }

}


void myGraph::Eulerian_graph()
{
    vector<vector<int>> matrixEuler = matrixSmezhNotOrgr;
    vector <int> degreesVer(ver, 0);
    bool isEulerian = true;
    printMatrix(matrixSmezhNotOrgr, "Неориентированный граф. Матрица смежности.");

    if (ver <= 2)
    {
        cout << "Граф из 1-2 вершин не имеет эйлеровых циклов" << endl;
        return;
    }

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (matrixSmezhNotOrgr[i][j] != 0)
                degreesVer[i]++;
        }

        if (degreesVer[i] % 2 == 1)
            isEulerian = false;

    }

    cout << endl << "Степени вершин в исходном графе: ";
    for (int i = 0; i < ver; i++)
        cout << degreesVer[i] << " ";

    if (isEulerian)
        cout << endl << "Граф является эйлеровым" << endl;
    else
    {
        cout << endl << "Граф не является эйлеровым" << endl;

        while (!isEulerian)
        {
            //сначала обработаем висячие вершины, если есть
            for (int i = 0; i < ver; i++)
            {
                if (degreesVer[i] == 1)
                {
                    for (int j = 0; j < ver; j++)
                    {
                        if (matrixEuler[i][j] == 0 && i != j)
                        {
                            matrixEuler[i][j] = 1;
                            matrixEuler[j][i] = 1;
                            degreesVer[i]++;
                            degreesVer[j]++;
                            cout << "Добавляем ребро между вершинами " << i + 1 << " и " << j + 1 << endl;
                            break;
                        }
                    }
                }
            }

            for (int i = 0; i < ver; i++)
            {
                if (degreesVer[i] % 2 == 1)
                {
                    for (int j = 0; j < ver; j++)
                    {
                        if ((degreesVer[j] % 2 == 1) && i != j && matrixEuler[i][j] == 0)
                        {
                            matrixEuler[i][j] = 1;
                            matrixEuler[j][i] = 1;
                            degreesVer[i]++;
                            degreesVer[j]++;
                            cout << "Добавляем ребро между вершинами " << i + 1 << " и " << j + 1 << endl;
                            break;
                        }
                    }

                }
            }

            for (int i = 0; i < ver; i++)
            {
                if (degreesVer[i] % 2 == 1 && degreesVer[i] > 2)
                {
                    for (int j = 0; j < ver; j++)
                    {
                        if (degreesVer[j] % 2 == 1 && matrixEuler[i][j] == 1 && degreesVer[j] > 2)
                        {
                            matrixEuler[i][j] = 0;
                            matrixEuler[j][i] = 0;
                            degreesVer[i]--;
                            degreesVer[j]--;
                            cout << "Удаляем ребро между вершинами " << i + 1 << " и " << j + 1 << endl;
                            break;
                        }
                    }
                }
            }
     
            bool ready = true;
            for (int i = 0; i < ver; i++)
            {
                if (degreesVer[i] % 2 == 1)
                    ready = false;

            }

            if (ready)
            {
                isEulerian = true;
                cout << "Граф достроен до эйлерова графа" << endl;
                printMatrix(matrixEuler, "Эйлеровый граф");
                cout << endl << "Степени вершин в эйлеровом графе: ";

                for (int i = 0; i < ver; i++)
                    cout << degreesVer[i] << " ";
            }
        }
    }

    findEulerCycle(matrixEuler, degreesVer);

}


void myGraph::findHamiltonianCycles(ofstream &file, vector<vector<int>>& matrixWeights, vector<int>& path, vector<int>& minPath, int lenght, int& minLenght) const
{
    if (path.size() == ver)
    {
        if (matrixWeights[path[path.size() - 1]][0] != 0)
        {
            lenght += matrixWeights[path[path.size() - 1]][0];
            path.push_back(0);
            if (minLenght > lenght)
            {
                minPath = path;
                minLenght = lenght;
            }

            file << "Цикл: ";
            for (int i = 0; i < path.size(); i++)
            {
                file << path[i] + 1;
                if (i != path.size() - 1)
                    file << " ->";
            }

            file << " Вес: " << lenght << endl;
            path.pop_back();
        }
        return;    
    }
    else
    {
        for (int i = 0; i < ver; i++)
        {
            if (matrixWeights[path[path.size() - 1]][i] != 0)

            {
                bool inPath = false;

                for (int j = 0; j < path.size(); j++)
                {
                    if (path[j] == i)
                    {
                        inPath = true;
                        break;
                    }
                }

                if (!inPath)
                {
                    int newLenght = lenght + matrixWeights[path[path.size() - 1]][i];
                    path.push_back(i);
                    findHamiltonianCycles(file, matrixWeights, path, minPath, newLenght, minLenght);
                    path.pop_back();
                }
            }
        }
        return;
    }
}


void myGraph::Hamiltonian_graph(int matrix)
{
    ofstream file("HamiltionianCycles.txt");
    vector<vector<int>> matrixWeights;
    vector<int> degreesVer(ver, 0);
    bool isHamiltionian = true;

    if (ver <= 2)
    {
        cout << "Граф состоит всего из 1-2 вершин!" << endl;
        return;
    }

    if (matrix)
    {
        matrixWeights = matrixMixWeightNotOrgr;
        printMatrix(matrixWeights, "Матрица с отрицательными весами");
    }
    else
    {
        matrixWeights = matrixPosWeightNotOrgr;
        printMatrix(matrixWeights, "Матрица положительных весов");
    }

    for (int i = 0; i < ver; i++)
    {
        for (int j = 0; j < ver; j++)
        {
            if (matrixWeights[i][j] != 0)
                degreesVer[i]++;;
        }

        if (degreesVer[i] < ver / 2 || ver == 3)
            isHamiltionian = false;
    }

    cout << endl << "Степени вершин в исходном графе: ";

    for (int i = 0; i < ver; i++)
        cout << degreesVer[i] << " ";

    if (isHamiltionian)
        cout << endl << "Граф является гамильтоновым" << endl;
    else
    {
        cout << endl << "Граф не является гамильтоновым" << endl;

        while (!isHamiltionian)
        {
            bool ready = true;

            // обработаем висячие веришины, если они есть
            for (int i = 0; i < ver; i++)
            {
                if (degreesVer[i] == 1)
                {
                    ready = false;

                    for (int j = 0; j < ver; j++)
                    {
                        if (matrixWeights[i][j] == 0 && i != j)
                        {
                            degreesVer[i]++;
                            degreesVer[j]++;

                            int random;

                            if (matrix == 0)
                                random = rand() % 15 + 1;
                            else
                            {
                                do
                                {
                                    random = rand() % 30 - 20;
                                } while (random == 0);
                            }

                            matrixWeights[i][j] = random;
                            matrixWeights[j][i] = random;
                            cout << "Добавляем ребро между вершинами " << i + 1 << " и " << j + 1 << endl;
                            break;
                        }
                    }
                }
            }

            for (int i = 0; i < ver; i++)
            {
                if (degreesVer[i] < ver / 2)
                {
                    ready = false;

                    for (int j = 0; j < ver; j++)
                    {
                        if (matrixWeights[i][j] == 0 && i != j && degreesVer[j] < ver/2)
                        {
                            degreesVer[i]++;
                            degreesVer[j]++;

                            int random;

                            if (matrix == 0)
                                random = rand() % 15 + 1;                   
                            else
                            {
                                do
                                {
                                    random = rand() % 30 - 20;
                                } while (random == 0);
                            }

                            matrixWeights[i][j] = random;
                            matrixWeights[j][i] = random;
                            cout << "Добавляем ребро между вершинами " << i + 1 << " и " << j + 1 << endl;
                            break;
                        }
                        else if (matrixWeights[i][j] == 0 && i != j && degreesVer[j] >= ver / 2)
                        {
                            degreesVer[i]++;
                            degreesVer[j]++;

                            int random;

                            if (matrix == 0)
                                random = rand() % 15 + 1;
                            else
                            {
                                do
                                {
                                    random = rand() % 30 - 20;
                                } while (random == 0);
                            }

                            matrixWeights[i][j] = random;
                            matrixWeights[j][i] = random;
                            cout << "Добавляем ребро между вершинами " << i + 1 << " и " << j + 1 << endl;
                            break;
                        }
                    }
                }
            }

            if (ready)
            {
                isHamiltionian = true;
                cout << "Граф достроен до гамильтонова графа" << endl;
                printMatrix(matrixWeights, "Гамильтоновый граф");
                cout << endl << "Степени вершин в гамильтоновом графе: ";

                for (int i = 0; i < ver; i++)
                    cout << degreesVer[i] << " ";
                cout << endl;
            }
        }

        vector<int> path;
        vector<int> minPath;
        path.push_back(0);
        int length = 0;
        int minLenght = INT_MAX;

        findHamiltonianCycles(file,matrixWeights, path, minPath, length, minLenght);

        cout << "Все гамильтоновы циклы записаны в файл" << endl;
        cout << "Минимальный по весу цикл: ";

        for (int i = 0; i < minPath.size(); i++)
        {
            cout << minPath[i] + 1;
            if (i != minPath.size() - 1)
                cout << " ->";
        }
        cout << " Вес: " << minLenght << endl;
    }

    file.close();

}

