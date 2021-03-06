from conans import ConanFile, tools, python_requires
import os
import os.path as osp
import glob

tc = python_requires("tc/0.3.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True)


class CppServerConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "cppserver"
    license = "MIT"
    exports_sources = ["CMakeLists.txt", "patches/**"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings
    options, default_options = opts.options, opts.default

    scm = {
        "type": "git",
        "subfolder": "source_subfolder",
        "url": "https://github.com/chronoxor/CppServer.git",
        "revision": "master",
    }

    def build(self):
        os.chdir(self._source_subfolder)
        self.run("gil update")
        definitions = {
            "CPPSERVER_MODULE": "OFF",
        }
        return self.do_build(definitions=definitions)

    def package(self):
        inc_dir = osp.join(self._source_subfolder, "include")
        common_dir = osp.join(self._source_subfolder, "modules", "CppCommon", "include")
        fmt_dir = osp.join(self._source_subfolder, "modules", "CppCommon", "modules", "fmt", "include")
        asio_dir = osp.join(self._source_subfolder, "modules", "asio", "asio", "include")
        self.copy("*.h", dst="include", src=inc_dir)
        self.copy("*.h", dst="include", src=common_dir)
        self.copy("*.h*", dst="include", src=asio_dir)
        self.copy("*.h*", dst="include", src=fmt_dir)

        self.copy("*.inl", dst="include", src=common_dir)
        self.copy("*.inl", dst="include", src=inc_dir)
        self.copy("*.so", dst="lib", src=osp.join(self._build_subfolder, "lib"))

    def package_info(self):

        self.add_components(
            [
                {
                    "target": "cppcommon",
                    "libs": ["cppcommon"],
                    "requires": [],
                },
                {
                    "target": "asio",
                    "libs": ["asio"],
                    "requires": [],
                    "system_libs": ["pthread"],
                },
                {
                    "target": "lib",
                    "libs": ["cppserver"],
                    "requires": ["cppcommon", "asio"],
                },
            ]
        )
