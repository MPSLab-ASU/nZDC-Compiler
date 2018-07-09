# nZDC-Compiler
A LLVM-3.7 compiler with nZDC error detection transfromation

Prerequisites:
There are several prerequisite software packages required to build and run LLVM compiler infrastructure and gem5 simulator. In this document we assume that all the required packages has been installed by the user and we focus on compiling benchmarks with nZDC soft error protection transformation and running them on gem5 simulator. We will be using three tools: 

1) LLVM-nZDC compiler to compile a program 
2) GCC cross-compiler  to generate static binary of the compiled programs
3) Gem5 gcc-aarch64-linux-gnu to execute the binary

# Build and Install: 

A) llvm-nZDC from here: 
	https://github.com/cmlasu/nZDC-Compiler/ 

mkdir  ./build

cd ./build

../llvm/configure

make -j2

B) GCC cross-compiler for ARM V8
    sudo apt-get install gcc-aarch64-linux-gnu

C) Install gem5 SE mode

    http://www.gem5.org/Download
    
    https://www.youtube.com/watch?v=SW63HJ0nW90

# Compiling benchmarks

./LLVM3.7/build/bin/clang -I /usr/arm-linux-gnueabi/include/  -O3 -static -emit-llvm -target armv8-none-eabi  -S  ./programs/mm.c -o ./programs/mm.ll

./LLVM3.7/build/bin/llc -O3 -reserveRegs=true -enable-nZDC=true -march=aarch64 ./programs/mm.ll  -o ./programs/mmopt-nZDC.s

aarch64-linux-gnu-gcc -static -O3 ./programs/mmopt-nZDC.s -o ./programs/mmopt-nZDC

Running nZDC-protected binaries on Gem5 Simulator

gem5$: build/ARM/gem5.opt configs/example/se.py -c ./programs/mmopt-nZDC
