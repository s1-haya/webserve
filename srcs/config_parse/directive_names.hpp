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
 * @brief Directive in Server Context
 * @details Dir_name args;
 *
 * @note Host dir Not implemented yet
 */

extern const std::string HOST;
extern const std::string LISTEN;
extern const std::string SERVER_NAME;
extern const std::string ERROR_PAGE;
extern const std::string CLIENT_MAX_BODY_SIZE;

/**
 * @brief Directive in Location Context
 * @details Dir_name args;
 */

extern const std::string ALLOWED_METHODS;
extern const std::string RETURN;
extern const std::string ALIAS;
extern const std::string AUTO_INDEX;
extern const std::string INDEX;

extern const std::string VALID_ALLOWED_METHODS[];
extern const std::size_t VALID_ALLOWED_METHODS_SIZE;

/**
 * @brief Directive in Location Context for CGI
 * @details Dir_name args;
 *
 * @note Not Implemented yet
 */

extern const std::string CGI_EXTENSION;
extern const std::string UPLOAD_DIR;

} // namespace config

#endif
