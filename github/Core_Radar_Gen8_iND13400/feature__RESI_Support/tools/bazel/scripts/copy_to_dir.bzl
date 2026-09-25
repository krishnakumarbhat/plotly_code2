"""
Copy all the generated files from the provided srcs to a folder, specified as a relative path from the calling BUILD file.

Implementation of copy_file macro and underlying rules.
These rules copy a file to another location using Bash (on Linux/macOS) or
cmd.exe (on Windows). This implementation is based on the Bazel-Skylib rule copy_file.
See https://github.com/bazelbuild/bazel-skylib/blob/main/docs/copy_file_doc.md for more details.
"""

def _copy_cmd(ctx, src, dst):
    # Most Windows binaries built with MSVC use a certain argument quoting
    # scheme. Bazel uses that scheme too to quote arguments. However,
    # cmd.exe uses different semantics, so Bazel's quoting is wrong here.
    # To fix that we write the command to a .bat file so no command line
    # quoting or escaping is required.
    if ctx.attr.preserve_directories:
        bat = ctx.actions.declare_file("temp\\" + src.path.removeprefix(ctx.attr.strip_prefix) + "-cmd.bat")
    else:
        bat = ctx.actions.declare_file("temp\\" + src.basename + "-cmd.bat")

    ctx.actions.write(
        output = bat,
        # Do not use lib/shell.bzl's shell.quote() method, because that uses
        # Bash quoting syntax, which is different from cmd.exe's syntax.
        content = "@copy /Y \"%s\" \"%s\" >NUL" % (
            src.path.replace("/", "\\"),
            dst.path.replace("/", "\\"),
        ),
        is_executable = True,
    )
    ctx.actions.run(
        inputs = [src],
        tools = [bat],
        outputs = [dst],
        executable = "cmd.exe",
        arguments = ["/C", bat.path.replace("/", "\\")],
        mnemonic = "CopyFile",
        progress_message = "Copying files",
    )

def _copy_bash(ctx, src, dst):
    ctx.actions.run(
        inputs = [src],
        outputs = [dst],
        arguments = ["-f", src.path, dst.path],
        mnemonic = "CopyFile",
        progress_message = "Copying files",
        executable = ctx.files.copyExecutable[0],
    )

def _copy_file(ctx, fi, dst_file, is_windows):
    if is_windows:
        _copy_cmd(ctx, fi, dst_file)
    else:
        _copy_bash(ctx, fi, dst_file)

def _copy_to_dir_impl(ctx):
    files = []
    runfiles = []

    for fi in ctx.files.srcs:
        if ctx.attr.preserve_directories:
            dst_file = ctx.actions.declare_file(ctx.attr.outputDir + "/" + fi.path.removeprefix(ctx.attr.strip_prefix))
        else:
            dst_file = ctx.actions.declare_file(ctx.attr.outputDir + "/" + fi.basename)

        _copy_file(ctx, fi, dst_file, ctx.attr.is_windows)

        files.append(dst_file)
        runfiles.append(dst_file)

    # Do not include the copied file into the default runfiles of the
    # target, but ensure that it is picked up by native rule's data
    # attribute despite https://github.com/bazelbuild/bazel/issues/15043.
    return [DefaultInfo(files = depset(files), data_runfiles = ctx.runfiles(runfiles))]

_ATTRS = {
    "copyExecutable": attr.label(mandatory = False, allow_files = True),
    "srcs": attr.label_list(mandatory = True, allow_files = True),
    "outputDir": attr.string(mandatory = True),
    "strip_prefix": attr.string(mandatory = True),
    "is_windows": attr.bool(mandatory = True),
    "preserve_directories": attr.bool(mandatory = True),
}

_copy_to_dir = rule(
    implementation = _copy_to_dir_impl,
    provides = [DefaultInfo],
    attrs = _ATTRS,
)

def copy_to_dir(name, srcs, outputDir, preserve_directories = False, strip_prefix = "", **kwargs):
    """Copy files from a given target to another directory.

    This rule uses a Bash command on Linux/macOS/non-Windows, and a cmd.exe command on Windows (no Bash is required).

    Args:
      name: Name of the rule.
      srcs: A Label. The file to make a copy of. (Can also be the label of a rule
          that generates a file.)
      outputDir: Path of the output file, relative to this package.
      preserve_directories: if true then directory structure is preserved for source files.
      strip_prefix: strip prefix from file path
      **kwargs: further keyword arguments, e.g. `visibility`
    """
    copy_to_dir_impl = _copy_to_dir

    copy_to_dir_impl(
        name = name,
        copyExecutable = "@build-tools//:copy",
        srcs = srcs,
        outputDir = outputDir,
        is_windows = select({
            "@bazel_tools//src/conditions:host_windows": True,
            "//conditions:default": False,
        }),
        preserve_directories = preserve_directories,
        strip_prefix = strip_prefix,
        **kwargs
    )
