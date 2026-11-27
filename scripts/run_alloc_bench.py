#!/usr/bin/env python3
"""W06 (#57) runner/parser: page benchmark (baseline vs percpu, CPUS=1/2) + object 5-round."""

import csv
import datetime
import json
import os
import signal
import statistics
import subprocess
import sys
import time
import argparse
import select
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT_DIR = ROOT / "results" / "w06"
CLOCK_HZ = 10_000_000.0  # QEMU virt RISC-V time CSR = 10 MHz
QEMU_TIMEOUT_SEC = 90

MATRIX = [
    ("baseline", 1, 1),
    ("baseline", 2, 1),
    ("percpu", 1, 0),
    ("percpu", 2, 0),
]


def cmd_out(cmd):
    try:
        return subprocess.check_output(cmd, cwd=ROOT, stderr=subprocess.STDOUT, text=True).strip()
    except Exception as e:
        return f"unknown ({e})"


def parse_kv_line(line):
    parts = line.strip().split("|")
    data = {"_tag": parts[0]}
    for p in parts[1:]:
        if "=" in p:
            k, v = p.split("=", 1)
            data[k] = v
    return data


def run_case(backend, cpus, baseline_flag, sha, timeout_sec=QEMU_TIMEOUT_SEC):
    subprocess.run(
        ["make", "clean"],
        cwd=ROOT,
        check=True,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.STDOUT,
    )

    compile_cmd = [
        "make", "kernel/kernel", "fs.img",
        f"CPUS={cpus}",
        f"KALLOC_BASELINE={baseline_flag}",
        "ALLOC_BENCH=1",
    ]
    print(f"[BUILD] {' '.join(compile_cmd)}")
    subprocess.run(compile_cmd, cwd=ROOT, check=True)

    run_cmd = [
        "make", "qemu",
        f"CPUS={cpus}",
        f"KALLOC_BASELINE={baseline_flag}",
        "ALLOC_BENCH=1",
    ]
    print(f"[RUN  ] {' '.join(run_cmd)} (timeout={timeout_sec}s)")

    raw_lines = []
    timed_out = False
    saw_done = False
    saw_panic = False

    proc = subprocess.Popen(
        run_cmd,
        cwd=ROOT,
        stdin=subprocess.DEVNULL,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        bufsize=0,
        start_new_session=True,
    )

    pgid = proc.pid
    fd = proc.stdout.fileno()
    buf = b""
    deadline = time.monotonic() + timeout_sec

    try:
        while True:
            remaining = deadline - time.monotonic()
            if remaining <= 0:
                timed_out = True
                break

            ready, _, _ = select.select([fd], [], [], min(remaining, 0.2))
            if not ready:
                if proc.poll() is not None:
                    break
                continue

            chunk = os.read(fd, 4096)
            if not chunk:
                break

            buf += chunk
            while b"\n" in buf:
                raw_line, buf = buf.split(b"\n", 1)
                line = (raw_line + b"\n").decode("utf-8", errors="replace")
                raw_lines.append(line)
                low = line.lower()
                if "panic:" in low or "assertion" in low or "scause" in low:
                    saw_panic = True
                    break
                if line.startswith("AB|done|"):
                    saw_done = True
                    break

            if saw_panic or saw_done:
                break

        if buf:
            raw_lines.append(buf.decode("utf-8", errors="replace"))
    finally:
        try:
            os.killpg(pgid, signal.SIGTERM)
        except (ProcessLookupError, PermissionError, OSError):
            pass
        try:
            proc.wait(timeout=3)
        except subprocess.TimeoutExpired:
            try:
                os.killpg(pgid, signal.SIGKILL)
            except (ProcessLookupError, PermissionError, OSError):
                pass
            proc.wait()
        proc.stdout.close()

    raw_path = OUT_DIR / f"raw_{backend}_cpus{cpus}.log"

    meta = {}
    page_rows = []
    obj_rows = []
    for line in raw_lines:
        if line.startswith("AB|meta|"):
            meta = parse_kv_line(line)
        elif line.startswith("AB_PAGE|"):
            page_rows.append(parse_kv_line(line))
        elif line.startswith("AB_OBJ|"):
            obj_rows.append(parse_kv_line(line))

    measured_page = [r for r in page_rows if r.get("warmup") == "0"]
    measured_obj = [r for r in obj_rows if r.get("warmup") == "0"]
    any_row_fail = any(r.get("status") != "PASS" for r in page_rows + obj_rows)

    case_ok = (
        not timed_out
        and not saw_panic
        and saw_done
        and not any_row_fail
        and len(measured_page) >= 3
        and len(measured_obj) >= 20  # 5 rounds * 4 classes
    )

    reason = "OK"
    if timed_out:
        reason = "TIMEOUT"
    elif saw_panic:
        reason = "PANIC_OR_ASSERT"
    elif any_row_fail:
        reason = "ROW_FAIL"
    elif not saw_done:
        reason = "INCOMPLETE_NO_DONE"
    elif len(measured_page) < 3 or len(measured_obj) < 20:
        reason = "MISSING_RUNS"

    print(f"[DONE ] backend={backend} cpus={cpus} -> {'PASS' if case_ok else 'FAIL'} ({reason}), log={raw_path.name}")

    formatted_pages = []
    if page_rows:
        for r in page_rows:
            ticks = int(r.get("time_ticks", "0"))
            ops = int(r.get("ops", "0"))
            t_sec = ticks / CLOCK_HZ if ticks > 0 else 0.0
            tput = (ops / t_sec) if t_sec > 0 else 0.0
            row_status = "PASS" if (case_ok and r.get("status") == "PASS") else "FAIL"
            formatted_pages.append({
                "sha": sha,
                "backend": backend,
                "cpus": cpus,
                "workers": int(r.get("workers", cpus)),
                "workload": r.get("workload", "page_batch_af"),
                "run": int(r.get("run", "0")),
                "warmup": int(r.get("warmup", "0")),
                "time_ticks": ticks,
                "time_sec": f"{t_sec:.6f}",
                "ops": ops,
                "throughput_ops_s": f"{tput:.2f}",
                "alloc_ok": int(r.get("alloc_ok", "0")),
                "alloc_fail": int(r.get("alloc_fail", "0")),
                "local_hit": int(r.get("local_hit", "0")),
                "local_miss": int(r.get("local_miss", "0")),
                "steal_attempts": int(r.get("steal_attempts", "0")),
                "steal_success": int(r.get("steal_success", "0")),
                "status": row_status,
            })
    else:
        # Khong co dong do nao (do panic/timeout som): van ghi ban ghi FAIL de khong bo sot
        formatted_pages.append({
            "sha": sha,
            "backend": backend,
            "cpus": cpus,
            "workers": cpus,
            "workload": "page_batch_af",
            "run": 0,
            "warmup": 0,
            "time_ticks": 0,
            "time_sec": "0.000000",
            "ops": 0,
            "throughput_ops_s": "0.00",
            "alloc_ok": 0,
            "alloc_fail": 0,
            "local_hit": 0,
            "local_miss": 0,
            "steal_attempts": 0,
            "steal_success": 0,
            "status": "FAIL",
        })

    formatted_objs = []
    for r in obj_rows:
        row_status = "PASS" if (case_ok and r.get("status") == "PASS") else "FAIL"
        formatted_objs.append({
            "sha": sha,
            "backend": backend,
            "cpus": cpus,
            "workload": r.get("workload", "obj_4class_5r"),
            "round": int(r.get("round", "0")),
            "warmup": int(r.get("warmup", "0")),
            "class_size": int(r.get("class", "0")),
            "cap_per_slab": int(r.get("cap", "0")),
            "req_size": int(r.get("req_size", "0")),
            "test_objs": int(r.get("test_objs", "0")),
            "req_payload_B": int(r.get("req_payload_B", "0")),
            "stride_B": int(r.get("stride_B", "0")),
            "rounding_B": int(r.get("rounding_B", "0")),
            "meta_B": int(r.get("meta_B", "0")),
            "tail_pad_B": int(r.get("tail_pad_B", "0")),
            "base_live": int(r.get("base_live", "0")),
            "peak_live": int(r.get("peak_live", "0")),
            "after_live": int(r.get("after_live", "0")),
            "slab_pages": int(r.get("slab_pages", "0")),
            "empty_slabs": int(r.get("empty_slabs", "0")),
            "retained_B": int(r.get("retained_B", "0")),
            "status": row_status,
        })

    case_info = {
        "backend": backend,
        "cpus": cpus,
        "kalloc_baseline_flag": baseline_flag,
        "command": " ".join(run_cmd),
        "raw_log": str(raw_path.relative_to(ROOT)),
        "status": "PASS" if case_ok else "FAIL",
        "reason": reason,
        "meta": meta,
    }
    return case_info, formatted_pages, formatted_objs


