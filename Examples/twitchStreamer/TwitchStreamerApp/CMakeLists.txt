project(TwitchStreamerApp)

include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/libobs")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/Examples/twitchStreamer/TwitchStreamerLib/Include")

find_package(Threads REQUIRED)


set(TwitchStreamerApp_HEADERS)

set(TwitchStreamerApp_SOURCES
	winMain.cpp)

source_group("importers\\Source Files" FILES ${obs_importers_SOURCES})
source_group("importers\\Header Files" FILES ${obs_importers_HEADERS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})


if(WIN32)
	set(obs_PLATFORM_LIBRARIES
		crypt32
		blake2)
	if(CMAKE_SIZEOF_VOID_P EQUAL 4)	
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
	endif()
endif()

if(MSVC)
	set(obs_PLATFORM_LIBRARIES
		${obs_PLATFORM_LIBRARIES}
		w32-pthreads)
endif()

add_executable(TwitchStreamerApp WIN32
	${TwitchStreamerApp_HEADERS}
	${TwitchStreamerApp_SOURCES})

target_link_libraries(TwitchStreamerApp
	Threads::Threads
	libobs
	TwitchStreamerLib)

set_target_properties(TwitchStreamerApp PROPERTIES FOLDER "tests and examples")
define_graphic_modules(TwitchStreamerApp)

install_obs_core(TwitchStreamerApp)
#install_obs_data(TwitchStreamerApp data obs-studio)
#install_obs_data_file(TwitchStreamerApp ../../AUTHORS TwitchStreamerApp/authors)
