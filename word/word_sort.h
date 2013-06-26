#ifndef _WORD_SORT_H
#define _WORD_SORT_H

typedef struct word_list
{
	char word[20];
	char info[100];
	struct word_list *next;

}word_list;

typedef struct head_letter
{
	char index;
	struct  head_letter *next;
	struct word_list *word_head;
	int num;

}head_letter;

typedef head_letter *head_index;

int init_head(const char *file, head_letter *head, FILE **fd);
int read_word(FILE *file, head_letter *head);
head_letter* find_index(const char *word, head_letter *head);
int sort_word(word_list *wordinfo, head_letter *head);
int write_back(const char *file, head_letter *head);

#endif
