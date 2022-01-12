from conans import ConanFile, python_requires, tools

tc = python_requires('tc/0.1.0@tc/stable')
opts = tc.OptCreator() \
 .add_bool('shared', True)

class FmtConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
	name = 'asio'
	license = 'BSL-1.0'
	exports_sources = ['CMakeLists.txt']
	generators = tc.CmakeHelper.generators
	settings = tc.CmakeHelper.settings
	no_copy_source = True

	options, default_options = opts.options, opts.default

	@property
	def _source_subfolder(self):
		return "source_subfolder"

	def configure(s):
		del s.settings.compiler.cppstd

	def source(s):
		tools.get(**s.conan_data["sources"][s.version],
			destination=s._source_subfolder, strip_root=True)
		#return s.do_source()

	def build(s):
		s.do_build(definitions=None)

	def package(s):
		s.do_package()

	def package_info(s):
		s.add_components([
		    {
		        'target': 'lib',
		        'libs': ['asio'],
		    },
		])