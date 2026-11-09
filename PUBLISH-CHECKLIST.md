# Publish checklist — Project Plumb VR open source

**Status: STAGED — all decisions made 2026-11-09 (MIT license, holder
Project Plumb VR, repo `yoohoocoocoollawliet/project-plumb-vr`, publish
guides/docs now). The repo does NOT exist on GitHub yet. It is ready to
push once GitHub OAuth is connected and the empty repo is created.**

## Decisions needed from L (blocking)

- [x] **License:** MIT — chosen by L on 2026-11-09. See `LICENSE-CHOICE.md`.
- [x] **Copyright holder name** — Project Plumb VR, chosen by L on 2026-11-09.
- [x] **GitHub account** — yoohoocoocoollawliet, given by L on 2026-11-09 and verified on GitHub (created 2026-11-01, 0 public repos so far).
- [x] **GitHub repo name** — `project-plumb-vr`, chosen by L on 2026-11-09. Planned repo: yoohoocoocoollawliet/project-plumb-vr — NOT created yet, do not create it until VR code is ready.
- [x] **Publish now or wait?** — PUBLISH THE GUIDES/DOCS NOW, chosen by L on 2026-11-09 (overrides the earlier WAIT decision; L said do both the Discord server and the GitHub repo simultaneously). VR code will follow later — the README states clearly there is no playable build yet.
- [ ] **Repo visibility / community:** public from day one? Any
  collaborators to add immediately (Developer role holders)?

## Pre-publish verification (run every time, right before pushing)

- [ ] `git status` is clean of anything in `.gitignore` — especially
  `*.nsp`, `*.nso`, `*.xci`, `prod.keys`, `RomFS/`, `build/`, `*.apk`.
- [ ] Search the whole tree for secrets before pushing:
  `grep -rniE "prod.keys|BEGIN.*PRIVATE KEY|password|api[_-]?key" .`
  — expect zero hits except the *mentions* in README/docs that say
  these files are NOT included.
- [ ] No Nintendo assets: no models, textures, audio, RomFS extracts,
  no copied OdysseyDecomp source files. OdysseyDecomp stays a
  submodule / dependency, not vendored code.
- [x] LICENSE header has the final holder name (Project Plumb VR) and year (2026); the draft banner is removed from `README.md` and the license note in `LICENSE` confirms MIT.
- [x] `docs/` scope fixed — the 2026-11-09 snapshots no longer present the demo scope as a "conflict to confirm". The confirmed scope is BOTH parts together (opening → Cappy, plus wedding → Cap Kingdom → Topper); see `docs/NOTE.md`.
- [ ] README "Current status" section is still true on publish day.
  If the toolchain / NSO / first code has landed, update it — don't
  publish a stale status.
- [ ] Repo description set to the tagline: "Bringing the Odyssey into
  another dimension." Link the Discord server and the research page.

## Publish steps (only after the above)

1. Staging is already prepared in a clean directory
   (`repo-staging/project-plumb-vr/`), git-initialised, with the
   initial commit made — do NOT `git init` inside
   `~/workspace/project-plumb-vr/` itself, which contains the decomp
   snapshots, downloads scratch, and the NSP transfer working files.
2. Create the empty repo `yoohoocoocoollawliet/project-plumb-vr` on
   GitHub (requires GitHub OAuth to be connected first — the GitHub
   connector was not connected at staging time).
3. Add OdysseyDecomp as a submodule pointing at
   `https://github.com/MonsterDruide1/OdysseyDecomp` — do not copy its
   files in.
4. Push the staged commit to the repo, verify on github.com that no
   excluded file appears, then announce it in Discord
   #development-updates.

## What is NOT in this package (deliberately)

- The OdysseyDecomp snapshots (`OdysseyDecomp-master/`, `OdysseyDecomp/`)
  — separate project, stays a dependency
- `prod.keys` (`workspace/user/files/prod.keys`) — never publish
- Any NSP / NSO / RomFS — the 5.2 GB SMO v0 NSP transfer is separate
  and is never published
- `downloads/` (SwissTransfer helper scripts / test files) — scratch
- The research-page artifact (`project-plumb-vr-research`) — a web page,
  not repo content; link to it from the README instead if L wants
