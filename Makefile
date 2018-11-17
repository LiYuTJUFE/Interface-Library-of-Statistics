.PHONY: vlstar test

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

OBJS= utilities.o simulation.o data.o variables.o estimation.o

test.exe: test.o utilities.o
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB}  ${PETSC_TS_LIB}

mlr.exe: mlr.o app_mlr.o $(OBJS)
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB}  ${PETSC_TS_LIB}

vlstar.exe: vlstar.o app_vlstar.o $(OBJS)
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB}  ${PETSC_TS_LIB}

test: test.exe
	./$<

mlr: mlr.exe
	./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-6

vlstar: vlstar.exe
	./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-8
#	valgrind  --tool=memcheck --leak-check=full -v --track-origins=yes --show-reachable=yes --read-var-info=yes ./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-4

