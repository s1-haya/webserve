NAME			:=	a.out

# 1. Set each directory name
TEST_DIR		:=	split

LOG_DIR			:=	log
LOG_FILE_NAME	:=	$(TEST_DIR).log
LOG_FILE_PATH	:=	$(LOG_DIR)/$(LOG_FILE_NAME)

# 2. Add target webserv files
WS_SRCS_DIR		:=	../../../../srcs
WS_UTILS_DIR	:=	$(WS_SRCS_DIR)/utils
SRCS			+=	$(WS_UTILS_DIR)/color.cpp \
					$(WS_UTILS_DIR)/split.cpp

# 3. Add unit test files
SRCS	+=	test_split.cpp

# 4. Add directory for INCLUDE
INCLUDE_DIR	:=	$(WS_UTILS_DIR)

#--------------------------------------------
OBJ_DIR		:=	objs
OBJS		:=	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)

INCLUDES	:=	$(addprefix -I, $(INCLUDE_DIR))

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

.PHONY	: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY	: clean
clean:
	$(RM) -r $(OBJ_DIR)

.PHONY	: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY	: re
re: fclean all

#--------------------------------------------
# PIPESTATUSがbash固有のため
SHELL=/bin/bash

.PHONY	: run
run: all
	@$(MKDIR) $(dir $(LOG_FILE_PATH))
	@./$(NAME) 2>&1 | tee $(LOG_FILE_PATH); \
	status=$${PIPESTATUS[0]}; \
	echo -e "\nunit test's log =>" $(LOG_FILE_PATH); \
	exit $$status;

.PHONY	: val
val: all
	@valgrind ./$(NAME)

#--------------------------------------------
-include $(DEPS)
