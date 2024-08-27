#include "color.hpp"
#include "message_manager.hpp"
#include <cstdlib>
#include <iostream>
#include <list>
#include <sstream>  // ostringstream
#include <unistd.h> // sleep

namespace {

// このunit testがtimeout 3sであること前提で作られているので、
// Server::REQUEST_TIMEOUT を直に呼ばない
static const double REQUEST_TIMEOUT = 3.0;

typedef server::MessageManager::TimeoutFds TimeoutFds;

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
	std::string error_log;
};

int GetTestCaseNum() {
	static int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

void PrintOk() {
	std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
			  << std::endl;
}

void PrintNg() {
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
}

void PrintError(const std::string &message) {
	std::cerr << utils::color::RED << message << utils::color::RESET << std::endl;
}

template <typename T>
bool IsSame(const T &result, const T &expected) {
	return result == expected;
}

int Test(Result result) {
	if (result.is_success) {
		PrintOk();
		return EXIT_SUCCESS;
	}
	PrintNg();
	PrintError(result.error_log);
	return EXIT_FAILURE;
}

// -----------------------------------------------------------------------------
template <typename T>
void DeleteValue(TimeoutFds &lst, const T &value) {
	typedef typename TimeoutFds::iterator Itr;
	for (Itr it = lst.begin(); it != lst.end(); ++it) {
		if (*it == value) {
			lst.erase(it);
			return;
		}
	}
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::list<T> &lst) {
	typedef typename std::list<T>::const_iterator It;
	for (It it = lst.begin(); it != lst.end(); ++it) {
		os << "[" << *it << "]";
	}
	return os;
}

Result
RunIsSameTimeoutFds(server::MessageManager &manager, const TimeoutFds &expected_timeout_fds) {
	Result             result;
	std::ostringstream oss;

	const TimeoutFds &timeout_fds = manager.GetNewTimeoutFds(REQUEST_TIMEOUT);
	if (!IsSame(timeout_fds, expected_timeout_fds)) {
		result.is_success = false;
		oss << "timeout_fds" << std::endl;
		oss << "- result  : " << timeout_fds << std::endl;
		oss << "- expected: " << expected_timeout_fds << std::endl;
	}
	result.error_log = oss.str();
	return result;
}

// -----------------------------------------------------------------------------
// add fd            : 4 5         6
// timeout(3s)       :       4 5         6
// current time      : 0 1 2 3 4 5 6 7 8 9 10
// GetNewTimeoutFds():           *   *     *
// -----------------------------------------------------------------------------
int RunTestGetTimeoutFds() {
	int ret_code = EXIT_SUCCESS;

	server::MessageManager manager;
	TimeoutFds             expected_timeout_fds;

	// time(0), add fd: 4
	manager.AddNewMessage(4);

	sleep(1);
	// time(1), add fd: 5
	manager.AddNewMessage(5);

	sleep(2);
	// time(3), timeout fd: 4
	expected_timeout_fds.push_back(4);

	sleep(1);
	// time(4), timeout fd: 5
	expected_timeout_fds.push_back(5);

	sleep(1);
	// time(5), GetNewTimeoutFds: {4, 5}
	ret_code |= Test(RunIsSameTimeoutFds(manager, expected_timeout_fds)); // test1
	expected_timeout_fds.clear();

	sleep(1);
	// time(6), add fd: 6
	manager.AddNewMessage(6);

	sleep(1);
	// time(7), GetNewTimeoutFds: {}
	ret_code |= Test(RunIsSameTimeoutFds(manager, expected_timeout_fds)); // test2

	sleep(3);
	// time(9), timeout fd: 6
	expected_timeout_fds.push_back(6);

	sleep(1);
	// time(10), GetNewTimeoutFds: {6}
	ret_code |= Test(RunIsSameTimeoutFds(manager, expected_timeout_fds)); // test3

	return ret_code;
}

// -----------------------------------------------------------------------------
// add fd            : 4 5       6
// timeout(3s)       :       4 5       6
// current time      : 0 1 2 3 4 5 6 7 8
// UpdateMessage()   :               *
// -----------------------------------------------------------------------------
//       ↓ 古い5削除&5新規追加
// -----------------------------------------------------------------------------
// add fd            : 4         6   5
// timeout(3s)       :       4         6   5
// current time      : 0 1 2 3 4 5 6 7 8 9 10 11
// GetNewTimeoutFds():                   *    *
// -----------------------------------------------------------------------------
int RunTestDeleteSentResponseAndResetTime() {
	int ret_code = EXIT_SUCCESS;

	server::MessageManager manager;
	TimeoutFds             expected_timeout_fds;

	// time(0), add fd: 4
	manager.AddNewMessage(4);

	sleep(1);
	// time(1), add fd: 5
	manager.AddNewMessage(5);

	sleep(2);
	// time(3), timeout fd: 4
	manager.AddPrimaryResponse(4, server::message::CLOSE, "test response");
	expected_timeout_fds.push_back(4);

	sleep(1);
	// time(4), timeout fd: 5
	manager.AddPrimaryResponse(5, server::message::CLOSE, "test response");
	expected_timeout_fds.push_back(5);

	sleep(1);
	// time(5), add fd: 6
	manager.AddNewMessage(6);

	sleep(2);
	// time(7), update start_time, delete oldest response.
	manager.DeleteSentResponseAndResetTime(5);
	DeleteValue(expected_timeout_fds, 5);

	sleep(1);
	// time(8), timeout fd: 6
	expected_timeout_fds.push_back(6);

	sleep(1);
	// time(9), GetNewTimeoutFds: {4, 6}
	ret_code |= Test(RunIsSameTimeoutFds(manager, expected_timeout_fds)); // test4
	expected_timeout_fds.clear();

	sleep(1);
	// time(10), timeout fd: 5
	expected_timeout_fds.push_back(5);

	sleep(1);
	// time(11), GetNewTimeoutFds: {5}
	ret_code |= Test(RunIsSameTimeoutFds(manager, expected_timeout_fds)); // test5

	return ret_code;
}

int RunTestDeleteMessage() {
	int ret_code = EXIT_SUCCESS;

	server::MessageManager manager;
	TimeoutFds             expected_timeout_fds;

	// time(0), add fd: 4,5
	manager.AddNewMessage(4);
	manager.AddNewMessage(5);

	// time(0), delete fd: 5
	manager.DeleteMessage(5);

	// time(0), GetNewTimeoutFds: {}
	ret_code |= Test(RunIsSameTimeoutFds(manager, expected_timeout_fds)); // test6

	sleep(3);
	// time(3), timeout fd: 4
	expected_timeout_fds.push_back(4);

	sleep(1);
	// time(4), GetNewTimeoutFds: {4}
	ret_code |= Test(RunIsSameTimeoutFds(manager, expected_timeout_fds)); // test7
	expected_timeout_fds.clear();

	// time(4), GetNewTimeoutFds: {}
	ret_code |= Test(RunIsSameTimeoutFds(manager, expected_timeout_fds)); // test8

	return ret_code;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTestGetTimeoutFds();
	ret_code |= RunTestDeleteSentResponseAndResetTime();
	ret_code |= RunTestDeleteMessage();

	return ret_code;
}
