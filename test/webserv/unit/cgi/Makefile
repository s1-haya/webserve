NAME			:=	a.out

# 1. Set each directory name
TEST_DIR		:=	cgi

LOG_DIR			:=	log
LOG_FILE_NAME	:=	$(TEST_DIR).log
LOG_FILE_PATH	:=	$(LOG_DIR)/$(LOG_FILE_NAME)

# 2. Add target webserv files
WS_SRCS_DIR				:=	../../../../srcs
WS_UTILS_DIR			:=	$(WS_SRCS_DIR)/utils
WS_EXCEPTION_DIR		:=	$(WS_SRCS_DIR)/exception
WS_HTTP_DIR				:=	$(WS_SRCS_DIR)/http
WS_CGI_DIR				:=	$(WS_SRCS_DIR)/cgi
WS_HTTP_RESPONSE_DIR	:=	$(WS_HTTP_DIR)/response
WS_HTTP_CGI_PARSE_DIR	:=	$(WS_HTTP_RESPONSE_DIR)/cgi_parse

SRCS				+=	$(WS_CGI_DIR)/cgi.cpp \
						$(WS_CGI_DIR)/cgi_request.cpp \
						$(WS_HTTP_CGI_PARSE_DIR)/cgi_parse.cpp \
						$(WS_HTTP_DIR)/http_message.cpp \
						$(WS_HTTP_DIR)/http_exception.cpp \
						$(WS_HTTP_DIR)/status_code.cpp \
						$(WS_UTILS_DIR)/color.cpp \
						$(WS_EXCEPTION_DIR)/system_exception.cpp

# 3. Add unit test files
SRCS	+=	test_cgi.cpp

# 4. Add directory for INCLUDE
SRCS_DIR	:=	$(WS_UTILS_DIR) \
				$(WS_EXCEPTION_DIR) \
				$(WS_HTTP_DIR) \
				$(WS_HTTP_RESPONSE_DIR) \
				$(WS_HTTP_CGI_PARSE_DIR) \
				$(WS_CGI_DIR)

#--------------------------------------------
OBJ_DIR		:=	objs
OBJS		:=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

INCLUDES	:=	$(addprefix -I, $(SRCS_DIR))

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

.PHONY	: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

vpath %.cpp $(SRCS_DIR)
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
