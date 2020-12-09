load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "libleddartech_driver_component.so",
    linkshared = True,
    linkstatic = False,
    deps = [":leddartech_driver_component_lib",]
)

cc_library(
    name = "leddartech_driver_component_lib",
    srcs = ["leddartech_driver_component.cc"],
    hdrs = ["leddartech_driver_component.h"],
    copts = [
        '-DMODULE_NAME=\\"leddartech\\"',
        '-Iexternal/leddarSDK/src/Leddar',
        '-Iexternal/leddarSDK/src/LeddarTech',
        '-Iexternal/leddarSDK/src/shared',
        '-Iexternal/leddarSDK/libs/RapidJson',
        ],
    deps = [
        "//cyber",
        "//modules/common/util:message_util",
        "//modules/common/util",
        "//modules/drivers/lidar/leddartech/proto:config_cc_proto",
        "@leddarSDK//:lib",
    ],
)

cpplint()
