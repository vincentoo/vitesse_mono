#=========================================================
#Author		: 	vincenzo
#Date		: 	2015.9.15 0:28
#Discription	:
#=========================================================

HOST := 

export CC := $(HOST)gcc
export XX := $(HOST)g++
export LIBS := -lpthread -lm
#export CFLAGS := -I$(TOPDIR)/include -D__DEBUG__
export CFLAGS := -I$(TOPDIR)/include
export LDFLAGS := -g -O2 -Wl
export RM := rm -rf
