# -*- coding: utf-8 -*-

import re,os,stat,string,time,sys

sys.path.append( os.getcwd() + '/LMT' )
sys.path.append( os.getcwd() + '/LMTpp' )
import formal_lf

def get_files(cd,r):
    """ find recursively files in subdirectories """
    nl = os.listdir(cd)
    res = []
    for e in nl:
        if e == "_darcs": continue
        ed = cd+"/"+e
        if not os.access(ed,os.F_OK): continue
        if stat.S_ISDIR(os.stat(ed)[stat.ST_MODE]): res += get_files(ed,r)
        elif r.match(e): res += [ed]
    return res

def make_dep_py(env):
    """ ....py -> .h """
    nfd = re.compile( r'"([^"]*)"' )
    nfs = re.compile( r"'([^']*)'" )
    all_py = get_files(".", re.compile( '.*\.py$' ) )
    all_met = get_files(".", re.compile( '.*\.met$' ) )
    re_elem_py = re.compile( '.*element_.*\.py$' )
    # .h.met
    for i in filter( lambda x:'.h.met' in x, all_met ):
        corh = i[:-4]
        METILPATH = ""
        if env.has_key('METILPATH'): adl = env['METILPATH']
        METILEXEC = "metil"
        if env.has_key('METILEXEC'): METILEXEC = env['METILEXEC']
        env.Command(corh,i,'export METILPATH="'+METILPATH+':$METILPATH";'+METILEXEC+' %s %s'%(i,corh))
    # .h.py
    for i in filter(lambda x:'.h.py' in x or '.cpp.py' in x, all_py):
        corh = i[:-3]
        adl = ""
        if env.has_key('add_lib_path'): adl = env['add_lib_path']
        dep = []
        for l in file(i,'r').readlines():
            if 'formulations' in l:
                for bf in re.findall( nfd, l )+re.findall( nfs, l ):
                    bf = 'formulation_'+bf+'.py'
                    dep += filter( lambda x:bf in x, all_py )
                dep += get_files("LMT/formal_lf", re.compile( '.*\.py$' ) )
        env.Command(corh,[i]+dep,'export PYTHONPATH=".:LMT:$PYTHONPATH";export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:'+adl+'";python2 %s > %s'%(i,corh))
    # element_Quad.h depend de element_Quad.py
    #for i in filter(lambda x:re_elem_py.match(x), all_py):
    #    corh = 'LMT/include/mesh' + i[ i.rfind('/') : -3 ] + '.h'
    #    ne = i[ i.rfind('/')+9 : -3 ]
    #    env.Command( corh, [i,'LMT/make_elem.py'], 'export PYTHONPATH="'+os.getcwd()+':$PYTHONPATH";python LMT/make_elem.py %s > %s'%(ne,corh) )
    # meshcarac
    for i in get_files(".", re.compile( '.*\.meshcarac.py$' ) ):
        corh = i[:-12] + "h"
        env.Command(corh,i,'LMT/bin/lmtmeshcarac.py %s > %s'%(i,corh))
    # problem
    for i in get_files(".", re.compile( '.*\.problem.py$' ) ):
        corh = i[:-2] + "h"
        env.Command( corh, i, 'LMT/bin/lmtproblem.py %s' % i )
    
def linkflags(libs):
    """ find flags for libraries which has exetutables that return them... """
    return ' ' + string.join([ string.rstrip(os.popen(lib+' --libs').readline()) for lib in libs ]) + ' '
    
def cppflags(libs):
    """ find flags for libraries which has exetutables that return them... """
    return ' ' + string.join([ string.rstrip(os.popen(lib+' --cflags').readline()) for lib in libs ]) + ' '
    
