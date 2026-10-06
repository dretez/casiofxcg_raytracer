from collections.abc import Callable
from typing import cast

import fxconv

from tools.obj_primitives.sphere import Sphere
from tools.obj_primitives.triangle import Triangle
from tools.parsers.position import parse_vec3


def parse_objects(objects: object) -> tuple[fxconv.ObjectData, int]:
    assert isinstance(objects, list)
    objects = cast(list[dict[str, object]], objects)

    object_common = fxconv.ObjectData()
    object_data: dict[str, fxconv.ObjectData] = {}
    for obj in objects:
        assert "type" in obj
        assert isinstance(obj["type"], str)
        assert obj["type"] in __object_types__
        if obj["type"] not in object_data:
            object_data[obj["type"]] = fxconv.ObjectData()
        obj_struct, obj_data = __object_types__[obj["type"]](obj)
        object_common += obj_struct
        object_data[obj["type"]] += obj_data

    return object_common, len(objects)


def __parse_object__(
    obj: dict[str, object], vtable_id: str, type_data: fxconv.ObjectData
) -> fxconv.ObjectData:
    assert "material" in obj
    assert isinstance(obj["material"], str)

    o = fxconv.ObjectData()
    o += fxconv.ref(vtable_id)
    o += fxconv.ref(obj["material"])
    o += fxconv.ref(type_data)
    return o


def __parse_sphere__(
    obj: dict[str, object],
) -> tuple[fxconv.ObjectData, fxconv.ObjectData]:
    assert "center" in obj
    assert "radius" in obj
    assert isinstance(obj["radius"], (int, float))

    center = parse_vec3(obj["center"])
    radius = obj["radius"]

    sphere = Sphere(center, radius)

    sphere_o = sphere.to_ObjectData()

    o = fxconv.ObjectData()
    o += __parse_object__(obj, "sphere_vtable", sphere_o)
    return o, sphere_o


def __parse_triangle__(
    obj: dict[str, object],
) -> tuple[fxconv.ObjectData, fxconv.ObjectData]:
    assert "a" in obj
    assert "b" in obj
    assert "c" in obj

    a = parse_vec3(obj["a"])
    b = parse_vec3(obj["b"])
    c = parse_vec3(obj["c"])

    tri = Triangle(a, b, c)

    tri_o = tri.to_ObjectData()

    o = fxconv.ObjectData()
    o += __parse_object__(obj, "triangle_vtable", tri_o)
    return o, tri_o


__object_types__: dict[
    str, Callable[[dict[str, object]], tuple[fxconv.ObjectData, fxconv.ObjectData]]
] = {
    "sphere": __parse_sphere__,
    "triangle": __parse_triangle__,
}
