from conans import ConanFile, python_requires
import os

tc = python_requires('tc/0.3.0@tc/stable')
opts = (
    tc.OptCreator()
    .add_bool("shared", True)
    .add_bool("fPIC", True)
    .add_bool("system", False)
)
class SpdlogConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
	name = "spdlog"
	license = "MIT"
	settings = tc.CmakeHelper.settings
	exports_sources = ['CMakeLists.txt']
	generators = tc.CmakeHelper.generators
	options, default_options = opts.options, opts.default

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
		    },
		])