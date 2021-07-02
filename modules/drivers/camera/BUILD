load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")
load("//tools:cpplint.bzl", "cpplint")

package(default_visibility = ["//visibility:public"])
CAMERA_COPTS = ['-DMODULE_NAME=\\"camera\\"']

# cc_binary(
#     name = "libcamera_component.so",
#     linkopts = [
#         "-latomic",
#     ],
#     linkshared = True,
#     linkstatic = False,
#     deps = [
#         ":camera_component_lib",
#         ":compress_component_lib",
#         "@ffmpeg//:avcodec",
#         "@ffmpeg//:avformat",
#         "@ffmpeg//:swscale",
#     ],
# )

# cc_library(
#     name = "camera_component_lib",
#     srcs = ["camera_component.cc"],
#     hdrs = ["camera_component.h"],
#     copts = CAMERA_COPTS,
#     deps = [
#         ":camera",
#         "//cyber",
#         "//modules/common/proto:error_code_cc_proto",
#         "//modules/common/proto:header_cc_proto",
#         "//modules/drivers/proto:sensor_image_cc_proto",
#     ],
# )

cc_binary(
    name = "libsyn_camera_component.so",
    linkopts = [
        "-latomic",
    ],
    linkshared = True,
    linkstatic = False,
    deps = [
        ":syn_camera_component_lib",
        ":syn_compress_component_lib",
        "@boost",
    ],
)

# TODO(xiaochen-at-salford)
cc_library(
    name = "syn_camera_component_lib",
    srcs = ["syn_camera_component.cc"],
    hdrs = ["syn_camera_component.h"],
    copts = CAMERA_COPTS,
    deps = [
        ":gige_camera",
        "//cyber",
        "//modules/common/proto:error_code_cc_proto",
        "//modules/common/proto:header_cc_proto",
        "//modules/drivers/proto:sensor_image_cc_proto",
    ],
)

cc_library(
    name = "compress_component_lib",
    srcs = ["compress_component.cc"],
    hdrs = ["compress_component.h"],
    copts = CAMERA_COPTS,
    deps = [
        "//cyber",
        "//modules/common/proto:error_code_cc_proto",
        "//modules/common/proto:header_cc_proto",
        "//modules/drivers/camera/proto:config_cc_proto",
        "//modules/drivers/proto:sensor_image_cc_proto",
        "@opencv//:core",
        "@opencv//:highgui",
        "@opencv//:imgproc",
        "@opencv//:imgcodecs",
    ],
)

cc_library(
    name = "syn_compress_component_lib",
    srcs = ["syn_compress_component.cc"],
    hdrs = ["syn_compress_component.h"],
    copts = CAMERA_COPTS,
    deps = [
        "//cyber",
        "//modules/common/proto:error_code_cc_proto",
        "//modules/common/proto:header_cc_proto",
        "//modules/drivers/camera/proto:config_cc_proto",
        "//modules/drivers/proto:sensor_image_cc_proto",
        "@opencv//:core",
        "@opencv//:highgui",
        "@opencv//:imgproc",
        "@opencv//:imgcodecs",
    ],
)

# cc_library(
#     name = "camera",
#     srcs = select(
#         {
#             "//tools/platform:x86_mode": [
#                 "usb_cam.cc",
#                 "util.cc",
#             ],
#             "//tools/platform:aarch64_mode": [
#                 "usb_cam.cc",
#             ],
#         },
#         no_match_error = "Please Build with an ARM or Linux x86_64 platform",
#     ),
#     hdrs = select(
#         {
#             "//tools/platform:x86_mode": [
#                 "usb_cam.h",
#                 "util.h",
#             ],
#             "//tools/platform:aarch64_mode": [
#                 "usb_cam.h",
#             ],
#         },
#         no_match_error = "Please Build with an ARM or Linux x86_64 platform",
#     ),
#     copts = select({
#         "//tools/platform:x86_mode": ["-mavx2"],
#         "//tools/platform:aarch64_mode": [""],
#     }),
#     deps = select(
#         {
#             "//tools/platform:x86_mode": [
#                 "//cyber",
#                 "//modules/drivers/camera/proto:config_cc_proto",
#                 "@adv_plat",
#             ],
#             "//tools/platform:aarch64_mode": [
#                 "//cyber",
#                 "//modules/drivers/camera/proto:config_cc_proto",
#             ],
#         },
#         no_match_error = "Please Build with an ARM or Linux x86_64 platform",
#     ),
# )


cc_library(
    name = "gige_camera",
    srcs = [
        "gige_cam.cc",
        "util.cc", 
    ],
    hdrs = [
        "gige_cam.h",
        "util.h",
    ],
    deps = [
        "//modules/drivers/camera/hikro:hikro_lib",
        "//cyber",
        "//modules/drivers/camera/proto:config_cc_proto",
        "@boost",
    ],
    copts = ["-mavx2"],
)

cpplint()
