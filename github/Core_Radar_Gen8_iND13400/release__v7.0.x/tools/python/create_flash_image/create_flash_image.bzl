"""This rule takes in s19 files, merges them, and creates a flattened image with a copy table."""

def _create_flash_image_impl(ctx):
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

    outfile = ctx.actions.declare_file(ctx.label.name)

    args.add_all(files)
    args.add("--output")
    args.add(outfile)
    args.add("--pattern")
    args.add(ctx.attr.pattern)

    args.add("--entry-address")
    args.add(ctx.attr.entry_address)

    args.add("--start-address")
    args.add(ctx.attr.start_address)

    if ctx.attr.little_endian:
        args.add("--little-endian")
    else:
        args.add("--big-endian")

    ctx.actions.run(
        executable = ctx.executable.create_flash_image_script,
        arguments = [args],
        outputs = [outfile],
        inputs = files,
        progress_message = "Generating Flash Image...",
    )

    return [DefaultInfo(files = depset([
        outfile,
    ]))]

create_flash_image = rule(
    implementation = _create_flash_image_impl,
    attrs = {
        "create_flash_image_script": attr.label(default = ":create_flash_image", mandatory = False, executable = True, cfg = "exec", doc = "Label of the py_binary that contains the create_flash_image script"),
        "srcs": attr.label_keyed_string_dict(mandatory = True, doc = "List of file targets and file names to include in the output image"),
        "pattern": attr.string(mandatory = True, doc = "Pattern to be used for presence pattern"),
        "entry_address": attr.int(mandatory = False, doc = "Address of entry point or vector table"),
        "start_address": attr.int(mandatory = False, doc = "Start address of the flash image"),
        "little_endian": attr.bool(default = True, mandatory = False, doc = "Target is little endian"),
    },
)
