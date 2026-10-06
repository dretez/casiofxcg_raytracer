from math import sqrt
from typing import override

import fxconv

from tools.utils import color_to_bytes, float_to_colorval, float_to_geoval, geo_to_bytes


class Vec3:
    def __init__(self, x: float, y: float, z: float):
        self.x: float = x
        self.y: float = y
        self.z: float = z

    def __add__(self, other: "Vec3") -> "Vec3":
        return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other: "Vec3") -> "Vec3":
        return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)

    def __mul__(self, other: "int | float | Vec3") -> "Vec3":
        if isinstance(other, (int, float)):
            return Vec3(self.x * other, self.y * other, self.z * other)

        assert isinstance(other, Vec3)

        def cross(a1: float, a2: float, b1: float, b2: float) -> float:
            return (a1 * b1) - (a2 * b2)

        x = cross(self.y, self.z, other.z, other.y)
        y = cross(self.z, self.x, other.x, other.z)
        z = cross(self.x, self.y, other.y, other.x)
        return Vec3(x, y, z)

    def __truediv__(self, scalar: float) -> "Vec3":
        return Vec3(self.x / scalar, self.y / scalar, self.z / scalar)

    def dot(self, other: "Vec3") -> float:
        return self.x * other.x + self.y * other.y + self.z * other.z

    def __neg__(self):
        return Vec3(-self.x, -self.y, -self.z)

    @override
    def __format__(self, fmt: str) -> str:
        return f"{{x: {self.x}, y: {self.y}, z: {self.z}}}"

    def len(self) -> float:
        return sqrt(self.dot(self))

    def normalize(self):
        assert self.len() != 0.0
        return self / self.len()

    def to_ObjectData(self) -> fxconv.ObjectData:
        o = fxconv.ObjectData()
        o += geo_to_bytes(float_to_geoval(self.x), check=True)
        o += geo_to_bytes(float_to_geoval(self.y), check=True)
        o += geo_to_bytes(float_to_geoval(self.z), check=True)
        return o

    def to_color_ObjectData(self) -> fxconv.ObjectData:
        o = fxconv.ObjectData()
        o += color_to_bytes(float_to_colorval(self.x), check=True)
        o += color_to_bytes(float_to_colorval(self.y), check=True)
        o += color_to_bytes(float_to_colorval(self.z), check=True)
        return o
