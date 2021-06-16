load("@rules_cc//cc:defs.bzl", "cc_binary")
load("//tools:cpplint.bzl", "cpplint")

cc_binary(
    name = "test_boost_example",
    srcs = ["test_boost_example.cc"], 
    deps = [
        "@boost",
    ]
)

cc_binary(
    name = "test_boost_ip_utils",
    srcs = ["test_boost_ip_utils.cc"], 
    deps = [
        "@boost",
    ]
)

cc_binary(
    name = "test_find_a_gige_camera",
    srcs = ["test_find_a_gige_camera.cc"], 
    deps = [
        "@boost",
        "//modules/drivers/camera/hikro:hikro_lib", 
    ]
)




cpplint()