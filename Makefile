all:
#	make -C Shared/Release
#	make -C matelib/Release
	make -C Kernel
	make -C Memoria
	make -C Swap


clean:
	make clean -C Kernel
	make clean -C Memoria
	make clean -C Swap