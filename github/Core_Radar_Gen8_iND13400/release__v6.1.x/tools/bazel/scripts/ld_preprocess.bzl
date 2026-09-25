"""Definition of the ld_preprocess rule, which is used to preprocess .ld files"""

def _ld_preprocess_impl(ctx):
    src_file = ctx.file.src
    extension = src_file.extension
    base_name = src_file.basename[:-len(extension) - 1]  # Removes dot from extension as well
    new_name = base_name + "_preprocessed." + extension

    out_file = ctx.actions.declare_file(new_name)

    include_paths = []
    dep_lds = []

    for dep in ctx.attr.deps:
        ld_files = dep[DefaultInfo].files.to_list()
        dep_lds += ld_files
        for file in ld_files:
            include_paths.append(file.dirname)

    args = ctx.actions.args()
    args.add("-E", "-P")
    args.add("-x", "c")
    args.add_all(ctx.attr.args)
    args.add_all(ctx.attr.local_defines, before_each = "-D")
    args.add_all(include_paths, before_each = "-I", uniquify = True)
    args.add(src_file.path)
    args.add("-o", out_file)

    ctx.actions.run(
        inputs = [src_file] + dep_lds,
        outputs = [out_file],
        arguments = [args],
        executable = ctx.executable.preprocess_compiler_executable,
    )

    return [DefaultInfo(files = depset([out_file]))]

ld_preprocess = rule(
    implementation = _ld_preprocess_impl,
    attrs = {
        "src": attr.label(
            allow_single_file = True,
            doc = "Linker file that should be preprocessed for the compilers that don't provide this functionality out of the box.",
        ),
        "deps": attr.label_list(
            allow_files = [".ld"],
            doc = "Dependencies required for preprocess linker file.",
        ),
        "args": attr.string_list(
            doc = "Additional flags to pass to preprocessor.",
        ),
        "local_defines": attr.string_list(
            doc = "List of defines to add to the preprocessor command. Each string will be passed with -D prefix.",
        ),
        "preprocess_compiler_executable": attr.label(
            executable = True,
            allow_single_file = True,
            cfg = "exec",
            doc = "Any toolchain that provides preprocessing functionality.",
            default = Label("@cygwin_windows//:cmd/clang.exe"),
        ),
    },
)
