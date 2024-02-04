#!/usr/bin/env python3
import argparse
import subprocess
import sys

from lca_insert import main_impl as lca_insert


COMPILE_COMMAND = (
    "g++-11 -g -std=c++20 {code} -o {binary} -O2 -Wall -Wextra -Wshadow"
    #" -Wconversion"
    " -Wno-sign-compare"
    " -fmax-errors=1"
)

OPT_INFO_FLAGS = "-fopt-info-vec-optimized -fopt-info-loop-optimized".split()


def parse_args():
  parser = argparse.ArgumentParser()
  parser.add_argument("input", type=str)
  parser.add_argument("--output", "-o", type=str, default="to_submit.cpp",
                      help="standalone code")
  parser.add_argument("--lca", type=str, default=None,
                      help="root dir of lca")
  parser.add_argument("--extra_args", "-e", nargs=argparse.REMAINDER, default=[],
                      help="to be passed to the compiler")
  parser.add_argument("--opt-info", "-i", action="store_true", help="adds opt-info flags")
  return parser.parse_args()


def get_lca_root() -> str:
  return subprocess.run(["lca_root.sh"], capture_output=True, text=True).stdout.strip()


def main(args):
  binary = args.input[:-4] if args.input.endswith(".cpp") else args.input
  lca_insert(
      input_f=binary + ".cpp",
      output_f=args.output,
      lca_root=args.lca if args.lca is not None else get_lca_root(),
  )
  cmd = COMPILE_COMMAND.format(code=args.output, binary=binary).split()
  cmd += OPT_INFO_FLAGS if args.opt_info else []
  cmd += args.extra_args
  print(f"Running `{cmd}`")
  res = subprocess.run(cmd, capture_output=True)
  sys.stderr.buffer.write(res.stderr)
  sys.stdout.buffer.write(res.stdout)


if __name__ == "__main__":
  main(parse_args())
