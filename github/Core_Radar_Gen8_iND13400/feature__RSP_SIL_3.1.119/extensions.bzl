load("@bazel_skylib//lib:sets.bzl", "sets")
load("//tools/bazel/config:coverity.bzl", "register_predefined_coverity")

def _coverity_module_extension_impl(module_ctx):
    root_direct_deps = sets.make()
    root_direct_dev_deps = sets.make()

    for mod in module_ctx.modules:
        is_root = mod.is_root

        for coverity in mod.tags.coverity:
            register_predefined_coverity(
                major_version = coverity.major_version,
                register_toolchains = False,
            )
            if is_root:
                deps = root_direct_dev_deps if module_ctx.is_dev_dependency(coverity) else root_direct_deps
                [
                    sets.insert(deps, cov_repo)
                    for cov_repo in [
                        "coverity_{}_6_0_linux".format(coverity.major_version),
                        "coverity_{}_6_0_windows".format(coverity.major_version),
                        "coverity_toolchain",
                    ]
                ]

    return module_ctx.extension_metadata(
        root_module_direct_deps = sets.to_list(root_direct_deps),
        root_module_direct_dev_deps = sets.to_list(root_direct_dev_deps),
    )

_coverity_tag = tag_class(
    attrs = {
        "major_version": attr.string(values = ["2023", "2024"]),
    },
)

# Sets coverity major version
coverity = module_extension(
    _coverity_module_extension_impl,
    tag_classes = {
        "coverity": _coverity_tag,
    },
)
