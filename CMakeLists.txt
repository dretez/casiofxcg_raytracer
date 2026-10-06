# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(Raytracer)

include(GenerateG3A)
include(Fxconv)
find_package(Gint 2.9 REQUIRED)

set(SOURCES
  src/Renderer.c
  src/main.c

  src/Scene/Camera.c

  src/Scene/Light/PointLight.c
  src/Scene/Light/DiskAreaLight.c
  src/Scene/Light/VolumeLight.c

  src/Scene/Objects/Sphere.c
  src/Scene/Objects/Triangle.c

  src/Engine/Tracing.c
  src/Engine/Background.c
  src/Engine/Intersection.c
  src/Engine/Lighting.c
  src/Engine/Shading.c
)
# Shared assets, fx-9860G-only assets and fx-CG-50-only assets
set(ASSETS
  assets/materials/blue.json
  assets/materials/floor.json
  assets/materials/glass.json
  assets/materials/green.json
  assets/materials/matte.json
  assets/materials/mirror.json
  assets/materials/plastic.json
  assets/materials/polished.json
  assets/materials/red.json
  assets/materials/white.json

  assets/scenes/cornell_box.json
  assets/scenes/demo.json
)
set(ASSETS_cg
)
set(CONVERTERS
  tools/converters.py
)

set(CMAKE_FXCONV_COMPILE_OBJECT
"env PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR} PROJECT_ROOT=${CMAKE_CURRENT_SOURCE_DIR} \
<CMAKE_FXCONV_COMPILER> --toolchain=sh-elf --cg <SOURCE> -o <OBJECT>"
)

fxconv_declare_assets(${ASSETS} ${ASSETS_cg} WITH_METADATA)
fxconv_declare_converters(${CONVERTERS})

add_executable(raytracer ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(raytracer PRIVATE
  -Wall
  -Wextra
  -Os
  -g
  -mtas
  -flto
  # -fverbose-asm
  # -save-temps=obj
)
target_include_directories(raytracer PRIVATE header)
target_link_libraries(raytracer
  Gint::Gint
  m
)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET raytracer OUTPUT "Raytracer.g3a"
    NAME "Raytracer" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
