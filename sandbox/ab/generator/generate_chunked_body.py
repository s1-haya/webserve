import random
import string


def generate_random_chunked_body(min_total_size=2025):
    total_size = 0
    chunked_body = []
    all_chunked_data = []

    while total_size < min_total_size:
        # randomなchunk size(50〜500bytes程度）
        # 最後のchunkは合計を超えないように調整
        chunk_size = min(random.randint(50, 500), min_total_size - total_size)

        # chunk_data生成
        chunk_data = "".join(
            random.choices(string.ascii_letters + string.digits, k=chunk_size)
        )
        chunked_body.append(f"{chunk_size:x}\r\n{chunk_data}\r\n")

        # totalを保存
        total_size += chunk_size
        all_chunked_data += chunk_data

    # 終端追加
    chunked_body.append("0\r\n\r\n")

    return "".join(chunked_body), "".join(all_chunked_data)


# 生成したchunk body, unchunk body
chunked_body, unchunked_body = generate_random_chunked_body()
print(chunked_body)
print("=======================")
print(unchunked_body)
