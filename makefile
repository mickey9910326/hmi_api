
VERSION = 0.1.5

M128_PATH = ./m128/src
M128_DIST = ./dist/asa_hmi_m128_$(VERSION).tar.gz

MATLAB_PATH = ./pc/matlab/funs
MATLAB_DIST = ./dist/asa_hmi_matlab_$(VERSION).tar.gz

release:
	tar -zc -C $(M128_PATH) -f $(M128_DIST) ./
	tar -zc -C $(MATLAB_PATH) -f $(MATLAB_DIST) ./
