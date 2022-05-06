build:
	mpicc tema3.c -o tema3
run:
	mpirun -np 9 ./tema3 12 0 --oversubscribe

clean:
	rm tema3
