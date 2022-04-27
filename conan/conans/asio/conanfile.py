from conans import ConanFile, python_requires, tools
import os

tc = python_requires("tc/0.3.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True)


class AsioConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "args-parser"
    license = "MIT"
    exports_sources = ["CMakeLists.txt"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings

    options, default_options = opts.options, opts.default

    def source(self):
        tools.get(
            **self.conan_data["sources"][self.version],
            destination=self._source_subfolder,
            strip_root=True
        )

    def package(self):
        root_dir = os.path.join(self._source_subfolder, self.name)
        inc_dir = os.path.join(root_dir, "include")
        self.copy(pattern="LICENSE_1_0.txt", dst="licenses", src=root_dir)
        self.copy(pattern="*.hpp", dst="include", src=inc_dir)
        self.copy(pattern="*.ipp", dst="include", src=inc_dir)

    def package_id(s):
        s.info.header_only()

    def package_info(s):
        s.add_components(
            [
                {
                    "target": "lib",
                    "libs": ["asio"],
                },
            ]
        )
