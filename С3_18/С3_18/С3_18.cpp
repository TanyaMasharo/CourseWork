// —3_18.cpp: определ€ет точку входа дл€ консольного приложени€.
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

//даты, посещаемость зан€тий, сдача работ
template <class Gr, class fio>
struct data{
	Gr day;//дата(день)
	Gr month;//мес€ц
	Gr year;//год
	Gr presence;//присутствие
	Gr number;//номер работы
	Gr variant;//вариант
	Gr control_of_performance;//контроль выполнени€
	data *next;
	data *prev;
};
template <class Gr, class fio>
struct fio_s{
	Gr group;//группа
	fio familia[length+1];//фамили€
	fio name[length+1];//им€
	fio otchestvo[length+1];//отчество
	data<Gr, fio> *employment;//зан€ти€
	fio_s *next;
	fio_s *prev;	
};
template <class Gr, class fio>
class Date{
public:	
	fio_s<Gr, fio> * element_beg;
	fio_s<Gr, fio> * element_end;
	//---------------------------------------------------------------------------------
	//формирую первый элемент моего полного списка учеников
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
	// онструктор
	Date(){
		element_beg = first();
		element_end = element_beg;
	};
	//---------------------------------------------------------------------------------
	//ƒеструктор
	~Date(){

	};
	//---------------------------------------------------------------------------------
	//поиск элемента по заданному ключу...(на данный момент по фамилии...)
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
	//---------------------------------------------------------------------------------                                       // пересмотреть сортировку
	//ƒобавление элементов одновременно с сортировкой
	void add_sort(fio_s<Gr, fio> **mod_beg, fio_s<Gr, fio> **mod_end, fio_s<Gr, fio> * element){
		fio_s<Gr, fio> *pv = new fio_s<Gr, fio>; //добавл€емый элемент
		pv = element;
		fio_s<Gr, fio> *prim = *mod_beg;
		while (prim->next){  //просмотр списка
			if (((prim->group) == (pv->group)) && (((prim->next->group) > (pv->group)) || (prim->next == 0))) {
				if ((prim->familia) == (pv->familia)) {
					if ((prim->name) == (pv->name)){
						if ((prim->otchestvo) == (pv->otchestvo)){
							cout << endl << "“акой студент уже существует в списке" << endl; 
						} else goto metka;
					} else goto metka;
				} else if (((prim->familia) < (pv->familia)) && (((prim->next)->familia) > (pv->familia))){
					goto metka;
				} else goto metka;
			} else if (((prim->group) < (pv->group)) && ((prim->next->group) > (pv->group))) {
				metka:	
				pv->prev = prim;
				if (prim == *mod_beg){ // в начало
					pv->prev = 0;
					*mod_beg = pv;
				} else { // в середину
					(prim->prev)->prev = pv;
					pv->next = prim->next;
				};
				prim->next = pv;
				return;
			};
			prim = prim->next;
		}
		pv->next = 0;  //в конец списка
		pv->prev = *mod_end;
		(*mod_end)->next = pv;
		*mod_end = pv;
	};
	//---------------------------------------------------------------------------------
	//ƒобавление всех оставшихс€ ученика из файла в конец списка
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
	//”даление элемента по ключу
	bool remove(fio_s<Gr, fio> **mod_beg, fio_s<Gr, fio> ** mod_end){
		fio_s<Gr, fio> *pv = new fio_s<Gr, fio>;
		cout << " ¬вод данных делать на јнглийском!!!" << endl;
		cout << " ѕожалуйста вводите правильно, иначе программа вас не поймет!! " << endl << " “акже вводите ‘»ќ с большой буквы!!" << endl << "¬водите ‘»ќ английскими символами!" << endl << endl;
		cout << " ¬ведите группу ученика : "; cin >> pv->group;
		cout << " ¬ведите фамилию ученика : "; cin >> pv->familia;
		cout << " ¬ведите им€ ученика : "; cin >> pv->name;
		cout << " ¬ведите отчество ученика : "; cin >> pv->otchestvo;		
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
	//ƒобавление нового ученика в конец списка
	void add_man(fio_s<Gr, fio> ** mod_beg,fio_s<Gr, fio> **mod_end){
		fio_s<Gr, fio> *pv = new fio_s<Gr, fio>;
		cout << " ѕожалуйста вводите правильно, иначе программа вас не поймет!! " << endl << " “акже вводите ‘»ќ с большой буквы!!" << endl << "¬водите ‘»ќ английскими символами!" << endl << endl;
		cout << " ¬ведите группу ученика : "; cin >> pv->group;
		cout << " ¬ведите фамилию ученика : "; cin >> pv->familia;
		cout << " ¬ведите им€ ученика : "; cin >> pv->name;
		cout << " ¬ведите отчество ученика : "; cin >> pv->otchestvo;
		add_sort(mod_beg, mod_end, pv);
	};
	//---------------------------------------------------------------------------------
	//¬ывод на экран
	void output(fio_s<Gr, fio> * const mod_beg){
		cout << "¬ывод всего списка " << endl << endl;//вывод группа - фио
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
		
		cout << endl << endl << "Ќажмите Enter "; _getch();
	};
	//---------------------------------------------------------------------------------
	//–абота с датами: ѕосещаемость
	void attendance(fio_s<Gr, fio> ** mod_beg, fio_s<Gr, fio> ** mod_end){
		system("cls");
		cout << " ѕожалуйста вводите правильно, иначе программа вас не поймет!! " << endl << endl;
		cout << " ѕосещаемость " << endl << endl;
		cout << " ¬ведите дату зан€ти€ : " << endl;
		data<Gr, fio> * key = new data<Gr, fio>;
		cout << " ƒень : "; cin >> key->day;
		cout << " ћес€ц : "; cin >> key->month;
		cout << " √од : "; cin >> key->year;
		fio_s<Gr, fio> * bufer = *mod_beg;
		cout << "Ќапротив каждой фамилии : 1 - не был(а); 2 - бал(а)" << endl;
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
				cout << endl << "¬ы ввели неправильно! ¬ведите заново : "; cin >> key->presence;
			};*/			
			key->next = 0;
			key->prev = (*mod_end)->employment;
			((*mod_end)->employment) = key;
			(*mod_end) = (*mod_end)->next;
		};
		cout << " ƒл€ продолжени€ нажмите Enter"; _getch();
	};
	//---------------------------------------------------------------------------------
	//–аботы с датами: ¬ывод по дате
	void output_by_data(fio_s<Gr, fio> ** mod_beg, fio_s<Gr, fio> ** mod_end){
		system("cls");
		cout << " ѕожалуйста вводите правильно, иначе программа вас не поймет!! " << endl << endl;
		cout << " ¬ывод по дате " << endl << endl;
		cout << " ¬ведите дату зан€ти€ : " << endl;
		data<Gr, fio> * key = new data<Gr, fio>;
		cout << " ƒень : "; cin >> key->day;
		cout << " ћес€ц : "; cin >> key->month;
		cout << " √од : "; cin >> key->year;
		fio_s<Gr, fio> * bufer = *mod_beg;
		while ((*mod_end)->prev != bufer) (*mod_end) = (*mod_end)->prev;
		cout << "—писок тех кто €вилс€ на зан€тие : " << endl;
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
		cout << " ƒл€ продолжени€ нажмите Enter"; _getch();
	};
	//---------------------------------------------------------------------------------
	//–абота с датами: ¬ывод по номеру лабораторной работы
	void output_by_number(fio_s<Gr, fio> ** mod_beg, fio_s<Gr, fio> ** mod_end){
		system("cls");
		cout << " ѕожалуйста вводите правильно, иначе программа вас не поймет!! " << endl << endl;
		cout << " ¬ывод по номеру лабораторной работы " << endl << endl;
		data<Gr, fio> * key = new data<Gr, fio>;
		cout << " Ќомер лабораторной работы : "; cin >> key->number;
		fio_s<Gr, fio> * bufer = *mod_beg;
		while ((*mod_end)->prev != bufer) (*mod_end) = (*mod_end)->prev;
		cout << "—писок тех у кого введеный номер лабораторной работы : " << endl;
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
		cout << " ƒл€ продолжени€ нажмите Enter"; _getch();
	}
	//---------------------------------------------------------------------------------
	//ћеню работы со списком вывод на экран 
	int menu_spi(){
		char bufer[buf];
		int chislo;
		do {
			cout << " 1 - ƒобавить нового студента в список" << endl;
			cout << " 2 - ”далить студента из списка" << endl;
			//если будет врем€ сделать вывод по группам
			cout << " 3 - ¬ывести весь список" << endl;
			cout << " 4 - ¬ыход в главное меню " << endl;
			cin >> bufer;
			chislo = atoi(bufer);
		}while (!chislo);
		return chislo;
	};
	//---------------------------------------------------------------------------------
	//ћеню работы со списком передача функци€м
	void menu_spisok(){
		while (1){
			system("cls");
			cout << " ¬ данной программе не предусмотренны ошибки пользовател€, пожалуйста будьте аккуратны при изменении данных!" << endl;
			cout << " –абота с о списком " << endl << endl;
			switch(menu_spi()){
				case 1:{add_man(&element_beg, &element_end); break;};
				case 2:{remove(&element_beg, &element_end); break;};
				case 3:{output(element_beg); break;};
				case 4:{return;};
				default: cout << "Ќадо вводить числа от 1 до 4!!!" << endl; break;
			};
		};
	};
	//---------------------------------------------------------------------------------
	//ћеню работы с датами вывод на экран
	int menu_dat(){
		char bufer[buf];
		int chislo;
		do {
			cout << " 1 - Ќовое зан€тие: отметить посещаемость" << endl;
			cout << " 2 - ¬ывод по дате" << endl;
			cout << " 3 - ¬ывод по номеру лабораторной работы" << endl;
			cout << " 4 - ¬ыход в главное меню " << endl;
			cin >> bufer;
			chislo = atoi(bufer);
		}while (!chislo);
		return chislo;
	};
	//---------------------------------------------------------------------------------
	//ћеню работы с датами передача функци€м
	void menu_data(){
		while (1){
			system("cls");
			cout << " ¬ данной программе не предусмотренны ошибки пользовател€, пожалуйста будте аккуратны при изменении данных!" << endl << endl;
			cout << " –абота с датами " << endl << endl;
			switch(menu_dat()){
				case 1: {attendance(&element_beg, &element_end); break;};
				case 2: {output_by_data(&element_beg, &element_end); break;};
				case 3: {output_by_number(&element_beg, &element_end); break;};
				case 4: {return;};
				default: cout << "Ќадо вводить числа от 1 до 4!!!" << endl; break;
			};
		};		
	};
	//---------------------------------------------------------------------------------
	//ћеню общей работы(выбор к датам или списку)
	int menu_work(){
		char bufer[buf];
		int chislo;
		do {
			cout << " 1 - –аботать со списком студентов" << endl;
			cout << " 2 - –аботать с датами" << endl;
			cout << " 3 - ¬ыход" << endl;
			cin >> bufer;
			chislo = atoi(bufer);
		}while (!chislo);
		return chislo;
	};
	//---------------------------------------------------------------------------------
	//«апись в файл
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
	//работа со списком и датами
	void working_with_a_list_and_data(){
		add(&element_beg, &element_end);
		while (1){
			system("cls");
			cout << " ¬ данной программе не предусмотренны ошибки пользовател€, пожалуйста будьте аккуратны при изменении данных!" << endl << endl;
			switch(menu_work()){
				case 1: {menu_spisok(); break;};
				case 2: {menu_data(); break;};
				case 3: {return;};
				default: cout << "Ќадо вводить числа от 1 до 3!!!" << endl; break;
			};
		};	
		filing(element_beg);
	};
	//---------------------------------------------------------------------------------
};
int main()
{
	setlocale(LC_ALL, "");																	cout << " ¬ данной программе не предусмотренны ошибки пользовател€, пожалуйста будте аккуратны при изменении данных!" << endl << endl;
	//---------------------------------------------------------------------------------
	//работа со списком учеников
	Date<int, char> Mod;
	Mod.working_with_a_list_and_data(); 
	//---------------------------------------------------------------------------------
	return 0;
}
