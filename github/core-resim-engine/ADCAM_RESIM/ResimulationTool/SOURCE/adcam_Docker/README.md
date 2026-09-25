# APT_SRR_RESIM Singularity image

This directory now includes a prebuilt Singularity image at:

- `build/ifv600-resim.simg`

Use the helper script to run it on the host:

```bash
./run_singularity.sh <flist.txt> <output_path>
```

The script binds the MF4 root, the flist path, and the output directory into the container and then launches `ifv600-resim.simg`.

You can also run the rebuilt image directly:

```bash
singularity run build/ifv600-resim.simg <flist.txt> <output_path>
```

Notes:

- `flist.txt or flist.json` must contain absolute Linux paths to MF4 files.
- `output_path` can be absolute or relative; it will be created if needed.
- The Dockerfile remains available if you need to rebuild the OCI image path later.