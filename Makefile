CFLAGS = -O2 -g -std=gnu99 -Wall

test-rwlock: test-rwlock.o tbb_rwlock.o
	$(CC) $(CFLAGS) -lpthread $^ -o $@

clean:
	-rm -f *.o
	-rm -f test-rwlock
