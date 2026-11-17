# Payload Interpreter, rebuilt for 1.7

Payload Interpreter by D7ry, rebuilt so it loads on Skyrim Special Edition and Enderal Special
Edition 1.7.x. The original release stops at startup on 1.7 with "recompile for new address
library". Nothing about what it does has changed.

Original mod: https://www.nexusmods.com/skyrimspecialedition/mods/65089
Original source: https://github.com/D7ry/PayloadInterpreter

## What it does

Payload Interpreter lets an animation carry instructions in its annotations. When the animation
reaches that frame, the instruction runs on the actor playing it. Animation mods use it to cast
spells mid-swing, play particles, shake the camera on heavy hits, make a dodge untouchable for a
moment, or switch animation variables. See D7ry's original README, kept here as README-D7ry.md, for the full instruction format.

## Changes in this rebuild

- Built on Eddoursul's CommonLibSSE-GG instead of the original CommonLib, which has the 1.7
  address library.
- `ModActorValue` call updated to its new name in CommonLibSSE-GG. Same function.
- Plugin entry point moved to `SKSEPluginLoad`.
- One unused address lookup removed. It had no 1.7 entry and nothing called it.

Every instruction and its parameters are unchanged.

## Tested on 1.7.104

Each instruction was run in game on Enderal Special Edition 1.7.104 and its result checked:

| Instruction | Result |
|---|---|
| `@SETGHOST` on and off | works |
| `@APPLYSPELL` and `@UNAPPLYSPELL` | works |
| `@SGVB`, `@SGVF` and `@SGVI` | works |
| `@CAST` / `@CASTSPELL` | works |
| `@PLAYPARTICLE` | works |
| `@CAMSHAKE` | works |
| Delayed instructions, `![seconds]@...` | works |
| Custom `$` instructions from `SKSE\PayloadInterpreter\Config` | works |
| CPR events: `EnableAdvance`, `EnableBackoff`, `EnableCircling`, `EnableSurround`, `EnableFallback` | received and handled |

The CPR events set animation variables that only exist with Combat Pathing Revolution installed.
This test confirms Payload Interpreter receives and handles all five; what they do in combat is
up to Combat Pathing Revolution.

Note: `@APPLYSPELL` adds the spell to the actor's spell list and `@UNAPPLYSPELL` removes it, the
same as Papyrus `AddSpell` and `RemoveSpell`. On an actor that already knows the spell,
`@UNAPPLYSPELL` takes it away.

Note: `@PLAYPARTICLE` plays particle effects such as puffs and impacts. A model made to attach to
an actor, like a magic hit effect, may not show.

## Install

Put `PayloadInterpreter.dll` in `Data\SKSE\Plugins`. Needs SKSE for your game version and the
Address Library for SKSE Plugins.

## Build

Visual Studio 2022 with the C++ workload, CMake and Ninja. Run `build.cmd`. Libraries download on
the first build. The DLL ends up in `build\PayloadInterpreter.dll`.

## License

MIT, the same as the original. Copyright (c) 2021 D7ry. The changes in this rebuild are under the
same license. See `LICENSE`.

Rebuilt by ChiRhoXP for Enderal Co-op.

Enjoy!
