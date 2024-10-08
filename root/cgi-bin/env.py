#!/usr/bin/env python3

import os

def print_cgi_env():
    for key, value in os.environ.items():
        print(f"{key}: {value}")

if __name__ == "__main__":
    print("Content-Type: text/plain\r\n\r\n")
    print_cgi_env()