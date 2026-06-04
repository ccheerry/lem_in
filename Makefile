MAKEFLAGS	+= --no-print-directory

NAME		= lem-in

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I include -I lib

LIBFT_DIR	= lib
LIBFT		= $(LIBFT_DIR)/libft.a

OBJ_DIR		= objects

RESET		= \001\033[0m\002
CYAN		= \001\033[1;36m\002
RED			= \001\033[0;91m\002

SRCS		= srcs/main.c \
			  srcs/parsing/parse_input.c \
			  srcs/parsing/parse_rooms.c \
			  srcs/parsing/parse_links.c \
			  srcs/parsing/store_line.c \
			  srcs/parsing/read_stdin.c \
			  srcs/graph/graph_init.c \
			  srcs/graph/node_split.c \
			  srcs/graph/hash_table.c \
			  srcs/solver/algo.c \
			  srcs/solver/paths.c \
			  srcs/simulation/simulate.c \
			  srcs/utils/error.c \
			  srcs/utils/memory.c

OBJS		= $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBFT_SRC	= $(addprefix $(LIBFT_DIR)/, \
				  ft_memset.c ft_bzero.c ft_strlen.c ft_atoi.c ft_isdigit.c \
				  ft_isalpha.c ft_isprint.c ft_isascii.c ft_isalnum.c ft_memchr.c \
				  ft_memcpy.c ft_memcmp.c ft_memmove.c ft_strchr.c ft_strdup.c \
				  ft_strlcat.c ft_strlcpy.c ft_calloc.c ft_strncmp.c ft_toupper.c \
				  ft_tolower.c ft_strnstr.c ft_strrchr.c ft_substr.c ft_strtrim.c \
				  ft_strjoin.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
				  ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
				  ft_strstr.c ft_strcmp.c libft.h)

all: $(NAME)

$(LIBFT): $(LIBFT_SRC)
	@$(MAKE) -C $(LIBFT_DIR) > /dev/null

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@printf "$(CYAN)Compiled!$(RESET)\n"

$(OBJ_DIR)/%.o: %.c include/lem_in.h lib/libft.h
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@printf "$(RED)Cleaning...$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) clean > /dev/null 2>&1
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean > /dev/null 2>&1
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
