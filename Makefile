.PHONY: all clean cleanall test

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

OBJS= utilities.o simulation.o app.o data.o variables.o estimation.o

vlstar: vlstar.o $(OBJS)
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB}  ${PETSC_TS_LIB}

test: vlstar.exe
	./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-4
#	valgrind  --tool=memcheck --leak-check=full -v --track-origins=yes --show-reachable=yes --read-var-info=yes ./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-4

