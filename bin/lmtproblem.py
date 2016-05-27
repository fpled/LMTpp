#!/usr/bin/env python2
import sys, os, re, string

def usage():
    print "Usage : "+sys.argv[0]+" mypyproblemfile[.py]"
    print "  convert a mesh carac file to the mypycaracfile.h one."
    exit(0)

if len(sys.argv)==1: usage()

for name_file in sys.argv[1:]:
    print name_file
    #mc = MeshCarac(name_file)
    #mc.write_h_file()
