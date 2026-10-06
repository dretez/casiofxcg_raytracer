from typing import override

import fxconv

from tools.utils import float_to_geoval, geo_to_bytes
from tools.vec3 import Vec3


class Sphere:
    def __init__(self, center: Vec3, radius: float):
        self.center: Vec3 = center
        self.radius: float = radius

    @override
    def __format__(self, fmt: str) -> str:
        return f"{{center: {self.center}, radius: {self.radius}}}"

    def to_ObjectData(self) -> fxconv.ObjectData:
        o = fxconv.ObjectData()
        o += self.center.to_ObjectData()
        o += geo_to_bytes(float_to_geoval(self.radius), check=True)
        o += geo_to_bytes(float_to_geoval(self.radius**2), check=True)
        o += geo_to_bytes(float_to_geoval(1.0 / self.radius), check=True)
        return o
