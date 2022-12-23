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
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "polyfill": ["std", "boost", "mnmlstc", "experimental"],
        "with_ssl": [True, False],
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "polyfill": "boost",
        "with_ssl": True,
    }

    @property
    def _minimal_std_version(self):
        return {
            "std": "17",
            "experimental": "14",
            "boost": "11",
            "polyfill": "11"
        }[str(self.options.polyfill)]

    @property
    def _build_subfolder(s):
        return 'build_subfolder'

    def requirements(self):
        self.requires("mongoc/1.22.0@tc/stable")

    def source(self):
        get(self, **self.conan_data["sources"][self.version],
            destination=self.source_folder, strip_root=True)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BSONCXX_POLY_USE_MNMLSTC"] = self.options.polyfill == "mnmlstc"
        tc.variables["BSONCXX_POLY_USE_STD"] = self.options.polyfill == "std"
        tc.variables["BSONCXX_POLY_USE_STD_EXPERIMENTAL"] = self.options.polyfill == "experimental"
        tc.variables["BSONCXX_POLY_USE_BOOST"] = self.options.polyfill == "boost"
        tc.cache_variables["BUILD_VERSION"] = self.version
        tc.cache_variables["BSONCXX_LINK_WITH_STATIC_MONGOC"] = "OFF" if self.dependencies["mongoc"].options.shared else "ON"
        tc.cache_variables["MONGOCXX_LINK_WITH_STATIC_MONGOC"] = "OFF" if self.dependencies["mongoc"].options.shared else "ON"
        tc.variables["MONGOCXX_ENABLE_SSL"] = self.options.with_ssl
        if not valid_min_cppstd(self, self._minimal_std_version):
            tc.variables["CMAKE_CXX_STANDARD"] = self._minimal_std_version
        # Honor BUILD_SHARED_LIBS from conan_toolchain (see https://github.com/conan-io/conan/issues/11840)
        tc.cache_variables["CMAKE_POLICY_DEFAULT_CMP0077"] = "NEW"
        deps = CMakeDeps(self)
        deps.generate()
        mongoc_config_file = os.path.join(self.generators_folder, "mongoc-config.cmake")
        bson_config_file = os.path.join(self.generators_folder, "bson-config.cmake")
        if not os.path.exists(bson_config_file):
            self.output.info("Copying mongoc config file to bson")
            shutil.copy(src=mongoc_config_file, dst=bson_config_file)

    def build(self):
        self.do_build()

    def package(self):
        #self.copy("*.hpp", dst="include/bsoncxx", src=posixpath.join(self.build_folder, self._build_subfolder, "src"), keep_path=False)
        #self.copy("*.hpp", dst="include/mongocxx", src=posixpath.join(self.build_folder, self._build_subfolder, "src"), keep_path=False)
        #self.copy("*.so*", dst="lib", src=osp.join(self.build_folder, "src"))
        #self.copy("*.cmake", dst=osp.join("lib/cmake/bsoncxx-", self.version), src=posixpath.join(self.build_folder, self._build_subfolder, "src", "bsoncxx"))
        #self.copy("*.cmake", dst=osp.join("lib/cmake/mongocxx-", self.version), src=posixpath.join(self.build_folder, self._build_subfolder, "src", "mongocxx"))        #cmake = CMake(self)
        #cmake.definitions["CMAKE_INSTALL_INCLUDEDIR"] = osp.join("include", "mongocxx")
        cmake = self._configure_cmake()
        cmake.install()

        tools.rmdir(os.path.join(self.package_folder, 'share', 'man'))
        tools.rmdir(os.path.join(self.package_folder, 'lib', 'pkgconfig'))
        tools.rmdir(os.path.join(self.package_folder, 'cmake'))

    def package_info(self):
        self.add_components(
          [
              {
                 "target": "lib",
                 "libs": ["mongocxx"],
                 "requires": ["mongoc::mongoc"],
              },
              {
                 "target": "lib",
                 "libs": ["bsoncxx"],
                 "requires": ["mongoc::bson"],
              },
          ]
        )

