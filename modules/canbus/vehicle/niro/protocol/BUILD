load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

CANBUS_COPTS = ["-DMODULE_NAME=\\\"canbus\\\""]

# cc_library(
#     name = "canbus_niro_protocol",
#     deps = [
#     ],
# )

cc_library(
    name = "oscc_protocol_base",
    srcs = ["oscc_protocol_base.cc"],
    hdrs = ["oscc_protocol_base.h"],
    copts = CANBUS_COPTS,
    deps = [
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
        
)

cc_library(
    name = "dec112_hex70_brake_enable",
    srcs = ["dec112_hex70_brake_enable.cc"],
    hdrs = ["dec112_hex70_brake_enable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
        
)

cc_library(
    name = "dec113_hex71_brake_disable",
    srcs = ["dec113_hex71_brake_disable.cc"],
    hdrs = ["dec113_hex71_brake_disable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec114_hex72_brake_command",
    srcs = ["dec114_hex72_brake_command.cc"],
    hdrs = ["dec114_hex72_brake_command.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec115_hex73_brake_report",
    srcs = ["dec115_hex73_brake_report.cc"],
    hdrs = ["dec115_hex73_brake_report.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)



cpplint()