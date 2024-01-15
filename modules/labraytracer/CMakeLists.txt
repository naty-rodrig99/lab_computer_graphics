#--------------------------------------------------------------------
# Inviwo LabRaytracer Module
ivw_module(LabRaytracer)

#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/boundingbox.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bvhindexedtrianglemesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/bvtree.h
    ${CMAKE_CURRENT_SOURCE_DIR}/constantmaterial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/cooktorrancematerial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/indexedtrianglemesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/light.h
    ${CMAKE_CURRENT_SOURCE_DIR}/material.h
    ${CMAKE_CURRENT_SOURCE_DIR}/performancetimer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/phongmaterial.h
    ${CMAKE_CURRENT_SOURCE_DIR}/plane.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ray.h
    ${CMAKE_CURRENT_SOURCE_DIR}/rayintersection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/raytracer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/renderable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/scene.h
    ${CMAKE_CURRENT_SOURCE_DIR}/sphere.h
    ${CMAKE_CURRENT_SOURCE_DIR}/triangle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/util.h
    ${CMAKE_CURRENT_SOURCE_DIR}/csgspheregroup.h
)
#~ ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/boundingbox.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bvhindexedtrianglemesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bvtree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/constantmaterial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cooktorrancematerial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/light.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/material.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/performancetimer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/phongmaterial.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/plane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ray.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rayintersection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raytracer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/raytracer_scenes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/triangle.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/util.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/csgspheregroup.cpp
)
ivw_group("Sources" ${SOURCE_FILES} ${HEADER_FILES})


#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/glsl/labraytracerprocessor.frag
)
ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Add Unittests
set(TEST_FILES
	#${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/labraytracer-test.cpp
)
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)
