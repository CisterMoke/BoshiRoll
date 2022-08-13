add_library(algebra OBJECT 
    ${CMAKE_CURRENT_SOURCE_DIR}/Mat22.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Vec2.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Quadrant.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Vec2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Mat22.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Quadrant.cpp
    )

add_library(utils OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.h
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
    )


add_library(sprites OBJECT 
    ${CMAKE_CURRENT_SOURCE_DIR}/AnimSprite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FontSprite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseSprite.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Sprites.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AnimSprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FontSprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseSprite.cpp
    )
    

add_library(colliders OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Colliders.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Colliders.cpp
    )

add_library(entities OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Entity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Player.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Tongue.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Entity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Player.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Tongue.cpp
    )

add_library(level OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/Anchor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Level.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Anchor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Level.cpp
    )

add_library(rendering OBJECT
    
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderCommands.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderCommands.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp)


set(Headers
    ${CMAKE_CURRENT_SOURCE_DIR}/debug.h
    ${CMAKE_CURRENT_SOURCE_DIR}/globals.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Game.h
    )

set(Sources
    ${CMAKE_CURRENT_SOURCE_DIR}/debug.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${Headers} ${Sources})