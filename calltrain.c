#include<pthread.h>
#include "calltrain_base_files.h"

void station_init(struct station *station)
{
	station -> out_passengers = 0;
	station -> in_passengers = 0;
	pthread_mutex_init(&(station -> lock), NULL);
	pthread_cond_init(&(station -> train_arrived_cond),NULL);
	pthread_cond_init(&(station -> passengers_seated_cond),NULL);
	pthread_cond_init(&(station -> train_is_full_cond),NULL);


}

void station_load_train(struct station *station, int count)
{
	pthread_mutex_lock(&(station -> lock));
	while ((station -> out_passengers > 0) && (count > 0)){
		pthread_cond_signal(&(station -> train_arrived_cond));
		count--;
		pthread_cond_wait(&(station -> passengers_seated_cond), &(station -> lock));		}
	if (station -> in_passengers > 0){
		pthread_cond_wait(&(station -> train_is_full_cond), &(station -> lock));
	}

	pthread_mutex_unlock(&(station -> lock));
}

void station_wait_for_train(struct station *station)
{
	pthread_mutex_lock(&(station -> lock));
	station -> out_passengers++;
	pthread_cond_wait(&(station -> train_arrived_cond), &(station -> lock));
	station -> out_passengers--;
	station -> in_passengers++;

	pthread_mutex_unlock(&(station -> lock));

	pthread_cond_signal(&(station -> passengers_seated_cond));
}

void station_on_board(struct station *station)
{
	pthread_mutex_lock(&(station -> lock));
	station -> in_passengers--;
	pthread_mutex_unlock(&(station -> lock));

	if (station -> in_passengers == 0){
		pthread_cond_broadcast(&(station -> train_is_full_cond));
	}
}

