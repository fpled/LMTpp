

#CFLAGS= -O2 -fomit-frame-pointer -Wall -pipe -funroll-loops -fno-inline -mcpu=athlon-xp -DCPU=athlon-xp
CFLAGS= -O2 -fomit-frame-pointer -pipe -funroll-loops -fno-inline 


OBJS=  token.o util2html.o opdisplay.o target.o comment.o commentitem.o commentitemtxt.o commentitemkeyword.o commentitemcode.o commentitemsection.o  commentitemtable.o commentitemlist.o commentitemwebpage.o commentitemtutorial.o commentitemexample.o main.o pagecomment.o bloc.o classe.o parameter.o templateparameter.o function.o struct.o typedef.o tagcomment.o visitorbloc.o ophtml.o opdebug.o documentation.o opallclass.o visitorcommentitem.o names.o visitorcommentitem_tohtml.o commentitemgenericcomment.o  listtarget.o visitorcommentitem_listtarget.o opstructure_site.o visitorbloc_setpath_html.o visitorbloc_allclass_tohtml.o opallexample.o visitorcommentitem_setpath_html.o visitorcommentitem_example_tohtml.o visitorcommentitem_is_extuge.o opalltutorial.o visitorcommentitem_tutorial_tohtml.o visitorcommentitem_genericcomment_tohtml.o opgetbrief_for_principalname.o visitorcommentitem_getbrief.o visitorcommentitem_getbrief_of_extuge.o listtargetcommentitem.o visitorbloc_what_type.o opgetbrief_for_name.o visitorcommentitem_webpage_tohtml.o opallitemwebpage.o visitorcommentitem_webpage_exists.o opwebpage_exists.o commentitemlatex.o visitorcommentitem_getkeyword_of_extuge.o opget_listkeyword.o visitorbloc_get_listkeyword.o  commentitemverbatim.o opallgenericcomment2html.o listparameter.o visitorbloc_getsignature.o target_type_without_genericcomment.o visitorcommentitem_is_internal.o opinternal.o searchindex.o classemetil.o system.o nstring.o lexer.o lexemdefinition.o lexem.o errorlist.o bigunsigned.o visitorbloc_getproperty.o functionmetil.o visitorbloc_getstructclassfunction.o visitorbloc_getinheritance.o

all: main


main: $(OBJS)
	g++ $(OBJS) -fpic -lm -lboost_filesystem -lboost_system -o main
	# cd ../..;./doc/src/main . doc

.cpp.o:
	g++ $(CFLAGS) -c $*.cpp -o $*.o

clean: 
	rm   *.o
