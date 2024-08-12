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
	bool               IsRegularFile() const;
	bool               IsDirectory() const;
	bool               IsReadableFile() const;
	bool               IsWritableFile() const;
	const struct stat &GetStatBuffer();
	std::size_t        GetFileSize() const;

  private:
	Stat();
	struct stat stat_buf_;
};

} // namespace utils

#endif
