#
# Run:
#  $ make
# to build all the example programs.
#

#----------------------------------------------------
# ARM code
CC := gcc
CFLAGS := -O3 -mfpu=vfpv3 -mfloat-abi=hard -march=armv7 -I./include 
# LDFLAGS := -lprussdrv

SRCS := main.c prussdrv.c adcdriver_host.c spidriver_host.c
OBJS := main.o prussdrv.o adcdriver_host.o spidriver_host.o
EXES := main
INCLUDEDIR := ./include
INCLUDES := $(addprefix $(INCLUDEDIR)/, prussdrv.h pru_types.h __prussdrv.h pruss_intc_mapping.h spidriver_host.h adcdriver_host.h)

#----------------------------------------------------
# PRU code
DEVICE=am335x
PRU_CC := clpru
PRU_CC_FLAGS := --silicon_version=3 -I/usr/share/ti/cgt-pru/include -I./include -I/usr/include/arm-linux-gnueabihf -i/usr/share/ti/cgt-pru/lib -D$(DEVICE)
PRU_LINKER_SCRIPT := AM335x_PRU.cmd
PRU_INCLUDES := resource_table_empty.h pru_ctrl.h pru_intc.h pru_cfg.h pru_spi.h

# PRU0 is the SPI stuff.
PRU0_SRCS := pru0.c pru_spi.c
PRU0_OBJS := pru0.obj pru_spi.obj
PRU0_MAP := pru0.map
PRU0_EXES := data0.bin text0.bin

# PRU1 is the oscillator
PRU1_SRCS := pru1.c
PRU1_OBJS := pru1.obj
PRU1_MAP := pru1.map
PRU1_EXES := data1.bin text1.bin

PRU_HEXPRU_SCRIPT := bin.cmd

#----------------------------------------------------
# DSP code
DSP_CC := g++
DSP_CFLAGS := -O3 -mfpu=vfpv3 -mfloat-abi=hard -march=armv7 -I./include -I./DSP -I./gnuplot_i/src
DSP_OBJS := dsp_main.o functs.o tables.o plot.o gnuplot_i.o wav_h_gen.o services.o algos.o mem_man.o wsfir.o prussdrv.o adcdriver_host.o spidriver_host.o

#----------------------------------------------------
# gnuplot_i code

GPI_CFLAGS 	= -O3 -I./src

#=================================================
all: main pru0.bin pru1.bin dsp ADC_001-00A0.dtbo

bins: main pru0.bin dsp

dsp-only: dsp

.PHONY: dsp dsp_main.o

#--------------------------------
# Compile ARM sources for host.
main.o: $(SRCS)
	echo "--> Building main.o"
	$(CC) $(CFLAGS) -c $< -o $@

adcdriver_host.o: adcdriver_host.c ./include/adcdriver_host.h
	echo "--> Building adcdriver_host.o"
	$(CC) $(CFLAGS) -c $< -o $@

spidriver_host.o: spidriver_host.c ./include/spidriver_host.h
	echo "--> Building spidriver_host.o"
	$(CC) $(CFLAGS) -c $< -o $@

prussdrv.o: prussdrv.c # $(DEPS)
	echo "--> Building prussdrv.o"
	$(CC) $(CFLAGS) -c $< -o $@

# Link the ARM objects
main: $(OBJS) 
	echo "--> Linking ARM stuff...."
	$(CC) $(CFLAGS) $^ $(LIBLOCS) $(LDFLAGS) -o $@

gnuplot_i.o: gnuplot_i/src/gnuplot_i.c gnuplot_i/src/gnuplot_i.h
	$(CC) $(GPI_CFLAGS) -c -o gnuplot_i.o gnuplot_i/src/gnuplot_i.c

dsp_main.o:
	echo "--> Building DSP...."
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/dsp_main.cpp -o dsp_main.o
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/functs.cpp -o functs.o
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/plot.cpp -o plot.o
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/tables.cpp -o tables.o
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/wav_h_gen.cpp -o wav_h_gen.o
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/services.cpp -o services.o
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/algos.cpp -o algos.o
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/mem_man.cpp -o mem_man.o
	$(DSP_CC) $(DSP_CFLAGS) -c DSP/wsfir.cpp -o wsfir.o

dsp: gnuplot_i.o dsp_main.o
	echo "--> Linking DSP...."
	$(DSP_CC) $(DSP_CFLAGS) $(DSP_OBJS) -o dsp_examples 
$(OBJS): $(INCLUDES)

#--------------------------------
# Compile and link the PRU sources to create ELF executable
pru0.out: pru0.c pru_spi.c
	echo "--> Building and linking PRU0 stuff..."
	$(PRU_CC) $^ $(PRU_CC_FLAGS) -z $(PRU_LINKER_SCRIPT) -o $@ -m $(PRU0_MAP)

# Build PRU .bin file from ELF
pru0.bin: pru0.out $(PRU_HEXPRU_SCRIPT)
	echo "--> Running hexpru for PRU0..."
	hexpru $(PRU_HEXPRU_SCRIPT) $<
	-mv text.bin text0.bin
	-mv data.bin data0.bin

pru1.out: pru1.c
	echo "--> Building and linking PRU1 stuff..."
	$(PRU_CC) $^ $(PRU_CC_FLAGS) -z $(PRU_LINKER_SCRIPT) -o $@ -m $(PRU1_MAP)
        
# Build PRU .bin file from ELF
pru1.bin: pru1.out $(PRU_HEXPRU_SCRIPT)
	echo "--> Running hexpru for PRU1..."
	hexpru $(PRU_HEXPRU_SCRIPT) $<
	-mv text.bin text1.bin
	-mv data.bin data1.bin

#--------------------------------
# Build and install device tree overlay
ADC_001-00A0.dtbo: ADC_001.dts
	dtc -O dtb -o ADC_001-00A0.dtbo -b 0 -@ ADC_001.dts
	cp ADC_001-00A0.dtbo /lib/firmware

#-------------------------------
# Clean up directory -- remove executables and intermediate files.
clean:
	-rm -f *.o *.obj *.out *.map $(EXES) $(OBJS) \
	 $(PRU0_OBJS) $(PRU0_EXES) $(PRU1_OBJS) $(PRU1_EXES) *~ *.dtbo



