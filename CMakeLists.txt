cmake_minimum_required(VERSION 2.8.4)
project(S52)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(PACKAGE_NAME libs52)

set(SOURCE_FILES
    lib/freetype-gl/texture-atlas.c
    lib/freetype-gl/texture-atlas.h
    lib/freetype-gl/texture-font.c
    lib/freetype-gl/texture-font.h
    lib/freetype-gl/vec234.h
    lib/freetype-gl/vector.c
    lib/freetype-gl/vector.h
    lib/libtess/dict-list.h
    lib/libtess/dict.c
    lib/libtess/dict.h
    lib/libtess/geom.c
    lib/libtess/geom.h
    lib/libtess/gluos.h
    lib/libtess/memalloc.h
    lib/libtess/mesh.c
    lib/libtess/mesh.h
    lib/libtess/normal.c
    lib/libtess/normal.h
    lib/libtess/priorityq-heap.c
    lib/libtess/priorityq-heap.h
    lib/libtess/priorityq-sort.h
    lib/libtess/priorityq.c
    lib/libtess/priorityq.h
    lib/libtess/render.c
    lib/libtess/render.h
    lib/libtess/sweep.c
    lib/libtess/sweep.h
    lib/libtess/tess.c
    lib/libtess/tess.h
    lib/libtess/tesselator.h
    lib/libtess/tessmono.c
    lib/libtess/tessmono.h
    lib/parson/parson.c
    lib/parson/parson.h
    lib/parson/tests.c
    _GL1.i
    _GL2.i
    _GLU.i
    _S52.i
    gvS57layer.c
    gvS57layer.h
    S52.c
    S52.h
    S52CS.c
    S52CS.h
    S52GL.c
    S52GL.h
    S52MP.c
    S52MP.h
    S52PL.c
    S52PL.h
    S52utils.c
    S52utils.h
    S57data.c
    S57data.h
    S57gv.c
    S57gv.h
    S57ogr.c
    S57ogr.h)

ADD_DEFINITIONS("`pkg-config --cflags glib-2.0` `pkg-config --cflags egl` `pkg-config --libs glesv2`")

add_executable(S52 ${SOURCE_FILES})