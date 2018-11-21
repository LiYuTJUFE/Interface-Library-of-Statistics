.PHONY: ex1 mlr vmlr vlstar

include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules

OBJS= utilities.o data.o variables.o simulation.o estimation.o test.o

ex1.exe: ex1.o utilities.o
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB} ${PETSC_TS_LIB}

mlr.exe: mlr.o app_mlr.o $(OBJS)
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB} ${PETSC_TS_LIB}

vmlr.exe: vmlr.o app_vmlr.o $(OBJS)
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB} ${PETSC_TS_LIB}

vlstar.exe: vlstar.o app_vlstar.o $(OBJS)
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB} ${PETSC_TS_LIB}

vlstar0.exe: vlstar0.o app_vlstar0.o $(OBJS)
	-${CLINKER} -o $@ $^ ${PETSC_TAO_LIB} ${PETSC_TS_LIB}

ex1: ex1.exe
	./$<

mlr: mlr.exe
	./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-6

vmlr: vmlr.exe
	./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-6

vlstar0: vlstar0.exe
	./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-8
#	valgrind  --tool=memcheck --leak-check=full -v --track-origins=yes --show-reachable=yes --read-var-info=yes ./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-8

vlstar: vlstar.exe
	./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-8
#	valgrind  --tool=memcheck --leak-check=full -v --track-origins=yes --show-reachable=yes --read-var-info=yes ./$< -tao_smonitor -tao_type nls -tao_gatol 1.e-4

