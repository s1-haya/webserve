#ifndef STAT_HPP_
#define STAT_HPP_

#include <string>
#include <sys/stat.h>

namespace http {

class Stat {
  public:
	Stat(const std::string &path);
	Stat(const Stat &other);
	Stat &operator=(const Stat &other);
	~Stat();

	bool IsRegularFile() const;
	bool IsDirectory() const;
	bool IsReadableFile() const;
	bool IsWritableFile() const;

	std::size_t GetFileSize() const;

  private:
	Stat();
	struct stat stat_buf_;
};

} // namespace http

#endif
