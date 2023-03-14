#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSTUDENTS 32
#define DATALEN 20
#define STRLEN 100
#define MINS 2 
typedef struct student_node
{
	char surname[DATALEN];
	char name[DATALEN];
	struct date
	{
		long day;
		long month;
		long year;
	} birth_date;
	int marks[5];
	double avg_mark;
	struct student_node* next;
} stud;

void insert_node(stud** first, stud* new_stud)
{
	if (!(*first))
	{
		(*first) = new_stud;
		(*first)->next = NULL;
	}
	else
	{
		if (strcmp((*first)->surname, new_stud->surname) > 0)
		{
			new_stud->next = *first;
			*first = new_stud;
			return;
		}
		stud* curr = *first;
		while (curr->next)
		{
			if ((strcmp(curr->surname, new_stud->surname) < 0) &&
				(strcmp(new_stud->surname, curr->next->surname) < 0))
			{
				new_stud->next = curr->next;
				curr->next = new_stud;
				return;
			}
			else
				curr = curr->next;
		}
		curr->next = new_stud;
		new_stud->next = NULL;
	}
}

stud* add_student(FILE* file)
{
	char* pw;
	char str[STRLEN];
	stud* new_stud = (stud*)malloc(sizeof(stud));
	fgets(str, STRLEN, file);
	pw = strtok(str, "\t");
	strcpy(new_stud->surname, pw);
	pw = strtok(NULL, "\t");
	strcpy(new_stud->name, pw);
	pw = strtok(NULL, "\t");
	new_stud->birth_date.day = strtol(pw, &pw, 10);
	new_stud->birth_date.month = strtol(pw + 1, &pw, 10);
	new_stud->birth_date.year = strtol(pw + 1, &pw, 10);
	int marksum = 0;
	for (int i = 0; i < 5; i++)
	{
		pw = strtok(NULL, "\t");
		new_stud->marks[i] = atoi(pw);
		marksum += new_stud->marks[i];
	}
	new_stud->avg_mark = marksum / 5.0;
	new_stud->next = NULL;
	return new_stud;
}
stud* add_student_console()
{
	char str[STRLEN];
	stud* new_stud = (stud*)malloc(sizeof(stud));
	printf("Enter surname");
	scanf("%s", &str);
	strcpy(new_stud->surname, str);
	printf("Enter name");
	scanf("%s", &str);
	strcpy(new_stud->name, str);
	printf("Enter birth date");
	scanf("%s", &str);
	new_stud->birth_date.day = atoi(str);
	new_stud->birth_date.month = atoi(str+3);
	new_stud->birth_date.year = atoi(str+6);
	int marksum = 0;
	for (int i = 0; i < 5; i++)
	{
		printf("Enter mark %d", i+1);
		scanf("%s", &str);
		new_stud->marks[i] = atoi(str);
		marksum += new_stud->marks[i];
	}
	new_stud->avg_mark = marksum / 5.0;
	new_stud->next = NULL;
	return new_stud;
}

double avg_mark_all(stud* first)
{
	int i = 0;
	double avg_mark_all=0;
	while (first) {
		i++;
		avg_mark_all += first->avg_mark;
		first = first->next;
	}
	avg_mark_all /= i;

	return avg_mark_all;
}

stud* sort(stud* first, double min_mark) 
{
	stud *temp, *first_t;
		temp = first;
		first_t = temp;
	temp->next = NULL;
	while (first) 
	{
		temp = (stud*)malloc(sizeof(stud));
		if (first->avg_mark > min_mark)
		{
			temp = first;
			temp = temp->next;
		}
		first = first->next;
	}
	stud* del_temp;
	while (first) {
		del_temp = first;
		free(first);
		first = del_temp->next;
	}
	while (temp) {
		first = temp;
		temp = temp->next;
		first = first->next;
	}
	while (temp) {
		del_temp = temp;
		free(temp);
		temp = del_temp->next;
	}
		return first;
}

