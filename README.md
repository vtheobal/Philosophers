### Запрос  :
  ```
make && ./philo arg1 arg2 arg3 arg4 arg5
  ```

### Этапы разработки  :
- Этап 1 - Изучаем subject
- Этап 2 - Знакомство с потоками и мьютексами
- Этап 3 - Стратегия
- Этап 4 - Задержки
- Этап 5 - Утечки, сбои и гонки данных
- Этап 6 - Тесты
- Utilites des variables dans ma structure et culture g prog parallèle

# Этап 1 : Изучаем subject 
Задача проекта : не допускать смерти философов

- Каждый философ — это поток, **thread**, а каждая вилка — это мьютекс **mutex**
- Философы живут по расписанию (по порятку): едят - спят - думают (на самом деле они не думают, они ждут, пока их вилка освободится)
- На каждого философа приходится по одно вилке; чтобы поесть им нужно две вилки - своя и вилка соседа
- Если один из философов умирает, симуляция останавливается, и смерть должна отображаться максимум через 10 миллисекунд
- Необходимо фиксировать состояние каждого филосова

./philo arg1 arg2 arg3 arg4 arg5

- arg1 = number_of_philosophers (количество философов)
- arg2 = time_to_die (если философ не ел в течение time_to_die миллисекунд, он умирает)
- arg3 = time_to_eat (время еды с двумя вилками в миллисекундах)
- arg4 = time_to_sleep (время сна в миллисекундах)
- arg5 = number_of_times_each_philosopher_must_eat (сколько раз каждый философ должен есть, arg5 необязателен)


# Этап 2 - Знакомство с потоками и мьютексами

Всегда существует как минимум один поток для каждого процесса. Но мы можем создать больше потоков, каждый из которых включает в себе уникальные элементы всех других потоков того же процесса (раздел кода, раздел данных, ресурсы операционной системы, такие как открытые файлы и сигналы)...

Библиотеки, позволяющие управлять потоками: pthread и openmp

```
pthread_create(&p->ph[i].thread_id, NULL, thread, &p->ph[i]); //создать поток с помощью pthread
```

Если два потока одного и того же процесса хотят одновременно получить доступ к одной и той же переменной общей памяти, это создает неопределенное поведение (см. гонки данных ниже). Отсюда и возникает потребность в использование мьютексов. Мьютексы блокируют часть кода, и другим потокам приходится ждать, чтобы выполнить эту часть кода. Как ключ от туалета, каждый по очереди.

Например, если мы удалим мьютексы в примере ниже, то str, напечатанная потоком 1, перекрывается str, напечатанной потоком 2: "tread 1 :t rmeoaid f1i l:o smoofiy  f!i losofy !"

```
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef	struct              s_p
{
    pthread_mutex_t         mutex;
    int                     i;
}                           t_p;

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return (i);
}

void    *go1(void *pp)
{
    char *str;
    int i = 0;
    t_p *ppp;

    ppp = (t_p *)pp;
    str = "tread 1 : moi filosofy ! \n";
    pthread_mutex_lock(&ppp->mutex); // если убрать здесь
    while (str[i])
    {
        write(1, &str[i], 1);
        i++;
    }
    pthread_mutex_unlock(&ppp->mutex); // если убрать здесь
    return (NULL);
}

int main()
{
    pthread_t               thread_id1;
    pthread_t               thread_id2;
    t_p                     p;
    t_p                     *pp;

    pp = (malloc(sizeof(t_p) * 1));
    pp = &p;
    p.i = 3;

    pthread_mutex_init(&p.mutex, NULL);			// обязательно для инициализации мьютекса
    pthread_create(&thread_id1, NULL, go1, (void *)pp);
    pthread_create(&thread_id2, NULL, go1, (void *)pp);
    sleep(1);
}
```

И ещё : 
- Каждая вилка имеет свой собственный мьютекс, который позволяет заблокировать ее, когда ее подбирает философ.
- Мы также используем общий для всех философов мьютекс, который позволяет печатать текст без смешивания, как в примере выше.


# Этап 3 - Стратегия

- Необходимо сделать так, чтобы четные или нечетные философы брали вилки с задержкой. Если все философы начнут есть одновременно и возьмут правую вилку, никто не сможет есть (если все могут есть, то никто не может есть).
```
	if (ph->ph_id % 2 == 0)
		ft_sleep(ph->eat_tm / 2);
```

- Каждый философ имеет свою вилку слева (l_f) и заимствует вилку своего соседа справа благодаря указателю (\*r_f), указывающему на l_f соседа справа:

```
while (i < data->cnt_ph)
{
	pthread_mutex_init(&data->ph[ind].l_f, NULL);	// каждый философ держит свою вилку слева
	data->ph[ind].ph_id = ind + 1;
	if (ind == data->cnt_ph - 1)
		data->ph[ind].r_f = &data->ph[0].l_f;		// берёт вилку у соседа справа
	else
		data->ph[ind].r_f = &data->ph[ind + 1].l_f;	// берёт вилку у соседа справа
}
```

- Необходимо обязательно проверять смерти философов в отдельном потоке, иначе вовремя не понять, если один из них умрет. В случае обнаружения смерти одного из философов необходимо немедленно закончить работу (нет смысла программе работать в холостую). Таким образом, каждый раз, когда философ отправляется заниматься своими делами, запускается поток, проверяющий статус философа.

```
pthread_create(&death, NULL, check_death, data);

int	check_death_dop(t_philo ph)
{
	if (ph.cnt_eat >= ph.das->must_eat && ph.das->must_eat != -1)
		ph.das->counter++;
	if (ph.das->tm_die < time_get() - ph.life)
	{
		ph.dead_flag = 1;
		ft_print("died", &ph);
		return (1);
	}
	return (0);
}

void	*check_death(void	*das)
{
	t_data	*data;
	int		i;

	data = (t_data *)das;
	data->counter = 0;
	i = 0;
	while (1)
	{
		i = 0;
		while (i < data->cnt_ph)
		{
			if (check_death_dop(data->ph[i]))
				return (NULL);
			i++;
		}
		if (data->counter == data->cnt_ph)
		{
			pthread_mutex_lock(&data->write_mutex);
			return (NULL);
		}
	}
	return (NULL);
}
```

# Этап 4 - Задержки

| seconde | milliseconde | microseconde |
|----------|-------|-------|
| 1 | 1000 | 1e+6 |
| 0,001 | 1 | 1000 |

- Как работает gettimeofday :
```
if (gettimeofday(&current_time, NULL) == -1)
	ft_error("Gettimeofday returned -1\n");
```

- Чтобы узнать текущее время в миллисекундах с помощью gettimeofday :
```
long int	time_get(void)
{
	long int			time;
	struct timeval		current_time;

	time = 0;
	if (gettimeofday(&current_time, NULL) == -1)
		ft_error("Gettimeofday returned -1\n");
	time = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (time);
}
```

- Для более точного контроля времени создаём функцию ft_sleep, стандартная функция ожидания оринтируется только подоваемый ей аргумент (время для задержки).
```
void	ft_sleep(int time)
{
	long int	start;

	start = time_get();
	while ((time_get() - start) < (long int)time)
		usleep(time / 10);
````

# Этап 5 - Утечки, сбои и гонки данных

**Гонка данных** :
Два или более потока в одном процессе одновременно обращаются к одной и той же ячейке памяти, и по крайней мере один из доступов предназначен для записи, а потоки не используют никаких монопольных блокировок для управления своим доступом к ячейке памяти. В результате получаем недетерминирован доступ, что приводит к непредсказуемому результату от запуска к запуску в зависимости от порядка старта потоков (независимо от результата работы программа - гонка данных приводит к непредсказуемому результату, что является ошибкой).

Для предотвращения гонки данных используются Мьютексы. Как правило работа мьютексов проверяется вручну (ну а как иначе, это же си), но для дополнительный проверки вы можете использовать valgrind

# Этап 6 - Тесты

Для проверки работоспособности кода приведу в пример с параметрами запросов и поведение программы при таких параметрах

| запрос | ожидаемый результат |
|----------|-------|
| ./philo 1 200 200 200 | филосов берёт одну вилку и умирает через 200 ms |
| ./philo 2 800 200 200 | никто не умирает |
| ./philo 5 800 200 200 | никто не умирает |
| ./philo 5 800 200 200 7 | симуляция останавливается, когда каждый фило поест 7 раз|
| ./philo 4 410 200 200 | никто не умирает |
| ./philo 4 310 200 200 | философ умирает |
| ./philo 4 500 200 1.2 | невалидные аргументы |
| ./philo 4 0 200 200 | невалидные аргументы |
| ./philo 4 -500 200 200 | невалидные аргументы |
| ./philo 4 500 200 2147483647 | философ умирает через 500 ms |
| ./philo 4 2147483647 200 200 | никто не умирает |
| ./philo 4 214748364732 200 200 | невалидные аргументы |
| ./philo 4 200 210 200 | философ умирает, надо отобразить смерть не позднее 210 ms|