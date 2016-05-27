#!/usr/bin/env python2
import sys, os, re, string

class MeshCarac:
    class Parameter:
        def __init__(self,type_par,par_range,default_value):
            """ par_range = [1,3,4,...] if parameter is an unsigned parameter
                par_range = [-1,3,4,...] if parameter is an int parameter
                par_range = ['MyClass','',...] if parameter is a class parameter ^{.py}
            """
            self.par_range = par_range
            self.default_value = default_value
            self.type_par = type_par
        def __repr__(self):
            return self.name
            
    class DM:
        """ For static (nodal, elementary, ...) data """
        def __init__(self,name, dm_type='double', unit='', default_value=''):
            self.name = name
            self.dm_type = dm_type
            self.unit = unit
            self.default_value = default_value
        def __repr__(self):
            return 'DM_'+self.name
        def get_repr(self,number,sup_spaces=0):
            res = ''
            t = self.dm_type
            if string.find(self.dm_type,',')>=0 or string.find(self.dm_type,'>')>=0:
                res += ' '*sup_spaces + 'typedef %s T%s;\n' % ( self.dm_type, number )
                t = 'T%s' % number
            res += ' '*sup_spaces + 'CARACDMEXTNAME(%s,%s,%s,"%s");\n'%(number,t,self.name,self.unit)
            return res
        def __eq__(self,other):
            return self.name==other.name and self.dm_type==other.dm_type \
                and self.unit==other.unit and self.default_value==other.default_value
    
    class Carac:
        """ class that stores carac for mesh """
        def __init__(self):
            self.Tpos = 'double'
            self.nodaldata = []
            self.globaldata = []
            self.elemdata = []
            self.valid = True
    
    def get_parameters(self):
        """ Get parameters defined in config file """
        res = []
        for name,var in self.dict_e.items():
            if isinstance(var,self.Parameter):
                var.name = name
                res.append( var )
        return res
    
    def list_parameter_values(self):
        """ get a list with all possible values """
        d = 1
        par = []
        for var in self.get_parameters():
            d = d * len(var.par_range)
            par.append( var )
        res = []
        for i in range(d):
            pres = []
            all_none = True
            for p in par:
                par_value = p.par_range[ i%len(p.par_range) ]
                pres.append( (p,par_value) )
                if par_value != None: all_none = False
                i = i / len(p.par_range)
            if all_none: res = [ pres ] + res
            else : res.append( pres )
        return res
    
    def write_carac_for_particular_par(self,lst_par,name_mesh_carac,gnagna_DM,all_elements):
        """ return a string with specializations for lst_par
            and complete gnagna_DM with fields
        """
        lst_t = []
        for param,value in lst_par:
            if value==None:
                lst_t.append( param.type_par+' '+param.name )
    
        # set a new local context. Substitution of values
        old_glob = {}
        new_var = {}
        for name_var,var in self.dict_e.items():
            new_var[name_var] = var
            if isinstance(var,self.Parameter):
                for param,value in lst_par:
                    if param.name==var.name and value!=None:
                        new_var[name_var] = value
        for name_var,var in new_var.items():
            if name_var in globals(): old_glob[name_var] = globals()[name_var]
            globals()[name_var] = var
    
        # call of function carac    
        carac = self.Carac()
        self.dict_e["get_carac"](carac)
        if carac.valid == False:
            return ""
    
        #
        res = ""
        if len(lst_par):
            res += 'template<'+string.join(lst_t,',')+'>\n'
        res += 'class '+name_mesh_carac
        # if at least one param is specified, it is a specialization (class ...<...> {}; )
        if sum(map(lambda x:x[1]!=None,lst_par)):
            lst_sp = [ param.name*(value==None)+str(value)*(value!=None) for param,value in lst_par]
            res += '<'+string.join(lst_sp,',')+'>'
        res += ' { \n'
        res += 'public:\n'
        
        # use of carac
        res += '    static const unsigned dim = %s;\n' % carac.dim
        res += '    typedef %s Tpos;\n' % carac.Tpos
        res += '    typedef Vec<%s,%s> Pvec;\n' % (carac.Tpos,carac.dim)
        def out_dm(name,data,sup_spaces=0):
            res = ""
            res += ' '*sup_spaces + '    struct '+name+' {\n'
            if len(data):
                lst_def_val = []
                for dm in data:
                    if dm.default_value: lst_def_val.append( dm.name+'('+dm.default_value+')' )
                if len(lst_def_val):
                    res += ' '*sup_spaces + '        '+name+'():'+string.join(lst_def_val,',')+' {}\n'
                for dm,number in zip(data,range(len(data))):
                    gnagna_DM[dm.name] = 1
                    res += dm.get_repr(number,sup_spaces+8)
                res += ' '*sup_spaces + '        static const unsigned nb_params = %i;\n' % len(data)
            else:
                res += ' '*sup_spaces + '        VOIDDMSET;\n'
            
            res += '        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }\n'
            res += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
            res += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
            res += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
            res += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
            res += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
            res += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
	    for i in range(7):
	        res += '        void dm_data_set_field( const std::string field_name, const Mat<double, LMT::Gen<'+str(i)+', '+str(i)+'>, LMT::Dense<LMT::Col> > &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
            res += '        template<class __G__> __G__ dm_data_get_field( const std::string field_name, StructForType<__G__>, bool disp = true ) const { assert( 0 /*TODO*/ ); return __G__( 0.0 );  }\n'
            res += ' '*sup_spaces + '    };\n'
            return res
        carac.nodaldata.append( self.DM( name='pos', dm_type='Pvec', unit='m' ) )
        res += out_dm('NodalStaticData',carac.nodaldata)
        res += out_dm('GlobalStaticData',carac.globaldata)
        
        # elements
        res += '    template<unsigned nvi_to_subs,unsigned skin,unsigned num_sub_element,unsigned inner=0>\n'
        res += '    struct ElementChoice {\n'
        res += '        typedef void NE;\n'
        res += '        typedef DefaultBehavior BE;\n'
        res += '        typedef VoidDMSet TData;\n'
        res += '    };\n'
        
        tmp = ''
        all_edm = []
        for skin in [0,1]:
            for nvi_to_subs in range(skin,carac.nvi_to_subs_range+1):
                elements = self.dict_e["get_elem_choice"](nvi_to_subs,skin)
                for (NE,BE,CDM),num_sub_element in zip(elements,range(len(elements))):
                    all_elements[NE] = 1
                    tmp += '    template<unsigned inner> struct ElementChoice<%s,%s,%s,inner> {\n' % (nvi_to_subs,skin,num_sub_element)
                    tmp += '        typedef %s NE;\n' % NE
                    tmp += '        typedef %s BE;\n' % BE
                    if len(CDM):
                        similar_cdm = False
                        for i,num_i in zip(all_edm,range(len(all_edm))):
                            #print i,CDM,i==CDM
                            if i==CDM:
                                tmp += '        typedef TCDM%s TData;\n' % num_i
                                similar_cdm = True
                                break
                        if similar_cdm == False:
                            res += out_dm('TCDM%s'%len(all_edm),CDM)
                            tmp += '        typedef TCDM%s TData;\n' % len(all_edm)
                            all_edm.append( CDM )
                    else:
                        tmp += '        typedef VoidDMSet TData;\n'
                    
                    tmp += '        void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) { assert(0); /*TODO*/ }\n'
                    tmp += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,1> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
                    tmp += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,2> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
                    tmp += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,3> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
                    tmp += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,4> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
                    tmp += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,5> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
                    tmp += '        void dm_data_set_field( const std::string field_name, const Vec<Tpos,6> &value, bool disp = true ) { assert(0); /*TODO*/ }\n'
                    tmp += '    };\n'
        res += tmp
        res += '};\n\n'
        # reconstitution of globals
        for name_var,var in new_var.items(): del globals()[name_var]
        for n,i in old_glob.items(): globals()[n] = i
        
        return res
    
    def write_h_file(self):
        nameHEADER = 'LMT_' + string.upper( re.sub( '[/.]','_', self.name_file ) )
        print '#ifndef '+nameHEADER
        print '#define '+nameHEADER
        print 
        
        name_mesh_carac = string.capitalize( re.sub( '[.]','_', re.sub( '.*/','',self.name_file) ) )
        name_mesh_carac = string.replace( name_mesh_carac, '_meshcarac_py', '')

        # get combinaisons (sorted)
        lp = self.list_parameter_values()
        
    
        # write specializations    
        gnagna_DM = {}
        fbis = ""
        all_elements = {}
        for par in lp:
            fbis += self.write_carac_for_particular_par(par,name_mesh_carac,gnagna_DM,all_elements)

        for e in all_elements.keys():
            print '#include "mesh/'+string.lower(e)+'.h"'
                
        
        print 'namespace LMT {'
        print 
        
        # we make a basic declaration
        lst_sp = [ param.type_par+' '+param.name for param,value in lp[0] ]
        if len(lp):
            print 'template<'+string.join(lst_sp,',')+'>',
        print 'class '+name_mesh_carac+';\n'
        print 
        
        for name in gnagna_DM.keys():    
            print '#ifndef IFNDEF_'+name+'_DM'
            print '#define IFNDEF_'+name+'_DM'
            print '    struct '+name+'_DM {};'
            print '#endif // IFNDEF_'+name+'_DM\n'
        print fbis
        
        print 
        print '};'
        print 
        print '#endif // '+nameHEADER
        
    def __init__(self,name_file):
        """  """
        if not os.access( name_file, os.F_OK ):
            raise NameError, 'impossible to find %s file' % name_file
        self.dict_e = {
            "Parameter" : self.Parameter,
            "DM" : self.DM,
        }
        execfile( name_file, globals(), self.dict_e )
        self.name_file = name_file
        
def usage():
    print "Usage : "+sys.argv[0]+" mypycaracfile[.py]"
    print "  convert a mesh carac file to the mypycaracfile.h one."
    exit(0)

if len(sys.argv)==1: usage()

for name_file in sys.argv[1:]:
    mc = MeshCarac(name_file)
    mc.write_h_file()
    
