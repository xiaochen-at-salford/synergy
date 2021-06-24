load("@rules_cc//cc:defs.bzl", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "can_receiver",
    hdrs = ["can_receiver.h"],
    deps = [
        "//modules/common/proto:error_code_cc_proto",
        "//modules/drivers/canbus/can_client",
        "//modules/drivers/canbus/can_comm:message_manager_base",
    ],
)

cc_library(
    name = "can_sender",
    hdrs = ["can_sender.h"],
    deps = [
        "//modules/common/proto:error_code_cc_proto",
        "//modules/drivers/canbus/can_client",
        "//modules/drivers/canbus/can_comm:message_manager_base",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "message_manager_base",
    hdrs = [
        "message_manager.h",
        "protocol_data.h",
    ],
    deps = [
        "//cyber/common:log",
        "//cyber/time",
        "//modules/common/proto:error_code_cc_proto",
        "//modules/drivers/canbus/common:canbus_common",
    ],
)

cpplint()
