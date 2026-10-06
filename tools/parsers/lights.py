import random
from collections.abc import Callable
from typing import cast

import fxconv

from tools.parsers.color import parse_color_as_vec3
from tools.parsers.position import parse_position, parse_vec3
from tools.utils import color_to_bytes, float_to_colorval, float_to_geoval, geo_to_bytes
from tools.vec3 import Vec3


def parse_lights(lights: object) -> tuple[fxconv.ObjectData, int]:
    assert isinstance(lights, list)
    lights = cast(list[dict[str, object]], lights)

    parsed: dict[str, fxconv.ObjectData] = {}
    for light in lights:
        assert "type" in light
        assert isinstance(light["type"], str)
        assert light["type"] in __light_types__
        if light["type"] not in parsed:
            parsed[light["type"]] = fxconv.ObjectData()
        parsed[light["type"]] += fxconv.ref(__light_types__[light["type"]](light))

    o = fxconv.ObjectData()

    for lights_obj in parsed.values():
        o += lights_obj

    return o, len(lights)


def __parse_light__(
    light: dict[str, object], vtable_id: str, samples: int
) -> fxconv.ObjectData:
    assert "color" in light
    assert "position" in light

    color = parse_color_as_vec3(light["color"])
    if "intensity" in light:
        assert isinstance(light["intensity"], (int, float))
        color = color * light["intensity"]

    invsamples = float_to_colorval(1.0 / samples)

    o = fxconv.ObjectData()
    o += fxconv.ref(vtable_id)
    o += parse_position(cast(dict[str, int | float], light["position"]))
    o += color.to_color_ObjectData()

    o += color_to_bytes(invsamples, check=True)
    o += fxconv.u16(samples, check=True)
    return o


def __parse_point_light__(light: dict[str, object]) -> fxconv.ObjectData:
    o = fxconv.ObjectData()
    o += __parse_light__(light, "pointlight_vtable", 1)
    return o


def __parse_sphere_light__(light: dict[str, object]) -> fxconv.ObjectData:
    assert "position" in light
    pos = parse_vec3(light["position"])

    assert "radius" in light
    assert isinstance(light["radius"], float)

    radius = light["radius"]
    samples = fxconv.ObjectData()
    for _ in range(16):
        s = Vec3(random.random(), random.random(), random.random()) * radius + pos
        samples += s.to_ObjectData()

    o = fxconv.ObjectData()
    o += __parse_light__(light, "volumelight_vtable", 16)
    o += fxconv.u16(0)  # padding
    o += fxconv.ref(samples)
    o += geo_to_bytes(float_to_geoval(radius), check=True)
    return o


def __parse_disk_light__(light: dict[str, object]) -> fxconv.ObjectData:
    assert "radius" in light
    assert isinstance(light["radius"], float)

    uv = Vec3(0, 0, 0)

    o = fxconv.ObjectData()
    o += __parse_light__(light, "volumelight_vtable", 16)
    o += fxconv.u16(0)  # padding
    o += uv.to_ObjectData()
    o += uv.to_ObjectData()
    o += geo_to_bytes(float_to_geoval(light["radius"]), check=True)
    return o


__light_types__: dict[str, Callable[[dict[str, object]], fxconv.ObjectData]] = {
    "point": __parse_point_light__,
    "sphere": __parse_sphere_light__,
    "disk": __parse_disk_light__,
}
