#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "word_sort.h"

static char *up2low(const char *up, char *low, int max)
{
	int i = 0;

	assert(up || low);

	while (i < max && *up)
	{
		low[i++] = tolower(*up++);
	}

	low[i] = 0;

	return low;
}

int main(int argc, char **argv)
{
	FILE *src_fd;
	head_letter headinfo = {0, NULL,NULL, 0};

	if (argc < 3)
	{
		fprintf(stderr, "Argument error");
		exit (1);
	}

	if (init_head(argv[1], &headinfo, &src_fd) < 0)
	{
		exit (1);
	}

	read_word(src_fd, &headinfo);
	write_back(argv[2], &headinfo);

	return 0;

}

int init_head(const char *file, head_letter *head, FILE **fd)
{
	head_letter *new;
	char ch;

	for (ch = 'z'; ch >= 'a'; ch--)
	{
		if ((new = (head_letter *)malloc(sizeof (head_letter))) == NULL)
		{
			perror("Malloc error");
			exit(errno);
		}

		new->num = 0;
		new->index = ch;
		new->word_head = NULL;

		new->next = head->next;
		head->next = new;
	}

	if ((*fd = fopen(file, "r")) == NULL)
	{
		perror("open error");
		return -1;
	}
	return 0;
}

int read_word(FILE *fd, head_letter *head)
{
	char buf[1024];
	char *word;
	int i;
	char fword[20];
	word_list *wordinfo;
	head_letter *head_tmp;

	if (fd == NULL || head == NULL)
	{
		fprintf(stderr, "file or headlist not exist.\n");
		return -1;
	}

	while (fgets(buf, 1024, fd) != NULL)
	{
		if (buf[0] == '\r' || buf[0] == '\n')
			continue;


		word = buf;
		while (*word == ' ')
			word++;

		if (strncmp("UNIT",	word, 4) == 0)
			continue;

		if (( wordinfo = (word_list*) malloc(sizeof (word_list)) ) == NULL)	
		{
			perror("Malloc error");
			exit(errno);
		}

		i = 0;
		while (*word != ' ')
			fword[i++] = *word++;
		fword[i] = '\0';
		strncpy(wordinfo->word, fword, strlen(fword));
		strncpy( wordinfo->info, word, 1024);
		wordinfo->next = NULL;

		head_tmp = find_index(wordinfo->word, head);
		sort_word(wordinfo, head_tmp);
	}

	return 0;
}

head_letter* find_index(const char *word, head_letter *head)
{
	head_letter *tmp = head->next;
	
	while (tmp && (tolower(word[0]) != tmp->index))
		tmp = tmp->next;

	return tmp;
}

int sort_word(word_list *wordinfo, head_letter *head_index)
{
	word_list *word_tmp = head_index->word_head;

	char low_word[20] = {0};
	char low_old[20] = {0};

	up2low(wordinfo->word, low_word, 20);

	//char *low_word = wordinfo->word;
	
	if (word_tmp == NULL)
	{
		head_index->word_head = wordinfo;
		return 0;
	}

	up2low(word_tmp->word, low_old, 20);

	if (strcmp(low_word, low_old) < 0)
	{
		wordinfo->next = head_index->word_head;
		head_index->word_head = wordinfo;
		return 0;
	}

//	up2low(word_tmp->next->word, low_old, 20);

	while (	word_tmp->next )
	{
		up2low(word_tmp->next->word, low_old, 20);

		if (strcmp(low_word, low_old) > 0)
		{
			word_tmp = word_tmp->next;
		}
		else
		{
			break;
		}
	}

	wordinfo->next = word_tmp->next;
	word_tmp->next = wordinfo;

	head_index->num++;

	return 0;
}

int write_back(const char *file, head_letter *head)
{
	FILE *dst_fd;
	char buf[1024];
	char *word;
	word_list *wordinfo;
	head_letter *head_tmp = head->next;

	if ((dst_fd = fopen(file, "w+")) == NULL)
	{
		perror("dest file error");
		return -1;
	}
	
	while (head_tmp != NULL)
	{
		wordinfo = head_tmp->word_head;
		head->num += head_tmp->num;

		while (wordinfo != NULL)
		{
			fprintf(dst_fd, "%s %s", wordinfo->word, wordinfo->info);
			wordinfo = wordinfo->next;
		}
		fprintf(dst_fd, "\n\n");
		head_tmp = head_tmp->next;
	}

	fclose(dst_fd);
	return 0;
}

