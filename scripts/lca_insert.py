#!/usr/bin/env python3

import argparse
import sys, os


def parse_args():
  parser = argparse.ArgumentParser()
  parser.add_argument("input", type=str)
  parser.add_argument("--output", "-o", type=str, default="to_submit.cpp")
  parser.add_argument("--lca", type=str, required=True)
  return parser.parse_args()


def main(args):
  PATH_TO_LCA = args.lca
  if PATH_TO_LCA[-1] != '/':
    PATH_TO_LCA += '/'

  have = set()
  def expand(filename, ouf, wait = False, digraphs = False):
    realname = os.path.realpath(filename)
    if realname in have:
      ouf.write("// already included\n")
      return
    have.add(realname)
    cwd = os.getcwd()
    os.chdir(os.path.dirname(realname))
    print(f"copying {realname}" + (" with digraphs" if digraphs else ""))
    with open(realname, "r") as inf:
      for line in inf:
        if line.strip() == "#pragma once":
          wait = False
        elif not wait:
          if len(line) > 10 and line[:10] == "#include \"":
            ouf.write(f"// START {line}")
            expand(line.strip()[10:-1], ouf, True, digraphs)
            ouf.write(f"// END {line}")
          elif len(line) > 11 and line[:11] == "%:include \"":
            ouf.write(f"// START {line}")
            expand(PATH_TO_LCA + line.strip()[11:-1], ouf, True, True)
            ouf.write(f"// END {line}")
          elif digraphs and False: # this is stupid
            ouf.write(line.replace("[", "<:")
                .replace("]", ":>")
                .replace("{", "<%")
                .replace("}", "%>")
                .replace("#", "%:")
                .replace("&&", " and ")
                .replace("||", " or ")
            )
          else:
            ouf.write(line)
    os.chdir(cwd)

  ouf = open(args.output, "w")
  expand(args.input, ouf)
  ouf.close()


""" how to use
#!/bin/bash

PATH_TO_LCA="/path/to/lca"
python3 $PATH_TO_LCA/scripts/lca_insert.py ${1/.cpp}.cpp --lca $PATH_TO_LCA [ -o output_name.cpp ]

# in the cpp file
# Use '%:include "blah.h"' for paths relative to $PATH_TO_LCA
# Use '#include "blah.h"' for paths relative to code dir
"""
if __name__ == "__main__":
  main(parse_args())
