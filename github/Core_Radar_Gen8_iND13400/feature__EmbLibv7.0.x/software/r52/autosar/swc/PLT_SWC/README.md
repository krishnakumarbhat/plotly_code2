# Building Blocks Workspace Fetcher

This directory contains a script to download and extract required repositories, as well as clean them up when needed.

## How to Download Repositories

1. Ensure your `.netrc` file is set up with credentials for the required hosts.
2. Edit `requested_repos.txt` to list the repositories you want to fetch (one per line).

   - To fetch by name from `bb.bzl`, add the repo name:
     ```
     repo1
     repo2
     ```
   - To override the URL for a repo, use the format:
     ```
     repo3 = https://custom.url/repo3.zip
     ```

3. Run the following command from this directory:

   ```sh
   python fetch_from_workspace.py
   ```

   This will download and extract all repositories listed in `requested_repos.txt` into the `ext` folder.

## How to Clean (Delete Extracted Repositories)

To remove all extracted repositories listed in `requested_repos.txt`, run:

```sh
python fetch_from_workspace.py --cleanup
```

This will delete the corresponding folders from `ext`.

## Notes

- The script uses authentication from your `.netrc` file for private repositories.
- Make sure `bb.bzl` and `requested_repos.txt` are correctly configured.
- You can specify a custom URL for any repo in `requested_repos.txt` using the `repo_name = URL` format.