def write_csv(path, fieldnames, rows):
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=fieldnames)
        w.writeheader()
        w.writerows(rows)


def main():
    parser = argparse.ArgumentParser(description="W06 allocator benchmark runner")
    parser.add_argument(
        "--timeout",
        type=float,
        default=QEMU_TIMEOUT_SEC,
        help="Timeout per QEMU run in seconds (default: %(default)s)",
    )
    args = parser.parse_args()

    OUT_DIR.mkdir(parents=True, exist_ok=True)
    sha = cmd_out(["git", "rev-parse", "--short", "HEAD"])
    dirty = bool(cmd_out(["git", "status", "--porcelain"]))

    all_cases = []
    all_page_rows = []
    all_obj_rows = []

    try:
        for backend, cpus, bflag in MATRIX:
            cinfo, prows, orows = run_case(backend, cpus, bflag, sha, timeout_sec=args.timeout)
            all_cases.append(cinfo)
            all_page_rows.extend(prows)
            all_obj_rows.extend(orows)
    finally:
        # Don build ALLOC_BENCH=1 de lan chay test-xv6.py ke tiep luon dung build mac dinh
        subprocess.run(
            ["make", "clean"],
            cwd=ROOT,
            check=False,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.STDOUT,
        )

    page_fields = [
        "sha", "backend", "cpus", "workers", "workload", "run", "warmup",
        "time_ticks", "time_sec", "ops", "throughput_ops_s",
        "alloc_ok", "alloc_fail", "local_hit", "local_miss",
        "steal_attempts", "steal_success", "status",
    ]
    obj_fields = [
        "sha", "backend", "cpus", "workload", "round", "warmup",
        "class_size", "cap_per_slab", "req_size", "test_objs",
        "req_payload_B", "stride_B", "rounding_B", "meta_B", "tail_pad_B",
        "base_live", "peak_live", "after_live", "slab_pages", "empty_slabs",
        "retained_B", "status",
    ]

    # Luu day du audit (ca PASS va FAIL) + tap du lieu dat (CHI gom PASS sau warm-up)
    pass_page_rows = [r for r in all_page_rows if r["status"] == "PASS" and r["warmup"] == 0]
    pass_obj_rows = [r for r in all_obj_rows if r["status"] == "PASS" and r["warmup"] == 0]

    write_csv(OUT_DIR / "page_bench_all.csv", page_fields, all_page_rows)
    write_csv(OUT_DIR / "page_bench_pass.csv", page_fields, pass_page_rows)
    write_csv(OUT_DIR / "obj_bench_all.csv", obj_fields, all_obj_rows)
    write_csv(OUT_DIR / "obj_bench_pass.csv", obj_fields, pass_obj_rows)

    summary = {}
    for backend, cpus, _ in MATRIX:
        key = f"{backend}_cpus{cpus}"
        vals = [
            float(r["throughput_ops_s"])
            for r in pass_page_rows
            if r["backend"] == backend and r["cpus"] == cpus
        ]
        if vals:
            summary[key] = {
                "valid_runs": len(vals),
                "mean_ops_s": round(statistics.mean(vals), 2),
                "median_ops_s": round(statistics.median(vals), 2),
            }
        else:
            summary[key] = {"valid_runs": 0, "note": "NO_VALID_PASS_DATA"}

    manifest = {
        "generated_at": datetime.datetime.now().isoformat(),
        "git_sha": sha,
        "git_dirty": dirty,
        "gcc_version": cmd_out(["riscv64-unknown-elf-gcc", "--version"]).splitlines()[0],
        "qemu_version": cmd_out(["qemu-system-riscv64", "--version"]).splitlines()[0],
        "config": {
            "ram_mb": 128,
            "qemu_timeout_sec": args.timeout,
            "time_unit": "riscv time CSR ticks (10 MHz on QEMU virt)",
            "throughput_formula": "ops / (time_ticks / 10_000_000.0), 1 op = 1 kalloc()+kfree() pair",
            "page_warmup_runs": 1,
            "page_measured_runs": 3,
            "obj_warmup_rounds": 1,
            "obj_measured_rounds": 5,
            "baseline_method": "Same commit/source/slab/client/toolchain, switched via KALLOC_BASELINE=1",
            "counter_units": {
                "alloc_ok/alloc_fail/local_hit/local_miss": "kalloc() calls",
                "steal_attempts": "donor pool lock+try attempts",
                "steal_success": "successful 1-page steals (pages == successes)",
                "baseline_note": "single global lock has no local freelist -> local_hit/miss/steal = 0",
            },
        },
        "cases": all_cases,
        "pass_summary": summary,
    }
    (OUT_DIR / "manifest.json").write_text(json.dumps(manifest, indent=2), encoding="utf-8")

    print("\n=== SUMMARY (PASS DATA ONLY) ===")
    print(json.dumps(summary, indent=2))
    if any(c["status"] != "PASS" for c in all_cases):
        sys.exit(1)


if __name__ == "__main__":
    main()