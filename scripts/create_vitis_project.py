#export VITIS_XSA=/home/user/1work/AudioNEXT/xilinx/vivado/AudioNEXT/AudioNEXT.xsa

import os
import sys
from pathlib import Path
import vitis
import shutil

script_dir = Path(__file__).resolve().parent

hw_dir = (script_dir / ".." / "xilinx" / "vivado").resolve()
sw_dir = (script_dir / ".." / "sw").resolve()
xsa_files = list(hw_dir.rglob("*.xsa"))
if len(xsa_files) == 0:
    print(f"ERROR: XSA not found!", file=sys.stderr)
    sys.exit(2)
elif len(xsa_files) > 1:
    print(f"ERROR: More than one XSA found: {xsa_files}", file=sys.stderr)
    sys.exit(2)

# ========== CUSTOM PARAMETER ==========
xsa_file = xsa_files[0]
proj_name = xsa_file.stem
workspace = sw_dir
platform_name = f"{proj_name}_platform"
domain_name = f"{proj_name}_domain"
domain_cpu = "microblaze_0"
domain_os = "standalone"
app_name = f"audio_rms"
# ======== CUSTOM PARAMETER END ========

workspace.mkdir(parents=True, exist_ok=True)

# Moving src data to temp folder for app creation
temp_dir = workspace / "temp"
app_src_dir = workspace / app_name / "src"
temp_dir.mkdir(parents=True, exist_ok=True)

if app_src_dir.exists():
    for file in app_src_dir.glob("*.c"):
        if file.name != "platform.c":
            shutil.copy2(file, temp_dir / file.name)
    for file in app_src_dir.glob("*.h"):
        if file.name != "platform.h":
            shutil.copy2(file, temp_dir / file.name)
    for file in app_src_dir.glob("*.ld"):
        shutil.copy2(file, temp_dir / file.name)

app_dir = workspace / app_name
if app_dir.exists():
    shutil.rmtree(app_dir)
plat_dir = workspace / platform_name
if plat_dir.exists():
    shutil.rmtree(plat_dir)

print(f"Using XSA: {xsa_file}")
print(f"Workspace: {workspace}")
print(f"Platform: {platform_name}")
print(f"Application: {app_name}")

client = None
try:
    # =============== Vitis Client ===============
    client = vitis.create_client()
    client.set_workspace(str(workspace))

    # ================= Platform =================
    print(f"Creating platform: {platform_name}")
    platform = client.create_platform_component(
        name=platform_name,
        hw=str(xsa_file),
        os = domain_os,
        cpu = domain_cpu
    )
    print("Building platform...")
    platform.build()
    print("Platform build completed.")

    # ================ Application ===============
    print(f"Creating app: {app_name}")
    platform_xpfm = workspace / platform_name / "export" / platform_name / f"{platform_name}.xpfm"
    if not platform_xpfm.is_file():
        raise FileNotFoundError(f"Platform export not found: {platform_xpfm}")
    application = client.create_app_component(
        name=app_name,
        platform = str(platform_xpfm),
        domain = f"{domain_os}_{domain_cpu}",
        template = "hello_world"
    )

    # Restoring source files to the app
    helloworld_file = app_src_dir / "helloworld.c"
    if helloworld_file.exists():
        helloworld_file.unlink() # Deletes the file
    if temp_dir.exists():
        for file in temp_dir.glob("*.c"):
            shutil.copy2(file, app_src_dir / file.name)
        for file in temp_dir.glob("*.h"):
            shutil.copy2(file, app_src_dir / file.name)
        for file in temp_dir.glob("*.ld"):
            shutil.copy2(file, app_src_dir / file.name) # Overwrites created linker script without warning

    if temp_dir.exists():
        shutil.rmtree(temp_dir)
    
    print("Building application...")
    application.build()
    print("Application build completed.")

except Exception as exc:
    print(f"ERROR: Vitis platform creation failed: {exc}", file=sys.stderr)
    raise
finally:
    vitis.dispose()
