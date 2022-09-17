from conans import ConanFile, python_requires,
from conan.tools.cmake import CMakeDeps
from conan.tools.files import apply_conandata_patches, get
import os
import shutil
import os

tc = python_requires('tc/0.3.0@tc/stable')
opts = (tc.OptCreator().add_bool('shared', True).add_bool('fPIC', False))


class MongoConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "mongo-cxx-driver"
    description = "A Cross Platform MongoDB Client Library for C"
    license = "Apache-2.0"
    url = "https://github.com/conan-io/conan-center-index"
    homepage = "https://mongoc.org/"
    topics = ("libbson", "libmongoc", "mongo", "mongodb", "database", "db")
    exports_sources = ["CMakeLists.txt", "patches/*"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings
    options, default_options = opts.options, opts.default

    def requirements(self):
        self.requires("mongo-c-driver/1.22.0@tc/stable")

    def source(self):
        get(self, **self.conan_data["sources"][self.version],
            destination=self.source_folder, strip_root=True)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        mongoc_config_file = os.path.join(self.generators_folder, "mongoc-1.0-config.cmake")
        bson_config_file = os.path.join(self.generators_folder, "bson-1.0-config.cmake")
        if not os.path.exists(bson_config_file):
            self.output.info("Copying mongoc config file to bson")
            shutil.copy(src=mongoc_config_file, dst=bson_config_file)

    def build(self):
        apply_conandata_patches(self)
        self.do_build()

    def package(self):
        self.do_package()

    def package_info(self):
        self.add_components(
          [
              {
                 "target": "lib",
                 "libs": ["mongocxx"],
                 "requires": ["mongo-c-driver::mongoc"],
              },
              {
                 "target": "lib",
                 "libs": ["bsoncxx"],
                 "requires": ["mongo-c-driver::bson"],
              },
          ]
        )
