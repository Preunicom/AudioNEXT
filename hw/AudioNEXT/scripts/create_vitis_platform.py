#export VITIS_XSA=/home/user/1work/AudioNEXT/xilinx/vivado/AudioNEXT/AudioNEXT.xsa

import os
import sys
from pathlib import Path
import vitis

script_dir = Path(__file__).resolve().parent
proj_name = script_dir.parent.name
repo_root = script_dir.parents[3]

xsa_env = os.environ.get("VITIS_XSA") or os.environ.get("XSA_PATH")
xsa = Path(xsa_env).expanduser().resolve() if xsa_env else (
    repo_root / "xilinx" / "vivado" / proj_name / f"{proj_name}.xsa"
)
ws = repo_root / proj_name / "sw"

if not xsa.is_file():
    print(f"ERROR: XSA not found: {xsa}", file=sys.stderr)
    print("Hint: set VITIS_XSA or XSA_PATH to the absolute .xsa path.", file=sys.stderr)
    sys.exit(2)

ws.mkdir(parents=True, exist_ok=True)
print(f"Using XSA: {xsa}")
print(f"Workspace: {ws}")

client = None
try:
    client = vitis.create_client()
    client.set_workspace(str(ws))
    platform_name = f"{proj_name}_platform"
    app_name = os.environ.get("VITIS_APP_NAME", "audio_rms")
    domain_name = os.environ.get("VITIS_DOMAIN", "standalone_microblaze_0")
    template_name = os.environ.get("VITIS_TEMPLATE", "hello_world")

    print(f"Creating platform: {platform_name}")
    base_kwargs = {
        "name": platform_name,
        "os": "standalone",
        "cpu": "microblaze_0",
    }
    hw_arg_candidates = [
        ("hw_design", str(xsa)),
        ("hw", str(xsa)),
        ("hw_file", str(xsa)),
        ("hw_spec", str(xsa)),
    ]

    plat = None
    last_exc = None
    for key, value in hw_arg_candidates:
        try:
            plat = client.create_platform_component(**{**base_kwargs, key: value})
            print(f"Using hardware arg: {key}")
            break
        except TypeError as exc:
            last_exc = exc
            if "unexpected keyword argument" not in str(exc):
                raise
    if plat is None and last_exc is not None:
        raise last_exc

    print("Building platform...")
    plat.build()
    print("Platform build completed.")

    xpfm = ws / platform_name / "export" / platform_name / f"{platform_name}.xpfm"
    if not xpfm.is_file():
        raise FileNotFoundError(f"Platform export not found: {xpfm}")

    print(f"Creating app: {app_name}")
    try:
        client.create_app_component(
            name=app_name,
            platform=str(xpfm),
            domain=domain_name,
            template=template_name,
        )
    except Exception as exc:
        if "already exists" not in str(exc).lower():
            raise
        print(f"App already exists: {app_name}")
except Exception as exc:
    print(f"ERROR: Vitis platform creation failed: {exc}", file=sys.stderr)
    raise
finally:
    vitis.dispose()
