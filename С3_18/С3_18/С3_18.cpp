// �3_18.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <conio.h>
#include <locale.h>
using namespace std;

const int length = 20;
const int buf = 100;

//����, ������������ �������, ����� �����
template <class Gr, class fio>
struct data{
	Gr day;//����(����)
	Gr month;//�����
	Gr year;//���
	Gr presence;//�����������
	Gr number;//����� ������
	Gr variant;//�������
	Gr control_of_performance;//�������� ����������
	data *next;
	data *prev;
};
template <class Gr, class fio>
struct fio_s{
	Gr group;//������
	fio familia[length+1];//�������
	fio name[length+1];//���
	fio otchestvo[length+1];//��������
	data<Gr, fio> *employment;//�������
	fio_s *next;
	fio_s *prev;	
};
template <class Gr, class fio>
class Date{
public:	
	fio_s<Gr, fio> * element_beg;
	fio_s<Gr, fio> * element_end;
	//---------------------------------------------------------------------------------
	//�������� ������ ������� ����� ������� ������ ��������
	fio_s<Gr, fio> * first(){
		FILE * file;
		while ((file = fopen("Spisok.bin", "r")) == NULL) {
			cout << "ERROR OPEN\n"; return 0;
		};	
		fio_s<Gr, fio> * pv = new fio_s<Gr, fio>;
		fscanf_s(file, "%i", &(pv->group));
		fgets(pv->familia, length, file);
		fgets(pv->name, length, file);
		fgets(pv->otchestvo, length, file);
		int i = 100; int j = 0;		
		while(i != 0){
			data<Gr, fio> *buf = new data<Gr, fio>;
			//if (i != 100) buf = ((*buf).next);
			fscanf_s(file, "%i", &(i)); buf->day = i;
			fscanf_s(file, "%i", &(i)); buf->month = i;
			fscanf_s(file, "%i", &(i)); buf->year = i;
			fscanf_s(file, "%i", &(i)); buf->presence = i; 
			fscanf_s(file, "%i", &(i)); buf->number = i;
			fscanf_s(file, "%i", &(i)); buf->variant = i;
			fscanf_s(file, "%i", &(i)); buf->control_of_performance = i;
			fscanf_s(file, "%i", &(i));
			if (j == 0){				
				buf->next = 0;
				buf->prev = 0;
				pv->employment = buf;
				i++; j++;
			} else {
				buf->prev = pv->employment;
				pv->employment->next = buf;
				pv->employment = buf;
				if (i == 100)i++; j++;
			};
		};
		fclose(file);
		return pv;
	};
	//---------------------------------------------------------------------------------
	//�����������
	Date(){
		element_beg = first();
		element_end = element_beg;
	};
	//---------------------------------------------------------------------------------
	//����������
	~Date(){

	};
	//---------------------------------------------------------------------------------
	//����� �������� �� ��������� �����...(�� ������ ������ �� �������...)
	fio_s<Gr, fio> * find(fio_s<Gr, fio> * const mod_beg, fio_s<Gr, fio> * const mod_key){
		fio_s<Gr, fio> *pv = mod_beg;
		while(pv != NULL){
			if (pv->group == mod_key->group){
				if (!(strcmp(pv->familia, mod_key->familia))){
					break;
				};
			};
			pv = pv->next;
		};
		return pv;
	};
	//---------------------------------------------------------------------------------                                       // ������������ ����������
	//���������� ��������� ������������ � �����������
	void add_sort(fio_s<Gr, fio> **mod_beg, fio_s<Gr, fio> **mod_end, fio_s<Gr, fio> * element){
		fio_s<Gr, fio> *pv = new fio_s<Gr, fio>; //����������� �������
		pv = element;
		fio_s<Gr, fio> *prim = *mod_beg;
		while (prim->next){  //�������� ������
			if (((prim->group) == (pv->group)) && (((prim->next->group) > (pv->group)) || (prim->next == 0))) {
				if ((prim->familia) == (pv->familia)) {
					if ((prim->name) == (pv->name)){
						if ((prim->otchestvo) == (pv->otchestvo)){
							cout << endl << "����� ������� ��� ���������� � ������" << endl; 
						} else goto metka;
					} else goto metka;
				} else if (((prim->familia) < (pv->familia)) && (((prim->next)->familia) > (pv->familia))){
					goto metka;
				} else goto metka;
			} else if (((prim->group) < (pv->group)) && ((prim->next->group) > (pv->group))) {
				metka:	
				pv->prev = prim;
				if (prim == *mod_beg){ // � ������
					pv->prev = 0;
					*mod_beg = pv;
				} else { // � ��������
					(prim->prev)->prev = pv;
					pv->next = prim->next;
				};
				prim->next = pv;
				return;
			};
			prim = prim->next;
		}
		pv->next = 0;  //� ����� ������
		pv->prev = *mod_end;
		(*mod_end)->next = pv;
		*mod_end = pv;
	};
	//---------------------------------------------------------------------------------
	//���������� ���� ���������� ������� �� ����� � ����� ������
	void add(fio_s<Gr, fio> ** mod_beg, fio_s<Gr, fio> ** mod_end){
		//fio_s<Gr, fio> * pv = new fio_s<Gr, fio>;
		FILE * file;
		while ((file = fopen("Spisok.bin", "rb")) == NULL) {
			cout << "ERROR OPEN\n"; return ;
		};	
		while (!feof(file)){
			fio_s<Gr, fio> *pv = new fio_s<Gr, fio>;
			fscanf_s(file, "\n%i", &(pv->group));
			fgets(pv->familia, length, file);
			fgets(pv->name, length, file);
			fgets(pv->otchestvo, length, file);
			int i = 100; int j = 0;		
			while(i != 0){
				data<Gr, fio> *buf = new data<Gr, fio>;
				fscanf_s(file, "%i", &(i)); buf->day = i;
				fscanf_s(file, "%i", &(i)); buf->month = i;
				fscanf_s(file, "%i", &(i)); buf->year = i;
				fscanf_s(file, "%i", &(i)); buf->presence = i; 
				fscanf_s(file, "%i", &(i)); buf->number = i;
				fscanf_s(file, "%i", &(i)); buf->variant = i;
				fscanf_s(file, "%i", &(i)); buf->control_of_performance = i;
				fscanf_s(file, "%i", &(i));
				if (j == 0){				
					buf->next = 0;
					buf->prev = 0;
					pv->employment = buf;
					i++; j++;
				} else {
					buf->next = 0;
					buf->prev = pv->employment;
					pv->employment->next = buf;
					pv->employment = buf;
					if (i == 100)i++; j++;
				};
			};
			pv->next = 0;
			pv->prev = *mod_end;
			(*mod_end)->next = pv;
			*mod_end = pv;
			char bufer[buf+1]; 
			fgets(bufer, buf, file);
		};
		fclose(file);
	};
	//---------------------------------------------------------------------------------
	//�������� �������� �� �����
	bool remove(fio_s<Gr, fio> **mod_beg, fio_s<Gr, fio> ** mod_end){
		fio_s<Gr, fio> *pv = new fio_s<Gr, fio>;
		cout << " ���� ������ ������ �� ����������!!!" << endl;
		cout << " ���������� ������� ���������, ����� ��������� ��� �� ������!! " << endl << " ����� ������� ��� � ������� �����!!" << endl << "������� ��� ����������� ���������!" << endl << endl;
		cout << " ������� ������ ������� : "; cin >> pv->group;
		cout << " ������� ������� ������� : "; cin >> pv->familia;
		cout << " ������� ��� ������� : "; cin >> pv->name;
		cout << " ������� �������� ������� : "; cin >> pv->otchestvo;		
		pv->next = 0;
		pv->prev = 0;
		if (fio_s<Gr, fio> *mod_key = find(*mod_beg, pv)){
			if (mod_key == *mod_beg){
				*mod_beg = (*mod_beg)->next;
				(*mod_end)->next = 0;
			} else 
			if (mod_key == *mod_end){
				*mod_end = (*mod_end)->prev;
				(*mod_end)->next = 0;
			} else {
				(mod_key->prev)->next = mod_key->next;
				(mod_key->next)->prev = mod_key->prev;
			};
			delete mod_key;
			return true;
		};
		return false;
	};	
	//---------------------------------------------------------------------------------
	//���������� ������ ������� � ����� ������
	void add_man(fio_s<Gr, fio> ** mod_beg,fio_s<Gr, fio> **mod_end){
		fio_s<Gr, fio> *pv = new fio_s<Gr, fio>;
		cout << " ���������� ������� ���������, ����� ��������� ��� �� ������!! " << endl << " ����� ������� ��� � ������� �����!!" << endl << "������� ��� ����������� ���������!" << endl << endl;
		cout << " ������� ������ ������� : "; cin >> pv->group;
		cout << " ������� ������� ������� : "; cin >> pv->familia;
		cout << " ������� ��� ������� : "; cin >> pv->name;
		cout << " ������� �������� ������� : "; cin >> pv->otchestvo;
		add_sort(mod_beg, mod_end, pv);
	};
	//---------------------------------------------------------------------------------
	//����� �� �����
	void output(fio_s<Gr, fio> * const mod_beg){
		cout << "����� ����� ������ " << endl << endl;//����� ������ - ���
		cout << "^Group -         ^surname               ^name             ^patronymic" << endl;
		fio_s<Gr, fio> *pv = mod_beg;
		while(pv != NULL){
			printf("%5i", pv->group);
			cout << "  -  ";
			printf("%17s", pv->familia);
			printf("%17s", pv->name);
			printf("%17s", pv->otchestvo);
			pv = pv->next;
			cout << endl; 
		};
		
		cout << endl << endl << "������� Enter "; _getch();
	};
	//---------------------------------------------------------------------------------
	//������ � ������: ������������
	void attendance(fio_s<Gr, fio> ** mod_beg, fio_s<Gr, fio> ** mod_end){
		system("cls");
		cout << " ���������� ������� ���������, ����� ��������� ��� �� ������!! " << endl << endl;
		cout << " ������������ " << endl << endl;
		cout << " ������� ���� ������� : " << endl;
		data<Gr, fio> * key = new data<Gr, fio>;
		cout << " ���� : "; cin >> key->day;
		cout << " ����� : "; cin >> key->month;
		cout << " ��� : "; cin >> key->year;
		fio_s<Gr, fio> * bufer = *mod_beg;
		cout << "�������� ������ ������� : 1 - �� ���(�); 2 - ���(�)" << endl;
		while ((*mod_end)->prev != bufer) (*mod_end) = (*mod_end)->prev;
		while ((*mod_end)->next) {	
			//cout << endl;
			printf("%5i", (*mod_end)->group);
			cout << "  -  ";
			printf("%17s", (*mod_end)->familia);
			printf("%17s", (*mod_end)->name);
			printf("%17s", (*mod_end)->otchestvo);
			cout << " : "; cin >> key->presence;
			/*while (((key->presence) != 1) || ((key->presence) != 2)){
				cout << endl << "�� ����� �����������! ������� ������ : "; cin >> key->presence;
			};*/			
			key->next = 0;
			key->prev = (*mod_end)->employment;
			((*mod_end)->employment) = key;
			(*mod_end) = (*mod_end)->next;
		};
		cout << " ��� ����������� ������� Enter"; _getch();
	};
	//---------------------------------------------------------------------------------
	//������ � ������: ����� �� ����
	void output_by_data(fio_s<Gr, fio> ** mod_beg, fio_s<Gr, fio> ** mod_end){
		system("cls");
		cout << " ���������� ������� ���������, ����� ��������� ��� �� ������!! " << endl << endl;
		cout << " ����� �� ���� " << endl << endl;
		cout << " ������� ���� ������� : " << endl;
		data<Gr, fio> * key = new data<Gr, fio>;
		cout << " ���� : "; cin >> key->day;
		cout << " ����� : "; cin >> key->month;
		cout << " ��� : "; cin >> key->year;
		fio_s<Gr, fio> * bufer = *mod_beg;
		while ((*mod_end)->prev != bufer) (*mod_end) = (*mod_end)->prev;
		cout << "������ ��� ��� ������ �� ������� : " << endl;
		while ((*mod_end)->next) {	
			data<Gr, fio> * buf = (*mod_end)->employment;
			while (buf) {
				if (((buf->day) == (key->day)) && ((buf->month) == (key->month)) && ((buf->year) == (key->year)) && ((buf->presence) == 2)){
					printf("%5i", (*mod_end)->group);
					cout << "  -  ";
					printf("%17s", (*mod_end)->familia);
					printf("%17s", (*mod_end)->name);
					printf("%17s", (*mod_end)->otchestvo);
					cout << endl;
					break;				
				};
				buf = buf->prev; 
			};
			(*mod_end) = (*mod_end)->next;
		};
		cout << " ��� ����������� ������� Enter"; _getch();
	};
	//---------------------------------------------------------------------------------
	//������ � ������: ����� �� ������ ������������ ������
	void output_by_number(fio_s<Gr, fio> ** mod_beg, fio_s<Gr, fio> ** mod_end){
		system("cls");
		cout << " ���������� ������� ���������, ����� ��������� ��� �� ������!! " << endl << endl;
		cout << " ����� �� ������ ������������ ������ " << endl << endl;
		data<Gr, fio> * key = new data<Gr, fio>;
		cout << " ����� ������������ ������ : "; cin >> key->number;
		fio_s<Gr, fio> * bufer = *mod_beg;
		while ((*mod_end)->prev != bufer) (*mod_end) = (*mod_end)->prev;
		cout << "������ ��� � ���� �������� ����� ������������ ������ : " << endl;
		while ((*mod_end)->next) {	
			data<Gr, fio> * buf = (*mod_end)->employment;
			while (buf) {
				if ((buf->number) == (key->number)){
					printf("%5i", (*mod_end)->group);
					cout << "  -  ";
					printf("%17s", (*mod_end)->familia);
					printf("%17s", (*mod_end)->name);
					printf("%17s", (*mod_end)->otchestvo);
					cout << endl;
					break;				
				};
				buf = buf->prev; 
			};
			(*mod_end) = (*mod_end)->next;
		};
		cout << " ��� ����������� ������� Enter"; _getch();
	}
	//---------------------------------------------------------------------------------
	//���� ������ �� ������� ����� �� ����� 
	int menu_spi(){
		char bufer[buf];
		int chislo;
		do {
			cout << " 1 - �������� ������ �������� � ������" << endl;
			cout << " 2 - ������� �������� �� ������" << endl;
			//���� ����� ����� ������� ����� �� �������
			cout << " 3 - ������� ���� ������" << endl;
			cout << " 4 - ����� � ������� ���� " << endl;
			cin >> bufer;
			chislo = atoi(bufer);
		}while (!chislo);
		return chislo;
	};
	//---------------------------------------------------------------------------------
	//���� ������ �� ������� �������� ��������
	void menu_spisok(){
		while (1){
			system("cls");
			cout << " � ������ ��������� �� �������������� ������ ������������, ���������� ������ ��������� ��� ��������� ������!" << endl;
			cout << " ������ � � ������� " << endl << endl;
			switch(menu_spi()){
				case 1:{add_man(&element_beg, &element_end); break;};
				case 2:{remove(&element_beg, &element_end); break;};
				case 3:{output(element_beg); break;};
				case 4:{return;};
				default: cout << "���� ������� ����� �� 1 �� 4!!!" << endl; break;
			};
		};
	};
	//---------------------------------------------------------------------------------
	//���� ������ � ������ ����� �� �����
	int menu_dat(){
		char bufer[buf];
		int chislo;
		do {
			cout << " 1 - ����� �������: �������� ������������" << endl;
			cout << " 2 - ����� �� ����" << endl;
			cout << " 3 - ����� �� ������ ������������ ������" << endl;
			cout << " 4 - ����� � ������� ���� " << endl;
			cin >> bufer;
			chislo = atoi(bufer);
		}while (!chislo);
		return chislo;
	};
	//---------------------------------------------------------------------------------
	//���� ������ � ������ �������� ��������
	void menu_data(){
		while (1){
			system("cls");
			cout << " � ������ ��������� �� �������������� ������ ������������, ���������� ����� ��������� ��� ��������� ������!" << endl << endl;
			cout << " ������ � ������ " << endl << endl;
			switch(menu_dat()){
				case 1: {attendance(&element_beg, &element_end); break;};
				case 2: {output_by_data(&element_beg, &element_end); break;};
				case 3: {output_by_number(&element_beg, &element_end); break;};
				case 4: {return;};
				default: cout << "���� ������� ����� �� 1 �� 4!!!" << endl; break;
			};
		};		
	};
	//---------------------------------------------------------------------------------
	//���� ����� ������(����� � ����� ��� ������)
	int menu_work(){
		char bufer[buf];
		int chislo;
		do {
			cout << " 1 - �������� �� ������� ���������" << endl;
			cout << " 2 - �������� � ������" << endl;
			cout << " 3 - �����" << endl;
			cin >> bufer;
			chislo = atoi(bufer);
		}while (!chislo);
		return chislo;
	};
	//---------------------------------------------------------------------------------
	//������ � ����
	void filing (fio_s<Gr, fio> * const mod_beg){
		FILE * file;
		while ((file = fopen("Spisok", "wb")) == NULL) {
			cout << "ERROR OPEN\n"; return ;
		};	
		char bufer = ' ';
		fio_s<Gr, fio> *buf = mod_beg;
		while (!feof(file)){
			fprintf(file, "%i", buf->group); fputc(bufer, file);
			fprintf(file, "%20s", buf->familia); fputc(bufer, file);
			fprintf(file, "%20s", buf->name); fputc(bufer, file);
			fprintf(file, "%20s", buf->otchestvo); fputc(bufer, file);
			data<Gr, fio> *spisok = buf->employment;
			while (spisok){
				fprintf(file, "%i", spisok->day); fputc(bufer, file);
				fprintf(file, "%i", spisok->month); fputc(bufer, file);
				fprintf(file, "%i", spisok->year); fputc(bufer, file);
				fprintf(file, "%i", spisok->presence); fputc(bufer, file);
				fprintf(file, "%i", spisok->number); fputc(bufer, file);
				fprintf(file, "%i", spisok->variant); fputc(bufer, file);
				fprintf(file, "%i", spisok->control_of_performance); fputc(bufer, file);
				spisok = spisok->next;
			};
		};
		fclose(file);
	};
	//---------------------------------------------------------------------------------
	//������ �� ������� � ������
	void working_with_a_list_and_data(){
		add(&element_beg, &element_end);
		while (1){
			system("cls");
			cout << " � ������ ��������� �� �������������� ������ ������������, ���������� ������ ��������� ��� ��������� ������!" << endl << endl;
			switch(menu_work()){
				case 1: {menu_spisok(); break;};
				case 2: {menu_data(); break;};
				case 3: {return;};
				default: cout << "���� ������� ����� �� 1 �� 3!!!" << endl; break;
			};
		};	
		filing(element_beg);
	};
	//---------------------------------------------------------------------------------
};
int main()
{
	setlocale(LC_ALL, "");																	cout << " � ������ ��������� �� �������������� ������ ������������, ���������� ����� ��������� ��� ��������� ������!" << endl << endl;
	//---------------------------------------------------------------------------------
	//������ �� ������� ��������
	Date<int, char> Mod;
	Mod.working_with_a_list_and_data(); 
	//---------------------------------------------------------------------------------
	//������ � ������
	return 0;
}