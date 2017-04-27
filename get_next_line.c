/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anicusan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/25 16:02:58 by anicusan          #+#    #+#             */
/*   Updated: 2017/03/17 19:32:33 by anicusan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		is_newline(char *s, int fin, t_gline *x)
{
	int		i;

	i = 0;
	if (fin == 0 && x->size == 0)
		return (-2);
	else if (fin < 0)
		return (-3);
	while (i < fin)
	{
		if (s[i] == '\n')
		{
			s[i] = '\0';
			return (i);
		}
		i++;
	}
	return (-1);
}

void	join(t_gline *x)
{
	char	*aux;
	char	*aux2;
	long	i;

	i = 0;
	aux = (char*)ft_memalloc(sizeof(char) * (x->size + BUFF_SIZE + 1));
	while (i < x->size)
	{
		aux[i] = x->s[i];
		i++;
	}
	aux2 = x->s;
	x->s = aux;
	if (aux2)
		free(aux2);
}

char	*gotnewl(t_gline *x, long red, long *flag, int io)
{
	char	*line;
	char	*aux;

	if (io == 0)
	{
		if (red == 0)
			red = x->size;
		x->size = x->size - red + *flag + 1;
		line = ft_memcpy(ft_memalloc(sizeof(char) * x->size), x->s, x->size);
		aux = x->s;
		x->s = x->s + x->size;
		x->size = red - *flag - 1;
		x->s = ft_memcpy(ft_memalloc(sizeof(char) *
					x->size + 1), x->s, x->size);
		free(aux);
		return (line);
	}
	line = ft_memcpy(ft_memalloc(sizeof(char) * (x->size + 1)), x->s, x->size);
	x->size = 0;
	return (line);
}

char	*gnl(const int fd, t_gline *x, long *flag)
{
	long	red;

	red = 1;
	while (red > 0)
	{
		red = 0;
		if (!(x->size && (*flag = is_newline(x->s, x->size, x)) >= 0))
		{
			join(x);
			red = (long)read(fd, (x->s) + x->size, BUFF_SIZE);
			*flag = is_newline(x->s + x->size, red, x);
			x->size += red;
		}
		if (*flag >= 0)
			return (gotnewl(x, red, flag, 0));
		else if ((red < BUFF_SIZE && red > 0)
			|| (red == 0 && x->size > 0))
			return (gotnewl(x, red, flag, 1));
	}
	return (NULL);
}

int		get_next_line(const int fd, char **line)
{
	static t_gline	*x[1025];
	long			flag;

	if (fd < 0 || line == NULL || BUFF_SIZE <= 0 || fd > 1024)
		return (-1);
	if (!x[fd])
	{
		x[fd] = (t_gline*)malloc(sizeof(t_gline));
		x[fd]->size = 0;
		x[fd]->s = (char*)ft_memalloc(sizeof(char) * BUFF_SIZE + 1);
	}
	*line = gnl(fd, x[fd], &flag);
	if (flag == -3)
		return (-1);
	if (flag == -2)
		return (0);
	else
		return (1);
}
