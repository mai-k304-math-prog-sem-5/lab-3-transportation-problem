#pragma once
#define METHODS_H
#ifdef METHODS_H

void minElemMethod(
	int consumer_ct,			//����� ������������ (����� ��������)
	int provider_ct,			//����� �����������  (����� �����)

	int*** cost_mat,			//������� ��������� ��������
	int*** weight_mat,			//������� ���������� ��������

	int** consumer_vec,			//������ ������������ ������������
	int** provider_vec			//������ ������������ �����������
);

void westNorthCornerMethod(
	int consumer_ct,		//����� ������������ (����� ��������)
	int provider_ct,		//����� �����������  (����� �����)

	int*** cost_mat,			//������� ��������� ��������
	int*** weight_mat,			//������� ���������� ��������

	int** consumer_vec,			//������ ������������ ������������
	int** provider_vec			//������ ������������ �����������
);
#endif // METHODS_H