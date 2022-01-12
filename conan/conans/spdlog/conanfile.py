from conans import ConanFile, python_requires
from conans import CMake, ConanFile, tools
import os

tc = python_requires('tc/0.2.0@tc/stable')
opts = tc.OptCreator() \
 .add_bool('shared', True) \
 .add_bool('header_only', False)

class SpdlogConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
	name = "spdlog"
	license = "MIT"
	settings = tc.CmakeHelper.settings
	exports_sources = "CMakeLists.txt"
	generators = "cmake_find_package", "cmake"

	def requirements(s):
		s.requires("fmt/8.0.0")

	def source(s):
		return s.do_source()

	def build(s):
		definitions = {
		    'SPDLOG_INSTALL': True,
		    'SPDLOG_BUILD_SHARED': True,
		}
		s.do_build(definitions=definitions)

	def package(s):
		return s.do_package()

	def package_info(s):
		s.add_components([
		    {
		        'target': 'lib',
		        'libs': ['spdlog'],
						'requires': ['fmt::lib'],
		    },
		])