from conans import ConanFile, tools, python_requires
import os
import os.path as osp

tc = python_requires("tc/0.2.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True)

class CppCommonConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
	name = "cppcommon"
	license = "MIT"
	exports_sources = ["patches/**", "CMakeLists.txt"]
	generators = "cmake_find_package", "cmake"
	settings = tc.CmakeHelper.settings
	options, default_options = opts.options, opts.default

	@property
	def _build_subfolder(s):
		return "build_subfolder"

	def configure(s):
		del s.settings.compiler.cppstd

	def source(s):
		s.do_source()
		os.chdir(s._source_subfolder)
		s.run("gil update")
		s.run("cd modules/fmt && git checkout 8.0.0")
		#s.run("cd ../asio/asio && git checkout 1.20.0")

	def build(s):
		definitions = {
			"CPPCOMMON_MODULE": "OFF",
		}
		return s.do_build(definitions=definitions)

	def package(s):
		inc_dir = osp.join(s._source_subfolder, 'include')
		s.copy("*.h", dst="include", src=inc_dir)
		s.copy("*.inl", dst="include", src=inc_dir)
		s.copy("*so", dst="lib", keep_path=False)
		return s.do_package()

	def package_info(s):
		s.add_components([
		    {
		        'target': 'lib',
		        'libs': ['cppcommon'],
		    },
		])