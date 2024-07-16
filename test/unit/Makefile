NAME	:=	a.out

# unit test's log file path
LOG_DIR		:=	log
LOG_FILE	:=	$(LOG_DIR)/log_unit_test.txt

# for test main
SRC_DIR	:=	srcs
SRCS	:=	$(SRC_DIR)/main.cpp

# 1. add webserv file
WS_UTILS_DIR	:=	../../srcs/utils
# for split
SRCS			+=	$(WS_UTILS_DIR)/color.cpp \
                	$(WS_UTILS_DIR)/split.cpp
# for convert_str
SRCS			+=	$(WS_UTILS_DIR)/convert_str.cpp \
					$(WS_UTILS_DIR)/isdigit.cpp

# for http_parse
WS_HTTP_DIR := ../../srcs/http/

WS_HTTP_PARSE_DIR := ../../srcs/http/request/parse

SRCS			+=	$(WS_HTTP_DIR)/http_message.cpp \
					$(WS_HTTP_PARSE_DIR)/http_parse.cpp


# 2. add unit test file
# for split
SPLIT_DIR	:=	$(SRC_DIR)/split
SRCS		+=	$(SPLIT_DIR)/test_split.cpp
# for convert_str
CONVERT_STR_DIR	:=	$(SRC_DIR)/convert_str
SRCS		+=	$(CONVERT_STR_DIR)/test_convert_str.cpp
# for http_parse
HTTP_PARSE_DIR	:=	$(SRC_DIR)/http_parse
SRCS		+=	$(HTTP_PARSE_DIR)/test_http_parse.cpp

# for ~

# 3. add new dir
SRC_DIRS	:=	$(SRC_DIR) \
				$(WS_UTILS_DIR) \
				$(WS_HTTP_DIR) \
				$(WS_HTTP_PARSE_DIR) \
				$(SPLIT_DIR) \
				$(CONVERT_STR_DIR) \
				$(HTTP_PARSE_DIR)

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
# PIPESTATUSがbash固有のため
SHELL=/bin/bash

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
