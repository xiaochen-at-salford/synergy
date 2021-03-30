load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

CANBUS_COPTS = ["-DMODULE_NAME=\\\"canbus\\\""]

cc_library(
    name = "canbus_niro_protocol",
    deps = [
        ":oscc_protocol_base",
        ":0d112_0x70_brake_enable",
        ":0d113_0x71_brake_disable",
        ":0d114_0x72_brake_command",
        ":0d115_0x73_brake_report",
        ":0d128_0x80_steering_enable",
        ":0d129_0x81_steering_disable",
        ":0d130_0x82_steering_torque_command",
        ":0d131_0x83_steering_report",
        ":0d184_0xb8_steering_angle_command",
        ":0d185_0xb9_steering_angle_report",
        ":0d144_0x90_throttle_enable",
        ":0d145_0x91_throttle_disable",
        ":0d146_0x92_throttle_command",
        ":0d147_0x93_throttle_report",
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
    name = "0d112_0x70_brake_enable",
    srcs = ["0d112_0x70_brake_enable.cc"],
    hdrs = ["0d112_0x70_brake_enable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d113_0x71_brake_disable",
    srcs = ["0d113_0x71_brake_disable.cc"],
    hdrs = ["0d113_0x71_brake_disable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d114_0x72_brake_command",
    srcs = ["0d114_0x72_brake_command.cc"],
    hdrs = ["0d114_0x72_brake_command.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d115_0x73_brake_report",
    srcs = ["0d115_0x73_brake_report.cc"],
    hdrs = ["0d115_0x73_brake_report.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d128_0x80_steering_enable",
    srcs = ["0d128_0x80_steering_enable.cc"],
    hdrs = ["0d128_0x80_steering_enable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d129_0x81_steering_disable",
    srcs = ["0d129_0x81_steering_disable.cc"],
    hdrs = ["0d129_0x81_steering_disable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d130_0x82_steering_torque_command",
    srcs = ["0d130_0x82_steering_torque_command.cc"],
    hdrs = ["0d130_0x82_steering_torque_command.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d131_0x83_steering_report",
    srcs = ["0d131_0x83_steering_report.cc"],
    hdrs = ["0d131_0x83_steering_report.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d184_0xb8_steering_angle_command",
    srcs = ["0d184_0xb8_steering_angle_command.cc"],
    hdrs = ["0d184_0xb8_steering_angle_command.h"], 
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d185_0xb9_steering_angle_report",
    srcs = ["0d185_0xb9_steering_angle_report.cc"],
    hdrs = ["0d185_0xb9_steering_angle_report.h"], 
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d144_0x90_throttle_enable",
    srcs = ["0d144_0x90_throttle_enable.cc"],
    hdrs = ["0d144_0x90_throttle_enable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d145_0x91_throttle_disable",
    srcs = ["0d145_0x91_throttle_disable.cc"],
    hdrs = ["0d145_0x91_throttle_disable.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d146_0x92_throttle_command",
    srcs = ["0d146_0x92_throttle_command.cc"],
    hdrs = ["0d146_0x92_throttle_command.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "0d147_0x93_throttle_report",
    srcs = ["0d147_0x93_throttle_report.cc"],
    hdrs = ["0d147_0x93_throttle_report.h"],
    copts = CANBUS_COPTS,
    deps = [
        ":oscc_protocol_base",
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cpplint()