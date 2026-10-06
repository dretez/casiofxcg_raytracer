from collections.abc import Callable

from tools.converter_material import convert_material
from tools.converter_scene import convert_scene
from tools.utils import ElfTarget

converters: dict[str, Callable[[str, str, dict[str, str], ElfTarget], None]] = {
    "material": convert_material,
    "scene": convert_scene,
}


def convert(input: str, output: str, params: dict[str, str], target: ElfTarget):
    if params["custom-type"] not in converters:
        return 1
    converters[params["custom-type"]](input, output, params, target)
    return 0
