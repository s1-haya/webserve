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

.PHONY	: test
test:
	@make unit && make e2e

.PHONY	: unit
unit:
	-@pytest ./test/unit

.PHONY	: e2e
e2e:
	-@pytest ./test/integration

#--------------------------------------------

WEBSERV_IMAGE_NAME := webserv-image
WEBSERV_IMAGE_TAG := production
WEBSERV_CONTAINER_NAME := webserv-container
WEBSERV_PORT := 8080

.PHONY	: build-webserv
build-webserv:
	@docker build -t $(WEBSERV_IMAGE_NAME):$(WEBSERV_IMAGE_TAG) .; \

.PHONY	: run-webserv
run-webserv:
	@if [ -z "$$(docker images -q $(WEBSERV_IMAGE_NAME):$(WEBSERV_IMAGE_TAG))" ]; then \
		echo "$(WEBSERV_IMAGE_NAME):$(WEBSERV_IMAGE_TAG) not found. Building the image..."; \
		make build-webserv; \
	else \
		echo "$(WEBSERV_IMAGE_NAME):$(WEBSERV_IMAGE_TAG) found."; \
	fi
	@sleep 1
	@if [ -z "$$(docker ps -aqf name=$(WEBSERV_CONTAINER_NAME))" ]; then \
		docker run -d --name $(WEBSERV_CONTAINER_NAME) -p $(WEBSERV_PORT):$(WEBSERV_PORT) $(WEBSERV_IMAGE_NAME):$(WEBSERV_IMAGE_TAG); \
	else \
		echo "$(WEBSERV_CONTAINER_NAME) is already running."; \
	fi

.PHONY	: stop-webserv
stop-webserv:
	@if [ -z "$$(docker ps -qf name=$(WEBSERV_CONTAINER_NAME))" ]; then \
		echo "$(WEBSERV_CONTAINER_NAME) already stopped."; \
	else \
		docker stop $(WEBSERV_CONTAINER_NAME); \
	fi

.PHONY	: rm-webserv
rm-webserv:
	@if [ -z "$$(docker ps -aqf name=$(WEBSERV_CONTAINER_NAME))" ]; then \
		echo "$(WEBSERV_CONTAINER_NAME) does not exist."; \
	else \
		docker rm $(WEBSERV_CONTAINER_NAME); \
	fi

.PHONY	: clean-webserv
clean-webserv:
	@make stop-webserv && make rm-webserv

.PHONY	: fclean-webserv
fclean-webserv:
	@make clean-webserv
	@if [ -z "$$(docker image ls -aqf reference=$(WEBSERV_IMAGE_NAME):$(WEBSERV_IMAGE_TAG))" ]; then \
		echo "$(WEBSERV_IMAGE_NAME):$(WEBSERV_IMAGE_TAG) does not exist."; \
	else \
		docker rmi $(WEBSERV_IMAGE_NAME):$(WEBSERV_IMAGE_TAG); \
	fi

.PHONY	: log-webserv
log-webserv:
	@if [ -z "$$(docker ps -qf name=$(WEBSERV_CONTAINER_NAME))" ]; then \
		echo "$(WEBSERV_CONTAINER_NAME) does not exist."; \
	else \
		docker logs $(WEBSERV_CONTAINER_NAME); \
	fi

.PHONY	: ps
ps:
	@docker ps -a

.PHONY	: login-webserv
login-webserv:
	@if [ -z "$$(docker ps -qf name=$(WEBSERV_CONTAINER_NAME))" ]; then \
		echo "$(WEBSERV_CONTAINER_NAME) does not exist."; \
	else \
		docker exec -it $(WEBSERV_CONTAINER_NAME) /bin/bash; \
	fi

#--------------------------------------------

-include $(DEPS)
