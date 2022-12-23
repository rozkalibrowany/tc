from conans import ConanFile, CMake, tools, python_requires
import os
import os.path as osp
import glob

tc = python_requires("tc/0.3.0@tc/stable")
opts = tc.OptCreator().add_bool("shared", True).add_bool("fPIC", True)


class CppServerConan(ConanFile, tc.SourceHelper, tc.CmakeHelper, tc.ComponentHelper):
    name = "cppserver"
    license = "MIT"
    exports_sources = ["CMakeLists.txt"]
    generators = tc.CmakeHelper.generators
    settings = tc.CmakeHelper.settings
    options, default_options = opts.options, opts.default

    scm = {
        "type": "git",
        "subfolder": "source_subfolder",
        "url": "https://github.com/rozkalibrowany/CppServer.git",
        "revision": "master",
    }

    def build(self):
        os.chdir(self._source_subfolder)
        self.run("gil update")
        return self.do_build()

    def package(self):
        inc_dir = osp.join(self._source_subfolder, "include")
        common_dir = osp.join(self._source_subfolder, "modules", "CppCommon", "include")
        self.copy("*.h*", dst="include", src=inc_dir)
        self.copy("*.h*", dst="include", src=common_dir)
        self.copy("*.inl", dst="include", src=common_dir)
        self.copy("*.inl", dst="include", src=inc_dir)
        self.copy("*.so", dst="lib", src=osp.join(self._build_subfolder, "lib"))


    def package_info(self):
        self.add_components(
            [
                {
                    "target": "lib",
                    "libs": ["fmt"],
                },
                {
                    "target": "lib",
                    "libs": ["cppserver"],
                },

            ]
        )
        #libdir_c = os.path.join(os.path.join(self.package_folder, "lib"), "cmake")
        #self.output.info("Appending PATH environment variable: {}".format(libdir_c))
        #self.env_info.PATH.append(libdir_c)
