from conans import ConanFile, python_requires, tools
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.scm import Version
from conan.tools.files import copy, rmdir, get
import os


tc = python_requires('tc/0.3.0@tc/stable')
opts = (tc.OptCreator().add_bool('shared', True).add_bool('fPIC', True).add_bool('with_ssl', True))


class MongoCConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "mongoc"
    description = "A Cross Platform MongoDB Client Library for C"
    license = "Apache-2.0"
    topics = ("libbson", "libmongoc", "mongo", "mongodb", "database", "db")
    exports_sources = ["CMakeLists.txt"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings
    options, default_options = opts.options, opts.default

    def configure(self):
        if self.options.shared:
            del self.options.fPIC

    def source(self):
        get(self, **self.conan_data["sources"][self.version],
            destination=self.source_folder, strip_root=True)

    def build(self):
        definitions = {
          "ENABLE_SSL": "OPENSSL",
          "ENABLE_MONGOC": "ON",
          "ENABLE_BSON":  "ON",
          "BUILD_SHARED_LIBS": "ON",
          "ENABLE_TESTS": "OFF",
          "ENABLE_EXAMPLES": "OFF",
          "ENABLE_STATIC": "OFF",
          "ENABLE_SHM_COUNTERS": "OFF"
        }
        self.do_build(definitions=definitions)

    def package(self):
        copy(self, "COPYING", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        copy(self, "THIRD_PARTY_NOTICES", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        rmdir(self, os.path.join(self.package_folder, "share"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        self.do_package()


    def package_info(self):
        self.add_components(
          [
              {
                 "target": "lib",
                 "libs": ["mongoc"],
              },
              {
                 "target": "lib",
                 "libs": ["bson"],
              },
          ]
        )

        libdir_c = os.path.join(os.path.join(self.package_folder, "lib"), "cmake")
        self.output.info("Appending PATH environment variable: {}".format(libdir_c))
        self.env_info.PATH.append(libdir_c)