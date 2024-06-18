IMAGE_NAME := webserv
CONTAINER_NAME := webserv-container

.PHONY	: build
build:
	@docker build -t $(IMAGE_NAME) .

.PHONY	: docker-run
docker-run:
	@docker run -d --name $(CONTAINER_NAME) -p 8080:8080 $(IMAGE_NAME)

.PHONY	: stop
stop:
	@docker stop $(CONTAINER_NAME)

.PHONY	: rm
rm:
	@docker rm $(CONTAINER_NAME)

.PHONY	: docker-clean
docker-clean:
	@make stop && make rm

.PHONY	: rmi
rmi:
	@make docker-clean && docker rmi $(IMAGE_NAME)

.PHONY	: log
log:
	@docker logs $(CONTAINER_NAME)

.PHONY	: ps
ps:
	@docker ps -a

.PHONY	: login
login:
	@docker exec -it $(CONTAINER_NAME) /bin/bash
