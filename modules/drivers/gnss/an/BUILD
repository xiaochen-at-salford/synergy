load("@rules_cc//cc:defs.bzl", "cc_binary","cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "an_gnss_lib",
    srcs = [
        "an_packet_protocol.c",
        "spatial_packets.c",
        "rs232/rs232.c",
        ],
    hdrs = [
        "an_packet_protocol.h",
        "rs232/rs232.h",
        "spatial_packets.h",
    ],
)

cc_binary(
  name = "test_print_imu",
  srcs = [
    "test_print_imu.cc"
  ],
  deps = [
    "//cyber",
    ":an_gnss_lib",
  ]
)

cc_binary(
  name = "test_print_imu_singledecode",
  srcs = [
    "test_print_imu_singledecode.cc"
  ],
  deps = [
    "//cyber",
    ":an_gnss_lib",
  ]
)

cc_binary(
  name = "test_apollo_rs232stream_imu",
  srcs = [
    "test_apollo_rs232stream_imu.cc"
  ],
  deps = [
    ":an_gnss_lib",
    "//modules/drivers/gnss/stream:serial_stream",
    "//modules/drivers/gnss/stream:gnss_stream",
    "//cyber",
  ]
)

cc_binary(
  name = "test_apollo_rs232stream_imu_singledecode",
  srcs = [
    "test_apollo_rs232stream_imu_singledecode.cc"
  ],
  deps = [
    ":an_gnss_lib",
    "//modules/drivers/gnss/stream:serial_stream",
    "//modules/drivers/gnss/stream:gnss_stream",
    "//cyber",
  ]
)



cpplint()