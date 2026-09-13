#include "codexion.h"

long get_time_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	return ((long)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void get_usleep_ms(long ms)
{
    usleep(ms * 1000);
}

long get_elapsed_time(t_simulation *simulation)
{
    return (get_time_ms() - simulation->start_time);
}
