#include "directive_names.hpp"

namespace config {

const std::string SERVER   = "server";
const std::string LOCATION = "location";

const std::string HOST                 = "host";
const std::string LISTEN               = "listen";
const std::string SERVER_NAME          = "server_name";
const std::string ERROR_PAGE           = "error_page";
const std::string CLIENT_MAX_BODY_SIZE = "client_max_body_size";

const std::string ALLOWED_METHODS = "allowed_methods";
const std::string RETURN          = "return";
const std::string ALIAS           = "alias";
const std::string AUTO_INDEX      = "autoindex";
const std::string INDEX           = "index";

extern const std::string VALID_ALLOWED_METHODS[] = {"GET", "DELETE", "POST"};
extern const std::size_t SIZE_OF_VALID_ALLOWED_METHODS =
	sizeof(VALID_ALLOWED_METHODS) / sizeof(VALID_ALLOWED_METHODS[0]);

const std::string CGI_EXTENSION = "cgi_extension";
const std::string UPLOAD_DIR    = "upload_dir";

} // namespace config
