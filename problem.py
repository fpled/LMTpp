#!/usr/bin/env python2
import sys,string
from formulation import *
# pouet
class Problem:
    def __init__(self,formulations,elements,dim='nvi',incpaths=['.','LMT/formulations']):
        self.formulations = formulations
        self.elements = elements
        self.dim_policy = dim
        self.incpaths = incpaths
        
    def write(self,f=sys.stdout):
        ifndef = string.join(self.formulations,'_')+'__'+string.join(self.elements,'_')+self.dim_policy
        print '#ifndef PROBLEM_' + ifndef
        print '#define PROBLEM_' + ifndef
        print
        #         for form in self.dict_e['Formulations']:
        #             f = Formulation( self.incpaths, dim, form )
        #             f.write( self.name_pb )
        print
        print '#endif // PROBLEM_' + ifndef

def write_pb(formulations,elements,dim='nvi',output=sys.stdout,incpaths=['.','LMT/formulations']):
    Problem pb(formulations,elements,dim,incpaths)
    pb.write(output)
# toto
