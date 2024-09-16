#include "test_case.hpp"

/**
 * @brief Test Http class: HttpResultの返り値を確認するテスト。
 *
 * テスト一覧（今後追加）
 * - メソッドが機能しているかどうか
 * 	- GET
 * 	- POST
 * 	- DELETE
 * - リクエストターゲットが機能しているかどうか(CGIは今後)
 * 	- 存在してるファイル
 * 	- 存在してないファイル
 * 	- 存在してるディレクトリ（末尾に/ありとなし）
 * 	- 存在してないディレクトリ（末尾に/ありとなし）
 * - ヘッダーフィールドごとに機能しているかどうか(今後、追加)
 * 	- Host
 * 	- Content-Length
 * - VirtualServer, Locationの各ディレクティブが機能しているかどうか
 * 	- VirtualServer
 * 	 - server_names
 * 	- Location
 * 	 - allow_methods
 *
 * responseの期待値はstatus_line, header_fields, CRLF, body_messageを組み合わせる。
 * status_lineとデフォルトのステータスコードを返すbody_messageはファイルで用意する。header_fieldsは可変のため文字列で作成する。
 * response以外のHttpResultのパラメータの期待値はテストごとに作成。
 *
 * HttpResult
 * - is_response_complete レスポンスの書式が完全かどうか
 * - is_connection_keep   次の接続が可能かどうか
 * - request_buf          バッファ
 * - response             文字列
 *
 * Locationのディレクティブを網羅するテストができるようなvirtual_serversを設定する。
 * client_infosはfdは1で固定する。request_bufはファイルから読み込むで文字列にする。
 *
 * @param client_infos
 * @param virtual_servers
 *
 * @return HttpResult indicating whether the response is complete
 *         and containing the response data.
 */

int main(void) {
	int ret_code = EXIT_SUCCESS;

	// todo: InitServerInfos;
	server::VirtualServerAddrList server_infos;
	ret_code |= test::test_no_connection(server_infos);

	return ret_code;
}
