import os
from os import path, system, environ
from pathlib import Path
import subprocess
import shutil

CMAKE = "T:/Program Files/JetBrains/CLion 2021.3.4/bin/cmake/win/bin/cmake.exe"
NINJA = "T:/Program Files/JetBrains/CLion 2021.3.4/bin/ninja/win/ninja.exe"
VCVARS = "T:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/Build/vcvarsall.bat"
VC_PLATEFORM = "x86_x64"



SOURCE_DIR =  path.dirname(path.realpath(__file__))
BINARY_DIR = path.join(SOURCE_DIR, "automate-build-release")

BACKEND_BINARY_DIR = path.join(BINARY_DIR, "backend", "Shared")
BACKEND_REDIST_DIR = path.join(SOURCE_DIR, "qtds", "imports", "backend")
BACKEND_REDIST_FILES = ["BackendShared.dll", "BackendShared.lib", "BackendShared.qmltypes", "BackendSharedplugin.dll", "BackendSharedplugin.lib", "qmldir"]

def mandatory_exec(args):
    print(" ".join(args))
    result = subprocess.run(args)
    if result.returncode !=0 : exit(result.returncode)
    return result

def reset_cmake(hard_reset=True):
    if hard_reset:
        try: shutil.rmtree(BINARY_DIR)
        except:pass
    mandatory_exec([CMAKE, "-DCMAKE_BUILD_TYPE=Release", f"-DCMAKE_MAKE_PROGRAM={NINJA}" , "-G", "Ninja", "-S", SOURCE_DIR, "-B", BINARY_DIR])

def build_backend():
    mandatory_exec([CMAKE, "--build", BINARY_DIR, "--target", "BackendSharedplugin"])

def install_backend():
    for file in BACKEND_REDIST_FILES:
        shutil.copy2(path.join(BACKEND_BINARY_DIR, file), BACKEND_REDIST_DIR)
    shutil.copy2(path.join(SOURCE_DIR, "backend", "renderer.h"), BACKEND_REDIST_DIR)

    if Path(BACKEND_REDIST_DIR) in [Path(p) for p in environ["PATH"].split(";")]:
        print("Backend is ready-to-use in QT Design Studio!")
    else:
        print(f"add '${BACKEND_REDIST_DIR}' to the PATH : this will import the QT Design Studio DLL (qml).")

def init_msvc():
    cmd = f"\"{VCVARS}\" {VC_PLATEFORM} && set"
    print(cmd)
    with subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=None, shell=True) as process:
        output = process.communicate()[0].decode("utf-8")
        if process.returncode ==0:
            print("load MSVC env:")
            for line in output.split("\r\n"):
                env_var = line.split("=");
                if len(env_var) ==2:
                    print(f" * import: {env_var[0]}={env_var[1]}")
                    environ[env_var[0]]=env_var[1]
                else:
                    print(f" * ignore line: {line}")
        else:
            print("can't load MSVC env!")
            exit(output.returncode)

init_msvc()
system(f"cd '{SOURCE_DIR}'")
reset_cmake(hard_reset=False)
build_backend()
install_backend()



