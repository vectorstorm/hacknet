bin_PROGRAMS = hacknet.tty

hacknet_tty_LDADD = $(CURSESLIB) $(THREADLIB) ../client/libclient.a ../common/libpacket.a \
			../common/libpoint.a ../common/libstatus.a
hacknet_tty_SOURCES = HN_DisplayTTY.cpp HN_DisplayTTY.h HN_Main_Client.cpp

INCLUDES = -I../client -I../common
