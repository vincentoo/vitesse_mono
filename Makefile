#=========================================================
#Author		: 	vincenzo
#Date		: 	2015.9.15 0:28
#Discription	:
#=========================================================

export TOPDIR := $(PWD)

include config.mk

TARGET := monod

COBJS = $(patsubst %.c,%.o,$(wildcard *.c))

all:$(COBJS)
	$(CC) $(CFLAGS) $(LIBS) $(LDFLAGS) *.o -o $(TARGET)

%.dep : %.c
	$(CC) -MM $(CFLAGS) $< > $@
-include $(COBJS:.o=.dep)

clean:
	@$(RM) *.o $(TARGET)

distclean:
	@$(RM) *.o $(TARGET)
	@$(RM) *.dep