cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#! ! ! ! ! ! !
#set this to true to ship the game!
#basically this will change the RESOURCES_PATH to be the local path
#! ! ! ! ! ! !
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)
#delete the out folder after changing if visual studio doesn recognize the change!

# FIX: Only set this once with generator expressions
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(IMGUI_INCLUDE_DIR "./thirdparty/imgui")
set(IMGUI_BACKENDS_INCLUDE_DIR "./thirdparty/imgui/backends")
set(SDL3_INCLUDE_DIR "./thirdparty/SDL/include")
set(GLAD_INCLUDE_DIR "./thirdparty/glad/include")
set(GLM_INCLUDE_DIR "./thirdparty/glm/glm")
set(ASSIMP_INCLUDE_DIR "./thirdparty/assimp/include/assimp/")

if(MSVC)
	add_compile_options(/MP)
endif()

#may give problems on linux
if(PRODUCTION_BUILD)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
else()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

if(MSVC) 
add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
endif()

project(tangoEngine)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
 
if(BUILD_SHARED_LIBS)
    message(STATUS "${PROJECT_NAME}: Build engine shared library")
    set(BUILD_LIB_FLAG "SHARED")
else()
    message(STATUS "${PROJECT_NAME}: Build engine static library")
    set(BUILD_LIB_FLAG "STATIC")
endif()

add_subdirectory(thirdparty/glad)				
add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/glm EXCLUDE_FROM_ALL)
add_subdirectory(thirdparty/assimp)

# MY_SOURCES is defined to be a list of all the source files for my game 
# DON'T ADD THE SOURCES BY HAND, they are already added with this macro
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_impl_opengl3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_impl_SDL3.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_tables.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_widgets.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_demo.cpp")
file(GLOB_RECURSE IMGUI_HEADER_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_impl_opengl3.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_impl_SDL3.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imconfig.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imgui_internal.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imstb_rectpack.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imstb_textedit.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/imgui/imstb_truetype.h")
file(GLOB_RECURSE STB_IMAGE_SOURCES CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/include/stb_image.cpp")

add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 23)

target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 

if(PRODUCTION_BUILD)
	# setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 

	# remove the option to debug asserts.
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=0) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=1) 

endif()

target_sources("${CMAKE_PROJECT_NAME}" PRIVATE ${MY_SOURCES} ${STB_IMAGE_SOURCES})

if(MSVC) # If using the VS compiler...
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()

#force remove unicode
if (WIN32)
	target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /UUNICODE /U_UNICODE)
endif()

if(WIN32)
    find_package(Freetype QUIET)
    if(NOT Freetype_FOUND)
        message(STATUS "FreeType not found, building from source...")
        include(FetchContent)
        FetchContent_Declare(
            freetype
            URL https://download.savannah.gnu.org/releases/freetype/freetype-2.13.2.tar.xz
        )

        set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)
        set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
        set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
        set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
        
        FetchContent_MakeAvailable(freetype)

        if(MSVC AND TARGET freetype)
            target_compile_definitions(freetype PRIVATE 
                _CRT_SECURE_NO_WARNINGS
                _CRT_NONSTDC_NO_WARNINGS
            )
        endif()
    endif()
    
    # Create consistent target name
    if(TARGET freetype AND NOT TARGET Freetype::Freetype)
        add_library(Freetype::Freetype ALIAS freetype)
    endif()
else()
    # On Linux/macOS, use find_package normally
    find_package(Freetype REQUIRED)
endif()

message(STATUS "${PROJECT_NAME}: Creating imgui lib")
add_library(imgui ${BUILD_LIB_FLAG} ${IMGUI_SOURCE_FILES} ${IMGUI_HEADER_FILES})

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(imgui PUBLIC ${IMGUI_INCLUDE_DIR})
target_include_directories(imgui PUBLIC ${IMGUI_BACKENDS_INCLUDE_DIR})
target_include_directories(imgui PUBLIC ${SDL3_INCLUDE_DIR})
target_include_directories(imgui PUBLIC ${GLAD_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${IMGUI_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${IMGUI_BACKENDS_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${GLAD_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${GLM_INCLUDE_DIR})
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC ${ASSIMP_INCLUDE_DIR})

if(TARGET Freetype::Freetype)
    target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC 
        ${FREETYPE_INCLUDE_DIRS}
        ${freetype_SOURCE_DIR}/include
        ${freetype_BINARY_DIR}/include
    )
endif()

if(TARGET SDL3::SDL3main)
    target_link_libraries("${CMAKE_PROJECT_NAME}" PUBLIC SDL3::SDL3main)
endif()

if(TARGET Freetype::Freetype)
    target_link_libraries("${CMAKE_PROJECT_NAME}" PUBLIC 
        SDL3::SDL3 glad imgui glm::glm assimp
        Freetype::Freetype
    )
else()
    target_link_libraries("${CMAKE_PROJECT_NAME}" PUBLIC 
        SDL3::SDL3 glad imgui glm::glm assimp
    )
    message(WARNING "FreeType not available, text rendering will not work")
endif()

set_target_properties(glad		  PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(glm		  PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(imgui		  PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(SDL_uclibc  PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(SDL3-static PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(assimp	  PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(uninstall	  PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(unit	      PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(zlibstatic  PROPERTIES FOLDER "CMakePredefinedTargets")
set_target_properties(freetype    PROPERTIES FOLDER "CMakePredefinedTargets")
