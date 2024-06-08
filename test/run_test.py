#!/usr/bin/python3

import subprocess

def run_test():
	try:
		subprocess.run(['pytest'], check=True)
		exit(1)
	except subprocess.CalledProcessError as e:
		print(f"pytest failed with exit code {e.returncode}")

if __name__ == "__main__":
    run_test()
