hacknet_LDADD = $(CURSESLIB) $(THREADLIB) $(SOCKETLIB) $(NSLLIB)
hacknetd_LDADD = $(SOCKETLIB) $(NSLLIB)

bin_PROGRAMS= hacknet hacknetd
hacknet_SOURCES = HN_Display.cpp HN_DisplayTTY.cpp MAP_Client.cpp HN_Point.cpp \
		HN_Version.cpp HN_Main_Client.cpp NET_Client.cpp NET_Packet.cpp

hacknetd_SOURCES = HN_Main_Server.cpp ENT_Base.cpp ENT_GridBug.cpp ENT_Human.cpp \
		OBJ_Base.cpp HN_Group.cpp HN_Game.cpp HN_Player.cpp HN_Dungeon.cpp MAP_Base.cpp \
		MAP_Client.cpp MAP_Hack.cpp MAP_Cavern.cpp HN_Version.cpp NET_Server.cpp \
		HN_Point.cpp NET_Packet.cpp
