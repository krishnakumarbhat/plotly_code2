from types import SimpleNamespace

import pytest

import generate_upload


class _FakeSftp:
    def __init__(self, files=None, fail_put=False):
        self.files = dict(files or {})
        self.fail_put = fail_put
        self.modes = {}

    def remove(self, path):
        if path not in self.files:
            raise FileNotFoundError(path)
        del self.files[path]

    class _Writer:
        def __init__(self, owner, path):
            self.owner = owner
            self.path = path

        def __enter__(self):
            self.owner.files.setdefault(self.path, b'')
            return self

        def __exit__(self, *_args):
            return False

        def set_pipelined(self, _enabled):
            return None

        def write(self, data):
            if self.owner.fail_put:
                self.owner.files[self.path] += data[:max(1, len(data) // 2)]
                raise OSError('simulated interrupted transfer')
            self.owner.files[self.path] += data

    def open(self, remote_path, mode):
        assert mode == 'ab'
        return self._Writer(self, remote_path)

    def chmod(self, path, mode):
        self.modes[path] = mode

    def stat(self, path):
        if path not in self.files:
            raise FileNotFoundError(path)
        return SimpleNamespace(st_size=len(self.files[path]))

    def posix_rename(self, source, destination):
        self.files[destination] = self.files.pop(source)


def test_atomic_upload_promotes_only_after_complete_transfer(tmp_path):
    local = tmp_path / 'main_html.simg'
    local.write_bytes(b'new-complete-image')
    remote = '/cluster/main_html.simg'
    sftp = _FakeSftp({remote: b'old-working-image'})

    result = generate_upload._atomic_sftp_put(sftp, local, remote)

    assert result.st_size == len(b'new-complete-image')
    assert sftp.files[remote] == b'new-complete-image'
    assert not any('.uploading.' in path for path in sftp.files)


def test_interrupted_upload_preserves_existing_live_file(tmp_path):
    local = tmp_path / 'main_html.simg'
    local.write_bytes(b'new-complete-image')
    remote = '/cluster/main_html.simg'
    sftp = _FakeSftp({remote: b'old-working-image'}, fail_put=True)

    with pytest.raises(OSError, match='interrupted transfer'):
        generate_upload._atomic_sftp_put(sftp, local, remote)

    assert sftp.files[remote] == b'old-working-image'
    assert not any('.uploading.' in path for path in sftp.files)


def test_resumable_upload_keeps_partial_stage_and_continues(tmp_path):
    local = tmp_path / 'main_html.simg'
    local.write_bytes(b'new-complete-image')
    remote = '/cluster/main_html.simg'
    stage = '/cluster/.main_html.simg.uploading'
    sftp = _FakeSftp({remote: b'old-working-image', stage: b'new-'})

    result = generate_upload._atomic_sftp_put(
        sftp,
        local,
        remote,
        preserve_partial=True,
    )

    assert result.st_size == len(b'new-complete-image')
    assert sftp.files[remote] == b'new-complete-image'
    assert stage not in sftp.files