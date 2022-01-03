from platform import version
from conans import ConanFile, CMake, tools
import os
from conans.model.options import PackageOptionValue


class SystemReqHelper:

	@property
	def system_packages(s):
		return []

	def do_system_requirements(s):
		if s.settings.os == 'Linux' and tools.os_info.is_linux:
			package_tool = tools.SystemPackageTool()
			packages = s.system_packages
			for package in packages:
				package_tool.install(package)


class VersionHelper(object):

	@property
	def version_major(s):
		return s.version.split('.')[0]

	@property
	def version_minor(s):
		return s.version.split('.')[1]

	@property
	def version_patch(s):
		return s.version.split('.')[2]


class Common:
	generators = ['cmake', 'cmake_find_package']
	settings = ['os', 'compiler', 'build_type', 'arch']


class Test:
	generators = Common.generators
	settings = Common.settings

	def do_build(s, definitions=None):
		cmake = CMake(s)
		if definitions:
			for key, val in definitions.items():
				cmake.definitions[key] = int(val in ['ON', 1, True])

		cmake.configure()
		cmake.build()

	def do_test(s, *ar):
		execs = list(set(list(ar)) | set(['test_package']))

		for exec in execs:
			s.run(os.path.join('bin', exec), run_environment=True)


class OptCreator:

	def __init__(s):
		s.options = {}
		s.default_options = {}

	@property
	def default(s):
		return s.default_options

	def add_bool(s, name, default):
		s.options[name] = [True, False]
		s.default_options[name] = default

		return s

	def add_any(s, name, default):
		s.options[name] = 'ANY'
		s.default_options[name] = default

		return s

	def add_onoff(s, name, default):
		s.options[name] = ['ON', 'OFF']
		s.default_options[name] = default

		return s

	def add_values(s, name, values, default):
		s.options[name] = values
		s.default_options[name] = default

		return s


class CmakeHelper:

	generators = Common.generators
	settings = Common.settings

	_cmake = None

	@property
	def _build_subfolder(s):
		return 'build_subfolder'

	@property
	def generators(s):
		return 'cmake', 'cmake_find_package'

	def _configure_cmake(self, definitions=None, source_folder=None):
		if self._cmake:
			return self._cmake

		cmake = self._cmake = CMake(self)
		if 'system' in self.options:
			self._cmake.definitions['BUILD_SYSTEM'] = int(bool(self.options.system))

		if definitions:
			for key, val in definitions.items():
				self._cmake.definitions[key] = val

		cmake.configure(build_folder=self._build_subfolder, source_folder=source_folder)

		return cmake

	def do_patch(s):
		if s.conan_data.get("patches", {}).get(s.version, []):
			for patch in s.conan_data['patches'][s.version]:
				tools.patch(**patch)

	def do_build(s, definitions=None, source_folder=None):
		s.do_patch()
		cmake = s._configure_cmake(definitions=definitions, source_folder=source_folder)
		cmake.build()

	def do_package(s, remove_cmake=True):
		cmake = s._configure_cmake()
		cmake.install()

		tools.rmdir(os.path.join(s.package_folder, 'share', 'man'))
		tools.rmdir(os.path.join(s.package_folder, 'lib', 'pkgconfig'))

		if remove_cmake:
			tools.rmdir(os.path.join(s.package_folder, 'cmake'))


class SourceHelper:

	@property
	def _source_subfolder(s):
		return 'source_subfolder'

	@property
	def _source_tar(s):
		return s.name + '-' + s.version

	def do_source(self):
		tools.get(**self.conan_data['sources'][self.version])
		extracted_dir = self._source_tar
		os.rename(extracted_dir, self._source_subfolder)


class ComponentHelper(object):

	def add_components(s, components):
		for component in components:
			s.add_component(component)

	def add_component(s, component):
		v = component
		target = v['target']
		c = s.cpp_info.components[target]
		c.names['cmake_find_package'] = target
		c.names['cmake_find_package_multi'] = target

		if 'system_libs' in v:
			c.system_libs = v['system_libs']

		if 'requires' in v:
			c.requires = v['requires']

		if 'includedirs' in v:
			c.includedirs = v['includedirs']

		if 'libdirs' in v:
			c.libdirs = v['libdirs']

		if 'libs' in v:
			c.libs = v['libs']

		if 'cxxflags' in v:
			for opt in v['cxxflags']:
				val = None
				if opt._value in ['True', 'False']:
					val = int(bool(opt._value))
				else:
					val = opt._value
				flag = '-D{}={}'.format(opt._name, val)

				c.cxxflags.append(flag)
				c.cflags.append(flag)

	def add_cxx_flag(s, opt):
		s.cpp_info.cxxflags.append('-D{}={}'.format(opt._name, int(bool(opt._value))))

	def add_c_flag(s, opt):
		s.cpp_info.cflags.append('-D{}={}'.format(opt._name, int(bool(opt._value))))



class TcConan(ConanFile):
	name = 'tc'
	version = '0.1.0'
	exports = '*'
	build_policy = 'missing'

	def package(self):
		self.copy('*', src='tc', dst='tc')

	def package_info(self):
		self.env_info.PYTHONPATH.append(self.package_folder)