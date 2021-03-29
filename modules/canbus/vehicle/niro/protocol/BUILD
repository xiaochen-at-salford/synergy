load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

CANBUS_COPTS = ["-DMODULE_NAME=\\\"canbus\\\""]

cc_library(
    name = "canbus_niro_protocol",
    deps = [
        ":oscc_protocol_base",
        ":dec112_hex70_brake_enable",
        ":dec113_hex71_brake_disable",
        ":dec114_hex72_brake_command",
        ":dec115_hex73_brake_report",
        ":dec128_hex80_steering_enable",
        ":dec129_hex81_steering_disable",
        ":dec130_hex82_steering_torque_command",
        ":dec131_hex83_steering_report",
        ":dec184_hexB8_steering_angle_command",
        ":dec185_hexB9_steering_angle_report",
        ":dec144_hex90_throttle_enable",
        ":dec145_hex91_throttle_disable",
        ":dec146_hex92_throttle_command",
        ":dec147_hex93_throttle_report",
    ],
)

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

cc_library(
    name = "dec128_hex80_steering_enable",
    srcs = ["dec128_hex80_steering_enable.cc"],
    hdrs = ["dec128_hex80_steering_enable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec129_hex81_steering_disable",
    srcs = ["dec129_hex81_steering_disable.cc"],
    hdrs = ["dec129_hex81_steering_disable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec130_hex82_steering_torque_command",
    srcs = ["dec130_hex82_steering_torque_command.cc"],
    hdrs = ["dec130_hex82_steering_torque_command.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec131_hex83_steering_report",
    srcs = ["dec131_hex83_steering_report.cc"],
    hdrs = ["dec131_hex83_steering_report.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec184_hexB8_steering_angle_command",
    srcs = ["dec184_hexB8_steering_angle_command.cc"],
    hdrs = ["dec184_hexB8_steering_angle_command.h"], 
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec185_hexB9_steering_angle_report",
    srcs = ["dec185_hexB9_steering_angle_report.cc"],
    hdrs = ["dec185_hexB9_steering_angle_report.h"], 
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec144_hex90_throttle_enable",
    srcs = ["dec144_hex90_throttle_enable.cc"],
    hdrs = ["dec144_hex90_throttle_enable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec145_hex91_throttle_disable",
    srcs = ["dec145_hex91_throttle_disable.cc"],
    hdrs = ["dec145_hex91_throttle_disable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec146_hex92_throttle_command",
    srcs = ["dec146_hex92_throttle_command.cc"],
    hdrs = ["dec146_hex92_throttle_command.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "dec147_hex93_throttle_report",
    srcs = ["dec147_hex93_throttle_report.cc"],
    hdrs = ["dec147_hex93_throttle_report.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)




cpplint()