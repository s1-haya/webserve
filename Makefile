
.PHONY: build up ps down stop rm loging-nginx

build:
	docker compose -f ./docker-compose.yml build

up:
	docker compose -f ./docker-compose.yml up -d

ps:
	docker compose -f ./docker-compose.yml ps

down: stop rm

stop:
	docker compose -f ./docker-compose.yml stop

rm:
	docker compose -f ./docker-compose.yml rm

login-nginx:
	docker exec -it webserve-nginx-1 /bin/bash



