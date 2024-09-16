NAME			:=	a.out

# 1. Set each directory name
TEST_DIR		:=	http

LOG_DIR			:=	log
LOG_FILE_NAME	:=	$(TEST_DIR).log
LOG_FILE_PATH	:=	$(LOG_DIR)/$(LOG_FILE_NAME)

# 2. Add target webserv files
WS_SRCS_DIR				:=	../../../../srcs
WS_UTILS_DIR			:= $(WS_SRCS_DIR)/utils
WS_HTTP_DIR				:=	$(WS_SRCS_DIR)/http
WS_HTTP_REQUEST_DIR		:=	$(WS_HTTP_DIR)/request
WS_HTTP_RESPONSE_DIR	:=	$(WS_HTTP_DIR)/response
WS_HTTP_PARSE_DIR		:=	$(WS_HTTP_REQUEST_DIR)/parse
WS_HTTP_SERVER_INFO_CHECK_DIR	:=	$(WS_HTTP_RESPONSE_DIR)/http_serverinfo_check
WS_VIRTUAL_SERVER_DIR			:=	$(WS_SRCS_DIR)/server/virtual_server

SRCS				+=	$(WS_UTILS_DIR)/color.cpp \
						$(WS_UTILS_DIR)/split_str.cpp \
						$(WS_UTILS_DIR)/isdigit.cpp \
						$(WS_UTILS_DIR)/convert_str.cpp \
						$(WS_UTILS_DIR)/system_exception.cpp \
						$(WS_HTTP_DIR)/status_code.cpp \
						$(WS_HTTP_DIR)/http_message.cpp \
						$(WS_HTTP_DIR)/http_exception.cpp \
						$(WS_HTTP_DIR)/http.cpp \
						$(WS_HTTP_REQUEST_DIR)/http_storage.cpp \
						$(WS_HTTP_RESPONSE_DIR)/http_response.cpp \
						$(WS_HTTP_RESPONSE_DIR)/http_method.cpp \
						$(WS_HTTP_RESPONSE_DIR)/stat.cpp \
						$(WS_HTTP_PARSE_DIR)/http_parse.cpp \
						$(WS_HTTP_SERVER_INFO_CHECK_DIR)/http_serverinfo_check.cpp \
						$(WS_VIRTUAL_SERVER_DIR)/virtual_server.cpp

# 2. Add srcs directory for INCLUDE
SRCS_DIR	:=	$(WS_UTILS_DIR) \
				$(WS_HTTP_DIR) \
				$(WS_HTTP_REQUEST_DIR) \
				$(WS_HTTP_RESPONSE_DIR) \
				$(WS_HTTP_PARSE_DIR) \
				$(WS_HTTP_SERVER_INFO_CHECK_DIR) \
				$(WS_VIRTUAL_SERVER_DIR) \

# 3. Add unit test files
TEST_CASE_DIR := test_case
# method
TEST_CASE_FOR_GET := get
# status_codes
TEST_CASE_FOR_200 := 200
TEST_CASE_FOR_400 := 400

SRCS	+=	test_http.cpp \
			$(TEST_CASE_DIR)/test_handler.cpp \
			$(TEST_CASE_DIR)/$(TEST_CASE_FOR_GET)/$(TEST_CASE_FOR_200)/test_no_connection.cpp \
			$(TEST_CASE_DIR)/$(TEST_CASE_FOR_GET)/$(TEST_CASE_FOR_400)/test_not_exist_file.cpp

# 4. Add unit test directory for INCLUDE
SRCS_DIR += $(TEST_CASE_DIR) \
			$(TEST_CASE_DIR)/$(TEST_CASE_FOR_GET)/$(TEST_CASE_FOR_200) \
			$(TEST_CASE_DIR)/$(TEST_CASE_FOR_GET)/$(TEST_CASE_FOR_400)

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
