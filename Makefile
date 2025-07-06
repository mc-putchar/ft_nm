NAME := ft_nm

SRCDIR := src
OBJDIR := build
INCDIR := include
LIBDIR := lib

SRC :=
vpath %.c $(SRCDIR)
SRC += main.c

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
	CPPFLAGS += -DDEBUG
	LDFLAGS += -ggdb3 -Og
endif

MKDIR := mkdir -p
RM := rm -f

RED := \033[31m
GRN := \033[32m
MAG := \033[35m
MAB := \033[1;35m
CYA := \033[36m
CYB := \033[1;36m
NC  := \033[0m

.PHONY: all debug clean fclean re help

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
	$(MAKE) -C $(LIBFTDIR) $(MAKECMDGOALS)

fclean: clean # Clean all compiled files
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFTDIR) $(MAKECMDGOALS)

re: fclean # Recompile all
	@$(MAKE) all
	$(MAKE) -C $(LIBFTDIR) $(MAKECMDGOALS)

debug: re

help:	# Show this helpful message
	@awk 'BEGIN { FS = ":.*#"; \
	printf "$(GRN)$(NAME)$(NC)\nby: $(AUTHORS)\t@$(GRN)42 Berlin$(NC)\n\n"; \
	printf "Usage:\n\t$(CYB)make $(MAG)<target>$(NC)\n" } \
	/^[A-Za-z_0-9-]+:.*?#/ { printf "$(MAB)%-16s $(CYA)%s$(NC)\n", $$1, $$2}' Makefile
