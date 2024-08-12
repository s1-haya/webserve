#ifndef STAT_HPP_
#define STAT_HPP_

#include <string>
#include <sys/stat.h>

namespace utils {

class Stat {
  public:
	Stat(const std::string &path);
	~Stat();
	Stat(const Stat &other);
	Stat              &operator=(const Stat &other);
	const struct stat &GetStatBuffer();

  private:
	Stat();
	struct stat stat_buf_;
};

} // namespace utils

#endif
