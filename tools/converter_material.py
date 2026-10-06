import json
from typing import cast

import fxconv

from tools.parsers.color import parse_color
from tools.utils import ElfTarget, float_to_colorval, float_to_geoval, geo_to_bytes


def convert_material(
    input: str, output: str, params: dict[str, str], target: ElfTarget
):
    with open(input, "r") as f:
        material_json = cast(object, json.load(f))

    assert isinstance(material_json, dict)

    assert "color" in material_json
    assert "ambient" in material_json
    assert "diffuse" in material_json
    assert "specular" in material_json
    assert "reflectivity" in material_json
    assert "transparency" in material_json
    assert "ior" in material_json
    assert "shininess" in material_json

    assert isinstance(material_json["color"], dict)
    assert isinstance(material_json["ambient"], float)
    assert isinstance(material_json["diffuse"], float)
    assert isinstance(material_json["specular"], float)
    assert isinstance(material_json["reflectivity"], float)
    assert isinstance(material_json["transparency"], float)
    assert isinstance(material_json["ior"], float)
    assert isinstance(material_json["shininess"], int)

    ambient = float_to_colorval(material_json["ambient"])
    diffuse = float_to_colorval(material_json["diffuse"])
    specular = float_to_colorval(material_json["specular"])
    reflectivity = float_to_colorval(material_json["reflectivity"])
    transparency = float_to_colorval(material_json["transparency"])
    ior = float_to_geoval(material_json["ior"])
    invior = float_to_geoval(1.0 / material_json["ior"])
    shininess = material_json["shininess"]

    flag_local = (ambient | diffuse | specular) != 0
    flag_reflection = reflectivity != 0
    flag_refraction = transparency != 0
    flags = flag_local + (flag_reflection << 1) + (flag_refraction << 2)

    local_weight = max(float_to_colorval(1.0) - reflectivity - transparency, 0)

    o = fxconv.ObjectData()

    o += geo_to_bytes(ior, check=True)
    o += geo_to_bytes(invior, check=True)

    o += parse_color(cast(object, material_json["color"]))

    o += fxconv.u16(local_weight, check=True)

    o += fxconv.u16(ambient, check=True)
    o += fxconv.u16(diffuse, check=True)
    o += fxconv.u16(specular, check=True)
    o += fxconv.u16(reflectivity, check=True)
    o += fxconv.u16(transparency, check=True)
    o += fxconv.u16(shininess, check=True)

    o += fxconv.u8(flags, check=True)

    fxconv.elf(o, output, "_" + str(params["name"]), **target)
