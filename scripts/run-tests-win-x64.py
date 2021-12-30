import os
import subprocess

ANSI_PASS = "\033[92m"
ANSI_FAIL = "\033[91m"
ANSI_BOLD = "\033[1m"
ANSI_END = "\033[0m"

num_tests = 0
num_passed_tests = 0

def run_test(path, name):
    global num_passed_tests

    test_working_directory = path
    run_command = os.path.join(".", "bin", "win-x64", "release", "test.exe")

    result = subprocess.run(run_command, shell = True, cwd = test_working_directory)

    if result.returncode == 0:
        print("---- " + ANSI_PASS + name + " passed" + ANSI_END)
        num_passed_tests += 1
    else:
        print("---- " + ANSI_FAIL + name + " failed with code " + str(result.returncode) + ANSI_END)

print("-- Running tests...")

for name in os.listdir(os.path.join(".", "tests")):
    path = os.path.join(".", "tests", name)
    if os.path.isdir(path):
        num_tests += 1
        run_test(path, name)

print("---- " + ANSI_BOLD + str(num_passed_tests) + "/" + str(num_tests) + " tests passed" + ANSI_END)