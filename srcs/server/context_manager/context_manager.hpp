#ifndef CONTEXT_MANAGER_HPP_
#define CONTEXT_MANAGER_HPP_

#include <string>

namespace server {

// holds and manages virtual server info and socket context(server socket info, client socket info).
class ContextManager {
  public:
	ContextManager();
	~ContextManager();
	ContextManager(const ContextManager &other);
	ContextManager &operator=(const ContextManager &other);
};

} // namespace server

#endif /* CONTEXT_MANAGER_HPP_ */