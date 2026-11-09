# License choice for Project Plumb VR

**Status: DECIDED — MIT, chosen by L on 2026-11-09. Copyright holder: Project Plumb VR.**

The file `LICENSE` in this package currently contains the MIT License as
the recommended default. This note explains the choice and the
alternatives, so L can pick with eyes open.

## Recommendation: MIT

**Why MIT fits this project:**

- Simplest and most common license for game-port / mod scaffolding work.
  Everyone understands it; no one has to ask a lawyer.
- Permissive: anyone can use, modify, and distribute the code, including
  in commercial / closed projects, as long as they keep the copyright
  notice. That lowers the barrier for contributors, testers, and anyone
  who wants to build on the VR host / renderer / input code.
- Compatible with the decomp ecosystem around it (smo-recomp, recomp
  tooling, and most Quest homebrew are MIT / permissive).
- Short — one page, easy to include in an APK's about screen.
- No copyleft obligations that would clash with linking proprietary
  libraries (Android NDK, OpenXR loader, Quest system libraries).

**Downside:** someone could take Plumb's VR code, improve it, and ship
it closed-source without giving anything back. MIT allows that. If that
bothers L, see GPLv3 below.

## Alternative 1: GPLv3

- **What it means:** copyleft. Anyone who distributes a modified version
  must release their source under GPLv3 too. Derivatives stay open.
- **Pros:** strongest guarantee the project (and anything built from it)
  stays open forever; no one can privatise L's work.
- **Cons:** scares off some contributors and downstream projects; it is
  harder to combine with proprietary SDKs / store distribution, and it
  puts a compliance burden on anyone sideloading or shipping builds.
  The Quest / Android ecosystem is mostly permissive, so GPLv3 would be
  swimming upstream. Also, GPLv3 cannot magically license Nintendo's
  code or OdysseyDecomp's unlicensed code — its scope would still be
  Plumb's original work only.

**Pick GPLv3 if:** keeping everything derived from Plumb open matters
more to L than maximising contributors and downstream use.

## Alternative 2: Apache License 2.0

- **What it means:** like MIT, permissive — use, modify, distribute,
  including commercially — plus an explicit patent grant from
  contributors, and a requirement to note changes.
- **Pros:** same openness as MIT, with better patent protection; a
  standard, well-understood license.
- **Cons:** longer and more formal than MIT; the patent clause is
  mostly irrelevant for a fan game port, so it adds words for little
  practical benefit here.

**Pick Apache 2.0 if:** L wants MIT-style permissiveness but with a
patent grant for peace of mind.

## What the license does NOT do

Whichever license L picks, it covers **only Project Plumb VR's original
code and docs** — the VR host, OpenXR renderer, input/camera code, build
scripts, and documentation written for this project.

It does **not** license:
- Nintendo's game, code, assets, characters, or trademarks
- OdysseyDecomp's code (separate project, no LICENSE file at the time
  of writing — do not copy its files into this repo)
- Any `.nsp` / `.nso` / RomFS dump, `prod.keys`, or other game files
  (never included, never licensed)

## Decision — made 2026-11-09

1. **MIT** — chosen by L.
2. Copyright holder: **Project Plumb VR**.
3. Copyright year: **2026**.

The final MIT text is in `LICENSE`. This file is kept as a record of
the alternatives considered.
