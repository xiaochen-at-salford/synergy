load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "libgnss_component.so",
    linkshared = True,
    linkstatic = False,
    deps = [ 
        ":gnss_component_lib",
    ]
)

cc_library(
    name = "gnss_component_lib",
    srcs = ["gnss_component.cc"],
    hdrs = ["gnss_component.h"],
    copts = ['-DMODULE_NAME=\\"gnss\\"'],
    deps = [
        "//cyber",
        "//modules/canbus/proto:chassis_cc_proto",
        "//modules/common/configs/proto:vehicle_config_cc_proto",
        "//modules/common/monitor_log",
        "//modules/common/proto:drive_state_cc_proto",
        "//modules/common/proto:vehicle_signal_cc_proto",
        "//modules/drivers/gnss/proto:gnss_status_cc_proto",
        "//modules/drivers/gnss/proto:ins_cc_proto",
        "//modules/drivers/gnss/stream:gnss_stream",
    ],
)

cc_binary(
    name = "liban_gnss_component.so",
    linkshared = True,
    linkstatic = False,
    deps = [
        ":an_gnss_component_lib",
        ":an_relay_component_lib",
    ],
)

cc_library(
    name = "an_gnss_component_lib",
    srcs = ["an_gnss_component.cc"],
    hdrs = ["an_gnss_component.h"],
    copts = ['-DMODULE_NAME=\\"an_gnss\\"'],
    deps = [
        "//cyber",
        "//modules/canbus/proto:chassis_cc_proto",
        "//modules/common/configs/proto:vehicle_config_cc_proto",
        "//modules/common/monitor_log",
        "//modules/common/proto:drive_state_cc_proto",
        "//modules/common/proto:vehicle_signal_cc_proto",
        "//modules/drivers/gnss/proto:gnss_status_cc_proto",
        "//modules/drivers/gnss/proto:ins_cc_proto",
        "//modules/drivers/gnss/proto:an_gnss_cc_proto",
        "//modules/drivers/gnss/stream:an_gnss_stream",
    ],
)

cc_library(
    name = 'an_relay_component_lib',
    srcs = ["an_relay_component.cc"],
    hdrs = ["an_relay_component.h"],
    copts = ['-DMODULE_NAME=\\"an_gnss\\"'],
    deps = [
        "//cyber",
        "//modules/drivers/gnss/proto:an_gnss_cc_proto",
        "//modules/localization/proto:imu_cc_proto",
    ]
)

cpplint()
