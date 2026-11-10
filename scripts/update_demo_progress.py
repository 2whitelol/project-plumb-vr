#!/usr/bin/env python3
"""Refresh the live demo-progress badge for Project Plumb VR.

Reads the local OdysseyDecomp clone's data/file_list.yml, counts Matching
functions across the 104 demo objects in docs/demo-objects.txt, and updates:
  - demo-progress.json  (shields.io endpoint badge data; the README badge
                         reads this file live from raw.githubusercontent.com)
  - docs/DEMO_PROGRESS.md (headline number, breakdown table, whole-decomp
                         line, badge text, milestone remaining counts)

Run locally on the VM that holds the OdysseyDecomp clone. The GitHub repo
only mirrors the result; no GitHub Actions workflow can count this, because
the matched work lives in the local tree (uncommitted + PR branches), not
on upstream master.

Usage: python3 scripts/update_demo_progress.py [--yml PATH]
Env:   DEMO_DECOMP_YML overrides the file_list.yml path.
Exit 0 when files were updated or already current; exit 1 on any error
(missing file, torn read, a doc pattern that no longer matches).
Writes nothing unless every replacement matched.
"""

import argparse
import json
import os
import re
import sys
import hashlib
from datetime import datetime
from zoneinfo import ZoneInfo

REPO = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEFAULT_YML = os.path.expanduser(
    "~/workspace/project-plumb-vr/OdysseyDecomp/data/file_list.yml")

# (table label, area key) in the order DEMO_PROGRESS.md lists them.
AREAS = [
    ("Topper (Boss/Stacker)", "Topper"),
    ("Cappy (Player/HackCap)", "Cappy"),
    ("NPCs (Frog, TalkNpc, Cap NPCs)", "NPCs"),
    ("Mario movement and player code", "Mario"),
    ("Bowser (Boss/Koopa)", "Bowser"),
    ("Demo flow, camera, opening scene", "Flow"),
]


def area_of(obj):
    if obj.startswith("Boss/Stacker/"):
        return "Topper"
    if obj == "Player/HackCap.o":
        return "Cappy"
    if obj.startswith("Boss/Koopa/"):
        return "Bowser"
    if obj.startswith("Npc/"):
        return "NPCs"
    if obj.startswith("Player/"):
        return "Mario"
    return "Flow"


def read_bytes_twice(path):
    """Read a file twice; refuse a torn read (file_list.yml is edited live
    by decomp agents, so a mid-write snapshot would corrupt the count)."""
    with open(path, "rb") as f:
        first = f.read()
    with open(path, "rb") as f:
        second = f.read()
    if hashlib.sha256(first).hexdigest() != hashlib.sha256(second).hexdigest():
        raise RuntimeError(f"{path} changed during read; retry next run")
    return first.decode("utf-8", errors="strict")


def count_progress(yml_path):
    text = read_bytes_twice(yml_path)
    total_all = len(re.findall(r"(?m)^\s+label:", text))
    matching_all = len(re.findall(r"(?m)^\s+status: Matching\s*$", text))

    with open(os.path.join(REPO, "docs", "demo-objects.txt")) as f:
        objects = [ln.strip() for ln in f if ln.strip()]

    per_obj_total = {}
    per_obj_match = {}
    current = None
    for line in text.splitlines():
        m = re.match(r"^(\S+\.o):\s*$", line)
        if m:
            current = m.group(1)
            per_obj_total[current] = 0
            per_obj_match[current] = 0
            continue
        if current is None:
            continue
        s = line.strip()
        if s.startswith("label:"):
            per_obj_total[current] += 1
        elif s == "status: Matching":
            per_obj_match[current] += 1

    missing = [o for o in objects if o not in per_obj_total]
    if missing:
        raise RuntimeError(f"demo objects missing from file_list.yml: {missing}")

    areas = {key: [0, 0] for _, key in AREAS}
    for o in objects:
        a = area_of(o)
        areas[a][0] += per_obj_match[o]
        areas[a][1] += per_obj_total[o]
    matched = sum(v[0] for v in areas.values())
    total = sum(v[1] for v in areas.values())
    return {
        "matched": matched,
        "total": total,
        "areas": areas,
        "overall_matched": matching_all,
        "overall_total": total_all,
    }


def badge_color(pct):
    if pct >= 80:
        return "2ECC71"
    if pct >= 50:
        return "E67E22"
    return "E74C3C"


def fmt(n):
    return f"{n:,}"


