NAME		:=	client

WS_SRCS_DIR		:=	../../../srcs
WS_UTILS_DIR	:=	$(WS_SRCS_DIR)/utils

SRCS		:=	client.cpp \
				main.cpp \
				$(WS_UTILS_DIR)/color.cpp

OBJ_DIR		:=	objs
OBJS		:=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

SRCS_DIR	:=	$(WS_UTILS_DIR)

INCLUDES	:=	$(addprefix -I, $(SRCS_DIR))

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

#--------------------------------------------
.PHONY	: all
all		: $(NAME)

vpath %.cpp $(SRCS_DIR)
$(OBJ_DIR)/%.o: %.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

.PHONY	: clean
clean:
	$(RM) -r $(OBJ_DIR)

.PHONY	: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY	: re
re: fclean all

#--------------------------------------------
DEFAULT_PORT	:=	8080
PORT			:=	$(DEFAULT_PORT)

# ex) make run INFILE_PATH=../request_messages/webserver/get/200_get_sub.txt
# ex) make run PORT=4242 INFILE_PATH=../request_messages/apache_root/get/200/connection-close.txt
.PHONY	: run
run: all
	@./$(NAME) $(PORT) $(INFILE_PATH)

.PHONY	: val
val: all
	@valgrind ./$(NAME) $(PORT) $(INFILE_PATH)

.PHONY	: check
check:
	@cppcheck --enable=all $$(find . -type f -name "*.cpp" | tr '\n' ' ')

#--------------------------------------------

-include $(DEPS)
