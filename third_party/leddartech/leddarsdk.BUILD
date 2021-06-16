load("@rules_cc//cc:defs.bzl", "cc_library")

package(
  default_visibility = ["//visibility:public"],
)

cc_library(
  name = "lib",
  srcs = glob([
    "src/Leddar/*.cpp",
    "src/LeddarTech/*.cpp",
  ]),
  hdrs = glob([
    "src/Leddar/*.h",
    "src/LeddarTech/*.h",
    "src/shared/comm/**/*.h",
    "libs/RapidJson/**/*.h",
  ]),
  copts = [
    "-Iexternal/leddarSDK/src/Leddar",
    "-Iexternal/leddarSDK/src/LeddarTech",
    "-Iexternal/leddarSDK/src/shared",
    "-Iexternal/leddarSDK/libs/RapidJson",
  ],
  linkopts = [
    "-pthread",
    "-MD",
    "-MP",
    "-fPIC",
    "-DPIC",
    "-pipe",
    "-O2",
    "-m64",
    "-W",
  ],
  linkstatic=1,
  alwayslink=1,
  visibility = ["//visibility:public"],
)
