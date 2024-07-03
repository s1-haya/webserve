NAME	:=	a.out

# unit test's log file path
LOG_DIR		:=	log
LOG_FILE	:=	$(LOG_DIR)/log_unit_test.txt

# for test main
SRC_DIR	:=	srcs
SRCS	:=	$(SRC_DIR)/main.cpp

# 1. add webserv file
WS_UTILS_DIR	:=	../../srcs/utils
SRCS			+=	$(WS_UTILS_DIR)/color.cpp \
                	$(WS_UTILS_DIR)/split.cpp

# 2. add unit test file
# for split
SPLIT_DIR	:=	$(SRC_DIR)/split
SRCS		+=	$(SPLIT_DIR)/test_split.cpp
# for ~

# 3. add new dir
SRC_DIRS	:=	$(SRC_DIR) \
				$(WS_UTILS_DIR) \
				$(SPLIT_DIR)

#--------------------------------------------
OBJ_DIR	:=	objs
OBJS	:=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

INCLUDES	:=	$(addprefix -I, $(SRC_DIRS))

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

.PHONY	: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

vpath %.cpp $(SRC_DIRS)
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
.PHONY	: run
run: all
	@$(MKDIR) $(dir $(LOG_FILE))
	@./$(NAME) 2>&1 | tee $(LOG_FILE); \
	status=$${PIPESTATUS[0]}; \
	echo -e "\nunit test's log =>" $(LOG_FILE); \
	exit $$status;

.PHONY	: val
val: all
	@valgrind ./$(NAME)

#--------------------------------------------
-include $(DEPS)
