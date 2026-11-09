# Why MIT

**Decided. MIT, picked by me on 2026-11-09. Copyright holder: Project Plumb VR.**

The `LICENSE` file has the MIT text. This file is just the reasoning, and what I passed on, so it's on the record.

## I picked MIT because it's simple

- It's the simplest license people actually know. Nobody has to ask a lawyer.
- It's permissive. Anyone can use the code, change it, share it, even in a closed project, as long as they keep the copyright notice. That keeps the door open for contributors, testers, and anyone who wants to build on the VR host, renderer, or input code.
- It fits the stuff around it. smo-recomp, recomp tooling, most Quest homebrew, they're MIT or permissive too.
- It's short. One page. Easy to put in an APK about screen.
- No copyleft strings attached, so it doesn't fight with linking the Android NDK, the OpenXR loader, or Quest system libraries.

**The catch.** Someone could take this VR code, make it better, and ship it closed without giving anything back. MIT lets them do that. I know that. I picked it anyway.

## The one I passed on: GPLv3

GPLv3 is copyleft. If you ship a changed version, you have to release your source under GPLv3 too. Everything built from it stays open.

That's the strongest way to keep this project open forever, and nobody can take my work private. But it scares some people off, it's harder to mix with proprietary SDKs and store distribution, and it puts paperwork on anyone sideloading or shipping builds. The Quest and Android world is mostly permissive, so GPLv3 would be swimming upstream here.

And it wouldn't change the important limits. GPLv3 can't license Nintendo's code or OdysseyDecomp's unlicensed code. It would still only cover my own work.

Pick GPLv3 if keeping everything derived from this open matters more than getting the most contributors and downstream use. It didn't, for me.

## The other one I passed on: Apache 2.0

Apache 2.0 is like MIT, permissive, use it, change it, ship it, even commercially. It adds an explicit patent grant from contributors, and you have to note your changes.

Same openness as MIT, better patent cover, standard license, everyone knows it. But it's longer and more formal, and the patent part doesn't really matter for a fan game port. More words for not much gain here.

Pick Apache 2.0 if you want MIT style with a patent grant for peace of mind. I didn't need it.

## What the license doesn't cover

Whatever license I picked, it only covers **Project Plumb VR's own code and docs**. The VR host, the OpenXR renderer, the input and camera code, the build scripts, the docs I wrote for this project.

It does **not** cover:

- Nintendo's game, code, assets, characters, or trademarks
- OdysseyDecomp's code. Separate project, no LICENSE file at the time of writing, so don't copy its files into this repo.
- Any `.nsp`, `.nso`, or RomFS dump, `prod.keys`, or other game files. Never included, never licensed.

## The decision, for the record

1. **MIT**, picked by L on 2026-11-09.
2. Copyright holder: **Project Plumb VR**.
3. Copyright year: **2026**.

The final MIT text is in `LICENSE`.
