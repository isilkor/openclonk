/*
 * OpenClonk, http://www.openclonk.org
 *
 * Copyright (c) 1998-2000, 2007-2008  Matthes Bender
 * Copyright (c) 2001-2002, 2004-2005, 2008  Sven Eberhardt
 * Copyright (c) 2004, 2006  Peter Wortmann
 * Copyright (c) 2005  Günther Brammer
 * Copyright (c) 2001-2009, RedWolf Design GmbH, http://www.clonk.de
 *
 * Portions might be copyrighted by other authors who have contributed
 * to OpenClonk.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * See isc_license.txt for full license and disclaimer.
 *
 * "Clonk" is a registered trademark of Matthes Bender.
 * See clonk_trademark_license.txt for full license.
 */

/* Main class to run the game */

#ifndef INC_C4Game
#define INC_C4Game

#ifdef C4ENGINE

#include <C4Def.h>
#include <C4Texture.h>
#include <C4RankSystem.h>
#include <C4GraphicsSystem.h>
#include <C4GraphicsResource.h>
#include <C4GameMessage.h>
#include <C4MouseControl.h>
#include <C4MessageInput.h>
#include <C4Weather.h>
#include <C4Material.h>
#include <C4GameObjects.h>
#include <C4Landscape.h>
#include <C4Scenario.h>
#include <C4MassMover.h>
#include <C4PXS.h>
#include <C4PlayerList.h>
#include <C4Teams.h>
#include <C4PlayerInfo.h>
#include <C4Control.h>
#include <C4PathFinder.h>
#include <C4ComponentHost.h>
#include <C4ScriptHost.h>
#include <C4Particles.h>
#include <C4GroupSet.h>
#include <C4Extra.h>
#include <C4GameControl.h>
#include <C4Effects.h>
#include <C4Fonts.h>
#include "C4LangStringTable.h"
#include "C4Scoreboard.h"
#include <C4Network2.h>
#include <C4Scenario.h>
#include <C4Client.h>
#include <C4Network2Reference.h>
#include <C4VideoPlayback.h>
#include <C4RoundResults.h>

