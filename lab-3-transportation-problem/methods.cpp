#include <iostream>
#include "methods.h"

using namespace std;

void minElemMethod(
	int consumer_ct,			//����� ������������ (����� ��������)
	int provider_ct,			//����� �����������  (����� �����)

	int*** cost_mat,			//������� ��������� ��������
	int*** weight_mat,			//������� ���������� ��������

	int** consumer_vec,			//������ ������������ ������������
	int** provider_vec			//������ ������������ �����������
)
{
	//�� ������ ����� ��� ��� ������ ������, ������ ����� �������� ����� ������������ ����� �������� �� ����� �����
	for (int ct = 0; ct < provider_ct * consumer_ct; ct++)
	{
		//���������� ���. ��������
		int min_cost_i = -1;
		int min_cost_j = -1;

		int consumer_sum = 0;		//����� � �������
		int provider_sum = 0;		//����� � ������

		//�������� ��� ������ �������� ������ �������������� ������� ��� �������
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

		//����� ����������� ���������
		for (int i = 0; i < provider_ct; i++)
		{
			for (int j = 0; j < consumer_ct; j++)
			{
				//������� ������ ���� ������ ������� ��������� � ���� ��������������
				if ((*cost_mat)[i][j] < (*cost_mat)[min_cost_i][min_cost_j] && (*weight_mat)[i][j] == -1)
				{
					min_cost_i = i;
					min_cost_j = j;

				}//..if

			}//..for j

		}//..for i

		//���� ����� � �������
		for (int i = 0; i < provider_ct; i++)
		{
			if ((*weight_mat)[i][min_cost_j] == -1) provider_sum += 0;
			else provider_sum += (*weight_mat)[i][min_cost_j];
		}//..for i

		//���� ����� � ������
		for (int j = 0; j < consumer_ct; j++)
		{
			if ((*weight_mat)[min_cost_i][j] == -1) consumer_sum += 0;
			else consumer_sum += (*weight_mat)[min_cost_i][j];
		}//..for i

		(*weight_mat)[min_cost_i][min_cost_j] = min((*provider_vec)[min_cost_i] - consumer_sum, (*consumer_vec)[min_cost_j] - provider_sum);

	}//..for ct

}//..minElemMethod()


void westNorthCornerMethod(
	int consumer_ct,		//����� ������������ (����� ��������)
	int provider_ct,		//����� �����������  (����� �����)

	int*** cost_mat,			//������� ��������� ��������
	int*** weight_mat,			//������� ���������� ��������

	int** consumer_vec,			//������ ������������ ������������
	int** provider_vec			//������ ������������ �����������
)
{
	//���������� ������� ������-��������� ����
	for (int i = 0; i < min(consumer_ct, provider_ct); i++)
	{
		int consumer_sum = 0;		//����� � �������
		int provider_sum = 0;		//����� � ������

		//����� ��������� � ������ � �������, �� �������� �������� �� ������� ��������� �������
		for (int j = 0; j < i; j++)
		{
			consumer_sum += (*weight_mat)[j][i];
			provider_sum += (*weight_mat)[i][j];

		}//..for j

		//�������� �������� �������� �� ������� ��������� ����� ����� �������� �� ���� ����� : ����������� ����� � ����������� � ���������� ������ � ����������
		(*weight_mat)[i][i] = min((*consumer_vec)[i] - consumer_sum, (*provider_vec)[i] - provider_sum);

		//���� ����� � ����������� ������, ��� � ���������� �������
		if ((*weight_mat)[i][i] == (*consumer_vec)[i] - consumer_sum)
		{
			//��� ��� ����� ������ ���, �� ����������� �� ������ ������ �������� � ������ �����������
			//������ ����������� � ���� � ��������� ������� ����� ����
			for (int j = i + 1; j < provider_ct; j++)
				(*weight_mat)[j][i] = 0;

			//������� ������ � ����������
			int remainder = (*provider_vec)[i] - provider_sum - (*weight_mat)[i][i];

			//��������� ������� ������� � ����������
			for (int j = i + 1; j < consumer_ct; j++)
			{
				//�������� ������� ����� �������� ������� � ������ � �����������
				(*weight_mat)[i][j] = min(remainder, (*consumer_vec)[j]);
				remainder -= (*weight_mat)[i][j];
			}//for j

		}
		else
		{
			//��� ��� � ���������� ������ ��� ������, �� ����������� ������ �� ������ �������� �����
			//������ ����� ������� � ���� ������������ ����� ����
			for (int j = i + 1; j < consumer_ct; j++)
				(*weight_mat)[i][j] = 0;

			//������� ����� � �����������
			int remainder = (*consumer_vec)[i] - consumer_sum - (*weight_mat)[i][i];

			//��������� ������� ����� � �����������
			for (int j = i + 1; j < consumer_ct; j++)
			{
				//�������� ������� ����� �������� ����� � �������� ������ � ����������
				(*weight_mat)[j][i] = min(remainder, (*provider_vec)[j]);
				remainder -= (*weight_mat)[j][i];
			}//for j

		}//..if..else

	}//..for i

}//..westNorthCornerMethod()