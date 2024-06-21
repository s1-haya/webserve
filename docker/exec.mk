include docker/config.mk
include docker/command.mk

.PHONY	: run-webserv
run-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		make run-docker-bg; \
		sleep 1; \
		docker exec $(CONTAINER_NAME) supervisorctl -c /etc/supervisor/conf.d/supervisord.conf start webserv; \
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
		docker exec $(CONTAINER_NAME) cat /var/log/webserv.log; \
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
		make run-docker-bg; \
		sleep 1; \
		docker exec $(CONTAINER_NAME) make val; \
		make clean-docker; \
	else \
		docker exec -d $(CONTAINER_NAME) make val; \
	fi

.PHONY	: check-webserv
check-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		make run-docker-bg; \
		sleep 1; \
		docker exec $(CONTAINER_NAME) make check; \
		make clean-docker; \
	else \
		docker exec -d $(CONTAINER_NAME) make check; \
	fi

.PHONY	: test-webserv
test-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		make run-docker-bg; \
		sleep 1; \
		docker exec $(CONTAINER_NAME) make test; \
		make clean-docker; \
	else \
		make test; \
	fi

.PHONY	: unit-webserv
unit-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		make run-docker-bg; \
		sleep 1; \
		docker exec $(CONTAINER_NAME) make unit; \
		make clean-docker; \
	else \
		make unit; \
	fi

.PHONY	: e2e-webserv
e2e-webserv:
	@if [ -z "$$(docker ps -qf name=$(CONTAINER_NAME))" ]; then \
		make run-docker-bg; \
		sleep 1; \
		docker exec $(CONTAINER_NAME) make e2e; \
		make clean-docker; \
	else \
		make e2e; \
	fi
