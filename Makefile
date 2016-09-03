EXE = fastcgi
CFLAGS = -Wall
LDFLAGS = -lfcgi -lm

$(EXE): main.o
	$(CC) -o $@ $< $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f *.o $(EXE)
