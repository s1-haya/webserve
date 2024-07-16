NAME		:=	a.out

TARGET_DIR	:=	../../../srcs/utils

SRCS		:=	main.cpp \
				create_cgi_env.cpp \
				create_cgi_argv.cpp \
				create_request_meta_variables.cpp \
				execve_cgi_script.cpp \
				run_cgi.cpp \
				is_executable.cpp \
				cgi.cpp \
				cgi_parse.cpp \
				$(TARGET_DIR)/color.cpp

OBJ_DIR		:=	objs
OBJS		:=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

INCLUDES	:=	-I$(TARGET_DIR)/

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -Wall -Wextra -Werror -MMD -MP -pedantic

DEPS		:=	$(OBJS:.o=.d)
MKDIR		:=	mkdir -p

#--------------------------------------------
.PHONY	: all
all		: $(NAME)

$(NAME): $(OBJS)
	$(CXX) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(TARGET_DIR)/%.cpp
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
run: re
	@./$(NAME)

.PHONY	: val
val: re
	@valgrind ./$(NAME)

#--------------------------------------------
-include $(DEPS)
