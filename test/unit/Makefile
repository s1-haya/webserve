NAME		:=	a.out

LOG_DIR		:=	log
LOG_FILE	:=	$(LOG_DIR)/log_unit_test.txt

UTILS_DIR	:=	../../srcs/utils

SRC_DIR		:=	srcs
SRCS		:=	$(shell find $(SRC_DIR) -type f -name "*.cpp") \
				$(UTILS_DIR)/color.cpp \
				$(UTILS_DIR)/split.cpp

OBJ_DIR		:=	objs
OBJS		:=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

INCLUDES	:=	-I$(SRC_DIR) -I$(UTILS_DIR)

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

#--------------------------------------------
.PHONY	: all
all		: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(UTILS_DIR)/%.cpp
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
	@$(MKDIR) $(LOG_DIR)
	@./$(NAME) >$(LOG_FILE) 2>&1 | cat

.PHONY	: val
val: all
	@$(MKDIR) $(LOG_DIR)
	@valgrind ./$(NAME) >$(LOG_FILE) 2>&1 | cat

#--------------------------------------------
-include $(DEPS)
