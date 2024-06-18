IMAGE_NAME := webserv-image
IMAGE_TAG := latest
CONTAINER_NAME := webserv-container

.PHONY	: build
build:
	@docker build -t $(IMAGE_NAME):$(IMAGE_TAG) .

.PHONY	: docker-run
docker-run:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		docker run -d --name $(CONTAINER_NAME) -p 8080:8080 $(IMAGE_NAME); \
	else \
		echo "Container $(CONTAINER_NAME) is already running."; \
	fi

.PHONY	: stop
stop:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) already stopped."; \
	else \
		docker stop $(CONTAINER_NAME); \
	fi

.PHONY	: rm
rm:
	@if [ -z "$$(docker ps -aqf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker rm $(CONTAINER_NAME); \
	fi

.PHONY	: docker-clean
docker-clean:
	@make stop && make rm

.PHONY	: rmi
rmi:
	@make docker-clean
	@if [ -z "$$(docker image ls -aqf reference=$(IMAGE_NAME):$(IMAGE_TAG))" ]; then \
		echo "Image $(IMAGE_NAME) does not exist."; \
	else \
		docker rmi $(IMAGE_NAME):$(IMAGE_TAG); \
	fi

.PHONY	: log
log:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker logs $(CONTAINER_NAME); \
	fi

.PHONY	: ps
ps:
	@docker ps -a

.PHONY	: login
login:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker exec -it $(CONTAINER_NAME) /bin/bash; \
	fi
