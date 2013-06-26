#include <pthread.h>
#include <stdio.h>
#include <malloc.h>

typedef struct my_struct_tag {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int value;
} my_struct_t;



int  main (int argc, char *argv[])
{
	my_struct_t *data;
	int status;

	data = (my_struct_t *) malloc (sizeof (my_struct_t));
	if (data == NULL)
		printf ("Allocate structure");
	status = pthread_mutex_init (&data->mutex, NULL);
	if (status != 0){
		printf ("Init mutex failed\n");
		return -1;
	}

	status = pthread_cond_init (&data->cond, NULL);
	if (status != 0) {
		printf ("Init condition\n");
		return -1;
	}
	status = pthread_cond_destroy (&data->cond);
	if (status != 0) {
		printf ("Destroy condition failed!\n");
		return -1;
	}
	status = pthread_mutex_destroy (&data->mutex);
	if (status != 0) {
		printf ("Destroy mutex failed!");
		return -1;
	}
	free (data);
	return status;
}
