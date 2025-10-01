/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yurimdm <yurimdm@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:02:26 by ymazzett          #+#    #+#             */
/*   Updated: 2025/06/05 21:54:10 by yurimdm          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

extern void		ft_bzero(void *s, size_t n);

extern int		ft_isalnum(int c);

extern int		ft_isalpha(int c);

extern int		ft_isascii(int c);

extern int		ft_isdigit(int c);

extern int		ft_isprint(int c);

extern void		*ft_memcpy(void *dst, const void *src, size_t n);

extern void		*ft_memmove(void *dst, const void *src, size_t len);

extern void		*ft_memset(void *b, int c, size_t len);

extern size_t	ft_strlen(const char *s);

extern int		ft_toupper(int c);

extern int		ft_tolower(int c);

extern size_t	ft_strlcpy(char *dst, const char *src, size_t len);

extern size_t	ft_strlcat(char *dst, const char *src, size_t len);

extern int		ft_strncmp(const char s1[], const char s2[], size_t n);

extern char		*ft_strrchr(const char *s, int c);

extern void		*ft_memchr(const void *s, int c, size_t n);

extern int		ft_memcmp(const void *s1, const void *s2, size_t n);

extern char		*ft_strnstr(
					const char *haystack, const char *needle, size_t len);

extern char		*ft_substr(char const *s, unsigned int start, size_t len);

extern char		*ft_strtrim(char const *s1, char const *set);

extern char		*ft_strjoin(char const *s1, char const *s2);

extern void		*ft_calloc(size_t count, size_t size);

extern char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));

extern void		ft_striteri(char const *s, void (*f)(unsigned int, char *));

extern char		*ft_strdup(const char *s);

extern char		**ft_split(char const *s, char c);

extern int		ft_atoi(const char *str);

extern char		*ft_itoa(int n);

extern void		ft_putchar_fd(char c, int fd);

extern void		ft_putendl_fd(char *s, int fd);

extern void		ft_putnbr_fd(int n, int fd);

extern void		ft_putstr_fd(char *s, int fd);
