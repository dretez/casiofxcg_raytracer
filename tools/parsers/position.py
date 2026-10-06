from typing import cast

import fxconv

from tools.vec3 import Vec3


def parse_position(pos: object) -> fxconv.ObjectData:
    vec = parse_vec3(pos)
    return vec.to_ObjectData()


def parse_vec3(pos: object) -> Vec3:
    assert isinstance(pos, dict)

    pos = cast(dict[str, int | float], pos)

    assert "x" in pos
    assert "y" in pos
    assert "z" in pos

    assert type(pos["x"]) in (int, float)
    assert type(pos["y"]) in (int, float)
    assert type(pos["z"]) in (int, float)

    return Vec3(float(pos["x"]), float(pos["y"]), float(pos["z"]))
