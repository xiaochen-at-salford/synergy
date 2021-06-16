load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "libleddartech_parser.so",
    linkshared = True,
    linkstatic = False,
    deps = ["//modules/drivers/lidar/leddartech/parser:parser"],
)

cc_library(
    name = "parser",
    srcs = [
        "leddartech_parser.cc",
    ],
    hdrs = [
        "leddartech_parser.h",
    ],
    copts = ['-DMODULE_NAME=\\"leddartech\\"'],
    deps = [
        "//cyber",
        "//modules/drivers/proto:pointcloud_cc_proto",
        "//modules/drivers/lidar/leddartech/proto:config_cc_proto",
    ],
)

cpplint()