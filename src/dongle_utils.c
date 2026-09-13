#include "codexion.h"

void create_dongles(t_simulation *simulation)
{
    int i;

    i = 0;
    while (i < simulation->cli_params.number_of_coders)
    {
        simulation->dongles[i].id = i;
        simulation->dongles[i].is_available = 1;
        i++;
    }
}

bool take_dongle(t_coder *coder)
{
    pthread_mutex_lock(&coder->simulation->mutex);
    while (!coder->dongles[0]->is_available || !coder->dongles[1]->is_available )
    {
        pthread_cond_wait(&coder->simulation->condition, &coder->simulation->mutex);
    }
    coder->dongles[0]->is_available = 0;
    coder->dongles[1]->is_available = 0;
    pthread_mutex_unlock(&coder->simulation->mutex);
    return (1);
}

void release_dongle(t_coder *coder)
{
    pthread_mutex_lock(&coder->simulation->mutex);
    coder->dongles[0]->is_available = 1;
    coder->dongles[1]->is_available = 1;
    pthread_cond_broadcast(&coder->simulation->condition);
    pthread_mutex_unlock(&coder->simulation->mutex);
}