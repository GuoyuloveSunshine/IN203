include Make_linux.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -Weffc++ -D_GLIBCXX_DEBUG
CXXFLAGS2 = CXXFLAGS
else
CXXFLAGS += -O3 -march=native
CXXFLAGS2 = ${CXXFLAGS} -O2 -march=native -Wall
endif




ALL = Mandelbrot_tbb.exe Mandelbrot_mpi.exe Mandelbrot_openmp.exe Maitre_esclave.exe	Mandelbrot_seq.exe raytracer.exe raytracer_tbb.exe

# Mandelbrot_tbbthread.exe
default:	help

all: $(ALL)

clean:
	@rm -fr *.o *.exe *~ *.tga *.ppm

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

Mandelbrot_seq.exe: Mandelbrot_seq.cpp
	$(CXX2) $(CXXFLAGS2) $^ -o $@ $(LIB2)

Mandelbrot_mpi.exe: Mandelbrot_mpi.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

Maitre_esclave.exe: Maitre_esclave.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

Mandelbrot_tbb.exe: Mandelbrot_tbb.cpp 
	$(CXX2) $(CXXFLAGS2) $^ -o $@ $(LIB2)

Mandelbrot_openmp.exe : Mandelbrot_openmp.cpp
	$(CXX2) $(CXXFLAGS) -o $@ $^ $(LIB2)

# Mandelbrot_tbthread.exe: Mandelbrot_tbbthread.cpp 
# 	$(CXX2) $(CXXFLAGS2) $^ -o $@ $(LIB2)	

raytracer.exe: raytracer.cpp
	$(CXX2) $(CXXFLAGS2) $^ -o $@ $(LIB2)	
raytracer_tbb.exe: raytracer_tbb.cpp
	$(CXX2) $(CXXFLAGS2) $^ -o $@ $(LIB2)	

help:
	@echo "Available targets : "
	@echo "    all            : compile all executables"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"
