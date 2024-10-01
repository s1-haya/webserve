#ifndef TEST_CASE_HPP_
#define TEST_CASE_HPP_

#include <list>

namespace server {

class VirtualServer;
typedef std::list<const VirtualServer *> VirtualServerAddrList;

} // namespace server

namespace test {

// 2xx
int TestGetOk1ConnectionClose(const server::VirtualServerAddrList &server_infos);
int TestGetOk2ConnectionKeep(const server::VirtualServerAddrList &server_infos);
int TestGetOk3SubConnectionClose(const server::VirtualServerAddrList &server_infos);
int TestGetOk4ConnectionKeepAndOkConnectionKeep(const server::VirtualServerAddrList &server_infos);
int TestGetOk5ConnectionCloseAndOkConnectionClose(const server::VirtualServerAddrList &server_infos
);
int TestGetOk6ConnectionKeepAndOkConnectionClose(const server::VirtualServerAddrList &server_infos);
int TestGetOk7DuplicateConnectionKeep(const server::VirtualServerAddrList &server_infos);
int TestGetOk8DuplicateConnectionClose(const server::VirtualServerAddrList &server_infos);
int TestGetOk9ConnectionKeepAndClose(const server::VirtualServerAddrList &server_infos);
int TestGetOk10ConnectionCloseAndKeep(const server::VirtualServerAddrList &server_infos);
int TestGetOk11UpperAndLowerHeaderFields(const server::VirtualServerAddrList &server_infos);
int TestGetOk12HeaderFieldValueSpace(const server::VirtualServerAddrList &server_infos);
int TestGetOk13SpaceHeaderFieldValue(const server::VirtualServerAddrList &server_infos);
int TestGetOk14ExtraRequest(const server::VirtualServerAddrList &server_infos);
int TestGetOk15BodyMessageDefault(const server::VirtualServerAddrList &server_infos);
int TestGetOk17NotExistHeaderField(const server::VirtualServerAddrList &server_infos);
int TestGetOk21NoConnection(const server::VirtualServerAddrList &server_infos);

// 4xx
int TestGetBadRequest1OnlyCrlf(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest2LowerMethod(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest3NoAsciiMethod(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest4NoRoot(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest5RelativePath(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest6LowerHttpVersion(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest7WrongHttpName(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest8WrongHttpVersion(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest9NoHost(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest10DuplicateHost(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest11NoHeaderFieldColon(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest12NoConnectionName(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest13NoConnectionValue(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest14WrongConnectionValue(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest15SpaceInHeaderFieldName(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest16HeaderFieldNameSpaceColon(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest17SpaceHeaderFieldName(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest18NonVchrHeaderFieldName(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest19NonVchrHeaderFieldValue(const server::VirtualServerAddrList &server_infos);

int TestGetNotFound1NotExistFile(const server::VirtualServerAddrList &server_infos);
int TestGetMethodNotAllowed(const server::VirtualServerAddrList &server_infos);
int TestGetTimeout1NoCrlf(const server::VirtualServerAddrList &server_infos);
// 5xx
int TestGetNotImplemented1NotExistMethod(const server::VirtualServerAddrList &server_infos);

// cgi
int TestCgiGetOk1PrintOkBodyMessage(const server::VirtualServerAddrList &server_infos);
int TestCgiGetOk2PrintOkInPerl(const server::VirtualServerAddrList &server_infos);
int TestCgiGetOk3PrintOkInPython(const server::VirtualServerAddrList &server_infos);
int TestCgiGetOk4PrintOkInShell(const server::VirtualServerAddrList &server_infos);

// GetErrorResponse
int TestRequestTimeoutResponse();
int TestInternalServerErrorResponse();

// GetResponseFromCgi
int TestGetResponseFromCgi1();
int TestGetResponseFromCgi2();

} // namespace test

#endif
