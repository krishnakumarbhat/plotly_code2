# A-PRISM — All Changes (3 repos)

Date: 2026-10-11. Working tree only — nothing committed, nothing pushed.
Plain-English presentation: `Desktop/final.html`.

## 2026-10-12 — OT ported into DC emb-library, OT repo relocated (update)

- All 10 OT items were ported from `gen_research/core-radar-object-tracker`
  into `gen_research/core-resim-dc-emb-library/Application/F360Tracker/Fusion360`
  (9 adapted source edits + `Fusion360/include/aprism_ot_hooks.h`).
  NOT portable 1:1: the log-`Clamp01` defense — this snapshot has no uint8
  mirror pack (float logs only), and the Track_Validity resolve covers it.
- `F360-Tracker` library builds clean with MSVC in **both** OFF (`build_off`)
  and ON (`build_on`, `/DAPRISM_OT_ENABLE=1`) configs — `f360_variant_A.lib`.
- Module-level before/after on the real libs (`dc_mod_off.exe` vs `dc_mod_on.exe`):
  T1 R-fill default 0.5/0.5 both (identical); T2 +spray: OFF 0.5 vs ON 3.6945
  (=0.5·e²); T3 ghost update: OFF mirror 0.0 vs ON −1.0 (assimilated);
  T4 behind-rail: OFF mirror 1.0 (penalized) vs ON −1.0 (protected).
- MF4 verdict: the 3 `CCA_9010...` logs hold raw CAN/ETH frames only
  (BUS 5ch, DEBUG 3ch, REF 2ch) — no decodable radar signals, so dummy data
  was used per the fallback plan.
- `core-radar-object-tracker` was reverted to clean (`git checkout`, backup at
  `Desktop/APRISM_OT_backup.patch` + `APRISM_OT_hooks_backup.h`) and relocated to
  `Desktop/core-radar-object-tracker` (1165 files, clean, same HEAD).
  An empty locked shell dir may remain at the old path until reboot.
- Repo 1 paths below now refer to the Desktop copy (clean, no changes).

---

## Repo 1 — `gen_research/core-radar-object-tracker` (branch `dev`, GitHub)

10 files modified + 1 new header. All new logic behind `APRISM_OT_ENABLE` (default OFF = bit-identical).

| File | Change |
|---|---|
| `emb_tracker/ROT/Fusion360/include/aprism_ot_hooks.h` | **NEW.** All OT helpers + sidecars: ghost R-inflation, adaptive gamma, rank gate, 2-scan confirm, Hermite rail fit, curvature bound, Doppler gate, XOR sentinel (`-1`) + resolve, det sidecar, WSE feed |
| `.../measurement_update_tracks/source/f360_msmt_update_obj_trks_cca_moveable.cpp` | OT-1: append one ghost range-rate row (paired + double-bounce + Doppler-consistent); `APRISM_OT_COUNT` keeps OFF identical |
| `.../measurement_update_tracks/source/f360_msmt_update_obj_trks_ctca.cpp` | OT-1/OT-5: shared-reflector R-inflation on paired ghost rows; marks XOR sidecar |
| `.../measurement_update_tracks/source/f360_msmt_update_support_functions_ctca.cpp` | OT-5: R scaling via WSE sidecar feed (default 1.00×) |
| `.../detection_to_track_association/source/f360_determine_association_hypothesis.cpp` | OT-3 accel-widened gate (clamped), N24 rank gate, OT-6 two-scan ghost confirm |
| `.../track_validity/source/f360_detect_multipath.cpp` | XOR: skip mirror penalty for tracks that assimilated a ghost this scan |
| `.../track_validity/source/f360_determine_reflected_obj.cpp` | XOR: same skip for behind-guardrail max-prob assignment |
| `.../track_validity/source/f360_is_host_reflected_track.cpp` | XOR: same skip (3 assignment sites) |
| `.../track_validity/source/f360_is_reflective_guardrail_track.cpp` | XOR skip + OT-4 Hermite curved-rail hypothesis + OT-2 rank-1 pair reject |
| `.../track_validity/source/f360_track_validity.cpp` | **This session:** scan close-out — resolve sentinel `-1→0`, reset det sidecar |
| `.../logging/source/f360_populate_objects_log.cpp` | **This session:** `Clamp01` defense so `-1` can never log as uint8 157 |

---

## Repo 2 — `gen_research/core-resim-dc-emb-library` (branch `main`, GitHub)

2 files modified + 1 new header. All new logic behind `APRISM_DC_ENABLE` (default OFF).

| File | Change |
|---|---|
| `DC_SIL/sil_wrappers/aprism_dc_hooks.h` | **NEW.** Preflight seed validate/store, `Seed_From_Header` (QF gate), rigid deskew, max-dt recorder, clock/bias solver + tracker |
| `DC_SIL/sil_wrappers/Tracker_VariantA_Wrapper.cpp` | DC-1: live DA seed from `dets_info.AutoAlign*/QF` (latest wins, applied in rad); DC-2: per-sensor epochs + real-ego deskew sidecar; DC-3: clock/bias proxy update |
| `DC_SIL/sil_wrappers/sil_wrapper.cpp` | DC-1: static Run-17 fallback seed, once-only, never overwrites a header seed |

---

## Repo 3 — `gen_research/Core_Radar_Gen8_iND13400` (branch `release/v7.0.x`, GitHub)

Prior-session Gen8 firmware work (staged + unstaged, uncommitted). Untouched this session.

Staged (new files): `APRISM_LANDING.md`, `aprism_host_tests/` (harness + 5 tests),
`dyn_alignment/ghost_calib.c/.h`, `emb_tracker/ghost_assoc_enabler.c/.h`,
`rdd_proc/imp/src/appl_adaptive_cfar.c/.h`, `rdd_proc/test/appl_adaptive_cfar_unit_test.cc`,
`src/cdc_priority.c/.h`, `src/residual_track_ifc.c/.h`.
Modified (unstaged): `.gitignore`, `dyn_alignment/{BUILD,DA.md,dyn_align_wrapper.c/.h}`,
`emb_tracker/BUILD`, `rdd_proc/{imp/src/{BUILD,appl_cfar_ifc.c},test/BUILD}`, `src/{BUILD,cdc_packing.c}`.
Untracked: `.env`, `APRISM_GEN8_WORKORDER.md`, `APRISM_OT_HANDOFF.md`, `CHANGES.md` (repo-local), `aprism_host_tests/test_ab_compare.c`.

---

## Verification (this session)

- Real headers compiled standalone OFF and ON with MSVC `cl /std:c++17` — clean.
- `Temp/opencode/aprism_ab_off.exe` and `aprism_ab_on.exe`: **40/40 ALL PASS** each.
- Guard audit: every added `Aprism_/`APRISM_`` line is under `APRISM_*_ENABLE` or an OFF-identical macro.
- Full Bazel/CMake builds + qual suites still need the team build farm (see `final.html` §6).

## View full diffs

```powershell
git -C "C:\Users\ouymc2\Desktop\gen_research\core-radar-object-tracker" diff
git -C "C:\Users\ouymc2\Desktop\gen_research\core-resim-dc-emb-library" diff
git -C "C:\Users\ouymc2\Desktop\gen_research\Core_Radar_Gen8_iND13400" diff
```
