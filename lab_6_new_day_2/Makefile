ifneq (${KERNELRELEASE},)
    obj-m:=proba_1_2.o
else
    KERNEL_SOURCE:=/lib/modules/3.11.0-26-generic/build
    PWD:=$(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules
clean:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
endif