from conans import ConanFile, tools, python_requires
import os

tc = python_requires('tc/0.1.0@tc/stable')
opts = tc.OptCreator() \
 .add_bool('shared', True) \
 .add_bool('fPIC', True)

class CppServerConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
	name = 'CppServer'
	license = "MIT"
	exports_sources = ['patches/**', 'CMakeLists.txt']
	requires = ["asio/1.19.1", "openssl/1.1.1k"]
	generators = tc.CmakeHelper.generators
	settings = tc.CmakeHelper.settings
	options, default_options = opts.options, opts.default

	@property
	def _source_subfolder(s):
			return "source_subfolder"
	@property
	def _build_subfolder(s):
			return "build_subfolder"

	#@property
	#def _source_tar(s):
	#	return s.name.lower() + '-' + s.version

	scm = {
		'type': 'git',
		'subfolder': 'source_subfolder',
		'url': 'https://github.com/chronoxor/CppServer',
		'revision': 'master',
	}

	def configure(s):
		del s.settings.compiler.cppstd

	#def source(s):
	#	s.do_source()
	#	chdir(s._source_subfolder)
	#	s.run('gil update')

	def build(s):
		os.chdir(s._source_subfolder)
		s.run('gil update')
		s.run('cd modules/CppCommon/modules/fmt && git checkout 8.0.0')
		print("PATH!: " + os.path.abspath(os.getcwd()))
		definitions = {
			'CPPSERVER_MODULE': False,
		}
		s.do_build(definitions=definitions)

	def package(s):
		s.do_package()

	#def package_info(s):
		#s.cpp_info.libs = tools.collect_libs(s)
	#	s.add_components([
	#		{
	#				'target': 'lib',
	#				'libs': ['cppserver'],
	#				'requires': ['asio::asio', 'openssl::openssl'],
	#		},
	#	])