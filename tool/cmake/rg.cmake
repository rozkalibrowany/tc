cmake_minimum_required(VERSION 3.16)

function(tc_set_colors)
	set(FOPT ON)
	set(FARGS)
	set(FMARGS)
	cmake_parse_arguments(TcSetColors "${FOPT}" "${FARGS}" "${FMARGS}" ${ARGN})

	STRING(ASCII 27 ESC)
	SET(TcCra  "${ESC}[m" CACHE STRING "TcCra" FORCE)    # RESET

	SET(TcCbb  "${ESC}[1;34m" CACHE STRING "TcCbb" FORCE) # BOLD BLUE
	SET(TcCbr  "${ESC}[1;31m" CACHE STRING "TcCbr" FORCE) # BOLD RED
	SET(TcCbw  "${ESC}[1;37m" CACHE STRING "TcCbw" FORCE) # BOLD WHITE
	SET(TcCby  "${ESC}[1;33m" CACHE STRING "TcCby" FORCE) # BOLD YELLOW
	SET(TcXc   "${RgCra}${RgCbw}:${RgCra}" CACHE STRING "TcXc" FORCE) # COLON
endfunction(tc_set_colors)


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
endfunction()

macro(tc_conan_cmake_run)
	parse_arguments(${ARGV})

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
				string(REGEX REPLACE "-s;compiler.version=[^;]*;" "-s;compiler.version=10;" settings "${settings}" )
				set(settings "${settings};-s;os.name=${RG_SYSTEM_NAME};-s;os.version=${TC_SYSTEM_VERSION};-s;os.variant=${TC_SYSTEM_VARIANT};")
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

endfunction()