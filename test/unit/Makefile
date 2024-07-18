#--------------------------------------------
# run all unit test
#--------------------------------------------
# Add target test directories.
# Each directory should have a Makefile with a 'run' target.
TEST_DIRS	:=	srcs/convert_str srcs/split

.PHONY	: run
run:
	@status=0; \
	for dir in $(TEST_DIRS); do \
		echo -e "\n\033[30mRunning unit tests in $$dir...\033[0m"; \
		(MAKEFLAGS="-s" make -C $$dir run); \
		dir_status=$$?; \
		status=$$((status | dir_status)); \
	done; \
	exit $$status;
