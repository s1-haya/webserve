include docker/config.mk

.PHONY	: run-webserv
run-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker exec $(CONTAINER_NAME) supervisorctl -c /etc/supervisor/conf.d/supervisord.conf start webserv; \
	fi

.PHONY	: stop-webserv
stop-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker exec $(CONTAINER_NAME) supervisorctl -c /etc/supervisor/conf.d/supervisord.conf stop webserv; \
	fi

.PHONY	: ps-webserv
ps-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker exec $(CONTAINER_NAME) ps aux; \
	fi

.PHONY	: log-webserv
log-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker exec $(CONTAINER_NAME) cat /var/log/webserv.err.log; \
	fi
	
.PHONY	: error-log-webserv
error-log-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker exec $(CONTAINER_NAME) cat /var/log/webserv.err.log; \
	fi

.PHONY	: val-webserv
val-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker exec -d $(CONTAINER_NAME) make val; \
	fi

.PHONY	: check-webserv
check-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		echo "Container $(CONTAINER_NAME) does not exist."; \
	else \
		docker exec -d $(CONTAINER_NAME) make check; \
	fi
