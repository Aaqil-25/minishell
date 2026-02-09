NAME = minishell

OBJ = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS = -lreadline
RM = rm -f
OBJ_DIR = ./obj

SRC = main.c \
		token.c \
		utils.c \
		tokenize.c \
		execution.c \
		builtins.c \
		libft/gnl/get_next_line.c libft/gnl/get_next_line_utils.c \
		libft/ft_atoi.c libft/ft_bzero.c libft/ft_isalnum.c libft/ft_isalpha.c libft/ft_tolower.c \
		libft/ft_isascii.c libft/ft_isdigit.c libft/ft_isprint.c libft/ft_memchr.c libft/ft_memcpy.c \
		libft/ft_memcmp.c libft/ft_memset.c libft/ft_strlcat.c libft/ft_toupper.c \
		libft/ft_strlcpy.c libft/ft_strlen.c libft/ft_strncmp.c libft/ft_calloc.c libft/ft_strrchr.c \
		libft/ft_memmove.c libft/ft_strdup.c libft/ft_strnstr.c libft/ft_strjoin.c libft/ft_putchar_fd.c \
		libft/ft_putstr_fd.c libft/ft_putendl_fd.c libft/ft_putnbr_fd.c libft/ft_strmapi.c \
		libft/ft_strtrim.c libft/ft_substr.c libft/ft_split.c libft/ft_itoa.c libft/ft_striteri.c


all: $(NAME)


$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)


$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I. -I./libft -I./libft/gnl


clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