class C4Game
  {
	private:
		// used as StdCompiler-parameter
		struct CompileSettings
			{
			bool fScenarioSection;
			bool fPlayers;
			bool fExact;

			CompileSettings(bool fScenarioSection, bool fPlayers, bool fExact)
				: fScenarioSection(fScenarioSection), fPlayers(fPlayers), fExact(fExact) { }
			};

		// struct of keyboard set and indexed control key
		struct C4KeySetCtrl
			{
			int32_t iKeySet, iCtrl;

			C4KeySetCtrl(int32_t iKeySet, int32_t iCtrl) : iKeySet(iKeySet), iCtrl(iCtrl) {}
			};

  public:
    C4Game();
		~C4Game();
	public:
		C4ClientList			 &Clients; // Shortcut
		C4GameParameters		Parameters;
		C4TeamList				 &Teams; // Shortcut
		C4PlayerInfoList	 &PlayerInfos; // Shortcut
		C4PlayerInfoList	 &RestorePlayerInfos; // Shortcut
		C4RoundResults      RoundResults;
		C4GameMessageList		Messages;
    C4Weather						Weather;
		C4MaterialMap				Material;
		C4GameObjects				Objects;
		C4ObjectList				BackObjects;		// objects in background (C4D_Background)
		C4ObjectList				ForeObjects;		// objects in foreground (C4D_Foreground)
    C4Scenario					C4S;
		C4ComponentHost			Info;
		C4ComponentHost			Title;
		C4ComponentHost			Names;
		C4ComponentHost			GameText;
		C4GameScriptHost		Script;
		C4LangStringTable   MainSysLangStringTable, ScenarioLangStringTable, ScenarioSysLangStringTable;
		C4PlayerList				Players;
		StdStrBuf           PlayerNames;
		C4GameControl				Control;
		C4Control					 &Input; // shortcut

		C4PathFinder				PathFinder;
		C4TransferZones			TransferZones;
    C4Group							ScenarioFile;
		C4GroupSet					GroupSet;
		C4Group             *pParentGroup;
		C4Extra							Extra;
		C4GUIScreen         *pGUI;
		C4ScenarioSection   *pScenarioSections, *pCurrentScenarioSection;
		C4Effect            *pGlobalEffects;
#ifndef USE_CONSOLE
		// We don't need fonts when we don't have graphics
		C4FontLoader        FontLoader;
#endif
		C4Scoreboard        Scoreboard;
		C4VideoPlayer       VideoPlayer;
		class C4Network2Stats *pNetworkStatistics; // may be NULL if no statistics are recorded
		class C4KeyboardInput &KeyboardInput;
		class C4FileMonitor *pFileMonitor;
		class C4GameSec1Timer *pSec1Timer;

		char                CurrentScenarioSection[C4MaxName+1];
    char ScenarioFilename[_MAX_PATH+1];
		StdCopyStrBuf ScenarioTitle;
		char PlayerFilenames[20*_MAX_PATH+1];
		char DefinitionFilenames[20*_MAX_PATH+1];
		char DirectJoinAddress[_MAX_PATH+1];
		class C4Network2Reference *pJoinReference;
		int32_t StartupPlayerCount;
		int32_t FPS,cFPS;
		int32_t HaltCount;
    bool GameOver;
	  bool Evaluated;
		bool GameOverDlgShown;
		bool fScriptCreatedObjects;
		bool fLobby;
		int32_t iLobbyTimeout;
    bool fObserve;
		bool fReferenceDefinitionOverride;
		bool NetworkActive;
		bool Record;
		bool RegJoinOnly; // default false; set to true only by command line parameter or in console builds
		StdStrBuf RecordDumpFile;
		StdStrBuf RecordStream;
		bool TempScenarioFile;
		bool fPreinited; // set after PreInit has been called; unset by Clear and Default
		int32_t FrameCounter;
		int32_t iTick2,iTick3,iTick5,iTick10,iTick35,iTick255,iTick500,iTick1000;
		bool TimeGo;
		int32_t Time;
		int32_t StartTime;
		int32_t InitProgress; int32_t LastInitProgress; int32_t LastInitProgressShowTime;
		int32_t RandomSeed;
		int32_t ObjectEnumerationIndex;
		int32_t Rules;
		bool GameGo;
		bool FullSpeed;
		int32_t FrameSkip; bool DoSkipFrame;
		uint32_t FoWColor;				// FoW-color; may contain transparency
		bool fResortAnyObject; // if set, object list will be checked for unsorted objects next frame
		bool IsRunning;        // (NoSave) if set, the game is running; if not, just the startup message board is painted
		bool PointersDenumerated; // (NoSave) set after object pointers have been denumerated
		size_t StartupLogPos, QuitLogPos; // current log positions when game was last started and cleared
		bool fQuitWithError; // if set, game shut down irregularly
		int32_t iMusicLevel;      // scenario-defined music level
		// current play list
		StdCopyStrBuf PlayList;
		bool DebugMode;
		// next mission to be played after this one
		StdCopyStrBuf NextMission, NextMissionText, NextMissionDesc;

  public:
    // Init and execution
		void Default();
		void Clear();
		void Abort(bool fApproved = false); // hard-quit on Esc+Y (/J/O)
	  void Evaluate();
		void ShowGameOverDlg();
		bool DoKeyboardInput(C4KeyCode vk_code, C4KeyEventType eEventType, bool fAlt, bool fCtrl, bool fShift, bool fRepeated, class C4GUI::Dialog *pForDialog=NULL, bool fPlrCtrlOnly=false);
		void DrawCursors(C4TargetFacet &cgo, int32_t iPlayer);
		bool LocalControlKey(C4KeyCodeEx key, C4KeySetCtrl Ctrl);
		bool LocalControlKeyUp(C4KeyCodeEx key, C4KeySetCtrl Ctrl);
	  void LocalPlayerControl(int32_t iPlayer, int32_t iCom);
	  void FixRandom(int32_t iSeed);
		bool Init();
		bool PreInit();
	  void ParseCommandLine(const char *szCmdLine);
    BOOL Execute();
	  class C4Player *JoinPlayer(const char *szFilename, int32_t iAtClient, const char *szAtClientName, C4PlayerInfo *pInfo);
	  BOOL DoGameOver();
		bool CanQuickSave();
	  BOOL QuickSave(const char *strFilename, const char *strTitle, bool fForceSave=false);
		void SetInitProgress(float fToProgress);
		void OnResolutionChanged(unsigned int iXRes, unsigned int iYRes); // update anything that's dependant on screen resolution
		void InitFullscreenComponents(bool fRunning);
		bool ToggleChat();
		// Pause
		bool TogglePause();
		bool Pause();
		bool Unpause();
		bool IsPaused();
		// Network
	  void Synchronize(BOOL fSavePlayerFiles);
	  void SyncClearance();
		BOOL ReSync();
		void SyncCheckFiles(); // check if files are in sync
    // Editing
		BOOL DropFile(const char *szFilename, float iX, float iY);
		BOOL CreateViewport(int32_t iPlayer, bool fSilent=false);
	  BOOL DropDef(C4ID id, float iX, float iY);
	  BOOL LoadDef(const char *szFilename);
	  BOOL ReloadFile(const char *szPath);
	  BOOL ReloadDef(C4ID id);
		BOOL ReloadParticle(const char *szName);
    // Object functions
    void ClearPointers(C4Object *cobj);
    C4Object *CreateObject(C4ID type, C4Object *pCreator, int32_t owner=NO_OWNER,
                           int32_t x=50, int32_t y=50, int32_t r=0,
                           FIXED xdir=Fix0, FIXED ydir=Fix0, FIXED rdir=Fix0, int32_t iController=NO_OWNER);
    C4Object *CreateObjectConstruction(C4ID type,
		                                   C4Object *pCreator,
                                       int32_t owner,
                                       int32_t ctx=0, int32_t bty=0,
                                       int32_t con=1, BOOL terrain=FALSE);
    C4Object *CreateInfoObject(C4ObjectInfo *cinf, int32_t owner,
                               int32_t tx=50, int32_t ty=50);
    void BlastObjects(int32_t tx, int32_t ty, int32_t level, C4Object *inobj, int32_t iCausedBy, C4Object *pByObj);
		void ShakeObjects(int32_t tx, int32_t ry, int32_t range);
    C4Object *OverlapObject(int32_t tx, int32_t ty, int32_t wdt, int32_t hgt,
                            int32_t category);
    C4Object *FindObject(C4ID id,
												 int32_t iX=0, int32_t iY=0, int32_t iWdt=0, int32_t iHgt=0,
												 DWORD ocf=OCF_All,
												 const char *szAction=NULL, C4Object *pActionTarget=NULL,
												 C4Object *pExclude=NULL,
												 C4Object *pContainer=NULL,
												 int32_t iOwner=ANY_OWNER,
												 C4Object *pFindNext=NULL);
		C4Object *FindVisObject( // find object in view at pos, regarding parallaxity and visibility (but not distance)
		                     float tx, float ty, int32_t iPlr, const C4Facet &fctViewport,
		                     float iX=0, float iY=0, float iWdt=0, float iHgt=0,
												 DWORD ocf=OCF_All,
												 C4Object *pExclude=NULL,
												 int32_t iOwner=ANY_OWNER,
												 C4Object *pFindNext=NULL);
    int32_t ObjectCount(C4ID id,
										int32_t x=0, int32_t y=0, int32_t wdt=0, int32_t hgt=0,
										DWORD ocf=OCF_All,
										const char *szAction=NULL, C4Object *pActionTarget=NULL,
										C4Object *pExclude=NULL,
										C4Object *pContainer=NULL,
										int32_t iOwner=ANY_OWNER);
	  C4Object *FindBase(int32_t iPlayer, int32_t iIndex);
	  C4Object *FindFriendlyBase(int32_t iPlayer, int32_t iIndex);
		C4Object *FindObjectByCommand(int32_t iCommand, C4Object *pTarget=NULL, C4Value iTx=C4VNull, int32_t iTy=0, C4Object *pTarget2=NULL, C4Object *pFindNext=NULL);
		void CastObjects(C4ID id, C4Object *pCreator, int32_t num, int32_t level, int32_t tx, int32_t ty, int32_t iOwner=NO_OWNER, int32_t iController=NO_OWNER);
		void BlastCastObjects(C4ID id, C4Object *pCreator, int32_t num, int32_t tx, int32_t ty, int32_t iController=NO_OWNER);
		C4Object *PlaceVegetation(C4ID id, int32_t iX, int32_t iY, int32_t iWdt, int32_t iHgt, int32_t iGrowth);
		C4Object *PlaceAnimal(C4ID idAnimal);

		BOOL LoadScenarioSection(const char *szSection, DWORD dwFlags);
	  BOOL SaveDesc(C4Group &hGroup, BOOL fSaveGame=FALSE, BOOL fReference=FALSE, BOOL fLobby=FALSE, BOOL fUnregistered=FALSE, BOOL fRecord=FALSE);

		bool DrawTextSpecImage(C4FacetSurface &fctTarget, const char *szSpec, uint32_t dwClr=0xff);
		bool SpeedUp();
		bool SlowDown();
		bool InitKeyboard(); // register main keyboard input functions

  protected:
		bool InitSystem();
		void InitInEarth();
		void InitVegetation();
		void InitAnimals();
	  void InitGoals();
	  void InitRules();
		void InitValueOverloads();
	  void InitEnvironment();
	  void UpdateRules();
    void CloseScenario();
    void DeleteObjects(bool fDeleteInactive);
    void ExecObjects();
		void Ticks();
	  const char *FoldersWithLocalsDefs(const char *szPath);
	  BOOL CheckObjectEnumeration();
	  BOOL LocalFileMatch(const char *szFilename, int32_t iCreation);
		BOOL DefinitionFilenamesFromSaveGame();
	  bool LoadScenarioComponents();
		bool LoadScenarioScripts();
	public:
	  BOOL SaveGameTitle(C4Group &hGroup);
	protected:
		BOOL InitGame(C4Group &hGroup, bool fLoadSection, bool fLoadSky);
		BOOL InitGameFinal();
		BOOL InitNetworkFromAddress(const char *szAddress);
		BOOL InitNetworkFromReference(const C4Network2Reference &Reference);
		BOOL InitNetworkHost();
		BOOL DoLobby();
		BOOL PreInitControl();
		BOOL InitControl();
		BOOL InitScriptEngine();
		BOOL LinkScriptEngine();
		BOOL InitPlayers();
		BOOL InitRecord();
    BOOL OpenScenario();
    BOOL InitDefs();
    BOOL InitMaterialTexture();
    BOOL EnumerateMaterials();
		BOOL GameOverCheck();
		BOOL PlaceInEarth(C4ID id);
		BOOL Compile(const char *szSource);
		BOOL Decompile(StdStrBuf &rBuf, bool fSaveSection, bool fSaveExact);
	public:
		void CompileFunc(StdCompiler *pComp, CompileSettings comp);
	  BOOL SaveData(C4Group &hGroup, bool fSaveSection, bool fInitial, bool fSaveExact);
	protected:
	  BOOL CompileRuntimeData(C4ComponentHost &rGameData);
	  BOOL StoreParticipantPlayers();
	  BOOL RecreatePlayerFiles();

    // Object function internals
    C4Object *NewObject( C4Def *ndef, C4Object *pCreator,
                         int32_t owner, C4ObjectInfo *info,
                         int32_t tx, int32_t ty, int32_t tr,
                         FIXED xdir, FIXED ydir, FIXED rdir,
                         int32_t con, int32_t iController);
    void ClearObjectPtrs(C4Object *tptr);
    void ObjectRemovalCheck();

		bool ToggleDebugMode(); // dbg modeon/off if allowed
		bool ActivateMenu(const char *szCommand); // exec given menu command for first local player

	public:
		bool ToggleChart(); // chart dlg on/off
		void SetMusicLevel(int32_t iToLvl); // change game music volume; multiplied by config volume for real volume
  };


const int32_t C4RULE_StructuresNeedEnergy			 = 1,
							C4RULE_ConstructionNeedsMaterial = 2,
							C4RULE_FlagRemoveable						 = 4,
							C4RULE_StructuresSnowIn          = 8;

extern C4Game					Game;

// a global wrapper
inline StdStrBuf GetKeyboardInputName(const char *szKeyName, bool fShort = false, int32_t iIndex = 0)
{
	return Game.KeyboardInput.GetKeyCodeNameByKeyName(szKeyName, fShort, iIndex);
}

#endif // C4ENGINE

#endif
