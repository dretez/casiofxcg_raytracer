from collections.abc import Callable

from tools.converter_material import convert_material
from tools.utils import ElfTarget

converters: dict[str, Callable[[str, str, dict[str, str], ElfTarget], None]] = {
    "material": convert_material,
}


def convert(input: str, output: str, params: dict[str, str], target: ElfTarget):
    if params["custom-type"] not in converters:
        return 1
    converters[params["custom-type"]](input, output, params, target)
    return 0
