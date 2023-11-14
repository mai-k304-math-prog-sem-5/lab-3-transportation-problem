#include <iostream>
#include <fstream>
#include "methods.h"

using namespace std;

int main()
{
	//system("color F0");

	int consumer_ct = 3;		//число потребителей (число столбцов)
	int provider_ct = 4;		//число поставщиков  (число строк)

	int** cost_mat = new int* [provider_ct];			//матрица стоимости доставки
	int** weight_mat = new int* [provider_ct];			//матрица количества доставки
	int** potential_mat = new int* [provider_ct];		//матрица потенциалов
	bool** base_mat = new bool* [provider_ct];			//вспомогательная матрица для базиса

	int* consumer_vec = new int[consumer_ct];			//вектор потребностей потребителей
	int* provider_vec = new int[provider_ct];			//вектор возможностей поставщикоу

	int* consumer_potencial_vec = new int[consumer_ct];	//вектор потенциалов потребителя
	int* provider_potencial_vec = new int[provider_ct];	//вектор возможностей поставщика

	//координаты ячейки с мин. потенциалом
	int min_i = 0;
	int min_j = 0;

	//значение функции
	int total_cost = 0;

	//выделение памяти под матрицы таблицы
	for (int i = 0; i < provider_ct; i++)
	{
		cost_mat[i] = new int[consumer_ct];
		weight_mat[i] = new int[consumer_ct];
		potential_mat[i] = new int[consumer_ct];
		base_mat[i] = new bool[consumer_ct];

	}//..for i

	//открытие файла с данными
	ifstream fin("cost_source.txt");

	//заполнение матриц-таблиц
	for (int i = 0; i < provider_ct; i++)
	{
		for (int j = 0; j < consumer_ct; j++)
		{
			int file_read;
			fin >> file_read;
			cost_mat[i][j] = file_read;
			weight_mat[i][j] = -1;
			potential_mat[i][j] = 0;

		}//..for j
	}//..for i

	//заполнение векторов с потенциалами и ограничениями возможностей
	for (int i = 0; i < consumer_ct; i++)
	{
		int file_read;
		fin >> file_read;
		consumer_vec[i] = file_read;
		consumer_potencial_vec[i] = 999;
	}//..for i

	for (int i = 0; i < provider_ct; i++)
	{
		int file_read;
		fin >> file_read;
		provider_vec[i] = file_read;
		provider_potencial_vec[i] = 999;
	}//..for i

	//закрытие файла с данными
	fin.close();

	minElemMethod(consumer_ct, provider_ct, &cost_mat, &weight_mat, &consumer_vec, &provider_vec);

	//счетчик итераций
	int iters = 1;

	//минимизация функции
	while (true)
	{
		//поиск базисов
		for (int i = 0; i < provider_ct; i++)
		{
			for (int j = 0; j < consumer_ct; j++)
			{
				if (weight_mat[i][j] != 0)
					base_mat[i][j] = true;
				else
					base_mat[i][j] = false;

			}//..for j

		}//..for i

		//заполнение потенциалов
		for (int i = 0; i < provider_ct; i++)
		{
			//в предыдущей итерации м должны были узнать потенциал строки,
			//но так как i = 0 это 1-ая итерация, то потенциал будет равен 0
			if (i == 0) provider_potencial_vec[0] = 0;

			//ищем в строке базовывй элемент, так как потенциал строки известен,
			//то можем при помощи него найти потенциал столбца
			for (int j = 0; j < consumer_ct; j++)
			{
				if (base_mat[i][j] == true)
				{
					consumer_potencial_vec[j] = cost_mat[i][j] - provider_potencial_vec[i];

					//теперь, зная потенциал столбца, можно найти потенциал следующей строки,
					//для следующей итерации
					for (int k = i + 1; k < provider_ct; k++)
					{
						if (base_mat[k][j] == true)
						{
							provider_potencial_vec[k] = cost_mat[k][j] - consumer_potencial_vec[j];
						}//..if

					}//..for k

				}//..if

			}//..for j

		}//..for i

		//расчет потенциалов для каждой ячейки
		for (int i = 0; i < provider_ct; i++)
		{
			for (int j = 0; j < consumer_ct; j++)
				potential_mat[i][j] = cost_mat[i][j] - provider_potencial_vec[i] - consumer_potencial_vec[j];

		}//..for i


		//значение функции
		total_cost = 0;

		//считаем значение функции
		for (int i = 0; i < provider_ct; i++)
		{
			for (int j = 0; j < consumer_ct; j++)
				total_cost += weight_mat[i][j] * cost_mat[i][j];

		}//..for i

		//вывод таблицы
		cout << "ITERATION #" << iters << endl;
		cout << consumer_ct << endl;
		cout << provider_ct << endl;
		cout << cost_mat << endl;
		cout << weight_mat << endl;
		cout << potential_mat << endl;
		cout << consumer_vec << endl;
		cout << provider_vec << endl;
		cout << consumer_potencial_vec << endl;
		cout << provider_potencial_vec << endl;
		//cout << consumer_ct << provider_ct << &cost_mat << &weight_mat << &potential_mat << &consumer_vec << &provider_vec << &consumer_potencial_vec << &provider_potencial_vec << endl;
		//printTab(consumer_ct, provider_ct, &cost_mat, &weight_mat, &potential_mat, &consumer_vec, &provider_vec, &consumer_potencial_vec, &provider_potencial_vec);


		cout << "\nF = " << total_cost << endl;

		//координаты ячейки с мин. потенциалом
		min_i = 0;
		min_j = 0;

		//поиск ячейки с наименьшим потенциалом
		for (int i = 0; i < provider_ct; i++)
		{
			for (int j = 0; j < consumer_ct; j++)
			{
				if (potential_mat[min_i][min_j] > potential_mat[i][j])
				{
					min_i = i;
					min_j = j;
				}//..if

			}//..for j

		}//..for i

		//условие выходы из цикла
		if (potential_mat[min_i][min_j] >= 0) break;

		//поиск контура

		//флаг проверки на возможность замкунть прямоуголный контур
		bool have_rec_contur = false;

		//p для пересчета таблицы
		int p = 0;

		//#1 попробовать замкнуть прямоугольник
		for (int i = 0; i < provider_ct; i++)
		{
			if (i == min_i) continue;

			for (int j = 0; j < consumer_ct; j++)
			{
				if (j == min_j) continue;

				//контур сторится по элементам базиса
				if (
					base_mat[i][min_j] == true &&
					base_mat[min_i][j] == true &&
					base_mat[i][j] == true
					)
				{
					p = min(weight_mat[i][min_j], weight_mat[min_i][j]);

					weight_mat[i][min_j] -= p;
					weight_mat[min_i][j] -= p;
					weight_mat[i][j] += p;
					weight_mat[min_i][min_j] += p;

					have_rec_contur = true;
					break;

				}//..if

			}//..for j

		}//..for i


		//#2 если не удалось замкнуть прямоугольник, то ищем контур n-угольник
		if (!have_rec_contur)
		{
			int m = min_i;
			int n = min_j;

			int cur_i = min_i;
			int cur_j = min_j;

			while (true)
			{
				//ищем базисный элемент в текущем столбце
				m = 0;
				while (base_mat[m][n] != true || m == cur_i) m++;
				cur_i = m;

				//выбираем первый элемент в контуре, либо тот, что меньше текущего
				if (weight_mat[m][n] < p || n == min_j) p = weight_mat[m][n];

				//если мы находимся в той же строке, с которой начинали, то контур создан
				if (m == min_i) break;

				//ищем базисный элемент в текущей строке
				n = 0;
				while (base_mat[m][n] != true || n == cur_j) n++;
				cur_j = n;

			}//..while

			//пересчет весов по контуру
			m = min_i;
			n = min_j;

			cur_i = min_i;
			cur_j = min_j;

			weight_mat[m][n] += p;

			while (true)
			{
				//ищем базисный элемент в текущем столбце
				m = 0;
				while (base_mat[m][n] != true || m == cur_i) m++;
				cur_i = m;

				weight_mat[m][n] -= p;

				//если мы находимся в той же строке, с которой начинали, то контур создан
				if (m == min_i) break;

				//ищем базисный элемент в текущей строке
				n = 0;
				while (base_mat[m][n] != true || n == cur_j) n++;
				cur_j = n;

				weight_mat[m][n] += p;

			}//..while

		}//..if

		cout << "P = " << p << endl << endl;

		iters++;	//увеличение счетчика итераций

	}//..while

	//освобождение памяти от массивов
	for (int i = 0; i < provider_ct; i++)
	{
		delete[] cost_mat[i];
		delete[] weight_mat[i];
		delete[] potential_mat[i];
		delete[] base_mat[i];

	}//..for i

	delete[] cost_mat;
	delete[] weight_mat;
	delete[] potential_mat;
	delete[] base_mat;

	delete[] consumer_vec;
	delete[] provider_vec;

	delete[] consumer_potencial_vec;
	delete[] provider_potencial_vec;

	return 0;

}//..main()
