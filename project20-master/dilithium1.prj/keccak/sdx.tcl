# ==============================================================
# File generated on Wed Feb 05 16:21:33 EST 2020
# Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
# SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
# IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
# Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
# ==============================================================
add_files -tb ../../newtest_sign.c -cflags { -Wno-unknown-pragmas}
add_files sign.c
add_files rounding.c
add_files rng.c
add_files reduce.c
add_files polyvec.c
add_files poly.c
add_files packing.c
add_files ntt.c
add_files fips202.c
add_files aes.c
set_part xc7a200tfbg676-2
create_clock -name default -period 10
config_interface -clock_enable=0
config_interface -expose_global=1
config_interface -m_axi_addr64=0
config_interface -m_axi_offset=off
config_interface -register_io=off
config_interface -trim_dangling_port=0
