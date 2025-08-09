NAME := ft_nm

SRCDIR := src
OBJDIR := build
INCDIR := include
LIBDIR := lib

SRC :=
vpath %.c $(SRCDIR)
SRC += main.c
SRC += read_elf.c
SRC += read_section.c
SRC += read_sheader.c
SRC += load_symbols.c
SRC += options.c
SRC += print_symbols.c
SRC += print_info.c
SRC += print_sections.c
SRC += print_strings.c

OBJ := $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))

DEPS := $(OBJ:.o=.d)
-include $(DEPS)

LIBFTDIR := $(LIBDIR)/libft
LIBFT := $(LIBFTDIR)/libft.a
LIBFT_INC := $(LIBFTDIR)/include

CC := cc
CFLAGS := -Wall -Wextra -Werror -pedantic -Wconversion -Wunreachable-code -Wshadow
CPPFLAGS := -I$(INCDIR) -I$(LIBFT_INC) -MD -MP
LDFLAGS := -L$(LIBFTDIR)
LDLIBS := -lft

debug: DEBUG := 1
ifeq ($(DEBUG), 1)
	CFLAGS += -ggdb3 -Og
	CPPFLAGS += -DDEBUG=1
	LDFLAGS += -ggdb3 -Og -DDEBUG=1
endif
SAMPLE := sample.c

MKDIR := mkdir -p
RM := rm -f

PLATFORM := linux/amd64
SHELL := /bin/bash

RED := \033[31m
GRN := \033[32m
MAG := \033[35m
MAB := \033[1;35m
CYA := \033[36m
CYB := \033[1;36m
NC  := \033[0m

.PHONY: all debug clean fclean re run try help sample container exec shell
.DEFAULT_GOAL := all

all: $(NAME) # Compile all targets

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR):
	$(MKDIR) $@

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

clean: # Clean intermediary files
	$(RM) $(OBJ)
	$(RM) -r $(OBJDIR)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean # Clean all compiled files
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean

re: fclean # Recompile all
	@$(MAKE) all

run: $(NAME) $(SAMPLE) # Run the compiled program
	@./$(NAME) $(filter-out run,$(MAKECMDGOALS))

try: $(NAME) # Run the program with Valgrind
	@valgrind --show-leak-kinds=all --track-origins=yes ./$(NAME) $(filter-out try,$(MAKECMDGOALS))

debug: re

help:	# Show this helpful message
	@awk 'BEGIN { FS = ":.*#"; \
	printf "$(GRN)$(NAME)$(NC)\n"; \
	printf "Usage:\n\t$(CYB)make $(MAG)<target>$(NC)\n" } \
	/^[A-Za-z_0-9-]+:.*?#/ { printf "$(MAB)%-16s $(CYA)%s$(NC)\n", $$1, $$2}' Makefile

example: $(SAMPLE)

$(SAMPLE):
	$(CC) sample.c

container: # Build a Docker container for the project
	@docker build --platform $(PLATFORM) -t $(NAME) .

exec: container # Exec in the Docker container
	@docker run --rm --platform $(PLATFORM) -it $(NAME) /bin/sh -c 'make run $(filter-out exec,$(MAKECMDGOALS))'

shell: container # Run shell in the Docker container
	@docker run --rm --mount type=bind,src=${PWD},dst=/app --platform $(PLATFORM) -it $(NAME) /bin/sh

test: TARGET := $(filter-out test, $(MAKECMDGOALS))
test:	# Run tests against the compiled program
	@echo -e "$(GRN)Running tests for $(TARGET)$(NC)"
	(LANG=C diff -ys --color <(./ft_nm $(FLAGS) $(TARGET)) <(nm $(FLAGS) $(TARGET)) \
	&& echo -e "$(GRN)Test passed for $(TARGET)$(NC)") \
	|| echo -e "$(RED)Test failed for $(TARGET)$(NC)"
