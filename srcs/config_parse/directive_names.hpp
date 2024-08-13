#ifndef DIRECTIVE_NAMES_HPP_
#define DIRECTIVE_NAMES_HPP_

#include <string>

namespace config {

/**
 * @brief Block Directive (Context Directive)
 * @details Dir_name {}
 */

extern const std::string SERVER;
extern const std::string LOCATION;

/**
 * @brief Directive in Context
 * @details Dir_name args;
 */

extern const std::string SERVER_NAME;
extern const std::string LISTEN;
extern const std::string ALIAS;
extern const std::string INDEX;
extern const std::string AUTO_INDEX;
extern const std::string ERROR_PAGE;
extern const std::string CLIENT_MAX_BODY_SIZE;
extern const std::string ALLOWED_METHODS;
extern const std::string RETURN;

} // namespace config

#endif
