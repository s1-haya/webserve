// todo: 各メソッドを実行する関数
// - メソッドの権限の確認
//  -> 権限がない場合405 Not Allowed
// - 各メソッドを実行 

// todo: ExecuteGET
// 1. パスの作成(locationのpath (aliasがある場合は aliasのパス))
// - aliasがある場合aliasのパス。それ以外はdefault root(/html) + locationのパス
// 2. statクラスを作成
// 3. パスの中身確認
// ディレクトリの場合
// 1. indexディレクティブがある場合は確認あればファイル確認へ
// 2. autoindex確認。onの場合はExecuteAutoindex、なければ403
// ファイルの場合
// - 権限確認 なければ403 Forbidden
// 4. ボディメッセージに格納
// パスがディレクトリの場合(autoindexはonの場合は表示): 403 Forbidden
// 存在しないファイルの場合: 404 Not Found

// 301 Moved Permanetly
// - リクエストターゲットのパスがディレクトリかつ末尾に/がない
// alias, return, index, autoindexでファイルが特定

// 500 Internal Server Error<
// わからん

// todo: ExecutePost: ボディメッセージをリソースに書き込む。ファイルアップロード
// パスがディレクトリの場合(autoindexはon, off関係なし): 403 Forbidden
// ファイル権限がない場合: 403 Forbidden
// 存在しないファイルの場合: 404 Not Found
// ファイルの書き込みが失敗した場合: 500 Internal Server Error

// todo: ExcuteDelete: リソースの削除
// 成功した場合、204 No Content
// -> 詳細を表示しない設定にするため
// パスがディレクトリの場合(autoindexはon,
// off関係なし): 403 Forbidden
// ファイル権限がない場合: 403 Forbidden
// 存在しないファイルの場合: 404 Not Found
