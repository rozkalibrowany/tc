from conans import ConanFile, python_requires
import os

tc = python_requires("tc/0.3.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True)


class ArgsParserConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "mini"
    license = "MIT"
    exports_sources = ["CMakeLists.txt"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings

    options, default_options = opts.options, opts.default

    def source(s):
        return s.do_source()

    def package(s):
        src_dir = os.path.join(s._source_subfolder, "src")
        s.copy(
            "*.h",
            src=os.path.join(src_dir, "mini"),
            dst=os.path.join("include", "mini"),
        )

    def package_id(s):
        s.info.header_only()

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "mini")
        self.cpp_info.set_property("cmake_target_name", "mini::mini")
        self.cpp_info.includedirs.append(os.path.join("include", "mini"))
        self.cpp_info.bindirs = []
        self.cpp_info.frameworkdirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.resdirs = []
