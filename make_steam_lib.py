import os
import subprocess
import re
import shutil

VS_PATH = "C:/Program Files (x86)/Microsoft Visual Studio/2019\Community/VC\Tools/MSVC/14.29.30133/bin/Hostx86/x86/"
DLL_PATH = os.path.abspath("./steamsdk/redistributable_bin") + "/"

dumpbin = VS_PATH + "dumpbin.exe"
lib		= VS_PATH + "lib.exe"

steam_api_dll = DLL_PATH + "steam_api.dll"

steam_lib_dll = DLL_PATH + "steam_lib.dll"
steam_lib_exp = DLL_PATH + "steam_lib.exp"
steam_lib_def = DLL_PATH + "steam_lib.def"

dumped_exports = subprocess.run([dumpbin, "/EXPORTS", steam_api_dll], capture_output=True).stdout.decode("utf-8")

pattern = re.compile(r"\s+?(\d+?)\s+?[\da-fA-F]+?\s+?[\da-fA-F]+?\s+?(\w+)")

exports = re.findall(pattern, dumped_exports)

with open(steam_lib_def, "w") as f:
	f.write('LIBRARY "steam_lib.dll"\n')
	f.write('EXPORTS\n')
	for export in exports:
		f.write(export[1] + " @" + export[0] + "\n")

subprocess.run([lib, "/MACHINE:X86", "/DEF:" + steam_lib_def], cwd=DLL_PATH, capture_output=True)

shutil.copyfile(steam_api_dll, steam_lib_dll)
os.remove(steam_lib_exp)
os.remove(steam_lib_def)