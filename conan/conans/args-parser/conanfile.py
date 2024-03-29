from conans import ConanFile, python_requires
import os

tc = python_requires("tc/0.3.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True)


class ArgsParserConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "args-parser"
    license = "MIT"
    exports_sources = ["CMakeLists.txt"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings

    options, default_options = opts.options, opts.default

    def configure(s):
        del s.settings.compiler.cppstd

    def source(s):
        return s.do_source()

    def build(s):
        s.do_build(definitions=None)

    def package(s):
        s.copy(
            "*.hpp",
            src=os.path.join(s._source_subfolder, "args-parser"),
            dst=os.path.join("include", "args-parser"),
        )

    def package_id(s):
        s.info.header_only()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "args-parser")
        self.cpp_info.set_property("cmake_target_name", "args-parser::args-parser")
        self.cpp_info.includedirs.append(os.path.join("include", "args-parser"))
        self.cpp_info.bindirs = []
        self.cpp_info.frameworkdirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.resdirs = []
