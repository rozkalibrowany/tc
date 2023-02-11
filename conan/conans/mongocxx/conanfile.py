from conans import ConanFile, python_requires, tools, CMake
from conan.tools.cmake import CMakeDeps, CMakeToolchain
from conan.tools.files import apply_conandata_patches, get
from conan.tools.build import check_min_cppstd, valid_min_cppstd

import os
import shutil
import os
import os.path as osp
import posixpath
from urllib.parse import urljoin

tc = python_requires('tc/0.3.0@tc/stable')
opts = (tc.OptCreator().add_bool('shared', True).add_bool('fPIC', True).add_bool('with_ssl', True)).add_any('polyfill', 'mnmlstc')
# "polyfill": ["std", "boost", "mnmlstc", "experimental"]
class MongoCxxConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "mongocxx"
    description = "A Cross Platform MongoDB Client Library for C"
    license = "Apache-2.0"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://mongoc.org/"
    topics = ("libbson", "libmongoc", "mongo", "mongodb", "database", "db")
    exports_sources = ["CMakeLists.txt"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings
    options, default_options = opts.options, opts.default

    def requirements(self):
        self.requires("mongoc/1.23.2@tc/stable")

    def source(self):
        get(self, **self.conan_data["sources"][self.version],
            destination=self.source_folder, strip_root=True)

    def build(self):
        definition = {
          "ENABLE_TESTS": "OFF"
        }
        self.do_build(definitions=definition)

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
        self.copy("*.h*", dst="include", src=osp.join(self.package_folder, "include/bsoncxx/v_noabi"), keep_path=True)
        self.copy("*.h*", dst="include", src=osp.join(self.package_folder, "include/mongocxx/v_noabi"), keep_path=True)
        self.copy("*.so*", dst="lib", src=self._build_subfolder, keep_path=False)
        tools.rmdir(os.path.join(self.package_folder, 'share', 'man'))
        tools.rmdir(os.path.join(self.package_folder, 'lib', 'pkgconfig'))
        tools.rmdir(os.path.join(self.package_folder, 'cmake'))

    def package_info(self):
        self.add_components(
          [
              {
                 "target": "lib",
                 "libs": ["mongocxx"],
                 "requires": ["mongoc::lib"],
              },
              {
                 "target": "bson",
                 "libs": ["bsoncxx"],
                 "requires": ["mongoc::bson"],
              },
          ]
        )

