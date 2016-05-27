from LMT.formal_lf import *

write_pb(
    name = 'pb_elasticity_dep_static',
    formulations = ['elasticity_dep_static'],
    elements = ['Hexa'],
    options = { 'behavior_simplification' : 'plane stress' },
)

