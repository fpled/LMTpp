#!/usr/bin/env python
import sys,string,copy
from formulation import *
from interpolation import *

def replace_dim_bis( X, dim ):
    if X in ['dim','DIM']: return dim
    return X
def replace_dim( lst, dim ):
    return map( lambda x:replace_dim_bis( x, dim ), lst )

class Problem:
    def __init__(self,
            name,
            formulations,
            elements,
            dim='nvi',
            incpaths=['.','LMT/formulations'],
            options={},
            additional_fields = {}):
        self.name = name
        self.formulations = formulations
        self.elements = elements
        self.dim_policy = dim
        self.incpaths = incpaths
        self.options = options
        self.additional_fields = additional_fields

    def write_mesh_carac( self, dim, fe_sets, output ):
        # get mesh data
        all_vars,all_is_vars = {},{}
        for f,e in fe_sets:
            if f.dim==dim:
               for namevar,var in f.__dict__.items():
                  if isinstance(var,Variable):
                    if namevar in all_vars: all_vars[namevar].join(var)
                    else:                   all_vars[namevar] = var
                  if isinstance(var,ISVariable):
                    if namevar in all_is_vars: all_is_vars[namevar].join(var)
                    else:                      all_is_vars[namevar] = var
        # additional_fields
        for namevar,var in self.additional_fields.items():
            if namevar in all_vars:
                all_vars[namevar].join(var)
            else:
                all_vars[namevar] = var

        # mesh_carac_...
        for n,v in all_vars.items():
            for d in range(v.nb_der+1):
                nn = ('der'+str(d)+'_')*(d!=0) + n
                output.write( "#ifndef IFNDEF_%s_DM\n" % nn )
                output.write( "#define IFNDEF_%s_DM\n" % nn )
                output.write( "    struct %s_DM { static std::string name() { return \"%s\"; } };\n" % ( nn, nn ) )
                output.write( "#endif // IFNDEF_%s_DM\n" % nn )
                output.write( "\n" )

        output.write( "template<class TP>\n" )
        output.write( "struct Mesh_carac_%s<TP,%i> {\n" % (self.name,dim) )
        output.write( "    typedef TP Tpos;\n" )
        output.write( "    static const unsigned dim = %i;\n" % dim )
        output.write( "    typedef Vec<TP,%i> Pvec;\n" % dim )

        #
        def write_static_data( nf, cond, in_vec, nb_sp = 4 ):
            #
            output.write( ' '*nb_sp+"struct %s {\n"%nf )

            # get typenames
            type_vars = {}
            for namevar,var in all_vars.items():
                if cond(var):
                    T = var.type_to_str(in_vec(var),cond(var))
                    if not T in type_vars: type_vars[T] = "T"+str(len(type_vars))
            for Ti,To in type_vars.items():
                output.write( ' '*nb_sp+'    typedef '+Ti+' '+To+';\n' )

            #
            output.write( ' '*nb_sp+"    %s()"%nf )
            cpt = 0
            for namevar,var in all_vars.items():
                if cond(var):
                    if type(var.default_value) is list:
                        default_value = var.default_value + ["0.0"] * var.nb_der
                    else:
                        default_value = [ var.default_value ] + ["0.0"] * var.nb_der
                    for i in range(0,var.nb_der+1):
                        if str(default_value[i])=="":
                            continue
                        output.write( [':',','][cpt!=0]+('der%i_'%i)*(i!=0)+namevar+'('+str(default_value[i])+')' )
                        cpt += 1
            output.write( " {}\n" )
            # fields
            cpt = 0
            for namevar,var in all_vars.items():
                if cond(var):
                    if var.dont_use_caracdm:
                        output.write( ' '*nb_sp+'    '+type_vars[var.type_to_str(in_vec(var),cond(var))]+' '+namevar+';\n')
                    else:
                        for i in range(0,var.nb_der+1):
                            der_str = ("der"+str(i)+"_")*(i!=0)
                            output.write( ' '*nb_sp+'    CARACDMEXTNAME( '+str(cpt)+', '+type_vars[var.type_to_str(in_vec(var),cond(var))]+', '+der_str+namevar+', "'+var.unit+'" );\n')
                            cpt += 1
            if cpt==0: output.write( ' '*nb_sp+"    VOIDDMSET;\n" )
            else: output.write( ' '*nb_sp+"    static const unsigned nb_params = "+str(cpt)+";\n" )

            # dm_data_set_field
            output.write( ' '*nb_sp+"    void dm_data_set_field( const std::string field_name, Tpos value, bool disp = true ) {\n" )
            for namevar, var in all_vars.items():
                if cond( var ) and len( var.nb_dim ) <= 1 and not var.id_dependant:
                    if in_vec( var ):
                        for i in range( cond(var) ):
                            if len( var.T ) == 0:
                                output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { '+namevar+'['+str(i)+'] = value; return; } // hum\n' )
                    else:
                        if len( var.T ) == 0:
                            output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { '+namevar+' = value; return; }\n' )
            output.write( ' '*nb_sp+'        if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }\n' )
            output.write( ' '*nb_sp+"    }\n" )
            for d in range( 1, 7 ):
                output.write( ' '*nb_sp+"    void dm_data_set_field( const std::string field_name, const Vec<Tpos,"+str(d)+"> &value, bool disp = true ) {\n" )
                for namevar, var in all_vars.items():
                    if cond( var ) and var.nb_dim == [d] and not var.id_dependant:
                        if in_vec( var ):
                            for i in range( cond(var) ):
                                if len( var.T ) == 0:
                                    output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { '+namevar+'['+str(i)+'] = value; return; } // hum\n' )
                        else:
                            if len( var.T ) == 0:
                                output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { '+namevar+' = value; return; }\n' )
                output.write( ' '*nb_sp+'        if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }\n' )
                output.write( ' '*nb_sp+"    }\n" )
            for d in range( 1, 7 ):
                output.write( ' '*nb_sp+"    void dm_data_set_field( const std::string field_name, const Mat<Tpos,Gen<"+str(d)+"> > &value, bool disp = true ) {\n" )
                for namevar, var in all_vars.items():
                    if cond( var ) and var.nb_dim == [d,d] and not var.sym and not var.id_dependant:
                        if in_vec( var ):
                            for i in range( cond(var) ):
                                if len( var.T ) == 0:
                                    output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { '+namevar+'['+str(i)+'] = value; return; } // hum\n' )
                        else:
                            if len( var.T ) == 0:
                                output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { '+namevar+' = value; return; }\n' )
                output.write( ' '*nb_sp+'        if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }\n' )
                output.write( ' '*nb_sp+"    }\n" )

            # dm_data_get_field
            output.write( ' '*nb_sp+"    Tpos dm_data_get_field( const std::string field_name, StructForType<Tpos>, bool disp = true ) const {\n" )
            for namevar, var in all_vars.items():
                if cond( var ) and len( var.nb_dim ) == 0 and not var.id_dependant:
                    if in_vec( var ):
                        for i in range( cond(var) ):
                            if len( var.T ) == 0:
                                output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { return '+namevar+'['+str(i)+']; } // hum\n' )
                    else:
                        if len( var.T ) == 0:
                            output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { return '+namevar+'; }\n' )
            output.write( ' '*nb_sp+'        if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }\n' )
            output.write( ' '*nb_sp+"        return Tpos(0);\n" )
            output.write( ' '*nb_sp+"    }\n" )
            for d in range( 1, 7 ):
                output.write( ' '*nb_sp+"    Vec<Tpos,"+str(d)+"> dm_data_get_field( const std::string field_name, StructForType<Vec<Tpos,"+str(d)+"> >, bool disp = true ) const {\n" )
                for namevar, var in all_vars.items():
                    if cond( var ) and var.nb_dim == [d] and not var.id_dependant:
                        if in_vec( var ):
                            for i in range( cond(var) ):
                                if len( var.T ) == 0:
                                    output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { return '+namevar+'['+str(i)+']; } // hum\n' )
                        else:
                            if len( var.T ) == 0:
                                output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { return '+namevar+'; }\n' )
                output.write( ' '*nb_sp+'        if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }\n' )
                output.write( ' '*nb_sp+"        return Vec<Tpos,"+str(d)+">();\n" )
                output.write( ' '*nb_sp+"    }\n" )
            for d in range( 1, 7 ):
                output.write( ' '*nb_sp+"    Mat<Tpos,Gen<"+str(d)+"> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Gen<"+str(d)+","+str(d)+"> > >, bool disp = true ) const {\n" )
                for namevar, var in all_vars.items():
                    if cond( var ) and var.nb_dim == [d, d] and var.sym and not var.id_dependant:
                        if in_vec( var ):
                            for i in range( cond(var) ):
                                if len( var.T ) == 0:
                                    output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { return '+namevar+'['+str(i)+']; } // hum\n' )
                        else:
                            if len( var.T ) == 0:
                                output.write( ' '*nb_sp+'        if ( field_name == "'+namevar+'" ) { return '+namevar+'; }\n' )
                output.write( ' '*nb_sp+'        if ( disp == true ) { std::cerr << "There is no variable named " << field_name << " in data struct" << std::endl; }\n' )
                output.write( ' '*nb_sp+"        return Mat<Tpos,Gen<"+str(d)+","+str(d)+"> >();\n" )
                output.write( ' '*nb_sp+"    }\n" )
            for d in range( 1, 4 ):
                output.write( ' '*nb_sp+"    Mat<Tpos,Sym<"+str(d)+"> > dm_data_get_field( const std::string field_name, StructForType<Mat<Tpos,Sym<"+str(d)+"> > >, bool disp = true ) const {\n" )
                output.write( ' '*nb_sp+"        assert( 0 /*TODO*/ );\n" )
                output.write( ' '*nb_sp+"        return Mat<Tpos,Sym<"+str(d)+"> >();\n" )
                output.write( ' '*nb_sp+"    }\n" )

            output.write( ' '*nb_sp+"};\n" )

        class Toto:
            def get_nb_var(self,name): return 0
            def sum_nb_nodes_of_each_children(self): return 0
            nb_nodes = 0
            nb_gauss_points = 0
        write_static_data( 'NodalStaticData',  lambda var:std_interpolations[var.interpolation](Toto()).nb_nodal,  lambda var:std_interpolations[var.interpolation](Toto()).in_vec )
        write_static_data( 'GlobalStaticData', lambda var:std_interpolations[var.interpolation](Toto()).nb_global, lambda var:std_interpolations[var.interpolation](Toto()).in_vec )

        output.write( "    typedef Node<dim,Tpos,NodalStaticData> TNode;\n" )
        output.write( "    typedef ElementAncestor<TNode> EA;\n" )

        # elements
        output.write( "    template<unsigned nvi_to_subs,unsigned skin,unsigned num_sub_element,unsigned inner=0> struct ElementChoice { typedef void NE; typedef DefaultBehavior BE; typedef VoidDMSet TData; };\n" )

        def find_sub_elems_rec(elements,nvi):
            for e in elements[nvi]:
                cn = {}
                for c in e.children: cn[c['name']] = 1
                for c in cn.keys():
                    if not c in [ie.name for ie in elements[nvi+1]]:
                        axy = ( 'behavior_simplification' in self.options and self.options['behavior_simplification']=='axysymmetric' )
                        elements[nvi+1].append( Element(c,self.dim,self.incpaths) )
            if nvi<self.dim: find_sub_elems_rec(elements,nvi+1)
        elements = {0:[],1:[],2:[],3:[],4:[]}
        for f,e in fe_sets:
            if e.dim == dim:
                elements[0].append( e )
        find_sub_elems_rec(elements,0)
        #print >> sys.stderr, e.name
        #print >> sys.stderr, std_interpolations[ "gauss" ]( e ).nb_elementary

        for nvi, el in elements.items():
            cpt = 0
            for e in el:
                output.write( '    template<unsigned skin,unsigned inner> struct ElementChoice<'+str(nvi)+',skin,'+str(cpt)+',inner> { typedef '+e.name+' NE; typedef DefaultBehavior BE; ' )
                if nvi >= 2:
                    output.write( 'typedef VoidDMSet TData;' )
                elif nvi == 1:
                    output.write( '\n' )
                    write_static_data( 'TData', lambda var:std_interpolations[var.interpolation]( e ).nb_skin_elementary, lambda var:std_interpolations[var.interpolation]( e ).in_vec, 8 )
                else:
                    output.write( '\n' )
                    write_static_data( 'TData', lambda var:std_interpolations[var.interpolation]( e ).nb_elementary, lambda var:std_interpolations[var.interpolation]( e ).in_vec, 8 )
                output.write( '    };\n' )
                #if nvi:
                #    output.write( '    template<unsigned inner> struct ElementChoice<'+str(nvi)+',1,'+str(cpt)+',inner> { typedef '+e.name+' NE; typedef DefaultBehavior BE; typedef VoidDMSet TData; };\n' )
                cpt += 1

        # that's all for mesh carac
        output.write( "};\n" )

    # return fe_sets and all_dims
    def get_fe_sets_and_dims(self):
        fe_sets = []
        all_dims = {}
        for ne in self.elements:
            e = Element(ne,3,self.incpaths)
            nvi = e.nb_var_inter
            exec( 'dim = '+self.dim_policy )
            for n,v in self.additional_fields.items():
                for d in v.nb_dim:
                    exec( 'd = '+d )
            all_dims[dim] = 1
            e = Element(ne,dim,self.incpaths)
            for nf in self.formulations:
                f = Formulation(nf,dim,0,self.incpaths,options=self.options)
                f.set_variable_expressions( e )
                
                print >> sys.stderr, e.name, "nb_gauss_points =", e.nb_gauss_points
                fe_sets.append( (f, e) )
        return fe_sets, all_dims

    #
    def write_all_mesh_carac(self, output, fe_sets, all_dims ):
        ifndef = 'Mesh_carac_%s_HEADER' % self.name
        output.write( '#ifndef %s\n' % ifndef )
        output.write( '#define %s\n' % ifndef )
        output.write( '#include "mesh/displayparaview.h"\n' )
        for e in self.elements:
            output.write( '#include "mesh/' + e +'.h"\n' )
        output.write( 'namespace LMT {\n' )
        output.write( '\n' )
        output.write( "template<class TP,unsigned dim> struct Mesh_carac_%s {};\n"%self.name )
        for dim in all_dims.keys():
            self.dim = dim

            cp = copy.deepcopy( self.additional_fields )

            for n,v in self.additional_fields.items():
                v.T = string.replace( v.T, 'dim', str(dim) )
                v.T = string.replace( v.T, 'DIM', str(dim) )
                v.nb_dim = replace_dim( v.nb_dim, dim )

            self.write_mesh_carac( dim, fe_sets, output )

            self.additional_fields = cp

        output.write( '} // namespace LMT\n' )
        output.write( '#endif // %s\n' % ifndef )

    # write all in a single file
    def write( self, output = sys.stdout, name_der_vars = [] ):
        fe_sets, all_dims = self.get_fe_sets_and_dims()

        ifndef = self.name
        output.write( '#ifndef PROBLEM_' + ifndef + '\n' )
        output.write( '#define PROBLEM_' + ifndef + '\n' )
        output.write( '\n' )
        # output.write( '#include "formulation/formulation.h"\n' )
        #for f, e in fe_sets:
            #output.write( '#include "mesh/' + e.name + '.h"\n' )
        #output.write( '\n' )

        # mesh carac
        self.write_all_mesh_carac( output, fe_sets, all_dims )
        
        # formulations
        for f, e in fe_sets:
            f.write( e, output, name_der_vars = name_der_vars )

        output.write( '\n' )

        output.write( '\n' )
        output.write( '#endif // PROBLEM_' + ifndef + '\n' )

def write_pb(name,formulations,elements,dim='nvi',output=sys.stdout,incpaths=['.','LMT/formulations'],options={},additional_fields={},types=['double'],name_der_vars=[]):
    """    dim -> for one element, say number of dimension we need
        Ex : write_pb('toto',[...],['Triangle','Tetra'],'3') -> Triangle and Tetra will cohabite in a mesh in 3D
        Ex : write_pb('toto',[...],['Triangle','Tetra'],'nvi') -> 2 meshes will be created. One in 2D with triangles and one in 3D with tetras
        Ex : write_pb('toto',[...],['Triangle','Quad','Tetra'],'nvi') -> 2 meshes will be created. One in 2D with [triangles,quad] and one in 3D with tetras
        Ex : write_pb('toto',[...],['Triangle','Bar'],'nvi+1') -> 2 meshes will be created. One in 2D with bars and one in 3D with Triangles

        options will be passed as local scope to def formulation() and apply_on_elements_after_solve...(...)
        Ex : options = {
            'behavior_simplification' : 'plane stress',
        } will cause ... Example of behavior_simplification : 'plane strain', 'axysymmetric', ...
    """
    pb = Problem(name,formulations,elements,dim,incpaths,options,additional_fields)
    pb.write( output, name_der_vars )

