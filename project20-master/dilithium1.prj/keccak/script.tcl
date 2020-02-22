############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project dilithium1.prj
set_top KeccakF1600_StatePermute
add_files aes.c
add_files fips202.c
add_files ntt.c
add_files packing.c
add_files poly.c
add_files polyvec.c
add_files reduce.c
add_files rng.c
add_files rounding.c
add_files sign.c
add_files -tb newtest_sign.c
open_solution "keccak"
set_part {xc7a200tfbg676-2}
create_clock -period 10 -name default
config_interface -clock_enable=0 -expose_global -m_axi_addr64=0 -m_axi_offset off -register_io off -trim_dangling_port=0
#source "./dilithium1.prj/keccak/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
