# Wanderer's Descent

Third-person exploration/combat adventure. Unreal Engine 5.7, C++. Due December 5, 2026; schedule in [MILESTONES.md](MILESTONES.md).

## Opening the project

- **Visual Studio 2026:** open `WanderersDescent.sln`, set `WanderersDescent` as the startup project, and press Ctrl+F5 to build and launch the editor.
- **Or** double-click `WanderersDescent.uproject`.

If you edit C++ while the editor is open, use Live Coding (Ctrl+Alt+F11 in the editor). Close the editor before adding new classes or changing headers heavily.

## How the code is organized

`Source/WanderersDescent/` holds two kinds of code:

- **Template code (from Epic):** `WanderersDescentCharacter`, `Variant_Combat/`, `Variant_Platforming/`, `Variant_SideScrolling/`. We build on `Variant_Combat`, which provides:
  - a combo attack (light) and a hold-to-charge attack (heavy), with hit detection triggered by animation notifies
  - damage, knockback, death and respawn, plus life bars
  - a StateTree-driven enemy (`CombatEnemy`), a training dummy, checkpoints and lava

  Leave these files alone where possible, so it stays clear what's ours.
- **Our code, in `Game/`:**
  - `WandererCharacter`: the player. Extends `CombatCharacter`, adding stamina costs on attacks, sprint, a dodge with invincibility frames, and power-up keys. Prints combat events and stamina on screen while there's no HUD; turn off with `bShowDebugMessages`.
  - `StaminaComponent`: a stamina pool that regenerates after a short delay.
  - `PowerUpComponent`: power-up slots sharing a 10 s cooldown. Broadcasts `OnPowerUpActivated(Name)`; each power-up's actual effect isn't implemented yet.
  - `BossCharacter`: the Soul Valley boss. Extends `CombatEnemy` (so it reuses the enemy AI) and moves through 3 phases at 66% and 33% health. Put phase-specific behavior in the `OnEnterPhase` event.

Don't name a source folder with a prefix of `WanderersDescent` (e.g. `Wanderer/`): Unreal's header tool then generates broken include paths for the template classes.

## One-time editor setup

Do these once, in order:

1. **Input actions.** In `Content/Wanderer/Input/`, create Input Actions (right-click → Input → Input Action, default bool value type): `IA_Dodge`, `IA_Sprint`, `IA_PowerUp1` to `IA_PowerUp4`.
2. **Key bindings.** Open `Content/Variant_Combat/Input/IMC_Combat` and add mappings:
   - `IA_Dodge` → Space Bar (the combat template doesn't bind jump)
   - `IA_Sprint` → Left Shift
   - `IA_PowerUp1` to `4` → keys 1 to 4
3. **Player Blueprint.** Duplicate `Content/Variant_Combat/Blueprints/BP_CombatCharacter` into `Content/Wanderer/` as `BP_WandererCharacter`. Open it and use File → Reparent Blueprint → `WandererCharacter`. In Class Defaults, set `Dodge Action`, `Sprint Action` and the four `Power Up Actions`. Select the `PowerUps` component and add 4 placeholder names to `Power Up Slots`.
4. **Player controller.** Duplicate `BP_CombatPlayerController` as `BP_WandererPlayerController` and set **Respawn → Character Class** to `BP_WandererCharacter`. The controller spawns this class on respawn.
5. **Game mode.** Duplicate `BP_CombatGameMode` as `BP_WandererGameMode`. Set Default Pawn Class to `BP_WandererCharacter` and Player Controller Class to `BP_WandererPlayerController`.
6. **Level and defaults.** Open `Content/Variant_Combat/Lvl_Combat`. In World Settings, set GameMode Override to `BP_WandererGameMode`. Then in Project Settings → Maps & Modes, set Default GameMode to `BP_WandererGameMode` and Editor Startup Map to `Lvl_Combat`.
7. **Boss (when you get to it).** Duplicate `Content/Variant_Combat/Blueprints/AI/BP_CombatEnemy` as `BP_Boss` and reparent it to `BossCharacter`.

## Quick test (Lvl_Combat)

- **Attacks:** spam left-click. Stamina should drain about 10 per hit until the combo ends with "not enough stamina".
- **Dodge:** Space with a direction held launches you that way; with no direction held you hop backwards. Getting hit during the first 0.3 s shows "Hit avoided (i-frames)".
- **Sprint:** hold Shift while moving. Stamina drains, and sprint stops when it runs out.
- **Power-ups:** keys 1 to 4. A second press within 10 s reports the remaining cooldown.

## Version control

The repo uses Git LFS for `.uasset`, `.umap` and other binaries (see `.gitattributes`). Run `git lfs install` once per machine before cloning or committing.
