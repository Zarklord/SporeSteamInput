# SteamInput

# Build instructions:
* Install Python 3
* Install the Steamworks SDK to ./steamsdk/
* Build

# Installation Instructions:
This assumes you are going to use your stub game as Steam Spore, if you are not obtain the appid of your stub app from https://steamdb.info/

Install the SteamInput.dll to the Spore ModAPI Launcher.
* Copy the steam_lib.dll located at ./steamsdk/redistributable_bin/ to your SporebinEP1 folder
* Copy the steam_appid.txt located at ./InstallationFiles/ to your SporebinEP1 folder
* If your using a different stub game, edit the appid inside steam_appid to match your stub game.

## Optional for setting up a custom steam controller config, otherwise you'll need to get somebody else's config for Spore
* Copy the game_actions_\<appid\>.vdf in ./InstallationFiles/ to <SteamInstallFolder>/controller_config (you might need to make this directory)
* Grab your controller and start mapping stuff.
* Save your config.
