include docker.mk

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

#--------------------------------------------
# for test
#--------------------------------------------
.PHONY	: req
req:
	echo -n "Hello from client" | nc 127.0.0.1 8080

.PHONY	: test
test:
	@make unit
	@make e2e

.PHONY	: unit
unit:
	@make docker-run
	@sleep 5
	-@pytest ./test/unit
	@make docker-clean

.PHONY	: e2e
e2e:
	@make docker-run
	@sleep 5
	-@pytest ./test/integration
	@make docker-clean

#--------------------------------------------
-include $(DEPS)
