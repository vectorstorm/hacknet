#ifndef __HN_DUNGEON_H__
#define __HN_DUNGEON_H__

class mapBase;

class hnDungeon
{
	mapBase **		m_levelMap;
	int			m_levelCount;
	
	static hnDungeon *	s_instance;
	
				hnDungeon( int depth, int width, int height ) ;
				~hnDungeon();
public:
	static void 		Startup( int depth, int width, int height );
	static void 		Shutdown();
	
	static hnDungeon *	GetInstance();
	static mapBase * 	GetLevel( int levelNumber );

	int			GetLevelCount() { return m_levelCount; }
};



#endif

