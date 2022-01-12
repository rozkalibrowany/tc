#ifndef A719A074_6EC7_4C03_A025_880CD7C92228
#define A719A074_6EC7_4C03_A025_880CD7C92228
cmake_minimum_required(VERSION 3.16)

function(tc_set_colors)
	set(FOPT ON)
	set(FARGS)
	set(FMARGS)
	cmake_parse_arguments(TcSetColors "${FOPT}" "${FARGS}" "${FMARGS}" ${ARGN})

	STRING(ASCII 27 ESC)
	SET(TcCra  "${ESC}[m" CACHE STRING "TcCra" FORCE)    # RESET

	# NORMAL
	SET(TcCb   "${ESC}[1m"  CACHE STRING "TcCb" FORCE)  # BOLD
	SET(TcCr   "${ESC}[31m" CACHE STRING "TcCr" FORCE)  # RED
	SET(TcCg   "${ESC}[32m" CACHE STRING "TcCg" FORCE)  # GREEN
	SET(TcCy   "${ESC}[33m" CACHE STRING "TcCy" FORCE)  # YELLOW
	SET(TcCb   "${ESC}[34m" CACHE STRING "TcCb" FORCE)  # BLACK
	SET(TcCm   "${ESC}[35m" CACHE STRING "TcCm" FORCE)  # MAGENTA
	SET(TcCc   "${ESC}[36m" CACHE STRING "TcCc" FORCE)  # CYAN
	SET(TcCw   "${ESC}[37m" CACHE STRING "TcCw" FORCE)  # WHITE

	# BOLD
	SET(TcCbr  "${ESC}[1;31m" CACHE STRING "TcCbm" FORCE) # BOLD RED
	SET(TcCbg  "${ESC}[1;32m" CACHE STRING "TcCbg" FORCE) # BOLD GREEN
	SET(TcCby  "${ESC}[1;33m" CACHE STRING "TcCby" FORCE) # BOLD YELLOW
	SET(TcCbb  "${ESC}[1;34m" CACHE STRING "TcCbb" FORCE) # BOLD BLUE
	SET(TcCbm  "${ESC}[1;35m" CACHE STRING "TcCbm" FORCE) # BOLD MAGENTA
	SET(TcCbc  "${ESC}[1;36m" CACHE STRING "TcCbc" FORCE) # BOLD CYAN
	SET(TcCbw  "${ESC}[1;37m" CACHE STRING "TcCbw" FORCE) # BOLD WHITE

	# SPECIAL
	SET(TcXlb  "${TcCra}${TcCbw}[${TcCra}" CACHE STRING "TcXlb" FORCE) # LEFT BRACKET
	SET(TcXrb  "${TcCra}${TcCbw}]${TcCra}" CACHE STRING "TcXrb" FORCE) # RIGHT BRACKET
	SET(TcXs   "${TcCra}${TcCbw}/${TcCra}" CACHE STRING "TcXs" FORCE) # FORWARD SLASH
	SET(TcXc   "${TcCra}${TcCbw}:${TcCra}" CACHE STRING "TcXc" FORCE) # COLON
	SET(TcXon  "${TcCbg}ON${TcCra}" CACHE STRING "TcXon" FORCE) # RIGHT BRACKET
	SET(TcXof  "${TcCby}OFF${TcCra}" CACHE STRING "TcXof" FORCE) # RIGHT BRACKET
	SET(TcXfound  "${TcCbg}FOUND${TcCra}" CACHE STRING "TcXfound" FORCE) # RIGHT BRACKET
	SET(TcXmissing  "${TcCby}MISSING${TcCra}" CACHE STRING "TcXmissing" FORCE) # RIGHT BRACKET

	SET(TcXc   "${TcCra}${TcCbw}:${TcCra}" CACHE STRING "TcXc" FORCE) # COLON
endfunction(tc_set_colors)

macro(TcTarget)
	set(opts OPTIONAL HEADER)
	set(args TARGET)
	set(argsn VERSION)
	cmake_parse_arguments(TcTarget "${opts}" "${args}" "${argsn}" ${ARGN} )

	set(target ${TcTarget_TARGET})
	list(GET TcTarget_VERSION 0 major)
	list(GET TcTarget_VERSION 1 minor)
	list(GET TcTarget_VERSION 2 patch)

	set_target_properties(${target} PROPERTIES VERSION ${major}.${minor}.${patch} SOVERSION ${major}.${minor}.${patch})
