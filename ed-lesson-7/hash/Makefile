TESTFLAGS=-fno-sanitize=all -fPIC -Wvla -Werror -fsanitize=address -g

hash.o: hash.c hash.h
	gcc -fsanitize=address -fPIC -c hash.c -g -Werror -Wvla

test.o: hash.o test-hash.c
	gcc $^ -o $@ $(TESTFLAGS)