def makedist(target,source,env):
    """ dist """
    ere = ".*Cons|Makefile|Doxygen|TODO|Doxyfile|.sconsign"
    for i in ["cpp","cc","c","h","xml","py","pl","msh","i","dgibi","avs","unv","msh","dcofa"]:
        ere += '|.*\\.'+i+"$"
    fl = get_files( ".", re.compile( ere ) )
    name_tbz = "multi_v09_" + time.strftime("%d_%b_%Y_%H_%M_%S") + ".tar.bz2"
    os.system( "tar cjf "+name_tbz+" "+string.join(fl,' ') )
    #os.system( "cp "+name_tbz+" /mnt/usb" )
    #os.system( "eject /mnt/zip" )
    return None

def fetch_LMT(target,source,env):
    """ fetch last source files from repository """
#     os.system( "rsync --exclude '*~' --exclude '*.o' --exclude '*.so' --exclude '*.os' -rauv -e ssh caignot@ssh.lmt.ens-cachan.fr:/ul/leclerc/prog/LMTpp/LMT ." )

def problem_name( formulation, T, dimension ):
    Tbis = T.replace( "<" , "_LessThan_" )
    Tbis = Tbis.replace( ">" , "_GreaterThan_" )
    Tbis = Tbis.replace( "," , "_Comma_" )
    Tbis = Tbis.replace( ":" , "_Colon_" )
    Tbis = Tbis.replace( " " , "_Space_" )
    return 'Problem_'+formulation+'_'+Tbis+'_'+str(dimension)

class MakePb:
    def get_fe_sets_and_dims(self):
        """ return fe_sets and all_dims """
        fe_sets = []
        all_dims = {}
        for e_name, e_file in zip( self.elements, self.e_files ):
            e = formal_lf.Element( e_name, 3, name_file=e_file )
            nvi = e.nb_var_inter
            exec( 'dim = '+self.dim_policy )
            for n,v in self.additional_fields.items():
                for d in v.nb_dim:
                    exec( 'd = '+d )
            all_dims[dim] = 1
            e = formal_lf.Element( e_name, dim, name_file=e_file )
            for f_name, f_file in zip( self.formulations, self.f_files ):
                f = formal_lf.Formulation( f_name, dim, 0, self.incpaths, options=self.options, name_file = f_file )
                fe_sets.append( (f, e) )

        map_f = {}
        for f,e in fe_sets: map_f[f.name] = []
        for f,e in fe_sets: map_f[f.name].append( (f,e) )
        
        return fe_sets, all_dims, map_f
    
    def write_mesh_carac( self, target, source, env ):
        # import formal_lf
        self.incpaths += ['LMT/formulations']
        pb = formal_lf.Problem( self.name,self.formulations,self.elements,self.dim_policy,self.incpaths,self.options,self.additional_fields )
        fe_sets, all_dims = pb.get_fe_sets_and_dims()
        pb.write_all_mesh_carac( file( str(target[0]), 'w' ), fe_sets, all_dims )
        
    def write_problem_h_from_scons( self, target, source, env ):
        output = file( str(target[0]), 'w' )
        
        output.write( '#ifndef PROBLEM_'+self.name+'_H\n' )
        output.write( '#define PROBLEM_'+self.name+'_H\n' )
        for f_name in self.map_f.keys():
            output.write( '#ifndef has_formulation_'+f_name+'\n' )
            output.write( '#define has_formulation_'+f_name+'\n' )
            output.write( '#endif\n' )
        
        for inc in self.includes:
            output.write( '#include "'+inc+'"\n' )
        output.write( 'namespace LMT {\n\n' )
        output.write( 'template<class T,unsigned dim> class Problem_'+self.name+';\n\n' )
        for d in self.all_dims.keys():
            for T in self.types:
                PN = problem_name( self.name, T, d )
                output.write( 'class '+PN+' : public ProblemAncestor<'+T+(T[-1:]=='>')*' '+'> {\n' )
                output.write( 'public:\n' )
                output.write( '    typedef Mesh<Mesh_carac_'+self.name+'<'+T+','+str(d)+'> > TM;\n' )
                output.write( '    '+PN+'() {}\n' )
                output.write( '    '+PN+'( TM &m, bool use_tim_davis=false ) {\n' )
                output.write( '        if ( use_tim_davis ) {\n' )
                for f_name in self.map_f.keys():
                    output.write( '            formulation_'+f_name+' = new_formulation_'+f_name+'( Number<true>(), m );\n' )
                output.write( '        } else {\n' )
                for f_name in self.map_f.keys():
                    output.write( '            formulation_'+f_name+' = new_formulation_'+f_name+'( Number<false>(), m );\n' )
                output.write( '        }\n' )
                output.write( '    }\n' )
                output.write( '    virtual unsigned nb_formulations() const { return '+str(len(self.map_f))+'; }\n' )
                output.write( '    virtual FormulationAncestor<'+T+(T[-1:]=='>')*' '+'> *formulation_nb(unsigned i) {\n' )
                output.write( '        switch(i) {\n' )
                cpt = 0
                for f_name in self.map_f.keys():
                    output.write( '          case '+str(cpt)+': return formulation_'+f_name+';\n' )
                    cpt += 1
                output.write( '          default: return NULL;\n' )
                output.write( '        }\n' )
                output.write( '    }\n' )
                for f_name in self.map_f.keys():
                    output.write( '    static FormulationAncestor<'+T+(T[-1:]=='>')*' '+'> *new_formulation_'+f_name+'( Number<false>, TM &m );\n' )
                    output.write( '    static FormulationAncestor<'+T+(T[-1:]=='>')*' '+'> *new_formulation_'+f_name+'( Number<true >, TM &m );\n' )
                for f_name in self.map_f.keys():
                    output.write( '    FormulationAncestor<'+T+(T[-1:]=='>')*' '+'> *formulation_'+f_name+';\n' )
                output.write( '};\n' )
                NPN = 'Problem_'+self.name+'<'+T+','+str(d)+'>'
                output.write( 'template<> class '+NPN+' : public '+PN+' {\npublic:\n    '+NPN+'(TM &m,bool use_tim_davis=false):'+PN+'(m,use_tim_davis) {}\n};\n\n' )
        output.write( '} // namespace LMT\n' )
        output.write( '#endif // PROBLEM_'+self.name+'_H\n' )

        
    def write_formulations_h_from_scons( self, target, source, env ):
        output = file( str(target[0]), 'w' )
        for f,e in self.fe_sets:
            output.write( '#include "formulation_%s_%s_%i.h"\n' % (f.name,e.name,e.dim) )

