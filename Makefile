MAKEFLAGS	+= --no-print-directory

NAME		= lem-in
VISU		= visu-hex
VISU_SRC	= bonus/visu-hex

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -O0
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
			  srcs/solver/algorithm.c \
			  srcs/solver/shortest.c \
			  srcs/solver/flow.c \
			  srcs/solver/paths.c \
			  srcs/solver/select.c \
			  srcs/solver/turns.c \
			  srcs/simulation/simulate.c \
			  srcs/simulation/output.c \
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
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@printf "$(CYAN)Ready!$(RESET)\n"

bonus: $(NAME) $(VISU)

$(VISU): $(VISU_SRC)
	@chmod +x $(VISU_SRC)
	@ln -sf $(VISU_SRC) $(VISU)
	@python3 -c "import tkinter" >/dev/null 2>&1 \
		&& printf "$(CYAN)Visualizer ready! ./lem-in < map | ./$(VISU)$(RESET)\n" \
		|| printf "$(RED)python3 cannot import tkinter: install Tk (python3-tk / tk)$(RESET)\n"

install:
	@command -v $(CC) >/dev/null || { printf "$(RED)$(CC) not found: install gcc or clang$(RESET)\n" >&2; exit 1; }
	@python3 -c "import tkinter" >/dev/null 2>&1 \
		|| printf "$(RED)bonus only: python3 with tkinter is needed by the visualizer (python3-tk / tk)$(RESET)\n"
	@printf "$(CYAN)dependencies ready: make, or make bonus for the visualizer$(RESET)\n"

# Valid and stress colonies must be solved, invalid ones must print ERROR on stderr.
test: $(NAME)
	@fail=0; \
	for f in maps/valid/*.map maps/stress/*.map; do \
		./$(NAME) < $$f > /dev/null 2>&1 || { printf "$(RED)FAIL$(RESET) %s\n" $$f; fail=1; }; done; \
	for f in maps/invalid/*.map; do \
		[ "$$(./$(NAME) < $$f 2>&1 > /dev/null)" = "ERROR" ] || { printf "$(RED)FAIL$(RESET) %s\n" $$f; fail=1; }; done; \
	[ $$fail = 0 ] && printf "$(CYAN)every map behaves as expected$(RESET)\n"

maps:
	@python3 maps/stress/generate.py

$(OBJ_DIR)/%.o: %.c include/lem_in.h lib/libft.h
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@printf "$(RED)Cleaning...$(RESET)\n"
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJ_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME) $(VISU)
	@find bonus -name __pycache__ -type d -exec rm -rf {} +

re: fclean all

help:
	@echo "make          build lib/libft.a, then ./lem-in"
	@echo "make install  check the compiler, and python3 with tkinter for the visualizer"
	@echo "make bonus    also link ./visu-hex to the visualizer"
	@echo "make test     run every map of maps/ and check the result of each one"
	@echo "make maps     write the large stress colonies to maps/stress/generated/"
	@echo "make clean    remove the objects"
	@echo "make fclean   remove the objects, libft.a, the binary and the visualizer link"
	@echo "make re       fclean, then build"

.PHONY: all help install bonus test maps clean fclean re
