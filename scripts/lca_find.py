#!/usr/bin/env python3

import argparse
import subprocess

from difflib import SequenceMatcher


def parse_args():
  parser = argparse.ArgumentParser()
  parser.add_argument("pattern", type=str)
  parser.add_argument("--tolerance", "-t", type=float, default=0.8)
  parser.add_argument("--verbose", "-v", action="store_true")
  return parser.parse_args()


def main(args):
  contents_out = subprocess.check_output(["lca_contents.sh"], text=True)
  contents = contents_out.split("\n")[2:]
  L = len(args.pattern)

  matches = []
  for f in contents:
    res = SequenceMatcher(a=args.pattern, b=f).ratio()
    for i in range(len(f) - L + 1):
      res = max(res, SequenceMatcher(a=args.pattern, b=f[i:i+L]).ratio())
    if res > args.tolerance:
      matches.append((f, res))

  for f, r in sorted(matches, reverse=True, key=lambda x: x[1]):
    if args.verbose:
      print(f"{r:0.2f}: {f}")
    else:
      print(f)


""" how to use
#!/bin/bash

pattern="$1"

PATH_TO_LCA="/path/to/lca"
python3 $PATH_TO_LCA/scripts/find_lca.py $pattern --lca $PATH_TO_LCA

# attempts to fuzzy find in filenames
"""
if __name__ == "__main__":
  main(parse_args())
