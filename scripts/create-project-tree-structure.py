import os

# Note: Directories in "./bin" are created by CMake,
#       and directories in "./build" are created by Python build scripts.

os.makedirs(os.path.join(".", "bin"), exist_ok = True)
os.makedirs(os.path.join(".", "build"), exist_ok = True)
os.makedirs(os.path.join(".", "docs"), exist_ok = True)
os.makedirs(os.path.join(".", "scripts"), exist_ok = True)
os.makedirs(os.path.join(".", "src"), exist_ok = True)
os.makedirs(os.path.join(".", "third-party", "include"), exist_ok = True)
os.makedirs(os.path.join(".", "third-party", "lib"), exist_ok = True)