endmacro(TcTarget)

macro(TcAddLibrary)
	set(opts OPTIONAL HEADER STATIC)
	set(args TARGET SOURCE)
	set(argsn INCLUDE LIBRARIES VERSION META REQUIRE SOURCE_REMOVE SOURCES COMPILE_DEFINITIONS_PRIVATE COMPILE_OPTIONS_PRIVATE LINK_FLAGS)
	cmake_parse_arguments(TcAddLibrary "${opts}" "${args}" "${argsn}" ${ARGN} )

	set(target ${TcAddLibrary_TARGET})
	file(GLOB_RECURSE sources "${TcAddLibrary_SOURCE}")
	if (TcAddLibrary_SOURCE_REMOVE)
		foreach(SRC_REMOVE IN LISTS TcLib_SOURCE_REMOVE)
			file(GLOB_RECURSE source_remove "${SRC_REMOVE}")
			foreach(S IN LISTS source_remove)
				list(REMOVE_ITEM sources ${S})
			endforeach()
		endforeach()
	endif()

	set(TcTargetBuild TRUE)
	foreach(TC_REQUIRE IN LISTS TcAddLibrary_REQUIRE)
		if (${TC_REQUIRE})
			set(TcTargetBuild TRUE)
		else()
			set(TcTargetBuild FALSE)
			break()
		endif()
	endforeach()

	foreach(S IN LISTS TcAddLibrary_SOURCES)
		list(APPEND sources ${S})
	endforeach()
	if (TcTargetBuild)
		link_directories(/usr/local/lib)  # CMAKE 3.15 target_link_directories

		if (TcAddLibrary_STATIC)
			add_library(${target} STATIC ${sources})
		else()
			add_library(${target} SHARED ${sources})
		endif()

		TcTarget(TARGET ${target} VERSION ${TcAddLibrary_VERSION})
		set_target_properties(${target} PROPERTIES DEFINE_SYMBOL "") # remove -Dtarget_EXPORTS
		target_link_libraries(${target} ${TcAddLibrary_LIBRARIES})
		target_include_directories(${target}
			PUBLIC ${TcAddLibrary_INCLUDE}
		)
		if (TcAddLibrary_LINK_FLAGS)
			set_target_properties(${target} PROPERTIES LINK_FLAGS ${TcAddLibrary_LINK_FLAGS})
		endif()

		TcInstall(TARGET ${target} LIB)

		if (TcAddLibrary_COMPILE_DEFINITIONS)
			target_compile_definitions(${target} PRIVATE ${TcAddLibrary_COMPILE_DEFINITIONS})
		endif()

		if (TcAddLibrary_COMPILE_OPTIONS_PRIVATE)
			target_compile_options(${target} PRIVATE ${TcAddLibrary_COMPILE_OPTIONS_PRIVATE})
		endif()

		foreach(META IN LISTS TcAddLibrary_META)
			add_dependencies(${META} ${target})
		endforeach()
	else()
		message("  ${TcXlb}${TcCbb}${target}${TcXrb} ${TcXof} ${TcXlb}${TcCbb}${TcLib_REQUIRE}${TcXrb}")
	endif()
endmacro(TcAddLibrary)

