#include <iostream>
#include "methods.h"

using namespace std;

void minElemMethod(
	int consumer_ct,			//число потребителей (число столбцов)
	int provider_ct,			//число поставщиков  (число строк)

	int*** cost_mat,			//матрица стоимости доставки
	int*** weight_mat,			//матрица количества доставки

	int** consumer_vec,			//вектор потребностей потребителей
	int** provider_vec			//вектор возможностей поставщикоу
)
{
	//мы должны найти вес для каждой ячейки, значит число итераций равно произведению числа столбцов на число строк
	for (int ct = 0; ct < provider_ct * consumer_ct; ct++)
	{
		//координаты мин. элемента
		int min_cost_i = -1;
		int min_cost_j = -1;

		int consumer_sum = 0;		//сумма в столбце
		int provider_sum = 0;		//сумма в строке

		//выбираем для поиска минимума первый неопределенный элемент как опорный
		for (int i = 0; i < provider_ct; i++)
		{
			for (int j = 0; j < consumer_ct; j++)
				if ((*weight_mat)[i][j] == -1)
				{
					min_cost_i = i;
					min_cost_j = j;
					break;
				}//..if

		}//..for i

		//поиск минимальной стоимости
		for (int i = 0; i < provider_ct; i++)
		{
			for (int j = 0; j < consumer_ct; j++)
			{
				//элемент должен быть меньше текуцей стоимости и быть неопределенным
				if ((*cost_mat)[i][j] < (*cost_mat)[min_cost_i][min_cost_j] && (*weight_mat)[i][j] == -1)
				{
					min_cost_i = i;
					min_cost_j = j;

				}//..if

			}//..for j

		}//..for i

		//ищем сумму в столбце
		for (int i = 0; i < provider_ct; i++)
		{
			if ((*weight_mat)[i][min_cost_j] == -1) provider_sum += 0;
			else provider_sum += (*weight_mat)[i][min_cost_j];
		}//..for i

		//ищем сумму в строке
		for (int j = 0; j < consumer_ct; j++)
		{
			if ((*weight_mat)[min_cost_i][j] == -1) consumer_sum += 0;
			else consumer_sum += (*weight_mat)[min_cost_i][j];
		}//..for i

		(*weight_mat)[min_cost_i][min_cost_j] = min((*provider_vec)[min_cost_i] - consumer_sum, (*consumer_vec)[min_cost_j] - provider_sum);

	}//..for ct

}//..minElemMethod()


void westNorthCornerMethod(
	int consumer_ct,		//число потребителей (число столбцов)
	int provider_ct,		//число поставщиков  (число строк)

	int*** cost_mat,			//матрица стоимости доставки
	int*** weight_mat,			//матрица количества доставки

	int** consumer_vec,			//вектор потребностей потребителей
	int** provider_vec			//вектор возможностей поставщикоу
)
{
	//заполнение методом северо-западного угла
	for (int i = 0; i < min(consumer_ct, provider_ct); i++)
	{
		int consumer_sum = 0;		//сумма в столбце
		int provider_sum = 0;		//сумма в строке

		//сумма элементов в строке и столбце, до текущего элемента на главной диагонали матрицы
		for (int j = 0; j < i; j++)
		{
			consumer_sum += (*weight_mat)[j][i];
			provider_sum += (*weight_mat)[i][j];

		}//..for j

		//значение текущего элемента на главной диагонале будет равно минимуму от двух чисел : отсавшегося места у потребителя и имеющегося товара у постовщика
		(*weight_mat)[i][i] = min((*consumer_vec)[i] - consumer_sum, (*provider_vec)[i] - provider_sum);

		//если места у потребителя меньше, чем у поставщика товаров
		if ((*weight_mat)[i][i] == (*consumer_vec)[i] - consumer_sum)
		{
			//так как места больше нет, то потребитель не сможет больше заказать у других поставщиков
			//значит потребление у него в остальных строках равно нулю
			for (int j = i + 1; j < provider_ct; j++)
				(*weight_mat)[j][i] = 0;

			//остаток товара у поставщика
			int remainder = (*provider_vec)[i] - provider_sum - (*weight_mat)[i][i];

			//заполняем остатки заказов у поставщика
			for (int j = i + 1; j < consumer_ct; j++)
			{
				//выбираем минимум между остатком заказов и местом у потербителя
				(*weight_mat)[i][j] = min(remainder, (*consumer_vec)[j]);
				remainder -= (*weight_mat)[i][j];
			}//for j

		}
		else
		{
			//так как у поставщика больше нет товара, то потребители больше не смогут заказать товар
			//значит число заказов у всех потребителей равно нулю
			for (int j = i + 1; j < consumer_ct; j++)
				(*weight_mat)[i][j] = 0;

			//остаток места у потребителя
			int remainder = (*consumer_vec)[i] - consumer_sum - (*weight_mat)[i][i];

			//заполняем остатки места у потребителя
			for (int j = i + 1; j < consumer_ct; j++)
			{
				//выбираем минимум между остатком места и остатком товара у поставщика
				(*weight_mat)[j][i] = min(remainder, (*provider_vec)[j]);
				remainder -= (*weight_mat)[j][i];
			}//for j

		}//..if..else

	}//..for i

}//..westNorthCornerMethod()