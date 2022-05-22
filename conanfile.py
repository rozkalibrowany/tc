from conans import ConanFile, python_requires

tc = python_requires("tc/0.3.0@tc/stable")
opts = tc.OptCreator()


class Os:
    @property
    def os_name(s):
        return s.settings.os.name

    @property
    def os_version(s):
        return s.settings.os.version

    @property
    def os_variant(s):
        return s.settings.os.variant

    @property
    def os_version(s):
        return s.settings.os.version

    @property
    def build_type(s):
        return s.settings.build_type


class TcConan(ConanFile, Os):
    settings = "os"
    requires = ""
    generators = "cmake_find_package", "cmake_paths"
    options, default_options = opts.options, opts.default_options

    def configure(s):
        s.options["*"].shared = True
        s.options["*/@tc/*"].compiler = "gcc"
        s.options["*/@tc/*"].compiler.version = "10"
        s.options["*/@tc/*"].compiler.libcxx = "libstdc++11"
        s.options["*/@tc/*"].compiler.cppstd = "20"

    def requirements(self):
        print(self.os_variant, self.os_version)
        self.requires("tc/0.3.0@tc/stable")
        self.requires("cppserver/master@tc/stable")
        self.requires("jsoncpp/1.9.4@tc/stable")
        self.requires("spdlog/1.9.2@tc/stable")
        self.requires("args-parser/6.2.0.1@tc/stable")
        self.requires("vscode/1.0.2@tc/stable")

    def imports(self):
        self.copy("*", src="crt", dst="crt")
        self.copy("*", src="etc", dst="etc")
        self.copy("*", src="bin", dst="bin")
        self.copy("*.so.*", src="lib", dst="lib")
        self.copy("*.so", src="lib", dst="lib")
        self.copy("*", src="include", dst="include")