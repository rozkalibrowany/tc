from conans import ConanFile, python_requires

tc = python_requires("tc/0.3.0@tc/stable")
opts = (
    tc.OptCreator()
    .add_bool("shared", True)
    .add_bool("fPIC", True))


class JsoncppConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "jsoncpp"
    license = "MIT"
    exports_sources = ["CMakeLists.txt"]

    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings

    options, default_options = opts.options, opts.default

    def configure(s):
        del s.settings.compiler.cppstd

    def source(s):
        s.do_source()

    def build(s):
        definitions = {
            "BUILD_STATIC_LIBS": False,
            "JSONCPP_WITH_TESTS": False,
            "JSONCPP_WITH_CMAKE_PACKAGE": False,
            "JSONCPP_WITH_STRICT_ISO": False,
            "JSONCPP_WITH_PKGCONFIG_SUPPORT": False,
            "JSONCPP_WITH_EXAMPLE": False,
        }
        s.do_build(definitions=definitions)

    def package(s):
        s.do_package()

    def package_info(s):
        s.add_components(
            [
                {
                    "target": "lib",
                    "libs": ["jsoncpp"],
                },
            ]
        )
