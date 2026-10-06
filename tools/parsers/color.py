import fxconv

from tools.utils import float_to_colorval
from tools.vec3 import Vec3


def parse_color(color: object) -> fxconv.ObjectData:
    assert isinstance(color, dict)

    assert "r" in color
    assert "g" in color
    assert "b" in color

    assert isinstance(color["r"], float)
    assert isinstance(color["g"], float)
    assert isinstance(color["b"], float)

    r = float_to_colorval(color["r"])
    g = float_to_colorval(color["g"])
    b = float_to_colorval(color["b"])

    o = fxconv.ObjectData()

    o += fxconv.u16(r, check=True)
    o += fxconv.u16(g, check=True)
    o += fxconv.u16(b, check=True)

    return o


def parse_color_as_vec3(color: object) -> Vec3:
    assert isinstance(color, dict)

    assert "r" in color
    assert "g" in color
    assert "b" in color

    assert isinstance(color["r"], float)
    assert isinstance(color["g"], float)
    assert isinstance(color["b"], float)

    return Vec3(color["r"], color["g"], color["b"])
