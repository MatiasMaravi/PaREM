p1_openmp_cpp:
	g++ -fopenmp -o p1_openmp_cpp p1.cpp && ./p1_openmp_cpp && rm p1_openmp_cpp

p2_mpi_cpp:
	mpic++ -o p2_mpi_cpp p2.cpp && mpirun -np 4 p2_mpi_cpp && rm p2_mpi_cpp
