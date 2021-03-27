load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "niro_vehicle_factory",
    srcs = [
        "niro_vehicle_factory.cc",
    ],
    hdrs = [
        "niro_vehicle_factory.h",
    ],
    deps = [
        ":niro_controller",
        ":niro_message_manager",
        "//modules/canbus/vehicle:abstract_vehicle_factory",
    ],
)

cc_library(
    name = "niro_message_manager",
    srcs = ["niro_message_manager.cc"],
    hdrs = ["niro_message_manager.h"],
    deps = [
        "//modules/canbus/proto:chassis_detail_cc_proto",
        "//modules/canbus/vehicle/niro/protocol:canbus_niro_protocol",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cc_library(
    name = "niro_controller",
    srcs = ["niro_controller.cc"],
    hdrs = ["niro_controller.h"],
    deps = [
        ":niro_message_manager",
        "//modules/canbus/common:canbus_common",
        "//modules/canbus/proto:canbus_conf_cc_proto",
        "//modules/canbus/proto:chassis_cc_proto",
        "//modules/canbus/proto:vehicle_parameter_cc_proto",
        "//modules/canbus/vehicle:vehicle_controller_base",
        "//modules/canbus/vehicle/niro/protocol:canbus_niro_protocol",
        "//modules/common/proto:error_code_cc_proto",
        "//modules/control/proto:control_cmd_cc_proto",
    ],
)

cpplint()