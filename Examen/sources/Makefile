include Make_linux.inc
#include Make_intel.inc
# include Make_msys2.inc
#include Make_osx.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS += -O3 -march=core-avx2 -Wall
endif

default:	help

all:	fourier_compression.exe fourier_compression_OMP.exe fourier_compression_MPI.exe

lodepng/lodepng.cpp:
	git submodule init
	git submodule update

fourier_compression.exe:	fourier_compression.cpp lodepng/lodepng.cpp
	$(CXX) $(CXXFLAGS) -o fourier_compression.exe fourier_compression.cpp lodepng/lodepng.cpp $(LIB)

fourier_compression_OMP.exe:	fourier_compression_OMP.cpp lodepng/lodepng.cpp
	$(CXXomp) $(CXXFLAGS) -o fourier_compression_OMP.exe fourier_compression_OMP.cpp lodepng/lodepng.cpp $(LIB)

fourier_compression_MPI.exe:	fourier_compression_MPI.cpp lodepng/lodepng.cpp
	$(CXX) $(CXXFLAGS) -o fourier_compression_MPI.exe fourier_compression_MPI.cpp lodepng/lodepng.cpp $(LIB)

help: 
	@echo "Available targets : "
	@echo "    all                               : compile all executables"
	@echo "    fourier_compression.exe           : compile HelloWorld executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"

clean:
	@rm -f *.exe *~ *.dat

