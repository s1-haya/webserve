#include "cgi_manager.hpp"
#include "system_exception.hpp"
#include <cerrno>
#include <cstring>   // strerror
#include <stdexcept> // logic_error

namespace server {

CgiManager::CgiManager() {}

CgiManager::~CgiManager() {
	for (ItrCgiAddrMap it = cgi_addr_map_.begin(); it != cgi_addr_map_.end(); ++it) {
		delete it->second;
	}
}

// todo: 1complete_request(1CGI)につき2回以上この関数が呼ばれないかhttpの実装を確認
// throw(SystemException)
void CgiManager::AddNewCgi(int client_fd, const cgi::CgiRequest &request) {
	Cgi *cgi = new (std::nothrow) Cgi(request);
	if (cgi == NULL) {
		throw SystemException("AddNewCgi: Failed to allocate memory");
	}
	if (cgi_addr_map_.count(client_fd) != 0) {
		throw std::logic_error("AddNewCgi: client_fd already exists");
	}
	cgi_addr_map_[client_fd] = cgi;
}

// throw(SystemException)
void CgiManager::RunCgi(int client_fd) {
	Cgi *cgi = GetCgi(client_fd);

	cgi->Run();
	// pipe_fdとclient_fdの紐づけをFdMapに追加
	if (cgi->IsReadRequired()) {
		client_fd_map_[cgi->GetReadFd()] = client_fd;
	}
	if (cgi->IsWriteRequired()) {
		client_fd_map_[cgi->GetWriteFd()] = client_fd;
	}
}

void CgiManager::DeleteCgi(int client_fd) {
	const Cgi *cgi = GetCgi(client_fd);

	// FdMapから削除
	if (cgi->IsReadRequired()) {
		client_fd_map_.erase(cgi->GetReadFd());
	}
	if (cgi->IsWriteRequired()) {
		client_fd_map_.erase(cgi->GetWriteFd());
	}
	// cgi*削除
	delete cgi;
	// CgiAddrMapから削除
	cgi_addr_map_.erase(client_fd);
}

CgiManager::GetFdResult CgiManager::GetReadFd(int client_fd) const {
	if (cgi_addr_map_.count(client_fd) == 0) {
		throw std::logic_error("GetReadFd: client_fd doesn't exists");
	}

	GetFdResult result;
	const Cgi  *cgi = cgi_addr_map_.at(client_fd);
	if (!cgi->IsReadRequired()) {
		result.Set(false);
		return result;
	}
	result.SetValue(cgi->GetReadFd());
	return result;
}

CgiManager::GetFdResult CgiManager::GetWriteFd(int client_fd) const {
	if (cgi_addr_map_.count(client_fd) == 0) {
		throw std::logic_error("GetWriteFd: client_fd doesn't exists");
	}

	GetFdResult result;
	const Cgi  *cgi = cgi_addr_map_.at(client_fd);
	if (!cgi->IsWriteRequired()) {
		result.Set(false);
		return result;
	}
	result.SetValue(cgi->GetWriteFd());
	return result;
}

int CgiManager::GetClientFd(int pipe_fd) const {
	try {
		return client_fd_map_.at(pipe_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("GetClientFd: " + std::string(e.what()));
	}
}

const std::string &CgiManager::GetRequest(int client_fd) const {
	try {
		const Cgi *cgi = cgi_addr_map_.at(client_fd);
		return cgi->GetRequest();
	} catch (const std::exception &e) {
		throw std::logic_error("GetRequest: " + std::string(e.what()));
	}
}

cgi::CgiResponse CgiManager::AddAndGetResponse(int client_fd, const std::string &read_buf) {
	try {
		Cgi *cgi = cgi_addr_map_.at(client_fd);
		return cgi->AddAndGetResponse(read_buf);
	} catch (const std::exception &e) {
		throw std::logic_error("AddAndGetResponse: " + std::string(e.what()));
	}
}

void CgiManager::ReplaceNewRequest(int client_fd, const std::string &new_request_str) {
	try {
		Cgi *cgi = cgi_addr_map_.at(client_fd);
		cgi->ReplaceNewRequest(new_request_str);
	} catch (const std::exception &e) {
		throw std::logic_error("ReplaceNewRequest: " + std::string(e.what()));
	}
}

bool CgiManager::IsCgiExist(int fd) const {
	// fd: client_fd
	if (cgi_addr_map_.count(fd) != 0) {
		return true;
	}
	// fd: pipe_fd
	if (client_fd_map_.count(fd) != 0) {
		return true;
	}
	return false;
}

CgiManager::Cgi *CgiManager::GetCgi(int client_fd) {
	try {
		return cgi_addr_map_.at(client_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("GetCgi: " + std::string(e.what()));
	}
}

const CgiManager::Cgi *CgiManager::GetCgi(int client_fd) const {
	try {
		return cgi_addr_map_.at(client_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("GetCgi: " + std::string(e.what()));
	}
}

} // namespace server
