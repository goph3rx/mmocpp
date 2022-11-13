from conans import ConanFile, CMake


class MMOCpp(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "asio/1.24.0", "gtest/cci.20210126", "icu/71.1", "libpqxx/7.7.4", "openssl/1.1.1q", "protobuf/3.21.4", "spdlog/1.10.0"
    generators = "cmake"

    def configure(self):
        if self.settings.os != "Windows":
            self.options["icu"].data_packaging = "static"
