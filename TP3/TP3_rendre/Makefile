include Make_linux.inc

CXXFLAGS = -std=c++14
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
CXXFLAGS2 = CXXFLAGS
else
CXXFLAGS2 = ${CXXFLAGS} -O2 -march=native -Wall
CXXFLAGS += -O3 -march=native -Wall
endif

ALL= dotproduct.exe dotproduct_thread.exe TestProductMatrix.exe TestProductMatrix_Bloc.exe bitonic.exe bitonic_thread.exe bitonic_thread_corrige.exe bhudda.exe bhudda_omp.exe

default:	help

all: $(ALL)

clean:
	@rm -fr *.o *.exe *~ *.tga *.gp *.ppm *.out

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

dotproduct.exe:dotproduct.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIB)	
dotproduct_thread.exe:dotproduct_thread.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIB)	
TestProductMatrix.exe : TestProductMatrix.cpp Matrix.hpp Matrix.cpp ProdMatMat.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIB)	
TestProductMatrix_Bloc.exe : TestProductMatrix_Bloc.cpp Matrix.hpp Matrix.cpp ProdMatMat_Bloc.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIB)	
bitonic.exe : bitonic.cpp Vecteur.hpp Vecteur.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)	
bitonic_thread.exe : bitonic_thread.cpp Vecteur.hpp Vecteur.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)	
bitonic_thread_corrige.exe : bitonic_thread_corrige.cpp Vecteur.hpp Vecteur.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)	


bhudda_omp.exe:bhudda_omp.cpp
	$(CXX) $(CXXFLAGS2) $^ -o $@ $(LIB)	

help:
	@echo "Available targets : "
	@echo "    all            : compile all executables"
	@echo "    matvec.exe     : compile matrix vector product executable"
	@echo "    Mandelbrot.exe : compile Mandelbrot set computation executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"
