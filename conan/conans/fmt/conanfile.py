from conans import ConanFile, CMake
import os

class Fmt(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    name = 'fmt'
    url = 'https://github.com/fmtlib/fmt.git'
    license = 'MIT'
    version = '8.0.1'
    no_copy_source = True
    exports_sources = "src/*"
    scm = {
        "type": "git",
        "url": "https://extgit.iaik.tugraz.at/conan/fmt.git",
        "revision": "auto"
    }
    options = {
        "header_only": [True, False],
        "shared": [True, False],
        "fPIC": [True, False]
    }
    default_options = {
        "header_only": False,
        "shared": True,
        "fPIC": True
    }
    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="src")
        cmake.build()
        cmake.install()

    def package_id(self):
        self.info.header_only()
