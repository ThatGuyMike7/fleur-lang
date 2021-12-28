import os
import subprocess

subprocess.run("python " + os.path.join(".", "scripts", "create-project-tree-structure.py"), shell = True)

cmake_working_directory = os.path.join(".", "build", "win-x64", "debug")
cmake_config_command = (
    "cmake ../../.. " +
    "-G\"Visual Studio 16\" " +
    "-DCMAKE_GENERATOR_PLATFORM=x64 " +
    "-DCMAKE_BUILD_TYPE=Debug"
)
cmake_build_command = "cmake --build . --config Debug"

os.makedirs(cmake_working_directory, exist_ok = True)
subprocess.run(cmake_config_command, shell = True, cwd = cmake_working_directory)
subprocess.run(cmake_build_command, shell = True, cwd = cmake_working_directory)
