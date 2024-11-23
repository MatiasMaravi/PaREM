GXX = mpic++
NAME1 = version1
NAME2 = version2
NAME3 = version3
P = 4
version1:
	$(GXX) -o $(NAME1).out $(NAME1).cpp && mpirun -np $(P) $(NAME1).out

version2:
	$(GXX) -o $(NAME2).out $(NAME2).cpp && mpirun -np $(P) $(NAME2).out
	
version3:
	$(GXX) -o $(NAME3).out $(NAME3).cpp && mpirun -np $(P) $(NAME3).out

clean:
	rm -f *.out