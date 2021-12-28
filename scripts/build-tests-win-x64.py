import os
import subprocess

def build_test(test_path):
    cmake_working_directory = os.path.join(test_path, "build", "win-x64", "release")
    cmake_config_command = (
        "cmake ../../.. " +
        "-G\"Visual Studio 16\" " +
        "-DCMAKE_GENERATOR_PLATFORM=x64 " +
        "-DCMAKE_BUILD_TYPE=Release"
    )
    cmake_build_command = "cmake --build . --config Release"

    os.makedirs(cmake_working_directory, exist_ok = True)
    subprocess.run(cmake_config_command, shell = True, cwd = cmake_working_directory)
    subprocess.run(cmake_build_command, shell = True, cwd = cmake_working_directory)

subprocess.run("python " + os.path.join(".", "scripts", "create-project-tree-structure.py"), shell = True)

for name in os.listdir(os.path.join(".", "tests")):
    path = os.path.join(".", "tests", name)
    if os.path.isdir(path):
        build_test(path)