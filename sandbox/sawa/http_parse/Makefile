NAME	:=	a.out

# for test main
SRCS	:=	main.cpp \
			http_parse.cpp

# 1. add webserv file
WS_UTILS_DIR	:=	../../../srcs/utils
SRCS			+=	$(WS_UTILS_DIR)/color.cpp \
					$(WS_UTILS_DIR)/split.cpp

TARGET_DIR	:=	../../../srcs/http
SRCS			+=	$(TARGET_DIR)/http_message.cpp

# 3. add new dir
SRC_DIRS	:=	$(WS_UTILS_DIR) \
				$(TARGET_DIR)

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
