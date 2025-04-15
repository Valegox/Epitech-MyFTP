##
## EPITECH PROJECT, 2023
## myftp
## File description:
## Makefile
##

GREEN = \033[1;32m
LIGHT_GREEN = \033[1;32m
LIGHT_CYAN = \033[1;36m
RED = \033[1;31m
MAGENTA = \033[1;35m
BLUE = \033[1;34m
RESET = \033[0m

TARGET = myftp

SRC =   src/main.c 					\
		src/server.c				\
		src/handle_clients.c		\
		src/client_lifecycle.c		\
		src/handle_single_client.c  \
		src/handle_commands.c		\
		src/data_transfer.c			\
		src/commands/user_cmd.c		\
		src/commands/pass_cmd.c		\
		src/commands/quit_cmd.c		\
		src/commands/pwd_cmd.c		\
		src/commands/cwd_cmd.c		\
		src/commands/cdup_cmd.c		\
		src/commands/dele_cmd.c		\
		src/commands/pasv_cmd.c		\
		src/commands/port_cmd.c		\
		src/commands/retr_cmd.c		\
		src/commands/list_cmd.c		\
		src/commands/stor_cmd.c		\
		src/commands/help_cmd.c		\
		src/commands/noop_cmd.c		\
		src/utils/concat_str.c		\
		src/utils/get_file_type.c	\
		src/utils/is_int.c			\
		src/utils/remove_char.c		\
		src/utils/replace_char.c

CFLAGS = -Iinclude -Wall -Wextra -g3

LDFLAGS =

DEP = $(OBJ:%.o=%.d)

OBJ = $(SRC:%.c=%.o)

TEST_PORT = 4242

TEST_PATH = tests/test_ftp.sh

TEST_ADDR = 127.0.0.1

$(TARGET): $(OBJ)
	gcc $(OBJ) -o $(TARGET) $(LDFLAGS)
	@echo -e "$(BLUE)Finished compiling with\
	\nCompilations flags: $(CFLAGS)\
	\nLinking flags: $(LDFLAGS)$(RESET)"


-include $(DEP)

%.o: %.c
	gcc $(CFLAGS) -MMD -c $(firstword $^) -o $@

.PHONY : all clean fclean re

all: $(TARGET)

clean:
	@rm -f $(OBJ)
	@rm -f $(DEP)
	@echo -e "$(LIGHT_GREEN)Removed .o and .d files$(RESET)"

fclean: clean
	rm -f $(TARGET)
	@echo -e "$(LIGHT_GREEN)Removed target file$(RESET)"

test_server_run:
	@echo -e "$(LIGHT_GREEN)Running tests$(RESET)"
	@./$(TARGET) $(TEST_PORT) .

test_run:
	@sh $(TEST_PATH) $(TEST_ADDR) $(TEST_PORT)


re: fclean all
