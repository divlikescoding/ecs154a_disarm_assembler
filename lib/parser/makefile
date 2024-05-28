main: lex yacc
	mkdir bin/
	mv lex.yy.c bin/
	mv y.tab.* bin/

lex:
	lex -d config/rules.l

yacc:
	yacc -d config/rules.y

clean:
	rm -rf lex.yy.c y.tab.* *.o