//void best_2_student(stud* first) {
//	stud* temp;
//	double max=first->avg_mark;
//
//	for (int i = 0; i < 2; i++) {
//		while(first)
//		if (temp == first) {
//			continue;
//		}
//		if (max < first->avg_mark) {
//			max = first->avg_mark;
//			temp = first;
//		}
//		
//	}
//
//}

void print_data(stud* first)
{
	printf("Surname Name Date of birth\tExamination marks\n");
	printf("------------------------------------------------------------------\n");
		stud * curr = first;
	while (curr)
	{
		printf("%-16s%-12s%02i.%02i.%04i\t\t%-3i %-3i %-3i %-3i %-3i\n", curr -> surname, curr->name,curr->birth_date.day, curr->birth_date.month, curr -> birth_date.year,curr->marks[0], curr->marks[1], curr->marks[2], curr->marks[3],
			curr->marks[4]);
		curr = curr->next;
	}
}
void max_mark(stud* first, stud** mins)
{
	for (int i = 0; i < MINS; i++)
	{
		stud* curr = first;
		while (curr)
		{
			if (!mins[i])
			{
				mins[i] = curr;
			}
			else
				if (mins[i]->avg_mark < curr->avg_mark)
				{
					int used = 0;
					for (int j = 0; j < MINS; j++)
						if (curr == mins[j])
							used = 1;
					if (!used)
						mins[i] = curr;
				}
			curr = curr->next;
		}
	}
}

int main()
{
	stud* first_stud = NULL;
	stud* curr_stud = NULL;
	FILE* file = fopen("db.txt", "r");
	stud** mins = (stud**)calloc(MINS, sizeof(stud*));
	while (!feof(file))
	{
		curr_stud = add_student(file);
		insert_node(&first_stud, curr_stud);
	}
	fclose(file);
	print_data(first_stud);
				/*printf("\n\nTop 2 best students:\n");
				printf("Surname Name Date of birth\tExamination marks\n");
				printf("------------------------------------------------------------------\n");
				max_mark(first_stud, mins);
				for (int i = 0; i < MINS; i++)
				{
					printf("%-16s%-12s%02i.%02i.%04i\t\t%-3i %-3i %-3i %-3i %-3i\n",
						mins[i]->surname, mins[i]->name,
						mins[i]->birth_date.day, mins[i]->birth_date.month, mins[i]->birth_date.year,
						mins[i]->marks[0], mins[i]->marks[1], mins[i]->marks[2],
						mins[i]->marks[3], mins[i]->marks[4]);
				}
				printf("-----------------------------------------------------------------\n");
				first_stud = sort(first_stud, avg_mark_all(first_stud));*/
	first_stud = sort(first_stud, avg_mark_all(first_stud));
	print_data(first_stud);
	free(mins);
	return 0;
}

//int main()
//{
//	stud* first_stud = NULL;
//	stud* curr_stud = NULL;
//	FILE* file = fopen("db.txt", "r");
//	stud** mins = (stud**)calloc(MINS, sizeof(stud*));
//	while (!feof(file))
//	{
//		curr_stud = add_student(file);
//		insert_node(&first_stud, curr_stud);
//	}
//	fclose(file);
//	print_data(first_stud);
//	printf("\n\nTop 2 best students:\n");
//	printf("Surname Name Date of birth\tExamination marks\n");
//	printf("------------------------------------------------------------------\n");
//	max_mark(first_stud, mins);
//	for (int i = 0; i < MINS; i++)
//	{
//		printf("%-16s%-12s%02i.%02i.%04i\t\t%-3i %-3i %-3i %-3i %-3i\n",
//			mins[i]->surname, mins[i]->name,
//			mins[i]->birth_date.day, mins[i]->birth_date.month, mins[i]->birth_date.year,
//			mins[i]->marks[0], mins[i]->marks[1], mins[i]->marks[2],
//			mins[i]->marks[3], mins[i]->marks[4]);
//	}
//	printf("-----------------------------------------------------------------\n");
//		first_stud = sort(first_stud, avg_mark_all(first_stud));
//	print_data(first_stud);
//	free(mins);
//	return 0;
//}