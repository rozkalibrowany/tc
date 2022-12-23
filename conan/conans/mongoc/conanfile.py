from conans import ConanFile, python_requires, tools
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.scm import Version
from conan.tools.files import copy, rmdir, get, export_conandata_patches, apply_conandata_patches, replace_in_file
from conan.tools.gnu import PkgConfigDeps
import os


tc = python_requires('tc/0.3.0@tc/stable')
opts = (tc.OptCreator().add_bool('shared', True).add_bool('fPIC', True).add_bool('with_ssl', True)).add_any('polyfill', 'mnmlstc')\
  .add_bool('with_snappy', True).add_bool('with_zlib', True).add_bool('with_zstd', True).add_bool('with_icu', False).add_bool('srv', True)

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

    def _patch_sources(self):
        apply_conandata_patches(self)
        to_replace_old_new = [
            # Fix Snappy
            {"old": "include (FindSnappy)\nif (SNAPPY_INCLUDE_DIRS)",
             "new": "if(ENABLE_SNAPPY MATCHES \"ON\")\n  find_package(Snappy REQUIRED)"},
            {"old": "SNAPPY_LIBRARIES", "new": "Snappy_LIBRARIES"},
            {"old": "SNAPPY_INCLUDE_DIRS", "new": "Snappy_INCLUDE_DIRS"},
            # Fix LibreSSL
            {"old": "set (SSL_LIBRARIES -ltls -lcrypto)", "new": ""},
        ]
        for old_new in to_replace_old_new:
            replace_in_file(self, os.path.join(self.source_folder, "src", "libmongoc", "CMakeLists.txt"),
                                  old_new["old"], old_new["new"])
        # cleanup rpath
        replace_in_file(self, os.path.join(self.source_folder, "CMakeLists.txt"),
                              "set (CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)", "")
    def build(self):
        definition = {
          "ENABLE_TESTS": "OFF"
				}
        self.do_build(definitions=definition)

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
        copy(self, "COPYING", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        copy(self, "THIRD_PARTY_NOTICES", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        self.copy("Find*.cmake", ".", ".")


    def package_info(self):
        self.add_components(
          [
              {
                 "target": "bson",
                 "libs": ["bson-1.0"],
              },
              {
                 "target": "lib",
                 "libs": ["mongoc-1.0"],
              },

          ]
        )
        cmake_dir = os.path.join(os.path.join(self.package_folder, "lib"), "cmake")
        self.output.info("Appending PATH environment variable: {}".format(cmake_dir))
        self.env_info.PATH.append(cmake_dir)