import json
from typing import cast

import fxconv

from tools.parsers.camera import parse_camera
from tools.parsers.lights import parse_lights
from tools.parsers.objects import parse_objects

from .utils import ElfTarget


def convert_scene(input: str, output: str, params: dict[str, str], target: ElfTarget):
    with open(input, "r") as f:
        scene_json = cast(object, json.load(f))

    assert isinstance(scene_json, dict)
    scene_json = cast(dict[str, object], scene_json)

    assert "camera" in scene_json

    o = fxconv.ObjectData()

    if "objects" in scene_json:
        obj, length = parse_objects(scene_json["objects"])
        o += fxconv.ref(obj)
        o += fxconv.i32(length)
    else:
        o += fxconv.u32(0)
        o += fxconv.i32(0)

    if "lights" in scene_json:
        obj, length = parse_lights(scene_json["lights"])
        o += fxconv.ref(obj)
        o += fxconv.i32(length)
    else:
        o += fxconv.u32(0)
        o += fxconv.i32(0)

    o += parse_camera(cast(dict[str, object], scene_json["camera"]))

    fxconv.elf(o, output, "_" + str(params["name"]), **target)
