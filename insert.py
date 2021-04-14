#!/usr/bin/env python3

import sys, os

have = set()
def expand(filename, ouf, wait = False):
    realname = os.path.realpath(filename)
    if realname in have:
        ouf.write("// already included\n")
        return
    have.add(realname)
    cwd = os.getcwd()
    os.chdir(os.path.dirname(realname))
    inf = open(realname)
    print(f"copying {realname}")
    for line in inf:
        if line.strip() == "#pragma once":
            wait = False
        elif not wait:
            if len(line) > 10 and line[:10] == "#include \"":
                ouf.write(f"// START {line}")
                expand(line.strip()[10:-1], ouf, True)
                ouf.write(f"// END {line}")
            else:
                ouf.write(line)
    os.chdir(cwd)

ouf = open("to_submit.cpp", "w")
expand(sys.argv[1], ouf)
ouf.close()
