/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: siwhusse <siwhusse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 00:56:29 by siwhusse          #+#    #+#             */
/*   Updated: 2026/02/10 17:52:55 by siwhusse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_routine(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time();
	print_state(philo, "is eating");
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(data->time_to_eat * 1000, philo);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	check_which_first(t_philo *philo)
{
	if (philo->id % 2 == 1)
		precise_sleep(200, philo);
	if (philo->left_fork < philo->right_fork)
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
	}
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (!check_stop(data))
	{
		print_state(philo, "is thinking");
		if (check_stop(data))
			break ;
		if (data->num_philos == 1 && print_state(philo, "has taken a fork"))
		{
			precise_sleep(data->time_to_die * 1000, philo);
			return (NULL);
		}
		check_which_first(philo);
		check_routine(philo, data);
		if (data->must_eat > 0
			&& philo->meals_eaten >= data->must_eat)
			break ;
		print_state(philo, "is sleeping");
		precise_sleep(data->time_to_sleep * 1000, philo);
	}
	return (NULL);
}
