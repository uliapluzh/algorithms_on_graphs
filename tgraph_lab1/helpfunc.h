#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

myGraph createGraph()
{

	int v;
	char a[20];

	cout << "Введите кол-во вершин в графе" << endl;
	cin >> a;

	while (sscanf(a, "%d", &v) != 1 || v <= 0)
	{
		cout << "Некорректный ввод.Повторите попытку: " << endl;
		cin >> a;
	}

	myGraph gr(v);

	return gr;
}

void menu()
{
	cout << "\nВыберите действие:" << endl;
	cout << "1.Метод Шимбелла" << endl;
	cout << "2.Найти кол-во маршрутов между вершинами" << endl;
	cout << "3.Распечатать матрицу смежности" << endl;
	cout << "4.Распечатать матрицу положительных весов" << endl;
	cout << "5.Распечатать матрицу с отрицательными весами" << endl;
	cout << "6.Алгоритм Дейкстры (классический)" << endl;
	cout << "7.Алгоритм Флойда-Уоршалла" << endl;
	cout << "8.Поиск максимального пути" << endl;
	cout << "9.Максимальный поток + поток минимальной стоимости" << endl;
	cout << "10.Найти кол-во остовных деревьев (матричная теорема Кирхгофа)" << endl;
	cout << "11.Алгоритм Краскала" << endl;
	cout << "12.Алгоритм Борувки" << endl;
	cout << "13.Код Прюфера + декодирование минимального остова" << endl;
	cout << "14.Построить эйлеров граф и цикл" << endl;
	cout << "15.Построить гамильтонов граф и циклы + задача коммивояжера" << endl;
	cout << "16.Пересоздать граф" << endl;
	cout << "17.Выход" << endl;
	printf("\n");
}

int getinteger() //проверка на некорректный ввод
{
	int number;
	char a[20];

	cin >> a;

	while (sscanf(a, "%d", &number) != 1 || number < 1 || number > 17)
	{
		cout << "Некорректный ввод.Повторите попытку: " << endl;
		cin >> a;
	}

	return number;
}

int getMinMax()
{
	int minMax;
	do
	{
		cout << "Если вы хотите найти максимальные пути в k дуг, нажмите 1, если минимальные, то 0: ";
		cin >> minMax;

		if (minMax != 0 && minMax != 1) cout << "Некорректный ввод. Повторите попытку!" << endl;

	} while (minMax != 0 && minMax != 1);

	return minMax;
}

int getMatrix()
{
	int matrix;
	do
	{
		cout << "Если вы хотите использовать матрицу с отрицательными весами, нажмите 1" << endl << 
			"если с положительными весами , то 0: ";
		cin >> matrix;

		if (matrix != 0 && matrix != 1) cout << "Некорректный ввод. Повторите попытку!" << endl;

	} while (matrix != 0 && matrix != 1);

	return matrix;
}

int getVer(int maxVer, int num)
{
	int v;
	do
	{
		cout << "Введите вершину " << num << endl;
		cin >> v;

		if (v < 1 || v > maxVer)  cout << "Некорректный ввод. Повторите попытку!" << endl;

	} while (v < 1 || v > maxVer);

	return v;
}

int getE(int eMax)
{
	int e;
	do
	{
		cout << "Введите кол-во дуг " << endl;;
		cin >> e;

		if (e < 1 || e > eMax - 1)  cout << "Некорректный ввод. Повторите попытку!" << endl;

	} while (e < 1 || e > eMax - 1);

	return e;
}