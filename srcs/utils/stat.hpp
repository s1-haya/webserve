#ifndef STAT_HPP_
#define STAT_HPP_

namespace utils {

class Stat {
  public:
	Stat();
	~Stat();
	Stat(const Stat &other);
	Stat &operator=(const Stat &other);
};

} // namespace utils

#endif
