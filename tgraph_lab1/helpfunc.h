#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;

myGraph createGraph()
{

	int v;
	char a[20];

	cout << "������� ���-�� ������ � �����" << endl;
	cin >> a;

	while (sscanf(a, "%d", &v) != 1 || v <= 0)
	{
		cout << "������������ ����.��������� �������: " << endl;
		cin >> a;
	}

	myGraph gr(v);

	return gr;
}

void menu()
{
	cout << "\n�������� ��������:" << endl;
	cout << "1.����� ��������" << endl;
	cout << "2.����� ���-�� ��������� ����� ���������" << endl;
	cout << "3.����������� ������� ���������" << endl;
	cout << "4.����������� ������� ������������� �����" << endl;
	cout << "5.����������� ������� � �������������� ������" << endl;
	cout << "6.�������� �������� (������������)" << endl;
	cout << "7.�������� ������-��������" << endl;
	cout << "8.����� ������������� ����" << endl;
	cout << "9.������������ ����� + ����� ����������� ���������" << endl;
	cout << "10.����� ���-�� �������� �������� (��������� ������� ��������)" << endl;
	cout << "11.�������� ��������" << endl;
	cout << "12.�������� �������" << endl;
	cout << "13.��� ������� + ������������� ������������ ������" << endl;
	cout << "14.��������� ������� ���� � ����" << endl;
	cout << "15.��������� ����������� ���� � ����� + ������ ������������" << endl;
	cout << "16.����������� ����" << endl;
	cout << "17.�����" << endl;
	printf("\n");
}

int getinteger() //�������� �� ������������ ����
{
	int number;
	char a[20];

	cin >> a;

	while (sscanf(a, "%d", &number) != 1 || number < 1 || number > 17)
	{
		cout << "������������ ����.��������� �������: " << endl;
		cin >> a;
	}

	return number;
}

int getMinMax()
{
	int minMax;
	do
	{
		cout << "���� �� ������ ����� ������������ ���� � k ���, ������� 1, ���� �����������, �� 0: ";
		cin >> minMax;

		if (minMax != 0 && minMax != 1) cout << "������������ ����. ��������� �������!" << endl;

	} while (minMax != 0 && minMax != 1);

	return minMax;
}

int getMatrix()
{
	int matrix;
	do
	{
		cout << "���� �� ������ ������������ ������� � �������������� ������, ������� 1" << endl << 
			"���� � �������������� ������ , �� 0: ";
		cin >> matrix;

		if (matrix != 0 && matrix != 1) cout << "������������ ����. ��������� �������!" << endl;

	} while (matrix != 0 && matrix != 1);

	return matrix;
}

int getVer(int maxVer, int num)
{
	int v;
	do
	{
		cout << "������� ������� " << num << endl;
		cin >> v;

		if (v < 1 || v > maxVer)  cout << "������������ ����. ��������� �������!" << endl;

	} while (v < 1 || v > maxVer);

	return v;
}

int getE(int eMax)
{
	int e;
	do
	{
		cout << "������� ���-�� ��� " << endl;;
		cin >> e;

		if (e < 1 || e > eMax - 1)  cout << "������������ ����. ��������� �������!" << endl;

	} while (e < 1 || e > eMax - 1);

	return e;
}