macro(TcAddExecutable target src lib)
	set(opts "")
	set(args "")
	set(argsn SOURCES INCLUDE LINK_FLAGS META REQUIRE LIBS LIBRARIES LINK_DIRECTORIES COMPILE_DEFINITIONS)
	cmake_parse_arguments(TcAddExecutable "${opts}" "${args}" "${argsn}" ${ARGN} )

	set(TcTargetBuild TRUE)
	foreach(TC_REQUIRE IN LISTS TcAddExecutable_REQUIRE)
		if (${TC_REQUIRE})
			set(TcTargetBuild TRUE)
		else()
			set(TcTargetBuild FALSE)
			break()
		endif()
	endforeach()

	set(sources "")
	set(sources_list ${src})
	list(APPEND sources_list ${TcAddExecutable_SOURCES})

	foreach(S IN LISTS sources_list)
		set(sources_tmp "")
		file(GLOB_RECURSE sources_tmp ${S})
		list(APPEND sources ${sources_tmp})
	endforeach()

	if (TcTargetBuild)
		if (TcAddExecutable_LINK_DIRECTORIES)
			link_directories(${TcAddExecutable_LINK_DIRECTORIES})
		endif()

		add_executable(${target}-bin "${sources}")
		target_link_libraries(${target}-bin ${lib})

		if (TcAddExecutable_INCLUDE)
			target_include_directories(${target}-bin PUBLIC ${TcAddExecutable_INCLUDE})
		endif()

		if (TcAddExecutable_LINK_FLAGS)
			set_target_properties(${target}-bin PROPERTIES LINK_FLAGS ${TcAddExecutable_LINK_FLAGS})
		endif()

		if (TcAddExecutable_COMPILE_DEFINITIONS)
			target_compile_definitions(${target}-bin PRIVATE ${TcAddExecutable_COMPILE_DEFINITIONS})
		endif()

		if (TcAddExecutable_LIBS)
			target_link_libraries(${target}-bin ${TcAddExecutable_LIBS})
		endif()

		if (TcAddExecutable_LIBRARIES)
			target_link_libraries(${target}-bin ${TcAddExecutable_LIBRARIES})
		endif()

		TcInstall(TARGET ${target}-bin BIN)
		set_target_properties(${target}-bin PROPERTIES OUTPUT_NAME ${target})
		set_target_properties(${target}-bin PROPERTIES LINKER_LANGUAGE CXX)
		foreach(META IN LISTS TcAddExecutable_META)
			add_dependencies(${META} ${target}-bin)
		endforeach()

	else()
		message("  ${TcXlb}${TcCbb}${target}${TcXrb} ${TcXof} ${TcXlb}${TcCbb}${TcExe_REQUIRE}${TcXrb}")
	endif()
endmacro(TcAddExecutable)

function(TcOptMsg description name)
	message("    ${TcCbb}${description}${TcXc} ${${name}} ")
endfunction(TcOptMsg)

