/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xxxxxxxx <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/27 12:07:01 by xxxxxxxx          #+#    #+#             */
/*   Updated: 2017/11/27 12:08:01 by xxxxxxxx         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <string.h>
# define BUFF_SIZE 1
# define FDS 3000

int			get_next_line(const int fd, char **line);
void		*ft_memalloc(size_t size);
void		*ft_realloc(void *src, size_t new_size);
void		*ft_memcpy(void *dst, const void *src, size_t n);
size_t		ft_strlen(const char *s);
char		*ft_strcpy(char *dest, const char *src);
void		ft_bzero(void *s, size_t n);
char		*ft_strncpy(char *dest, const char *src, size_t n);
void		*ft_realloc_safe(void *src, size_t old_size, size_t new_size);

#endif
