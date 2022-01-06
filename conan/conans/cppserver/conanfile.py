from conans import ConanFile, tools, python_requires
import os
import os.path as osp

tc = python_requires("tc/0.1.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True)

class CppServerConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
	name = "cppserver"
	license = "MIT"
	exports_sources = ["patches/**", "CMakeLists.txt"]
	#requires = ["asio/1.19.1"]
	generators = "cmake_find_package", "cmake"
	settings = tc.CmakeHelper.settings
	options, default_options = opts.options, opts.default

	@property
	def _build_subfolder(s):
		return "build_subfolder"

	def configure(s):
		del s.settings.compiler.cppstd

	def source(s):
		return s.do_source()

	def build(s):
		os.chdir(s._source_subfolder)
		s.run("gil update")
		s.run("cd modules/CppCommon/modules/fmt && git checkout 8.0.0")
		definitions = {
			"CPPSERVER_MODULE": "OFF",
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
		        'libs': ['cppserver'],
#						'requires': ['asio'],
		    },
		])