from typing import override

import fxconv

from tools.utils import float_to_geoval, geo_to_bytes
from tools.vec3 import Vec3


class Triangle:
    def __init__(self, a: Vec3, b: Vec3, c: Vec3):
        self.a: Vec3 = a
        self.b: Vec3 = b
        self.c: Vec3 = c

        e0 = b - a
        e1 = c - b
        e2 = a - c

        e0N = e0.normalize()
        e1N = (c - a).normalize()
        normal = (e0N * e1N).normalize()

        d = normal.dot(a)

        self.eN0: Vec3 = normal * e0
        self.eN1: Vec3 = normal * e1
        self.eN2: Vec3 = normal * e2
        self.eD0: float = self.eN0.dot(a)
        self.eD1: float = self.eN1.dot(b)
        self.eD2: float = self.eN2.dot(c)

        self.normal: Vec3 = normal
        self.d: float = d

    @override
    def __format__(self, fmt: str) -> str:
        return f"{{a: {self.a}, b: {self.b}, c: {self.c}}}"

    def to_ObjectData(self) -> fxconv.ObjectData:
        o = fxconv.ObjectData()
        o += self.normal.to_ObjectData()
        o += geo_to_bytes(float_to_geoval(self.d), check=True)
        o += self.eN0.to_ObjectData()
        o += geo_to_bytes(float_to_geoval(self.eD0), check=True)
        o += self.eN1.to_ObjectData()
        o += geo_to_bytes(float_to_geoval(self.eD1), check=True)
        o += self.eN2.to_ObjectData()
        o += geo_to_bytes(float_to_geoval(self.eD2), check=True)
        return o
