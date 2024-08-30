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
typedef std::deque<std::string>            ResponseDeque;
typedef server::message::Response          Response;

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

// -----------------------------------------------------------------------------
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

std::ostream &operator<<(std::ostream &os, const ResponseDeque &dq) {
	typedef typename ResponseDeque::const_iterator It;
	for (It it = dq.begin(); it != dq.end(); ++it) {
		os << "[" << *it << "]";
	}
	return os;
}

bool IsSameResponseDeque(
	server::MessageManager &manager,
	ResponseDeque          &result_responses,
	ResponseDeque           expected_responses,
	int                     client_fd
) {
	// 単純なgetterがないので比較対象のResponseDequeの中身を取り出す
	while (manager.IsResponseExist(client_fd)) {
		const Response &response = manager.PopHeadResponse(client_fd);
		result_responses.push_back(response.response_str);
	}
	return result_responses == expected_responses;
}

Result RunIsSameResponseDeque(
	server::MessageManager &manager, const ResponseDeque &expected_responses, int client_fd
) {
	Result             result;
	std::ostringstream oss;

	ResponseDeque result_responses;
	if (!IsSameResponseDeque(manager, result_responses, expected_responses, client_fd)) {
		result.is_success = false;
		oss << "response_deque" << std::endl;
		oss << "- result  : " << result_responses << std::endl;
		oss << "- expected: " << expected_responses << std::endl;
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
int RunTestUpdateTime() {
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
	manager.UpdateTime(5);
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

void PushBackResponse(
	server::MessageManager &manager,
	ResponseDeque          &expected_responses,
	int                     client_fd,
	const std::string      &response
) {
	manager.AddNormalResponse(client_fd, server::message::KEEP, response);
	expected_responses.push_back(response);
}

void PushFrontResponse(
	server::MessageManager &manager,
	ResponseDeque          &expected_responses,
	int                     client_fd,
	const std::string      &response
) {
	manager.AddPrimaryResponse(client_fd, server::message::KEEP, response);
	expected_responses.push_front(response);
}

/*
MessageManager class主な使用関数
- AddNormalResponse()
- AddPrimaryResponse()
- PopHeadResponse()
- IsResponseExist()
*/
int RunTestResponseDeque() {
	int ret_code = EXIT_SUCCESS;

	server::MessageManager manager;
	ResponseDeque          expected_responses;

	static const int client_fd = 4;
	// add fd: 4
	manager.AddNewMessage(client_fd);

	// push_back response : {res1}
	PushBackResponse(manager, expected_responses, client_fd, "res1");
	// push_back response : {res1, res2}
	PushBackResponse(manager, expected_responses, client_fd, "res2");
	// push_front response: {res3, res1, res2}
	PushFrontResponse(manager, expected_responses, client_fd, "res3");

	ret_code |= Test(RunIsSameResponseDeque(manager, expected_responses, client_fd)); // test9

	return ret_code;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTestGetTimeoutFds();
	ret_code |= RunTestUpdateTime();
	ret_code |= RunTestDeleteMessage();
	ret_code |= RunTestResponseDeque();

	return ret_code;
}
