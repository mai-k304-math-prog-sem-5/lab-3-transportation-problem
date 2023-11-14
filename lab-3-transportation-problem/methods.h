#pragma once
#define METHODS_H
#ifdef METHODS_H

void minElemMethod(
	int consumer_ct,			//число потребителей (число столбцов)
	int provider_ct,			//число поставщиков  (число строк)

	int*** cost_mat,			//матрица стоимости доставки
	int*** weight_mat,			//матрица количества доставки

	int** consumer_vec,			//вектор потребностей потребителей
	int** provider_vec			//вектор возможностей поставщикоу
);

void westNorthCornerMethod(
	int consumer_ct,		//число потребителей (число столбцов)
	int provider_ct,		//число поставщиков  (число строк)

	int*** cost_mat,			//матрица стоимости доставки
	int*** weight_mat,			//матрица количества доставки

	int** consumer_vec,			//вектор потребностей потребителей
	int** provider_vec			//вектор возможностей поставщикоу
);
#endif // METHODS_H