# Add target test directories.
# Each directory should have a Makefile with a 'run' target.
TEST_DIRS	:=	convert_str \
				http \
				http_parse \
				http_storage \
				http_serverinfo_check \
				sock_context \
				split_str \
				virtual_server \
				virtual_server_storage \
				config_parse/lexer \
				config_parse/parser \
				config_parse \
				stat

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
