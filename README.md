# **The Sims 4 FlexMod**

[![GitHub All Releases](https://img.shields.io/github/downloads/iArtorias/sims4_flexmod/total.svg)](https://github.com/iArtorias/sims4_flexmod/releases)

### An utility to set the custom path for The Sims 4 mods directory.

## Installation

1. Download and unzip the latest [release](https://github.com/iArtorias/sims4_flexmod/releases)
2. Copy `version.dll` and `mods_path.txt` to the game folder (`Game\Bin`) 
3. Specify the **FULL** path to the mods directory inside `mods_path.txt` if needed (by default it will append to `Game\Bin\Mods` folder)
4. Start the game

## Idea

#### The vanilla game doesn't support a feature of setting the custom path for the mods directory, instead it uses `My Documents` folder. And sometimes one could experience a lack of free space on a system drive. With that being said, this utility can solve such an issue.

## Stuff used

- **MinHook** (https://github.com/TsudaKageyu/minhook)

## Compilation notes

- **Visual Studio 2019** is required to compile this project
