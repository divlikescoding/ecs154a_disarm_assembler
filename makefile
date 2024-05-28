assembler: 
	gcc -g -c -o yacc.o lib/parser/bin/y.tab.c
	g++ -g -c -o token_schema.o lib/parser/src/token_schema.cpp
	g++ -g -c -o main.o src/main.cpp
	gcc -g -v -o disarm_as yacc.o token_schema.o main.o -lstdc++
	rm *.o

clean:
	rm -rf *.o disarm_as