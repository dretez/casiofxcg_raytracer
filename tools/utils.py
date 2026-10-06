import os
from math import copysign
from pathlib import Path
from typing import TypedDict

import fxconv

fxerr = fxconv.FxconvError
PROJECT_ROOT = Path(os.environ["PROJECT_ROOT"])


def asset_path(relative_path: str) -> Path:
    return PROJECT_ROOT / relative_path


class ElfTarget(TypedDict):
    toolchain: str
    arch: str
    section: str
    assembly: str
    outputtarget: str


def u64(x: int, check: bool = False) -> bytes:
    if check and not (0 <= x < 2**64):
        raise fxerr(f"integer {x} out of range for u64")
    return bytes(
        [
            (x >> 56) & 255,
            (x >> 48) & 255,
            (x >> 40) & 255,
            (x >> 32) & 255,
            (x >> 24) & 255,
            (x >> 16) & 255,
            (x >> 8) & 255,
            x & 255,
        ]
    )


def i64(x: int, check: bool = False) -> bytes:
    if check and not (-(2**63) <= x < 2**63):
        raise fxerr(f"integer {x} out of range for i64")
    return bytes(
        [
            (x >> 56) & 255,
            (x >> 48) & 255,
            (x >> 40) & 255,
            (x >> 32) & 255,
            (x >> 24) & 255,
            (x >> 16) & 255,
            (x >> 8) & 255,
            x & 255,
        ]
    )


def float_to_geoval(v: float) -> int:
    v = int(v * (1 << 32) + copysign(0.5, v))
    v = min(v, 0x7FFF_FFFF_FFFF_FFFF)
    v = max(v, -0x8000_0000_0000_0000)
    return v


def geo_to_bytes(x: int, check: bool = False) -> bytes:
    return i64(x, check)


def float_to_colorval(v: float) -> int:
    v = int(v * ((1 << 16) - 1) + 0.5)
    v = min(v, (1 << 16) - 1)
    v = max(v, 0)
    return v


def color_to_bytes(x: int, check: bool = False) -> bytes:
    return fxconv.u16(x, check)
