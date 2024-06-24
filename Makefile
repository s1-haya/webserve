include docker/command.mk

NAME		:=	webserv

SRC_DIR		:=	srcs
SRC_DIRS	:=	$(shell find $(SRC_DIR) -type d)
SRCS		:=	$(shell find $(SRC_DIR) -type f -name "*.cpp")

OBJ_DIR		:=	objs
OBJS		:=	$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

INCLUDES	:=	$(addprefix -I,$(SRC_DIRS))

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

#--------------------------------------------
.PHONY	: all
all		: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

.PHONY	: clean
clean:
	$(RM) -r $(OBJ_DIR)

.PHONY	: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY	: re
re: fclean all

#--------------------------------------------
PATH_CONFIG	:=	default.conf

.PHONY	: run
run: all
	@./$(NAME) $(PATH_CONFIG)

.PHONY	: val
val: all
	@valgrind ./$(NAME) $(PATH_CONFIG)

.PHONY	: check
check:
	@cppcheck --enable=all $$(find . -type f -name "*.cpp" | tr '\n' ' ')

.PHONY	: test
test:
	@make unit && make e2e

.PHONY	: debug
debug:
	-@pytest -s ./test/unit
	-@pytest -s ./test/integration

.PHONY	: unit
unit:
	-@pytest ./test/unit

.PHONY	: e2e
e2e:
	-@pytest ./test/integration

#--------------------------------------------
-include $(DEPS)
