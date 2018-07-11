CROSS_GCC = /opt/air_toolchains/products/Air4920m/bin/arm-uclibc-linux-2.6.36-gcc
HOST_GCC = gcc
LD_FLAGS = -lrt

default: all

all:
	$(HOST_GCC) writer.c -o writer $(LD_FLAGS)
	$(HOST_GCC) reader.c -o reader $(LD_FLAGS)
	$(CROSS_GCC) writer.c -o writer_arm $(LD_FLAGS)
	$(CROSS_GCC) reader.c -o reader_arm $(LD_FLAGS)

clean:
	rm -f writer reader writer_arm reader_arm
