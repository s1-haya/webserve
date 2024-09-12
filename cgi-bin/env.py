#!/usr/bin/env python3

import os

def print_cgi_env():
    # CGI環境変数を取得してプリント
    for key, value in os.environ.items():
        print(f"{key}: {value}")

if __name__ == "__main__":
    print_cgi_env()