#--------------------------------------------------------------------------
# meshRenderer project
#--------------------------------------------------------------------------

PROJECT(meshRenderer)
FILE(GLOB meshRenderer_headers code/*.h)
FILE(GLOB meshRenderer_sources code/*.cc)

SET(files_meshRenderer ${meshRenderer_headers} ${meshRenderer_sources})
SOURCE_GROUP("meshRenderer" FILES ${files_meshRenderer})

ADD_EXECUTABLE(meshRenderer ${files_meshRenderer})
TARGET_LINK_LIBRARIES(meshRenderer core render)
ADD_DEPENDENCIES(meshRenderer core render)

SET_PROPERTY(TARGET meshRenderer PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")