class MakePbFE:
    def write_formulation_h_from_scons( self, target, source, env ):
        output = file( str(target[0]), 'w' )
        asmout = file( str(target[1]), 'w' )
        
        for f,e in self.fe_set:
            if e.dim == self.d:
                f.write( e, output, asmout = asmout, name_der_vars = self.name_der_vars )
    
        output.close()
        asmout.close()
    
    def write_formulation_cpp_from_scons( self, target, source, env ):
        output = file( str(target[0]), 'w' )
        PN = problem_name( self.name, self.T, self.d ) 
        
        output.write( '#include "problem.h"\n' )
        output.write( '#include "formulation_'+str(self.d)+'_'+self.T+'_'+self.f_name+'.h"\n' )
        forms = {}
        for f,e in self.fe_set:
            if e.dim == self.d:
                forms[ f.name ] = f
        for f in forms.values():
            output.write( 'namespace LMT {\n' )
            for b in ['false','true ']:
                TF = 'Formulation<' + PN + '::TM,'+f.name+',DefaultBehavior,'+self.T+','+b+'>'
                output.write( 'FormulationAncestor<'+PN+'::T> *' + PN + '::new_formulation_' + f.name + '( Number<'+b+'>, ' + PN + '::TM &m ) { return new '+TF+'(m); }\n' )
            output.write( '}\n' )
        
    def use_asm( self ):
        res = False
        for f,e in self.fe_set:
            res |= f.use_asm
        return res


