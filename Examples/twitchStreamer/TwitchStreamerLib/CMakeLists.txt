project(TwitchStreamerLib)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")
include_directories(SYSTEM "Include")

add_definitions(-DTWITCH_STREAMING_LIBRARY_DLL)

find_package(Threads REQUIRED)

set(TwitchStreamerLib_HEADERS
	Include/Framework.h
	Include/Logger.h
	Include/ObsWrapper.h
	Include/SourceEnumerator.h
	Include/TwitchStreamingLib.h)

set(TwitchStreamerLib_Contexts_HEADERS
	Include/Contexts/ObsAudio.h
	Include/Contexts/ObsObjectContext.h
	Include/Contexts/ObsDisplay.h
	Include/Contexts/ObsFilter.h
	Include/Contexts/ObsObjectDescriptors.h
	Include/Contexts/ObsSource.h)

set(TwitchStreamerLib_SOURCES
	Source/Logger.cpp
	Source/ObsWrapper.cpp
	Source/SourceEnumerator.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

source_group("Source Files" FILES ${TwitchStreamerLib_SOURCES})
source_group("Header Files/Contexts" FILES ${TwitchStreamerLib_Contexts_HEADERS})
source_group("Header Files" FILES ${TwitchStreamerLib_HEADERS})


if(MSVC)
	set(obs_PLATFORM_LIBRARIES
		${obs_PLATFORM_LIBRARIES}
		w32-pthreads)
endif()

add_library(TwitchStreamerLib
	${TwitchStreamerLib_SOURCES}
	${TwitchStreamerLib_Contexts_HEADERS}
	${TwitchStreamerLib_HEADERS})

target_link_libraries(TwitchStreamerLib
	Threads::Threads
	libobs)

set_target_properties(TwitchStreamerLib
	PROPERTIES
		FOLDER "tests and examples"
		OUTPUT_NAME TwitchStreamerLib
		PREFIX "")

install_obs_core(TwitchStreamerLib)
