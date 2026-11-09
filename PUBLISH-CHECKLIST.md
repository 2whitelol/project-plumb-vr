# Publish checklist, Project Plumb VR

**Status: live, with a rewrite in staging.** The repo exists now. It went up 2026-11-09 as `yoohoocoocoollawliet/project-plumb-vr`. The guides and docs went in first, commit 68e6f32, then the logo went in, commit c3e4e66, "Add Project Plumb VR logo". This checklist is for the rewrite that's sitting in staging right now. Check it, then push it the same way.

## Decisions, all made 2026-11-09

- [x] **License:** MIT, picked by L. See `LICENSE-CHOICE.md`.
- [x] **Copyright holder:** Project Plumb VR.
- [x] **GitHub account:** yoohoocoocoollawliet. Verified on GitHub, created 2026-11-01, 0 public repos at the time.
- [x] **Repo name:** `project-plumb-vr`. Live at yoohoocoocoollawliet/project-plumb-vr.
- [x] **Publish now or wait?** Publish the guides and docs now. That replaced the earlier wait decision. White said do the Discord server and the GitHub repo at the same time. VR code comes later. The README says plainly there's no playable build yet.
- [ ] **Repo visibility and community:** public from day one, that's done. Any collaborators to add right away? People in the Developer role, if any?

## Check before every push

- [ ] `git status` is clean of anything in `.gitignore`. Especially `*.nsp`, `*.nso`, `*.xci`, `prod.keys`, `RomFS/`, `build/`, `*.apk`.
- [ ] Search the whole tree for secrets before pushing:
  `grep -rniE "prod.keys|BEGIN.*PRIVATE KEY|password|api[_-]?key" .`
  You should get zero hits, except the mentions in the README and docs that say these files are NOT included.
- [ ] No Nintendo assets. No models, textures, audio, RomFS extracts, no copied OdysseyDecomp source files. OdysseyDecomp stays a submodule or a dependency, not code pasted in here.
- [x] LICENSE has the final holder name (Project Plumb VR) and year (2026).
- [x] `docs/` scope is fixed. The 2026-11-09 snapshots don't present the demo scope as a conflict anymore. It's both parts together, opening to Cappy, plus wedding to Cap Kingdom to Topper. See `docs/NOTE.md`.
- [ ] README "Where things stand" is still true on push day. If the toolchain, the NSO, or the first code has moved on, update it. Don't push a stale status.
- [ ] Repo description is the tagline: "Bringing the Odyssey into another dimension." Link the Discord server too.

## How to push

1. The staging copy is a clean directory (`repo-staging/project-plumb-vr/`), git set up, initial commit made. Don't run `git init` inside `~/workspace/project-plumb-vr/` itself. That folder has the decomp snapshots, the downloads scratch, and the NSP files.
2. OdysseyDecomp goes in as a submodule pointing at `https://github.com/MonsterDruide1/OdysseyDecomp`. Don't copy its files in.
3. Push to the repo, check on github.com that no excluded file showed up, then say something in Discord #development-updates.

## What's NOT in this package, on purpose

- The OdysseyDecomp snapshots (`OdysseyDecomp-master/`, `OdysseyDecomp/`). Separate project, stays a dependency.
- `prod.keys` (`workspace/user/files/prod.keys`). Never publish it.
- Any NSP, NSO, or RomFS. The 5.2 GB SMO v0 NSP is separate and never gets published. It's done downloading now, 5,611,009,601 bytes, and NSO extraction has started, but that stays local.
- `downloads/` (SwissTransfer helper scripts and test files). Scratch.
- The research page artifact (`project-plumb-vr-research`). It's a web page, not repo content. Link to it from the README if I want it there.
