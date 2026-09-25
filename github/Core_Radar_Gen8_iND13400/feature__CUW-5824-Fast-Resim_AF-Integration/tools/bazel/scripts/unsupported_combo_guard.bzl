"""Rule used to abort a build during the analysis phase for unsupported flag combinations.

Wiring the resulting target into a top-level filegroup's srcs ensures Bazel evaluates
it before scheduling any compile actions, so a disallowed combination fails fast without
starting the SW compile.
"""

def _unsupported_combo_guard_impl(ctx):
    if ctx.attr.unsupported:
        fail("THIS VARIANT IS NOT SUPPORTED AT THIS TIME. Please contact ITC's Gen8 Radar SW Team")
    return [DefaultInfo()]

unsupported_combo_guard = rule(
    implementation = _unsupported_combo_guard_impl,
    attrs = {
        "unsupported": attr.bool(
            default = False,
            doc = "Set via select() to True for flag combinations that must abort the build.",
        ),
    },
)
