from conans.model import Generator
from conans.paths import BUILD_INFO
from conans import ConanFile

from collections import OrderedDict

import json


class VSCodeProperties(Generator):
    def _makeConfigEntry(self, config_name, includes):
        entry = OrderedDict()
        entry["name"] = config_name
        entry["includePath"] = includes
        entry["defines"] = [
            "_LINUX",
            "TRC",
            "CUDA",
            "GPU",
            "RG_WITH_OPENSSL",
            "RG_WITH_GNUTLS",
            "TFM_DESC",
        ]
        entry["compileCommands"] = "${workspaceFolder}/build/compile_commands.json"
        entry["compilerPath"] = "/usr/bin/g++-10"
        entry["cStandard"] = "c11"
        entry["cppStandard"] = "c++20"

        browse = OrderedDict()
        browse["limitSymbolsToIncludedHeaders"] = True
        browse["databaseFilename"] = ""
        entry["browse"] = browse

        return entry

    @property
    def filename(self):
        return "../.vscode/c_cpp_properties.json"

    @property
    def content(self):
        configurations = []
        includes = [
            "/usr/include",
            "/usr/local/include",
            "${workspaceFolder}/src",
        ] + self.deps_build_info.include_paths

        configurations = [self._makeConfigEntry("Linux", includes)]

        config = {"configurations": configurations}
        config["configurations"] = configurations
        return json.dumps(config, indent=4)


class VisualStuioCodePropertiesGenerator(ConanFile):
    name = "vscode"
    version = "1.0.2"
    description = "Simple visual studio code C/C++ extension property generator"
    license = "MIT"

    def build(self):
        pass

    def package_info(self):
        self.cpp_info.includedirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.bindirs = []