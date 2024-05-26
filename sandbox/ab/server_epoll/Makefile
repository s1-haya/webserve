NAME		:=	server

SRCS		:=	accept.cpp \
				bind.cpp \
				epoll.cpp \
				listen.cpp \
				main.cpp \
				socket.cpp

OBJ_DIR		:=	objs
OBJS		:=	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)

INCLUDE_DIR	:=	includes
INCLUDES	:=	-I./$(INCLUDE_DIR)/ -I.

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

#--------------------------------------------
.PHONY	: all
all		: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
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
.PHONY	: run
run: all
	@./$(NAME)

.PHONY	: val
val: all
	@valgrind ./$(NAME)

.PHONY	: check
check:
	@cppcheck --enable=all $$(find . -type f -name "*.cpp" | tr '\n' ' ')

#--------------------------------------------
.PHONY	: req
req:
	echo -n "Hello from client" | nc 127.0.0.1 8080

#--------------------------------------------
-include $(DEPS)
