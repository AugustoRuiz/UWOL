#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL/SDL_main.h>
#include "MusicManager.h"
#include "Game.h"
#include "TimerFuncs.h"
#include "GameData.h"

int main(int argc, char *argv[])
{
    Log::Initialize();

    // ----------------------------------------------------------------------------
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
        Log::Out << "Couldn't get file system representation! " << std::endl;
    }
    CFRelease(resourcesURL);
    
    chdir(path);
    Log::Out << "Current Path: " << path << endl;
#endif
    // ----------------------------------------------------------------------------
    
	try {
		Game *game = Game::GetInstance();
		MusicManager *musicMgr = MusicManager::GetInstance();
		Uint32 lastTick, currentTick;
		Uint32 delta;
		Uint32 accum = 0;

		bool attract = false;
		bool saveState = false;
		bool fullScreen = true;
        
		Log::Out << "main: Initializing Engine..." << endl;

		Log::Out << "Parsing parameters..." << endl;
		for (int param = 1; param < argc; param++)
		{
			if (!strcmp(argv[param], "-a") || !strcmp(argv[param], "-A") || !strcmp(argv[param], "/a") || !strcmp(argv[param], "/A"))
			{
				if (!saveState)
				{
					attract = true;
				}
				else
				{
					Log::Out << "main: Cannot use both -a and -s parameters. -s will be ignored." << endl;
				}
			}
			if (!strcmp(argv[param], "-s") || !strcmp(argv[param], "-S") || !strcmp(argv[param], "/s") || !strcmp(argv[param], "/S"))
			{
				if (!attract)
				{
					saveState = true;
				}
				else
				{
					Log::Out << "main: Cannot use both -s and -a parameters. -a will be ignored." << endl;
				}
			}
			if (!strcmp(argv[param], "-w") || !strcmp(argv[param], "-W") || !strcmp(argv[param], "/w") || !strcmp(argv[param], "/W"))
			{
				fullScreen = false;
			}
		}

		aliasing = false;
		scanlines = true;
		debugPaint = false;

		musicMgr->Initialize();
		// 512 x 384
		if (game->Initialize(640, 480, fullScreen, "UWOL, The Quest for Money"))
		{
			game->ShowCursor(false);

			game->SetAttractMode(attract);
			game->SetSaveAttract(saveState);

			Log::Out << "main: Starting main loop..." << endl;

			srand(1); //(unsigned)time( NULL ) );

			lastTick = TimerFuncs::GetTicks();

			while (game->Running())
			{
				// Qué tiempo ha pasado desde la última vez?
				currentTick = TimerFuncs::GetTicks();
				delta = currentTick - lastTick;

				accum += delta;
				while (accum >= TICKS_PER_FRAME)
				{
					// Mientras no toque pintar, actualizamos
					accum -= TICKS_PER_FRAME;
					game->Update(TICKS_PER_FRAME);
				}
				// Pintamos
				game->Render();
				lastTick = currentTick;
			}

			if (saveState)
			{
				game->SaveAttractModeData();
			}

			Log::Out << "main: Main loop finished." << endl;
			Log::Out << "Checking errors:" << endl << SDL_GetError() << endl << Mix_GetError();

			game->ShowCursor(true);
			game->Dispose();
		}
		musicMgr->Dispose();
		musicMgr->DeleteMusic();
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
		Log::Dispose();
		return -1;
	}

	Log::Dispose();
	return 0;
}
