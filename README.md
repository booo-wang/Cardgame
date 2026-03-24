# Card Solitaire Game

A C++ card solitaire matching game built with **Cocos2d-x 3.17.2**.

The assignment requirement is to match playfield cards against the tray's top card when the face value differs by exactly 1, support drawing from the stack, and support multi-step undo with visible card movement.

## Run Directly

This repository already includes a runnable Windows package in `Release/`.

1. Clone or download the repository
2. Open `Release/`
3. Run `CardGame.exe`

If the executable fails to start on a clean Windows machine, install the Microsoft Visual C++ Redistributable:
https://aka.ms/vs/17/release/vc_redist.x86.exe

## Gameplay

- `Playfield`: uncovered face-up cards can be selected
- `Tray`: current top card used for matching
- `Stack`: click to draw a new tray card
- `Undo`: reverts the last action and supports consecutive undo

Matching rule: the selected card's face value must differ by exactly 1 from the tray top card. Suits do not matter.

## Repository Contents

```text
CardGame/
├── Classes/               C++ source code organized in MVC-style modules
├── Resources/             Card images and level JSON
├── Release/               Prebuilt Windows executable plus runtime DLLs
├── proj.win32/            Win32 Visual Studio project entry files
├── README.md              Run/build instructions
└── 程序设计文档.html        Chinese design and implementation document
```

## Build From Source

The source project is included, but the Cocos2d-x engine is intentionally not vendored into this repository.

Environment used for the local successful build:

- Cocos2d-x `3.17.2`
- Visual Studio Build Tools `2022/2025`
- MSVC toolset `v143` or `v145`
- Windows SDK `10.0.26100.0`
- Target platform `Win32`

Build steps:

1. Download Cocos2d-x 3.17.2
2. Place it outside the repo, for example at `D:\cocos2d-x-3.17.2`
3. Create a junction named `cocos2d` in the project root:

```cmd
cd CardGame
mklink /J cocos2d D:\cocos2d-x-3.17.2
```

4. Build the Win32 project:

```cmd
MSBuild proj.win32\CardGame.vcxproj /p:Configuration=Debug /p:Platform=Win32 /m
```

5. Run the built executable:

```text
proj.win32\Debug.win32\CardGame.exe
```

Notes:

- `proj.win32/CardGame.vcxproj` already contains `/utf-8` and `VisualStudioVersion == 18.0 -> v145` support
- The project references the external `cocos2d` folder, so source builds will fail until that junction exists
- For a quick check or coursework submission, `Release/CardGame.exe` is the simplest path

## Assignment Coverage

- Initial tray card: implemented
- Playfield covered/uncovered logic: implemented
- Stack draw replacement: implemented
- Difference-of-1 matching rule: implemented
- Multi-step undo with movement animation: implemented
- Resolution `1080 x 2080` with `FIXED_WIDTH`: implemented
- JSON-based level configuration: implemented

## Documentation

Open `程序设计文档.html` in a browser for:

- requirement checklist against the PDF
- MVC architecture explanation
- bug-fix record
- animation design
- extension guide

## License

Educational use only.