def make_pb( env,
             opt,
             name,
             formulations,
             elements,
             dim = 'nvi',
             output = sys.stdout,
             incpaths = ['.',os.getcwd()+'/LMT/formulations'],
             options = {},
             additional_fields = {},
             types = ['double'],
             dep_py = True,
             name_der_vars = [],
			 includes = ['mesh_carac.h','formulation/problem_ancestor.h'] ):
   #
   if len( formulations ) == 0:
        print "Attention, tu demandes de générer des fichiers de problème mais aucune formulation n'a été spécifiée. On peut utiliser d'autres fonctions pour faire des MeshCarac."
   
   # find formulation and element files
   f_files, e_files = [], []
   for f in formulations:
      f = 'formulation_'+f+'.py'
      for inc in incpaths:
         n = inc + '/'*(len(inc)!=0) + f
         if os.access( n, os.F_OK ):
               f_files.append( n )
               break
      else:
         txt = 'impossible to find %s in %s'%(f,incpaths)
         raise NameError, txt
   for e in elements:
      e = 'element_'+e+'.py'
      for inc in incpaths:
         n = inc + '/'*(len(inc)!=0) + e
         if os.access( n, os.F_OK ):
               e_files.append( n )
               break
      else:
         txt = 'impossible to find %s in %s'%(e,incpaths)
         raise NameError, txt
   #
   directory = 'build/problem_'+name+'/'
   base_py_files = [] # ['LMT/formal_lf/formulation.py','LMT/formal_lf/element.py']
   pb = MakePb()
   pb.name              = name             
   pb.formulations      = formulations     
   pb.elements          = elements         
   pb.dim_policy        = dim
   pb.incpaths          = incpaths         
   pb.options           = options          
   pb.additional_fields = additional_fields
   pb.types             = types            
   pb.directory         = directory        
   pb.f_files           = f_files
   pb.e_files           = e_files
   pb.formulations      = formulations
   pb.elements          = elements
   pb.name_der_vars     = name_der_vars
   pb.includes          = includes
   fe_sets, all_dims, map_f = pb.get_fe_sets_and_dims()
   pb.fe_sets, pb.all_dims, pb.map_f = fe_sets, all_dims, map_f

   # mesh_carac
   mesh_carac = env.Command( directory + 'mesh_carac.h', f_files+e_files+base_py_files, pb.write_mesh_carac )
   
   # problem.h
   pb_h = env.Command( directory + 'problem.h', mesh_carac, pb.write_problem_h_from_scons )
   
   all_cpp = []
   for d in all_dims.keys():
      for T in types:
          for f_name in map_f.keys():
            bn = 'formulation_%s_%s_%s.cpp' % (d,T,f_name)
            ba = 'formulation_%s_%s_%s_.asm' % (d,T,f_name)
            bh = 'formulation_%s_%s_%s.h'   % (d,T,f_name)
            #fos_h.append( env.Command( directory + 'formulation_%s_%s_%i.h'%(f.name,e.name,e.dim), [f.name_file,e.name_file]+base_py_files, cp.write_formulation_h_from_scons ) )
            pbc = MakePbFE()
            pbc.d = d
            pbc.T = T
            pbc.name = name
            pbc.f_name = f_name
            pbc.fe_set = map_f[ f_name ]
            pbc.name_der_vars = name_der_vars
            f_h, f_asm = env.Command( [ directory + bh, directory + ba ], pb_h, pbc.write_formulation_h_from_scons )
            all_cpp += env.Command( directory + bn, f_h, pbc.write_formulation_cpp_from_scons , TARGET = bn + '_opt' * opt +'_debug'*(1-opt) + '.o' )
            if pbc.use_asm():
                all_cpp += [ f_asm ]
   return all_cpp





# CPU
try:    CPU = string.split(os.popen( "uname -m" ).read())[0]
except: CPU = 'unknown_CPU'


