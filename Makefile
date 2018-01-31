# Makefile for tex2gr
tex2gr: main.cpp
	icpc -O3 -o tex2gr main.cpp
