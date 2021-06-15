load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "oscc_can_client",
    srcs = ["oscc_can_client.cc"],
    hdrs = ["oscc_can_client.h"],
    deps = [
        "//modules/common/proto:error_code_cc_proto",
        "//modules/drivers/canbus/can_client",
    ],
)

cpplint()
