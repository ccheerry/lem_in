NAME		= lem-in

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I include -I lib

LIBFT_DIR	= lib
LIBFT		= $(LIBFT_DIR)/libft.a

OBJ_DIR		= obj

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

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
