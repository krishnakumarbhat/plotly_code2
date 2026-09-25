"""This rule creates spc file."""

def _spc_gen_impl(ctx):
    args = ctx.actions.args()
    outfile = ctx.actions.declare_file(ctx.label.name)
    args.add("--filename={}".format(outfile.path))
    args.add("--position={}".format(ctx.attr.sensor_position))
    args.add("--start_address={}".format(ctx.attr.start_address))
    args.add("--size={}".format(ctx.attr.size))

    ctx.actions.run(
        executable = ctx.executable.spc_gen_script,
        arguments = [args],
        outputs = [outfile],
        inputs = [ctx.executable.spc_gen_script],
        progress_message = "Generating SPC: {}".format(outfile.basename),
    )

    return [DefaultInfo(files = depset([outfile]))]

spc_gen = rule(
    implementation = _spc_gen_impl,
    attrs = {
        "start_address": attr.int(mandatory = True, doc = "Start address of the flash image"),
        "sensor_position": attr.int(mandatory = True, doc = "radar position"),
        "size": attr.int(mandatory = False, default = 0x10000, doc = "size of generated s19 file"),
        "spc_gen_script": attr.label(default = ":spc_gen", mandatory = False, executable = True, cfg = "exec", doc = "Label of the py_binary that contains the spc_gen script"),
    },
)
