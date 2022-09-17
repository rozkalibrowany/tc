from conans import ConanFile, python_requires, tools
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.scm import Version
from conan.tools.files import copy, rmdir
import os


tc = python_requires('tc/0.3.0@tc/stable')
opts = (tc.OptCreator().add_bool('shared', True).add_bool('fPIC', False).add_bool('with_ssl', True))


class MongoConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "mongo-c-driver"
    description = "A Cross Platform MongoDB Client Library for C"
    license = "Apache-2.0"
    topics = ("libbson", "libmongoc", "mongo", "mongodb", "database", "db")
    exports_sources = ["CMakeLists.txt", "patches/*"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings
    options, default_options = opts.options, opts.default

    def configure(self):
        if self.options.shared:
            del self.options.fPIC

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["ENABLE_SSL"] =  "ON"
        tc.cache_variables["ENABLE_STATIC"] = "OFF"
        tc.cache_variables["ENABLE_TESTS"] = "OFF"
        tc.cache_variables["ENABLE_EXAMPLES"] = "OFF"
        tc.cache_variables["ENABLE_MAINTAINER_FLAGS"] = "OFF"
        tc.cache_variables["ENABLE_AUTOMATIC_INIT_AND_CLEANUP"] = "ON"
        tc.cache_variables["ENABLE_CRYPTO_SYSTEM_PROFILE"] = "OFF"
        tc.cache_variables["ENABLE_TRACING"] = "OFF"
        tc.cache_variables["ENABLE_COVERAGE"] = "OFF"
        tc.cache_variables["ENABLE_SHM_COUNTERS"] = "OFF"
        tc.cache_variables["ENABLE_MONGOC"] = "ON"
        tc.cache_variables["ENABLE_BSON"] = "ON"
        tc.cache_variables["ENABLE_MAN_PAGES"] = "OFF"
        tc.cache_variables["ENABLE_HTML_DOCS"] = "OFF"
        tc.cache_variables["ENABLE_EXTRA_ALIGNMENT"] = "ON"
        tc.cache_variables["ENABLE_RDTSCP"] = "OFF"
        tc.cache_variables["ENABLE_APPLE_FRAMEWORK"] = "OFF"
        tc.cache_variables["ENABLE_UNINSTALL"] = "OFF"
        tc.cache_variables["ENABLE_CLIENT_SIDE_ENCRYPTION"] = "OFF"  # libmongocrypt recipe not yet in CCI
        tc.cache_variables["ENABLE_MONGODB_AWS_AUTH"] = "AUTO"
        # Avoid to install vc runtime stuff
        tc.variables["CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP"] = "TRUE"
        if Version(self.version) >= "1.20.0":
            tc.variables["MONGO_USE_CCACHE"] = "OFF"

        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def source(self):
        self.do_source()

    def build(self):
        definitions = {
          "ENABLE_SSL": "OPENSSL",
          "ENABLE_MONGOC": "ON",
          "ENABLE_BSON":  "ON",
          "BUILD_SHARED_LIBS": "ON",
          "ENABLE_TESTS": "OFF",
          "ENABLE_EXAMPLES": "OFF",
          "ENABLE_STATIC": "ON",
          "ENABLE_SHM_COUNTERS": "OFF"
        }
        self.do_build(definitions=definitions)

    def package(self):
        copy(self, "COPYING", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        copy(self, "THIRD_PARTY_NOTICES", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        rmdir(self, os.path.join(self.package_folder, "share"))
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        self.do_package()

    def package_info(self):
        self.cpp_info.names["cmake_find_package"] = "mongoc"
        self.cpp_info.names["cmake_find_package_multi"] = "mongoc"
        self.cpp_info.names["cmake_find_package"] = "bson"
        self.cpp_info.names["cmake_find_package_multi"] = "bson"

        mongoc_target = "mongoc_shared" if self.options.shared else "mongoc_static"
        self.cpp_info.set_property("cmake_file_name", "mongoc-1.0")
        self.cpp_info.set_property("cmake_target_name", f"mongo::{mongoc_target}")
        self.cpp_info.filenames["cmake_find_package"] = "mongoc-1.0"
        self.cpp_info.filenames["cmake_find_package_multi"] = "mongoc-1.0"
        self.cpp_info.names["cmake_find_package"] = "mongo"
        self.cpp_info.names["cmake_find_package_multi"] = "mongo"

        self.cpp_info.components["mongoc"].set_property("cmake_target_name", f"mongo::{mongoc_target}")
        self.cpp_info.components["mongoc"].set_property("pkg_config_name", "libmongoc-1.0" if self.options.shared else "libmongoc-static-1.0")
        self.cpp_info.components["mongoc"].names["cmake_find_package"] = mongoc_target
        self.cpp_info.components["mongoc"].names["cmake_find_package_multi"] = mongoc_target
        self.cpp_info.components["mongoc"].includedirs = [os.path.join("include", "libmongoc-1.0")]
        self.cpp_info.components["mongoc"].libs = ["mongoc-1.0" if self.options.shared else "mongoc-static-1.0"]
        if not self.options.shared:
            self.cpp_info.components["mongoc"].defines = ["MONGOC_STATIC"]
        self.cpp_info.components["mongoc"].requires = ["bson"]

        bson_target = "bson_shared" if self.options.shared else "bson_static"
        self.cpp_info.components["bson"].set_property("cmake_target_name", f"mongo::{bson_target}")
        self.cpp_info.components["bson"].set_property("pkg_config_name", "libbson-1.0" if self.options.shared else "libbson-static-1.0")
        self.cpp_info.components["bson"].names["cmake_find_package"] = bson_target
        self.cpp_info.components["bson"].names["cmake_find_package_multi"] = bson_target
        self.cpp_info.components["bson"].includedirs = [os.path.join("include", "libbson-1.0")]
        self.cpp_info.components["bson"].libs = ["bson-1.0" if self.options.shared else "bson-static-1.0"]
        if not self.options.shared:
            self.cpp_info.components["bson"].defines = ["BSON_STATIC"]