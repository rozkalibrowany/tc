from conans import ConanFile, python_requires

tc = python_requires('tc/0.3.0@tc/stable')
opts = tc.OptCreator() \
 .add_bool('shared', True)


class FmtConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "fmt"
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
        definitions = {
            "FMT_DOC": False,
            "FMT_TEST": False,
            "FMT_INSTALL": True,
        }
        s.do_build(definitions=definitions)

    def package(s):
        s.do_package()

    def package_info(s):
        s.add_components([
            {
                "target": "lib",
                "libs": ["fmt"],
            },
        ])
