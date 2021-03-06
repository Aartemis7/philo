#include "phile_one.h"

typedef struct timeval t_timeval;

t_timeval diff_time(t_timeval t1, t_timeval t2)
{
    unsigned long time1;
    unsigned long time2;
    t_timeval ret;

    time1 = t1.tv_sec * 1000000 + t1.tv_usec;
    time2 = t2.tv_sec * 1000000 + t2.tv_usec;
    time2 -= time1;
    ret.tv_sec = time2 / 1000000;
    ret.tv_usec = (int) time2 - ret.tv_sec * 1000000;
	return (ret);
}

void	*ft_philosopher(void *param)
{
	t_philo *philo;
	int j;
	t_timeval t_start;
	t_timeval last_time_eat;
	t_timeval t_now;
	t_timeval t_s;
	t_timeval t_time;
	int state; // 0 = philo / 1 = eat / 2 = sleep

	gettimeofday(&t_start, NULL);
	philo = (t_philo*)param;
	gettimeofday(&t_now, NULL);
	t_now = diff_time(t_start, t_now);
	last_time_eat = t_start;
	state = 0;
	printf("Lancement du philosophe %d a %ld:%.6d\n", philo->number, t_now.tv_sec, t_now.tv_usec);
	j = 0;
	while (j < philo->number_of_time_each_philosophers_must_eat)
	{
		gettimeofday(&t_now, NULL);
		t_now = diff_time(t_start, t_now);

		if (state == 0) // il philo
		{
			t_time = diff_time(last_time_eat, t_now);
			if (t_time.tv_sec * 1000000 + t_time.tv_usec > philo->time_to_die)
			{
				printf("Philosophe %d decede a %ld:%.6d\n", philo->number, t_now.tv_sec, t_now.tv_usec);
				philo->dead = 1;
				return(NULL);
			}
			if (1) //si il reussi
			{
				state = 1;
				printf("Philosophe %d mange a %ld:%.6d\n", philo->number, t_now.tv_sec, t_now.tv_usec);
				t_s = t_now;
			}
		}
		else if (state == 1) //il mange
		{
			t_time = diff_time(t_s, t_now);
			if (t_time.tv_sec * 1000000 + t_time.tv_usec >= philo->time_to_eat)
			{
				last_time_eat = t_now;
				printf("Philosophe %d a fini de manger a %ld:%.6d et va dormir\n", philo->number, t_now.tv_sec, t_now.tv_usec);
				state = 2;
				j++;
				t_s = t_now;
			}
		}
		else if (state == 2) //il dort
		{
			if (t_time.tv_sec * 1000000 + t_time.tv_usec > philo->time_to_die)
			{
				printf("Philosophe %d decede a %ld:%.6d\n", philo->number, t_now.tv_sec, t_now.tv_usec);
				philo->dead = 1;
				return(NULL);
			}
			t_time = diff_time(t_s, t_now);
			if (t_time.tv_sec * 1000000 + t_time.tv_usec >= philo->time_to_sleep)
			{
				printf("Philosophe %d a fini de dormir a %ld:%.6d et va philosopher\n", philo->number, t_now.tv_sec, t_now.tv_usec);
				state = 0;
			}

		}
	}
	philo->dead = 2;
	return (NULL);
}

void copy_struct(t_philo *paste, t_philo copy)
{
	paste->number_of_philosopher = copy.number_of_philosopher;
	paste->number_of_time_each_philosophers_must_eat = copy.number_of_time_each_philosophers_must_eat;
	paste->time_to_die = copy.time_to_die;
	paste->time_to_eat = copy.time_to_eat;
	paste->time_to_sleep = copy.time_to_sleep;
}

void create_start_philo(int nbr, t_philo philo)

{
	t_philo arr[nbr];
	pthread_t th[nbr];
	int j;

	j = 0;
	printf("%d\n", nbr);
	while (j < philo.number_of_philosopher)
	{
		copy_struct(&arr[j], philo);
		arr[j].dead = 0;
		arr[j].number = j;
		pthread_create(&th[j], NULL, &ft_philosopher, &arr[j]);
		++j;
	}
	pthread_mutext_t mutext[philo.number_of_philosopher];
	j = 0;
	while (1)
	{
		j = 0;
		while (j < philo.number_of_philosopher)
		{
			philo.mutext1 = &mutext[j];
			philo.mutext2 = (j + 1 == philo.number_of_philosopher) ?
				&mutext[0]: &mutext[j + 1];
			if (arr[j].dead == 1)
			{
				printf("philosophe nbr: %d est mort, arret\n", j);
				exit(1);
			}
			++j;
		}
		j = 0;
		while (arr[j].dead == 2)
		{
			if (j == philo.number_of_philosopher - 1)
			{
				printf("CONGRATULATION THEY WIN\n");
				exit(0);
			}
			++j;
		}
		usleep(1000);
	}
}

int main(int argc, char **argv)
{
	t_philo philo;
	int j;

	srand(time(0));
	if (fill_and_error(&philo, argv, argc) == 1)
		return (1);
	create_start_philo(philo.number_of_philosopher, philo);
	return (0);
}
