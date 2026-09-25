"""This script takes in s19 files and reports flash memory usage statistics."""

def _flash_memory_stats_impl(ctx):
    file_depsets = []
    files = []
    for file_label, file_name in ctx.attr.srcs.items():
        for fi in file_label.files.to_list():
            if fi.basename == file_name:
                file_depset = depset(direct = [fi])
                file_depsets.append(file_depset)

                file = file_depset.to_list()
                files = files + file

    args = ctx.actions.args()

    outfile = ctx.actions.declare_file("temp/" + ctx.label.name)

    args.add_all(files)
    args.add("--output")
    args.add(outfile)

    args.add("--flash_size")
    args.add(ctx.attr.flash_size)

    ctx.actions.run(
        executable = ctx.executable.flash_memory_stats_script,
        arguments = [args],
        outputs = [outfile],
        inputs = files,
        progress_message = "Calculating Flash Memory Statistics...",
    )

    return [DefaultInfo(files = depset([
        outfile,
    ]))]

flash_memory_stats = rule(
    implementation = _flash_memory_stats_impl,
    attrs = {
        "flash_memory_stats_script": attr.label(default = ":flash_memory_stats", mandatory = False, executable = True, cfg = "exec", doc = "Label of the py_binary that contains the flash_memory_stats script"),
        "srcs": attr.label_keyed_string_dict(mandatory = True, doc = "List of file targets and file names to include in the statistics"),
        "flash_size": attr.int(default = 4, mandatory = False, doc = "Flash size (MB)"),
    },
)
