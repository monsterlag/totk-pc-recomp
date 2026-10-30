"""Build and run the game-free ultrawide checks; CC can select an llvm-mingw compiler."""
import os
from pathlib import Path
import shutil
import subprocess

ROOT = Path(__file__).resolve().parents[1]
out = ROOT / 'build' / 'tests'
out.mkdir(parents=True, exist_ok=True)
cc = os.environ.get('CC') or shutil.which('clang') or r'C:\msys64\ucrt64\bin\clang.exe'
exe = out / ('aspect_check.exe' if os.name == 'nt' else 'aspect_check')
subprocess.run([cc, '-O2', '-Wall', '-Wextra', '-I', str(ROOT / 'runtime'),
                str(ROOT / 'tests' / 'aspect_check.c'), str(ROOT / 'runtime' / 'aspect.c'),
                '-o', str(exe)], check=True)
subprocess.run([str(exe)], check=True)
