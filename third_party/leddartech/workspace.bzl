load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def clean_dep(dep):
    return str(Label(dep))

def repo():
    http_archive(
        name = "leddarSDK",
        build_file = clean_dep("//third_party/leddartech:leddarsdk.BUILD"),
        sha256 = "1c839d02606930300fdea915a3df52aac488648d3be19ca24117a469de170d79",
        strip_prefix = "LeddarSDK-master",
        urls = [
            "https://github.com/mtetreault/LeddarSDK/archive/master.tar.gz",
        ],
    )