def replace_or_fail(text, pattern, repl, what):
    new, n = re.subn(pattern, repl, text, count=1)
    if n != 1:
        raise RuntimeError(f"pattern did not match exactly once: {what}")
    return new


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--yml", default=os.environ.get("DEMO_DECOMP_YML", DEFAULT_YML))
    args = ap.parse_args()

    try:
        p = count_progress(args.yml)
    except (OSError, RuntimeError, UnicodeDecodeError) as e:
        print(f"update_demo_progress: ERROR: {e}", file=sys.stderr)
        return 1

    matched, total = p["matched"], p["total"]
    pct = 100.0 * matched / total
    pct1 = f"{pct:.1f}"
    pct0 = f"{pct:.0f}"
    now = datetime.now(ZoneInfo("America/Regina")).strftime("%Y-%m-%d %H:%M %Z")
    message = f"{pct0}% ({fmt(matched)}/{fmt(total)} functions)"

    # ---- demo-progress.json (shields.io endpoint format) ----
    endpoint = {
        "schemaVersion": 1,
        "label": "Demo decomp",
        "message": message,
        "color": badge_color(pct),
    }
    json_path = os.path.join(REPO, "demo-progress.json")
    json_text = json.dumps(endpoint, indent=2) + "\n"

    # ---- docs/DEMO_PROGRESS.md ----
    md_path = os.path.join(REPO, "docs", "DEMO_PROGRESS.md")
    with open(md_path) as f:
        md = f.read()
    orig_md = md

    md = replace_or_fail(
        md,
        r"\*\*[\d,]+ / [\d,]+ demo functions matched \([\d.]+%\)\*\* as of [^\n]*",
        f"**{fmt(matched)} / {fmt(total)} demo functions matched ({pct1}%)** as of {now}.",
        "headline number",
    )
    for label, key in AREAS:
        m, t = p["areas"][key]
        md = replace_or_fail(
            md,
            r"\| " + re.escape(label) + r" \| \d+ \| [\d,]+ \| [\d.]+% \|",
            f"| {label} | {m} | {fmt(t)} | {100.0 * m / t:.1f}% |",
            f"breakdown row {label}",
        )
    md = replace_or_fail(
        md,
        r"\| \*\*Total\*\* \| \*\*[\d,]+\*\* \| \*\*[\d,]+\*\* \| \*\*[\d.]+%\*\* \|",
        f"| **Total** | **{matched}** | **{fmt(total)}** | **{pct1}%** |",
        "breakdown total row",
    )
    milestone_target = (total + 1) // 2  # 50% of the demo set, rounded up
    md = replace_or_fail(
        md,
        r"The demo is ready when \*\*[\d,]+ / [\d,]+ functions match \(50%\)\*\*. "
        r"That is \d+ more than the [\d,]+ counted now\.",
        f"The demo is ready when **{fmt(milestone_target)} / {fmt(total)} functions match (50%)**. "
        f"That is {fmt(milestone_target - matched)} more than the {fmt(matched)} counted now.",
        "milestone line",
    )
    top_left, cap_left, npc_left = (
        p["areas"]["Topper"][1] - p["areas"]["Topper"][0],
        p["areas"]["Cappy"][1] - p["areas"]["Cappy"][0],
        p["areas"]["NPCs"][1] - p["areas"]["NPCs"][0],
    )
    md = replace_or_fail(
        md,
        r"- Finish Topper \(\d+ left\), Cappy \(\d+ left\), and the NPCs \(\d+ left\): \d+",
        f"- Finish Topper ({top_left} left), Cappy ({cap_left} left), and the NPCs ({npc_left} left): "
        f"{top_left + cap_left + npc_left}",
        "milestone first bullet",
    )
    md = replace_or_fail(
        md,
        r"The whole decomp is [\d,]+ / [\d,]+ functions matched \([\d.]+%\) at the same time",
        f"The whole decomp is {fmt(p['overall_matched'])} / {fmt(p['overall_total'])} functions matched "
        f"({100.0 * p['overall_matched'] / p['overall_total']:.1f}%) at the same time",
        "whole-decomp line",
    )
    md = replace_or_fail(
        md,
        r"`Demo decomp — \d+% \([\d,]+/[\d,]+ functions\)`",
        f"`Demo decomp — {message}`",
        "badge-now line",
    )

    changed = []
    try:
        with open(json_path) as f:
            old_json = f.read()
    except OSError:
        old_json = None
    if old_json != json_text:
        with open(json_path, "w") as f:
            f.write(json_text)
        changed.append("demo-progress.json")
    if md != orig_md:
        with open(md_path, "w") as f:
            f.write(md)
        changed.append("docs/DEMO_PROGRESS.md")

    if changed:
        print(f"update_demo_progress: demo {matched}/{total} ({pct1}%) "
              f"overall {p['overall_matched']}/{p['overall_total']} "
              f"-> updated {', '.join(changed)}")
    else:
        print(f"update_demo_progress: demo {matched}/{total} ({pct1}%) — no change")
    return 0


if __name__ == "__main__":
    sys.exit(main())
