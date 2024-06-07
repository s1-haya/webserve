#!/usr/bin/python3

import subprocess

def run_pytest():
    try:
      result = subprocess.run(['pytest'], check=True, capture_output=True, stderr=subprocess.PIPE)
      stderr_output = result.stderr.decode()
      print(stderr_output)
    except subprocess.CalledProcessError as e:
      print(f"pytest failed: {e}")
      print(e.stderr.decode())

if __name__ == "__main__":
    run_pytest()
