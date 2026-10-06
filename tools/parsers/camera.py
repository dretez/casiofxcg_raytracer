import fxconv

from tools.parsers.position import parse_vec3
from tools.utils import float_to_geoval, geo_to_bytes
from tools.vec3 import Vec3


def parse_camera(cam: dict[str, object]) -> fxconv.ObjectData:
    assert isinstance(cam, dict)

    assert "position" in cam
    assert "target" in cam
    assert "world_up" in cam
    assert "fov" in cam
    assert "width" in cam
    assert "height" in cam

    assert isinstance(cam["fov"], float)
    assert isinstance(cam["width"], int) and not isinstance(cam["width"], bool)
    assert isinstance(cam["height"], int) and not isinstance(cam["height"], bool)

    fov = cam["fov"]
    w = cam["width"]
    h = cam["height"]

    aspect = w / h
    scale_y = fov / 2
    scale_x = aspect * scale_y
    px_scale_y = -2 * scale_y / h
    px_scale_x = 2 * scale_x / w

    offset_y = scale_y - scale_y / h
    offset_x = scale_x / w - scale_x

    screen_y = fxconv.ObjectData()
    for i in range(h):
        v = i * px_scale_y + offset_y
        screen_y += geo_to_bytes(float_to_geoval(v))
    screen_x = fxconv.ObjectData()
    for i in range(w):
        v = i * px_scale_x + offset_x
        screen_x += geo_to_bytes(float_to_geoval(v))

    position = parse_vec3(cam["position"])
    target = parse_vec3(cam["target"])
    up = parse_vec3(cam["world_up"]).normalize()

    forward = (target - position).normalize()
    if abs(forward.dot(up)) > 0.999:
        up = Vec3(1.0, 0.0, 0.0)

        if abs(forward.dot(up)) > 0.999:
            up = Vec3(0.0, 1.0, 0.0)
    right = (forward * up).normalize()
    up = right * forward

    o = fxconv.ObjectData()

    o += position.to_ObjectData()
    o += forward.to_ObjectData()
    o += up.to_ObjectData()
    o += right.to_ObjectData()

    o += fxconv.ref(screen_x)
    o += fxconv.ref(screen_y)

    return o
