NAME = libasm.a
ASM = nasm
CC = gcc
AR = ar
ASM_FLAGS = -f elf64
CC_FLAGS = -Wall -Wextra -Werror -fPIE
AR_FLAGS = rcs

OBJ_DIR = obj
TEST_DIR = test
SRC_DIR = src
LOG_DIR = .test_log
TEST_SCRIPT = ./test/test.sh

ASM_SRCS = $(notdir $(wildcard $(SRC_DIR)/*.s))
OBJS = $(addprefix $(OBJ_DIR)/, $(ASM_SRCS:.s=.o))

TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
TEST_NAMES = $(notdir $(TEST_FILES:.c=))

GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
YELLOW = \033[0;33m
NC = \033[0m

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	@echo "$(BLUE)Creating library $(NAME)...$(NC)"
	@$(AR) $(AR_FLAGS) $(NAME) $(OBJS)
	@echo "$(GREEN)✅ Library created!$(NC)"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	@echo "$(GREEN)Assembling $(YELLOW)$<...$(NC)"
	@$(ASM) $(ASM_FLAGS) $< -o $@

test%:
	@$(TEST_SCRIPT) $*

test:
	@$(TEST_SCRIPT)

help:
	@echo "$(BLUE)Available commands:$(NC)"
	@echo "  $(BLUE)=>$(MAGENTA) make$(NC)                - Build library"
	@echo "  $(BLUE)=>$(MAGENTA) make $(YELLOW)test$(NC)           - Run all tests"
	@for test_name in $(TEST_NAMES); do \
		printf "$(BLUE)  => $(MAGENTA)make $(YELLOW)test%-10s$(NC) - Run test for $(GREEN)%s.c$(NC)\n" "$$test_name" "$$test_name"; \
	done
	@echo "  $(BLUE)=>$(MAGENTA) make $(YELLOW)clean$(NC)          - Clean objects"
	@echo "  $(BLUE)=>$(MAGENTA) make $(YELLOW)fclean$(NC)         - Clean all"
	@echo "  $(BLUE)=>$(MAGENTA) make $(YELLOW)re$(NC)             - Rebuild"

clean:
	@echo "$(RED)Cleaning $(OBJ_DIR)$(NC)"
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)Cleaning $(LOG_DIR)$(NC)"
	@rm -rf $(LOG_DIR)

fclean: clean
	@echo "$(RED)Cleaning $(NAME)$(NC)"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test help
