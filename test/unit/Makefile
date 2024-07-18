#--------------------------------------------
# run each unit test
#--------------------------------------------
TEST_CONVERT_STR_DIR	:=	srcs/convert_str
.PHONY	: run-convert-str
run-convert-str:
	@make -C $(TEST_CONVERT_STR_DIR) run

TEST_SPLIT_DIR	:=	srcs/split
.PHONY	: run-split
run-split:
	@make -C $(TEST_SPLIT_DIR) run

#--------------------------------------------
# run all unit test
#--------------------------------------------
# Add target test directories.
# Each directory should have a Makefile with a 'run' target.
TEST_DIRS	:=	$(TEST_CONVERT_STR_DIR) \
				$(TEST_SPLIT_DIR)

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
