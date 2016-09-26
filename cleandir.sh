#!/bin/bash
make maintainer-clean
files=("Makefile.in" "config.h.in" "config.sub" "config.guess" "configure"\
 "compile" "ltmain.sh" "install-sh" "depcomp" "missing" "aclocal.m4")
dirs=("m4") 
for f in ${files[*]}
do
  if [ -f "$f" ]; then
    echo "rm $f"
    rm $f
  fi 
done
for d in ${dirs[*]}
do
  if [ -d "$d" ]; then
    echo "rm -Rf $d"
    rm -Rf $d
  fi
done