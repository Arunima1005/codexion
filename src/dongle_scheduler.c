#include "codexion.h"

static bool	register_request(t_coder *coder)
{
	coder->request_time = get_time_ms();
	coder->request_order = coder->simulation->next_request_order++;
	if (!add_coder_to_queue(coder->dongles[0], coder))
		return (false);
	if (!add_coder_to_queue(coder->dongles[1], coder))
		return (false);
	return (true);
}

static bool	try_grant_dongles(t_coder *coder, long now)
{
	t_dongle	*a;
	t_dongle	*b;

	a = coder->dongles[0];
	b = coder->dongles[1];
	if (a == b)
		return (false);
	if (!a->is_available || a->available_time > now)
		return (false);
	if (!b->is_available || b->available_time > now)
		return (false);
	if (a->queue.size == 0 || a->queue.coders[0] != coder)
		return (false);
	if (b->queue.size == 0 || b->queue.coders[0] != coder)
		return (false);
	a->is_available = false;
	b->is_available = false;
	priority_queue_pop(&a->queue);
	priority_queue_pop(&b->queue);
	return (true);
}

static long	compute_wake_time(t_coder *coder, long now)
{
	long	wake_time;

	wake_time = coder->dongles[0]->available_time;
	if (coder->dongles[1]->available_time > wake_time)
		wake_time = coder->dongles[1]->available_time;
	if (wake_time <= now)
		wake_time = now + 5;
	return (wake_time);
}

static bool	announce_grant(t_coder *coder)
{
	pthread_mutex_unlock(&coder->simulation->mutex);
	log_state(coder, "has taken a dongle");
	log_state(coder, "has taken a dongle");
	return (true);
}

bool	take_dongle(t_coder *coder)
{
	t_simulation		*simulation;
	struct timespec		timeout;
	long				now;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex);
	if (!register_request(coder))
	{
		pthread_mutex_unlock(&simulation->mutex);
		return (false);
	}
	while (!simulation->simulation_stop)
	{
		now = get_time_ms();
		if (try_grant_dongles(coder, now))
			return (announce_grant(coder));
		get_timespec(compute_wake_time(coder, now), &timeout);
		pthread_cond_timedwait(&simulation->condition, &simulation->mutex,
			&timeout);
	}
	priority_queue_remove(&coder->dongles[0]->queue, coder);
	priority_queue_remove(&coder->dongles[1]->queue, coder);
	pthread_mutex_unlock(&simulation->mutex);
	return (false);
}