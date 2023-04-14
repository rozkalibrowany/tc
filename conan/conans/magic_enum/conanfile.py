from conans import ConanFile, python_requires
from conan.tools.files import copy, get
import os

tc = python_requires("tc/0.3.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True)


class ArgsParserConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "magic_enum"
    license = "MIT"
    exports_sources = ["CMakeLists.txt"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings

    options, default_options = opts.options, opts.default

    def source(self):
        return self.do_source()

    def package(self):
        self.copy(
            "*.h*", src=os.path.join(self._source_subfolder, "include"), dst="include"
        )
        return self.do_package()

    def package_id(self):
        self.info.header_only()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "magic_enum")
        self.cpp_info.set_property("cmake_target_name", "magic_enum::magic_enum")
        self.cpp_info.includedirs.append("include")
        self.cpp_info.bindirs = []
        self.cpp_info.frameworkdirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.resdirs = []
