NAME			:=	a.out

# 1. Set each directory name
TEST_DIR		:=	method
TEST_CASE_DIR		:=	test

LOG_DIR			:=	log
LOG_FILE_NAME	:=	$(TEST_DIR).log
LOG_FILE_PATH	:=	$(LOG_DIR)/$(LOG_FILE_NAME)

# 2. Add target webserv files
WS_SRCS_DIR		:=	../../../srcs
WS_UTILS_DIR	:=	$(WS_SRCS_DIR)/utils
WS_HTTP_DIR				:=	$(WS_SRCS_DIR)/http
WS_HTTP_REQUEST_DIR	:=	$(WS_HTTP_DIR)/request
WS_HTTP_PARSE_DIR	:=	$(WS_HTTP_DIR)/request/parse
WS_HTTP_RESPONSE_DIR	:=	$(WS_HTTP_DIR)/response
WS_DTO_SERVER_TO_HTTP_DIR	:=	$(WS_SRCS_DIR)/server/dto
WS_VERTUAL_SERVER_DIR	:=	$(WS_SRCS_DIR)/server/context_manager/virtual_server

SRCS			+=	$(WS_UTILS_DIR)/color.cpp \
					$(WS_UTILS_DIR)/system_exception.cpp \
					$(WS_HTTP_DIR)/http_message.cpp \
					$(WS_HTTP_DIR)/status_code.cpp \
					$(WS_HTTP_DIR)/http_exception.cpp \
					$(WS_HTTP_RESPONSE_DIR)/stat.cpp \
					$(WS_HTTP_RESPONSE_DIR)/http_response.cpp \
					$(WS_HTTP_RESPONSE_DIR)/http_handle_method.cpp \
					$(WS_VERTUAL_SERVER_DIR)/virtual_server.cpp \

# 3. Add unit test files
SRCS	+=	test_http_method.cpp

# 4. Add directory for INCLUDE
SRCS_DIR	:=	$(WS_UTILS_DIR) \
				$(WS_HTTP_DIR) \
				$(WS_HTTP_REQUEST_DIR) \
				$(WS_HTTP_PARSE_DIR) \
				$(WS_HTTP_RESPONSE_DIR) \
				$(WS_DTO_SERVER_TO_HTTP_DIR) \
				$(WS_VERTUAL_SERVER_DIR)

#--------------------------------------------
OBJ_DIR		:=	objs
OBJS		:=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

INCLUDES	:=	$(addprefix -I, $(SRCS_DIR))

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

.PHONY	: all
all:  init $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

# create test file and directory
.PHONY	: init
init: $(TEST_CASE_DIR)/no_authority_file $(TEST_CASE_DIR)/no_authority_directory

$(TEST_CASE_DIR)/no_authority_file:
	-mkdir -p $(TEST_CASE_DIR) && touch $@ && chmod 000 $@

$(TEST_CASE_DIR)/no_authority_directory:
	-mkdir -p $(TEST_CASE_DIR) && mkdir -p $@ && chmod 000 $@

vpath %.cpp $(SRCS_DIR)
$(OBJ_DIR)/%.o: %.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.PHONY	: clean
clean: 
	-@make clean-test
	$(RM) -r $(OBJ_DIR)

.PHONY	: clean-test
clean-test:
	$(RM) $(TEST_CASE_DIR)/no_authority_file
	@chmod 700 $(TEST_CASE_DIR)/no_authority_directory
	$(RM) -r $(TEST_CASE_DIR)/no_authority_directory

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
