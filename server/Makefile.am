bin_PROGRAMS= hacknetd

hacknetd_LDADD =						\
		$(THREADLIB)					\
		../common/libpacket.a 				\
		../common/libpoint.a 				\
		../common/libstatus.a 				\
		../common/libmapclient.a 			\
		../common/librandom.a 
hacknetd_SOURCES =  						\
								\
		HN_Main_Server.cpp				\
		HN_Group.cpp HN_Group.h 			\
		HN_Game.cpp HN_Game.h 				\
		HN_Player.cpp HN_Player.h 			\
		HN_Dungeon.cpp HN_Dungeon.h 			\
		HN_Config_Server.h 				\
		HN_Properties.h					\
		HN_Convert.h					\
								\
		NET_Server.cpp NET_Server.h			\
								\
		MAP_Base.cpp MAP_Base.h 			\
		MAP_Hack.cpp MAP_Hack.h 			\
		MAP_Cavern.cpp MAP_Cavern.h 			\
								\
		ENT_Convert.h					\
		ENT_Base.cpp ENT_Base.h 			\
		ENT_Definition.cpp ENT_Definition.h 		\
		ENT_GridBug.cpp ENT_GridBug.h 			\
		ENT_Human.cpp ENT_Human.h			\
		ENT_Manager.cpp ENT_Manager.h			\
								\
		OBJ_Convert.h 					\
		OBJ_Base.cpp OBJ_Base.h				\
		OBJ_Definition.cpp OBJ_Definition.h		\
		OBJ_Amulet.cpp OBJ_Amulet.h 			\
		OBJ_Armour.cpp OBJ_Armour.h			\
		OBJ_Manager.cpp OBJ_Manager.h 			\
		OBJ_Potion.cpp OBJ_Potion.h			\
		OBJ_Ring.cpp OBJ_Ring.h				\
		OBJ_Food.cpp OBJ_Food.h				\
		OBJ_Weapon.cpp OBJ_Weapon.h			

INCLUDES = -I../common/
