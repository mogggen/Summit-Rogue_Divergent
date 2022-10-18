#--------------------------------------------------------------------------
# physics project
#--------------------------------------------------------------------------

PROJECT(meshRenderer)
FILE(GLOB physics_headers code/*.h)
FILE(GLOB physics_sources code/*.cc)

SET(piles_Physics ${physics_headers} ${physics_sources})
SOURCE_GROUP("physics" FILES ${piles_Physics})

ADD_EXECUTABLE(physics ${piles_Physics})
TARGET_LINK_LIBRARIES(physics core render)
ADD_DEPENDENCIES(physics core render)

SET_PROPERTY(TARGET physics PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")