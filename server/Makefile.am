bin_PROGRAMS= hacknetd

hacknetd_LDADD = ../common/libpacket.a ../common/libpoint.a ../common/libstatus.a \
		../common/libmapclient.a
hacknetd_SOURCES = HN_Main_Server.cpp ENT_Base.cpp ENT_Base.h ENT_GridBug.cpp ENT_GridBug.h \
		ENT_Human.cpp ENT_Human.h OBJ_Base.cpp OBJ_Base.h HN_Group.cpp HN_Group.h \
		HN_Game.cpp HN_Game.h HN_Player.cpp HN_Player.h HN_Dungeon.cpp HN_Dungeon.h \
		MAP_Base.cpp MAP_Base.h MAP_Hack.cpp MAP_Hack.h MAP_Cavern.cpp MAP_Cavern.h \
		NET_Server.cpp NET_Server.h HN_Config_Server.h \
		OBJ_Manager.cpp OBJ_Manager.h OBJ_Weapon.cpp OBJ_Weapon.h\
		OBJ_Definition.cpp OBJ_Definition.h

INCLUDES = -I../common/
