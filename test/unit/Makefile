# Add target test directories.
# Each directory should have a Makefile with a 'run' target.
TEST_DIRS	:=	convert_str \
				http_parse \
				split_str \
				virtual_server \
				virtual_server_storage

.PHONY	: run
run:
	@status=0; \
	for dir in $(TEST_DIRS); do \
		echo "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"; \
		echo "┃ $$dir"; \
		echo "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"; \
		(MAKEFLAGS="-s" make -C $$dir run); \
		dir_status=$$?; \
		status=$$((status | dir_status)); \
	done; \
	exit $$status;
