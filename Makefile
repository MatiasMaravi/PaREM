p1_openmp_cpp:
	g++ -fopenmp -o p1_openmp_cpp p1.cpp && ./p1_openmp_cpp && rm p1_openmp_cpp

p2_mpi_cpp:
	mpic++ -o p2_mpi_cpp p2.cpp && mpirun -np 4 p2_mpi_cpp && rm p2_mpi_cpp

p3_mpi_cpp:
	mpic++ -o p3_mpi_cpp p3.cpp && mpirun -np 4 p3_mpi_cpp && rm p3_mpi_cpp

p4_omp_cpp:
	g++ -fopenmp -o p4_omp_cpp p4.cpp && ./p4_omp_cpp && rm p4_omp_cpp

prueba:
	mpic++ -o prueba prueba.cpp && mpirun -np 4 prueba && rm prueba