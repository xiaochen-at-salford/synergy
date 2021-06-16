load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "hikro_lib",
    srcs = glob(["bin/*"]),
    hdrs = glob(["*.h"]),
    linkopts = [
        "-lpthread", 
        "-ldl",  
        "-lX11",
    ],
    deps = [
        "@boost",
    ],
)

cc_binary(
    name = "test_chunk_data",
    srcs = ["test_chunk_data.cc"],
    deps = [":hikro_lib",],
)

cc_binary(
    name = "test_connect_spec_camera",
    srcs = ["test_connect_spec_camera.cc"],
    deps = [":hikro_lib",],
)

cc_binary(
    name = "test_display",
    srcs = ['test_display.cc'],
    deps = [":hikro_lib"],
    # linkopts = ["-lX11"],
)

cc_binary(
    name = "test_events",
    srcs = ["test_events.cc"],
    deps = [":hikro_lib"],
    # linkopts = ["-lX11"],
)

cc_binary(
    name = "test_force_ip",
    srcs = ["test_force_ip.cc"],
    deps = [":hikro_lib"],
)

cc_binary(
    name = "test_grab_imagecallback",
    srcs = ["test_grab_imagecallback.cc"],
    deps = [":hikro_lib"],
)

cc_binary(
    name = "test_grab_image",
    srcs = ["test_grab_image.cc"],
    deps = [":hikro_lib"],
)

cc_binary(
    name = "test_grab_image_high_performance",
    srcs = ["test_grab_image_high_performance.cc"],
    deps = [":hikro_lib"],
)

cc_binary(
    name = "test_image_process",
    srcs = ["test_image_process.cc"],
    deps = [":hikro_lib"],
)




cpplint()