function(tc_conan_build_package)
	set(opts SYSTEM UBUNTU NOUPLOAD)
	set(args NAME VERSION BUILD_POLICY)
	set(argsn DEPENDS OPTIONS TARGETS REQUIRE SYSTEMS)
	cmake_parse_arguments(tc_conan_build_package "${opts}" "${args}" "${argsn}" ${ARGN} )

	if (NOT tc_conan_build_package_BUILD_POLICY)
		set(tc_conan_build_package_BUILD_POLICY outdated)
	endif()

	set(TcTargetBuild TRUE)
	foreach(TC_REQUIRE IN LISTS tc_conan_build_package_REQUIRE)
		if (${TC_REQUIRE})
			set(TcTargetBuild TRUE)
		else()
			set(TcTargetBuild FALSE)
			return()
		endif()
	endforeach()

	if (NOT tc_conan_build_package_SYSTEMS)
		if ("${TC_SYSTEM_NAME}" STREQUAL "ubuntu" AND NOT tc_conan_build_package_UBUNTU)
			return()
		endif()

		if ("${TC_SYSTEM_NAME}" STREQUAL "gentoo" AND NOT tc_conan_build_package_GENTOO)
			return()
		endif()
	else()
		set(SYSTEM_OK FALSE)
		foreach(SYSTEM ${tc_conan_build_package_SYSTEMS})
			string(REPLACE "-" ";" SYS ${SYSTEM})
			list(GET SYS 0 SYS_NAME)
			list(GET SYS 1 SYS_VERSION)
			if ("${TC_SYSTEM_NAME}" STREQUAL "${SYS_NAME}" AND "${TC_SYSTEM_VERSION}" STREQUAL "${SYS_VERSION}")
				set(SYSTEM_OK TRUE)
			endif()
		endforeach()

		if(NOT SYSTEM_OK)
			return()
		endif()
	endif()


	set(target ${tc_conan_build_package_NAME})
	set(version ${tc_conan_build_package_VERSION})

	set(OPTIONS "")
	foreach(OPTION ${tc_conan_build_package_OPTIONS})
		list(APPEND OPTIONS -o ${target}:${OPTION})
	endforeach()

	set(sys "")
	if (tc_conan_build_package_SYSTEM)
		list(APPEND OPTIONS -o ${target}:system=True)
		set(sys "system-")
	endif()

	list(APPEND OPTIONS -s os=Linux)
	list(APPEND OPTIONS -s os.name=${TC_SYSTEM_NAME})
	list(APPEND OPTIONS -s os.version=${TC_SYSTEM_VERSION})
	list(APPEND OPTIONS -s os.variant=${TC_SYSTEM_VARIANT})

	##
	# crate conan package
	# we only want to build outdated packages, this will reduce rebuilds in CI/DEV
	#
	add_custom_target(
		conan-${target}-${sys}build
		COMMAND
			CONAN_PRINT_RUN_COMMANDS=1
			VERBOSE=1
			conan create
			${CMAKE_CURRENT_SOURCE_DIR}/conans/${target} ${target}/${version}@tc/stable
			 -pr=default
			 --ignore-dirty
			 --build=${tc_conan_build_package_BUILD_POLICY}
			 --test-build-folder ${CMAKE_CURRENT_BINARY_DIR}/${target} ${OPTIONS}
		DEPENDS ${tc_conan_build_package_DEPENDS}
	)

	add_dependencies(conan-build conan-${target}-${sys}build)
	if (NOT tc_conan_build_package_NOUPLOAD)
		add_custom_target(
			conan-${target}-${sys}upload
			COMMAND conan upload ${target}/${version}@rg/stable --all -r=rg-oss
			DEPENDS conan-${target}-${sys}build
		)
		add_dependencies(conan-upload conan-${target}-${sys}upload)
	endif()

	add_custom_target(
		conan-${target}-${sys}remove
		COMMAND conan remove ${target}/${version}@rg/stable -r=rg-oss -p
	)
	add_dependencies(conan-remove conan-${target}-${sys}remove)

	add_custom_target(
		conan-${target}-${sys}remove-force
		COMMAND conan remove ${target}/${version}@rg/stable -r=rg-oss -p -f
	)
	add_dependencies(conan-remove-force conan-${target}-${sys}remove-force)

	foreach(TARGET ${tc_conan_build_package_TARGETS})
		add_dependencies(conan-build-${RG_SYSTEM_NAME}-${TARGET} conan-${target}-${sys}build)
	endforeach()
endfunction(tc_conan_build_package)


function (tc_compare_file)
	set(opts HASH TIMESTAMP VERBOSE)
	set(args FILE OUT)
	set(argsn)
	cmake_parse_arguments(tc_compare_file "${opts}" "${args}" "${argsn}" ${ARGN} )

	set(OUT ${tc_compare_file_OUT})
	set(${OUT}_CHANGED FALSE CACHE STRING "" FORCE)
	file(TIMESTAMP ${tc_compare_file_FILE} TIMESTAMP)

	if (NOT ${OUT}_TIMESTAMP_PREV_CACHE)
		set(${OUT}_TIMESTAMP_PREV 0 CACHE STRING "" FORCE)
	else()
		set(${OUT}_TIMESTAMP_PREV "${${OUT}_TIMESTAMP_PREV_CACHE}" CACHE STRING "" FORCE)
	endif()

	set(${OUT}_TIMESTAMP "${HASH}-${TIMESTAMP}" CACHE STRING "" FORCE)
	if (NOT "${${OUT}_TIMESTAMP_PREV_CACHE}" MATCHES "${${OUT}_TIMESTAMP}")
		set(${OUT}_CHANGED TRUE CACHE STRING "" FORCE)
	endif()
	set(${OUT}_TIMESTAMP_PREV_CACHE "${${OUT}_TIMESTAMP}" CACHE STRING "" FORCE)
endfunction()

function(tc_conan_get)
	if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
		message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
		file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.8/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
	endif()
endfunction()

