NAME			:=	a.out

# 1. Set each directory name
TEST_DIR		:=	config_parse

# 2. Add target webserv files
WS_SRCS_DIR		:=	../../../srcs
WS_UTILS_DIR	:=	$(WS_SRCS_DIR)/utils
WS_CONFIG_PARSE_DIR	:=	$(WS_SRCS_DIR)/config_parse
SRCS			+=	$(WS_CONFIG_PARSE_DIR)/parser.cpp \
					$(WS_CONFIG_PARSE_DIR)/lexer.cpp

# 3. Add unit test files
SRCS	+=	main.cpp

# 4. Add directory for INCLUDE
SRCS_DIR	:=	$(WS_CONFIG_PARSE_DIR)

#--------------------------------------------
OBJ_DIR		:=	objs
OBJS		:=	$(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))

INCLUDES	:=	$(addprefix -I, $(SRCS_DIR))

CXX			:=	c++
CXXFLAGS	:=	-std=c++98 -MMD -MP -pedantic # -Wall -Wextra -Werror

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

-include $(DEPS)
