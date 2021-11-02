#!/usr/bin/env python3

import sys, os

have = set()
def expand(filename, ouf, wait = False, digraphs = False):
    realname = os.path.realpath(filename)
    if realname in have:
        ouf.write("// already included\n")
        return
    have.add(realname)
    cwd = os.getcwd()
    os.chdir(os.path.dirname(realname))
    inf = open(realname)
    print(f"copying {realname}" + (" with digraphs" if digraphs else ""))
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
                expand(line.strip()[11:-1], ouf, True, True)
                ouf.write(f"// END {line}")
            elif digraphs:
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

ouf = open("to_submit.cpp", "w")
expand(sys.argv[1], ouf)
ouf.close()
