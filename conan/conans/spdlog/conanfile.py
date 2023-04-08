from conans import ConanFile, python_requires
import os

tc = python_requires("tc/0.3.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True).add_bool("fPIC", True)


class SpdlogConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "spdlog"
    license = "MIT"
    settings = tc.CmakeHelper.settings
    exports_sources = ["CMakeLists.txt"]
    generators = tc.CmakeHelper.generators
    options, default_options = opts.options, opts.default

    def configure(s):
        del s.settings.compiler.cppstd

    def requirements(s):
        s.requires("fmt/9.1.0@tc/stable")

    def source(s):
        s.do_source()

    def build(s):
        definitions = {
            "SPDLOG_FMT_EXTERNAL": True,
            "SPDLOG_BUILD_SHARED": True,
            #   "SPDLOG_INSTALL": True,
        }
        s.do_build(definitions=definitions)

    def package(s):
        s.copy("*.h", src=os.path.join(s._source_subfolder, "include"), dst="include")
        s.copy("*.so*", src=os.path.join(s._build_subfolder, "lib"), dst="lib")
        s.do_package()

    def package_info(s):
        s.add_components(
            [
                {
                    "target": "lib",
                    "libs": ["spdlog"],
                    "requires": ["fmt::lib"]
                },
            ]
        )
