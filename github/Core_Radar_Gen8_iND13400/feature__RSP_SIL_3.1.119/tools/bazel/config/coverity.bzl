load("@bazel_platform//toolchains/coverity:register.bzl", "register_coverity")
load("@bazel_platform//utils/repository_rules/http_archive_licence_override:http_archive_licence_override.bzl", "http_archive_licence_override")

# Common flags for all Coverity analysis.

CODING_STANDARD_CONFIG = "//tools/coverity/coding-standards/misrac2012:misrac2012-all.config"

CODING_STANDARD_PARAMS = [
    "--hfa",
    "--enable-default",
    "--enable-callgraph-metrics",
    "--enable-fnptr",
    "--enable-virtual",
    "--enable-constraint-fpp",
    "--enable-parse-warnings",
    "--allow-unmerged-emits",
    "--security",
    "--concurrency",
    "--tu-pattern",
    "file('.c$|.i$')",
    "--output-tag",
    "_combined",
    "--checker-option=DEADCODE:no_dead_default:true",
    "--checker-option=RESOURCE_LEAK:allow_main:true",
    "--checker-option=UNUSED_VALUE:report_unused_initializer:true",
]

COV_ANALYZE_PARAMS = [
    "--disable-default",
    "--hfa",
]

"""Utility macro to register predefined coverity toolchain."""

def register_predefined_coverity(major_version, register_toolchains = True):
    """Register Coverity toolchain with version predefined in bazel_platform.

    Args:
        major_version: String with major version of Coverity, minor version is
            dependent on bazel_platform. Currently version 2023 and 2024 is supported.
    """
    supported_versions = ["2023", "2024"]

    if major_version not in supported_versions:
        fail("register_default_coverity currently supports only: {} major versions, got:".format(", ".join(supported_versions)), major_version)

    licence_sha256 = "20daac4abedb1b71cb033e1d2a29fca4ce6fb490e5af15bdd7a7182d626e7841"
    licence_url = "https://jfrog.asux.aptiv.com/artifactory/bazel_tools_internal-generic-local/coverity/licences/license_06_07_2026.dat"
    licence_output = "bin/license.dat"

    if ("2023" == major_version):
        http_archive_licence_override(
            # Package
            name = "coverity_2023_6_0_linux",
            build_file = "@bazel_platform//quality/sca/coverity:archive.BUILD",
            sha256 = "b2403d468db53b18cb936c356fc7c3c74287d1e98ea1489f565a129f7681e56c",
            url = "https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-common_3rd_party-local/Synopsys/Coverity/2023.6.0/cov-analysis-linux64-2023.6.0-patch1-minimal.tar.zst",
            # Licence
            licence_sha256 = licence_sha256,
            licence_url = licence_url,
            licence_output = licence_output,
        )

        http_archive_licence_override(
            # Package
            name = "coverity_2023_6_0_windows",
            build_file = "@bazel_platform//quality/sca/coverity:archive.BUILD",
            sha256 = "89fd45bb76c9a8eac80be0130d86da12894f2bb32885d3bdc37dc75a64c80714",
            url = "https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-common_3rd_party-local/Synopsys/Coverity/2023.6.0/cov-analysis-win64-2023.6.0-patch1-minimal-with-clang.tar.zst",
            # Licence
            licence_sha256 = licence_sha256,
            licence_url = licence_url,
            licence_output = licence_output,
        )

    elif ("2024" == major_version):
        http_archive_licence_override(
            name = "coverity_2024_6_0_linux",
            build_file = "@bazel_platform//quality/sca/coverity:archive.BUILD",
            sha256 = "d7d1f6d2f0397c224909b88212c8305b419bcf80a65a4362c2a860b75af98f83",
            url = "https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-common_3rd_party-local/Synopsys/Coverity/2024.6.0/cov-analysis-linux64-2024.6.0-patch1-minimal.tar.xz",
            # Licence
            licence_sha256 = licence_sha256,
            licence_url = licence_url,
            licence_output = licence_output,
        )

        http_archive_licence_override(
            name = "coverity_2024_6_0_windows",
            build_file = "@bazel_platform//quality/sca/coverity:archive.BUILD",
            sha256 = "93940b2059615aa289f12ab98380e06e9d6d158ad2e38004fed5df1068218975",
            url = "https://jfrog.asux.aptiv.com/artifactory/core_radar-aptiv-00000000-common_3rd_party-local/Synopsys/Coverity/2024.6.0/cov-analysis-win64-2024.6.0-patch1-minimal.tar.xz",
            # Licence
            licence_sha256 = licence_sha256,
            licence_url = licence_url,
            licence_output = licence_output,
        )

    else:
        fail("No configuration exists for this version: {}".format(major_version))

    cov_version = "{}.6.0".format(major_version)
    cov_linux_repo = "@coverity_{}_6_0_linux".format(major_version)
    cov_win_repo = "@coverity_{}_6_0_windows".format(major_version)

    register_coverity(
        version = cov_version,
        linux_repo = cov_linux_repo,
        windows_repo = cov_win_repo,
        register_toolchains = register_toolchains,
    )