macro(tc_conan_cmake_run)
	cmake_parse_arguments(${ARGV})

	tc_compare_file(
		FILE ${CMAKE_SOURCE_DIR}/${ARGUMENTS_CONANFILE}
		TIMESTAMP HASH
		OUT TC_CONANFILE
	)

	set(TAG "${TcXlb}${TcCby}conan${TcCra}${TcXrb}")
	if (NOT TC_CONANFILE_CHANGED AND EXISTS "${CMAKE_BINARY_DIR}/conan_paths.cmake")
		message(STATUS "${TAG} ${ARGUMENTS_CONANFILE} not changed: ${TC_CONANFILE_TIMESTAMP}")
		message(STATUS "${TAG} shall not be run")
	else()
		message(STATUS "${TAG} ${ARGUMENTS_CONANFILE} changed: ${TC_CONANFILE_TIMESTAMP_PREV} -> ${TC_CONANFILE_TIMESTAMP}")


		if(CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE AND NOT CONAN_EXPORTED)
			set(CONAN_CMAKE_MULTI ON)
			message(STATUS "Conan: Using cmake-multi generator")
		else()
			set(CONAN_CMAKE_MULTI OFF)
		endif()
		if(NOT CONAN_EXPORTED)
			conan_cmake_setup_conanfile(${ARGV})
			if(CONAN_CMAKE_MULTI)
				foreach(CMAKE_BUILD_TYPE "Release" "Debug")
					conan_cmake_settings(settings)
					tc_conan_cmake_install(SETTINGS ${settings} ${ARGV})
				endforeach()
			set(CMAKE_BUILD_TYPE)
			else()
				conan_cmake_settings(settings)
				list(TRANSFORM settings REPLACE "build_type=Debug" "build_type=Release")

				string(REGEX REPLACE "-s;compiler=[^;]*;" "-s;compiler=gcc;" settings "${settings}" )
				string(REGEX REPLACE "-s;compiler.version=[^;]*;" "-s;compiler.version=11;" settings "${settings}" )
				set(settings "${settings};-s;os.name=${TC_SYSTEM_NAME};-s;os.version=${TC_SYSTEM_VERSION};-s;os.variant=${TC_SYSTEM_VARIANT};")
				message(STATUS "${TAG} settings: ${settings}")
				tc_conan_cmake_install(SETTINGS ${settings} ${ARGV})
			endif()
		endif()

		conan_load_buildinfo()

		if(ARGUMENTS_BASIC_SETUP)
			if(ARGUMENTS_CMAKE_TARGETS)
				if(ARGUMENTS_KEEP_RPATHS)
					conan_basic_setup(TARGETS KEEP_RPATHS)
				else()
					conan_basic_setup(TARGETS)
				endif()
			else()
				if(ARGUMENTS_KEEP_RPATHS)
					conan_basic_setup(KEEP_RPATHS)
				else()
					conan_basic_setup()
				endif()
			endif()
		endif()
	endif()
endmacro()


macro(TcInstall)
	set(opts LIB EXE)
	set(args TARGET)
	set(argsn)
	cmake_parse_arguments(TcInstall "${opts}" "${args}" "${argsn}" ${ARGN} )
	if (TcInstall_LIB)
		set(dst ${CMAKE_INSTALL_PREFIX}/lib)
		install(
			TARGETS ${TcInstall_TARGET}
			EXPORT ${TcInstall_TARGET}
			LIBRARY
			DESTINATION ${dst}
			PERMISSIONS
				OWNER_READ
				OWNER_WRITE
				OWNER_EXECUTE
				GROUP_READ
				GROUP_WRITE
				GROUP_EXECUTE
				WORLD_READ
				WORLD_EXECUTE
			OPTIONAL
		)
	else()
		set(dst ${CMAKE_INSTALL_PREFIX}/bin)
		install(
			TARGETS ${TcInstall_TARGET}
			EXPORT ${TcInstall_TARGET}
			DESTINATION ${dst}
			PERMISSIONS
				OWNER_READ
				OWNER_WRITE
				OWNER_EXECUTE
				GROUP_READ
				GROUP_WRITE
				GROUP_EXECUTE
				WORLD_READ
				WORLD_EXECUTE
			OPTIONAL
		)
	endif()
endmacro(TcInstall)


