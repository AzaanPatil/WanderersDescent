# Wanderer's Descent

A third-person exploration/combat game, built solo as a class project (4232, Fall 2026). Unreal Engine 5.7, C++ plus Blueprints. See [README.md](README.md) for the code layout, editor setup and quick tests.

## The game (from the proposal)

The player explores Nomad Canyons, a hub connecting 3 to 5 short themed zones, on the way to Soul Valley, where a single multi-phase boss fight ends the game.

**Mechanics**
- Light and heavy attacks.
- A dodge with brief invincibility frames.
- Stamina that governs how aggressively the player can act.
- Combat power-ups on several keys, with a 10 s cooldown between power-ups.
- Light traversal and environmental storytelling.

**Style**
- Simple geometric low-poly blockouts for environments.
- Hand-painted textures, trim sheets and lighting carry most of the atmosphere.
- Full detail is reserved for hero elements: the player, weapons and the boss.

## Schedule

- **Due:** Saturday, December 5, 2026.
- **Target:** feature-complete by Friday, Nov 20. Thanksgiving weekend (Nov 26–29) and the week after are polish and bug-fix buffer only, with no new features.
- **Other project:** this shares the Dec 5 deadline with the user's Job Tracker C# project (ITSC 3112), so weigh scope against both.

The user's own checklist is the main plan. It uses Sunday–Saturday weeks called "Depths"; Depth 05 onward weren't shared yet. `MILESTONES.md` is an older plan written by Claude and may be out of date.

| Depth | Dates | Items |
|---|---|---|
| 01 Pre-Production & Setup | Oct 5–11 | Project created; Git set up; Mannequin moving in a test level; moodboard and color palette; combat kit on paper (attack windows, i-frames, stamina costs) |
| 02 Movement & Camera Feel | Oct 12–18 | Tune camera (distance, lag, collision); movement feel (acceleration, turning, run/sprint); idle/walk/run in the Animation Blueprint; stamina data layer (regen, costs, no UI) |
| 03 Combat Prototype | Oct 19–25 | Light/heavy attacks with hit detection; damage and hit reactions on a test dummy; dodge/roll with i-frames; combat-feel pass against a static target |
| 04 Enemy AI v1 | Oct 26–Nov 1 | AI that patrols, detects, chases and attacks (the user wrote "behavior tree"; the template uses StateTree); one enemy type fighting the player; hit-stop, camera shake, hit VFX; playtest and tune the first encounter |

**Status as of Oct 16:** the code exists for:
- the stamina data layer, sprint, and the dodge with i-frames
- light/heavy attacks and hit detection, via the template
- a test dummy and an enemy AI, both from the template

A play test in `Lvl_Combat` passed. The dodge has no animation yet (launch only), and there's no stamina or power-up UI yet; on-screen debug messages stand in for it.

## Architecture decisions

- **Built on the template's `Variant_Combat`.** The player is `WandererCharacter`, extending `CombatCharacter`. The boss is `BossCharacter`, extending `CombatEnemy`, so it reuses the StateTree AI and life bar. Don't reimplement health, hit detection, combos or enemy AI; the template already has them.
- **Our C++ goes in `Source/WanderersDescent/Game/`.** Our assets go in `Content/Wanderer/`. Leave the template's C++ and assets unedited where possible. The only edited template assets are `IMC_Combat` (new key mappings) and `Lvl_Combat`'s GameMode Override.
- **Stamina is charged in two places:**
  - `HandleMontageStarted` charges every fresh attack, because the template can start attacks from buffered input that bypasses `DoComboAttackStart`.
  - `CheckCombo` charges each follow-up hit of a combo.

  The `Do...Start` overrides only block an attack when there isn't enough stamina.
- **Dodge invincibility** works by overriding `TakeDamage` and returning 0 while `bIsInvincible` is set.
- **Power-up effects aren't implemented yet.** `PowerUpComponent` only manages the slots and cooldown and broadcasts `OnPowerUpActivated(Name)`.
- **Template health numbers are small:** player MaxHP is 5, enemy MaxHP 3, melee damage 1. The boss defaults to 30 HP.

## Gotchas

- **Folder names:** never name a source folder with a prefix of `WanderersDescent` (e.g. `Wanderer/`). Unreal's header tool matches include paths by text prefix and generates broken includes (`DescentCharacter.h`) for the template classes. That's why our folder is `Game/`.
- **Respawn class:** the respawned player's class comes from the player controller's `CharacterClass` (`BP_WandererPlayerController`), not the game mode.
- **No jump:** the template's `CombatCharacter` doesn't bind jump. Space is used for dodge. If traversal needs a jump, it has to be added.
- **Packaged builds:** `GameDefaultMap` still points at `ThirdPerson/Lvl_ThirdPerson`. Change it before packaging.
- **Obsolete scaffold:** `School\WanderersDescent` (outside `Unreal Projects`) is an early hand-written scaffold targeting UE 5.4. It isn't part of this project.

## Building and verifying

Command-line build. The editor must be closed, or Live Coding will block it:

```
"C:\Program Files\Epic Games\UE_5.7\Engine\Build\BatchFiles\Build.bat" WanderersDescentEditor Win64 Development "-Project=C:\Users\azaan\Coding Projects\School\Unreal Projects\WanderersDescent\WanderersDescent.uproject" -WaitMutex
```

The toolchain is Visual Studio 2026 (MSVC 14.51). Errors land in `%LOCALAPPDATA%\UnrealBuildTool\Log.txt`.

The `.sln` and the VS Code workspace are generated files and are gitignored. Regenerate the workspace with **Tools → Generate Visual Studio Code Project** in the editor.

Binary assets go through Git LFS (`.gitattributes`). LFS is already installed on this machine.

## Working with the user

- **Commits:** the user makes commits themselves. Stage files and check what would be committed if useful, then give them the commit message rather than running `git commit`.
- **Editor work:** when a step needs the Unreal Editor (creating assets, Blueprints, settings), give a click-by-click walkthrough with exact menu names for 5.7, and say which tool each step happens in (Unreal vs Visual Studio).
- **Screenshots:** the user often shares screenshots. Check which folder or project they show, since mixing up the old scaffold folder has happened before.
