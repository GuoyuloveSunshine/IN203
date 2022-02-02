include Make_linux.inc

CXXFLAGS = -std=c++11 -fPIC
ifdef DEBUG
CXXFLAGS += -g -O0 -Wall -fbounds-check -pedantic -D_GLIBCXX_DEBUG
else
CXXFLAGS += -O3 -march=native -Wall
endif

ALL= Maitre_esclave.exe Mandelbrot_my.exe matvec_col.exe matvec_ligne.exe

default:	help

all: $(ALL)

clean:
	@rm -fr *.o *.exe *~ *.tga *.png

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)


help:
	@echo "Available targets : "
	@echo "    all            : compile all executables"
	@echo "    matvec.exe     : compile matrix vector product executable"
	@echo "    Mandelbrot_my.exe : compile Mandelbrot set computation executable"
	@echo "Add DEBUG=yes to compile in debug"
	@echo "Configuration :"
	@echo "    CXX      :    $(CXX)"
	@echo "    CXXFLAGS :    $(CXXFLAGS)"