function(tc_conan_cmake_install)
	parse_arguments(${ARGV})

	set(CONAN_BUILD_POLICY "")
	foreach(ARG ${ARGUMENTS_BUILD})
		if(${ARG} STREQUAL "all")
			set(CONAN_BUILD_POLICY ${CONAN_BUILD_POLICY} --build)
			break()
		else()
			set(CONAN_BUILD_POLICY ${CONAN_BUILD_POLICY} --build=${ARG})
		endif()
	endforeach()
	if(ARGUMENTS_CONAN_COMMAND)
		set(conan_command ${ARGUMENTS_CONAN_COMMAND})
	else()
		set(conan_command conan)
	endif()

	set(CONAN_OPTIONS "")
	if(ARGUMENTS_CONANFILE)
		set(CONANFILE ${CMAKE_CURRENT_SOURCE_DIR}/${ARGUMENTS_CONANFILE})
		# A conan file has been specified - apply specified options as well if provided
		foreach(ARG ${ARGUMENTS_OPTIONS})
			set(CONAN_OPTIONS ${CONAN_OPTIONS} -o ${ARG})
		endforeach()
	else()
		set(CONANFILE ".")
	endif()

	if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND ARGUMENTS_DEBUG_PROFILE)
		set(settings -pr ${ARGUMENTS_DEBUG_PROFILE})
	endif()

	if(CMAKE_BUILD_TYPE STREQUAL "Release" AND ARGUMENTS_RELEASE_PROFILE)
		set(settings -pr ${ARGUMENTS_RELEASE_PROFILE})
	endif()

	if(ARGUMENTS_PROFILE)
		set(settings -pr ${ARGUMENTS_PROFILE})
	endif()

	if(ARGUMENTS_UPDATE)
		set(CONAN_INSTALL_UPDATE --update)
	endif()

	set(conan_args install ${CONANFILE} ${settings} ${CONAN_BUILD_POLICY} ${CONAN_INSTALL_UPDATE} ${CONAN_OPTIONS})

	string (REPLACE ";" " " _conan_args "${conan_args}")
	message(STATUS "Conan executing: ${conan_command} ${_conan_args}")

	execute_process(COMMAND ${conan_command} ${conan_args}
										RESULT_VARIABLE return_code
										WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

	if(NOT "${return_code}" STREQUAL "0")
		set(TC_CONANFILE_TIMESTAMP_PREV_CACHE 0 CACHE STRING "" FORCE)
		message(FATAL_ERROR "Conan install failed='${return_code}'")
	endif()

endfunction()

function(tc_detect_system)
	find_program(TC_LSB_RELEASE_EXEC lsb_release)
	if (TC_LSB_RELEASE_EXEC)
		execute_process(COMMAND ${TC_LSB_RELEASE_EXEC} -is
			OUTPUT_VARIABLE VAR
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		string(TOLOWER "${VAR}" VAR)

		set(TC_SYSTEM_NAME ${VAR} PARENT_SCOPE)
		execute_process(COMMAND ${TC_LSB_RELEASE_EXEC} -rs
			OUTPUT_VARIABLE VAR
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		set(TC_SYSTEM_VERSION ${VAR} PARENT_SCOPE)
	endif()
endfunction(tc_detect_system)

function(tc_find_package_status)
	set(opts REQUIRED)
	set(args NAME)
	set(argsn )
	cmake_parse_arguments(ARG "${opts}" "${args}" "${argsn}" ${ARGN} )

	set(TAG "${TcXlb}${TcCby}find${TcCra}${TcXrb}")
	message(STATUS "${TAG} ${TcCbm}${ARG_NAME}${TcCra}")
	if (ARG_REQUIRED)
		find_package(${ARG_NAME} REQUIRED)
	else()
		find_package(${ARG_NAME} REQUIRED)
	endif()

	set(NAME "${TcCbm}${ARG_NAME}${TcCra}")
	if (${${ARG_NAME}_FOUND})
		message(STATUS "${TAG} ${NAME} ${TcXfound}")
	else()
		message(STATUS "${TAG} ${NAME} ${TcXmissing}")
	endif()

endfunction()



#endif /* A719A074_6EC7_4C03_A025_880CD7C92228 */
