
VERSION = 0.1.6

M128_PATH = ./m128/src
M128_DIST = ./dist/asa_hmi_m128_$(VERSION).tar.gz

MATLAB_PATH = ./pc/matlab/src
MATLAB_DIST = ./dist/asa_hmi_matlab_$(VERSION).tar.gz

release:
	tar -zc -C $(M128_PATH) -f $(M128_DIST) ./
	tar -zc -C $(MATLAB_PATH) -f $(MATLAB_DIST) ./
