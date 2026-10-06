import os
import shutil
import subprocess
import sys
from pathlib import Path
from typing import cast


def run_command(*args: str) -> str:
    try:
        result = subprocess.run(
            args,
            check=True,
            capture_output=True,
            text=True,
        )
    except FileNotFoundError:
        print(f"error: command not found: {args[0]}", file=sys.stderr)
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"error: command failed: {' '.join(args)}", file=sys.stderr)
        stderr = cast(str | None, e.stderr)
        if isinstance(stderr, str) and stderr:
            print(stderr.strip(), file=sys.stderr)
        sys.exit(1)

    return result.stdout.strip()


fxsdk = shutil.which("fxsdk")
gcc = shutil.which("sh-elf-gcc")
fxsdk_sysroot = Path(run_command("fxsdk", "path", "sysroot"))


def main() -> None:
    if fxsdk is None:
        print(
            "error: fxsdk was not found in PATH.\n"
            + "Please install/configure the FXSDK before running this script.",
            file=sys.stderr,
        )
        sys.exit(1)

    if gcc is None:
        print(
            """
            error: sh-elf-gcc was not found in PATH.\n
            Please make sure the FXSDK toolchain is available.
            """,
            file=sys.stderr,
        )
        sys.exit(1)

    generate_clangd(gcc)
    generate_pyrightconf()


def generate_clangd(gcc: str):

    fxsdk_include = Path(run_command("fxsdk", "path", "include"))

    project_root = get_project_root()
    project_headers = project_root / "header"

    gcc_version = run_command(gcc, "-dumpfullversion")
    if not gcc_version:
        print(
            "error: could not determine sh-elf-gcc version.",
            file=sys.stderr,
        )
        sys.exit(1)
    gcc_include = fxsdk_sysroot / "lib" / "gcc" / "sh3eb-elf" / gcc_version / "include"

    paths = {
        "project headers": project_headers,
        "FXSDK include": fxsdk_include,
        "GCC include": gcc_include,
    }
    for name, path in paths.items():
        if not path.is_dir():
            print(
                f"error: {name} directory does not exist:\n  {path}",
                file=sys.stderr,
            )
            sys.exit(1)

    clangd_contents = f"""\
CompileFlags:
    Add:
        - -ferror-limit=0
        - -Wall
        - -Wextra
        - -DFXCG50
        - -I{project_headers}
        - -I{fxsdk_include}
        - -I{gcc_include}
    CompilationDatabase: .
"""

    project_root = get_project_root()
    clangd_path = project_root / ".clangd"
    _ = clangd_path.write_text(clangd_contents)


def generate_pyrightconf():
    fxsdk_prefix = fxsdk_sysroot.parent.parent.parent
    fxconv_path = fxsdk_prefix / "bin"
    conf_contents = f"""\
{{
    "stubPath": "tools//stubs",
    "extraPaths": [
        ".",
        "{fxconv_path}"
    ]
}}
"""

    project_root = get_project_root()
    conf_path = project_root / "pyrightconfig.json"
    _ = conf_path.write_text(conf_contents)


def get_project_root():
    project_root_env = os.environ.get("PROJECT_ROOT")
    if not project_root_env:
        print(
            """
            error: PROJECT_ROOT environment variable is not set.\n"
            Run this script through the Makefile.
            """,
            file=sys.stderr,
        )
        sys.exit(1)
    project_root = Path(project_root_env).resolve()
    if not project_root.is_dir():
        print(
            f"error: PROJECT_ROOT does not point to a directory:\n  {project_root}",
            file=sys.stderr,
        )
        sys.exit(1)
    return project_root


if __name__ == "__main__":
    main()
