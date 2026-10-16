# Wanderer's Descent

Third-person exploration/combat adventure. UE 5.4, C++.

## First-time setup

1. Right-click `WanderersDescent.uproject` → **Generate Visual Studio project files** (or just double-click the `.uproject` with UE 5.4 installed; it will offer to generate it and build the module).
2. Open the generated `.sln` in Visual Studio, or open the `.uproject` directly, and build.
3. On first editor launch you'll be in an empty level — create a starter map under `Content/Maps/` and set it as the editor startup/default map in **Project Settings → Maps & Modes** (left out of `DefaultEngine.ini` for now since no map exists yet).

## What's scaffolded

`Source/WanderersDescent/`:

- **`Characters/WanderersDescentCharacter`** — third-person pawn (spring arm + camera, Enhanced Input movement/look) that owns the combat/stamina/power-up/health components below.
- **`Characters/BossCharacter`** — multi-phase boss stub. Watches its own `HealthComponent` and fires `OnEnterPhase` / `OnBossPhaseChanged` when health crosses `Phase2HealthThreshold` / `Phase3HealthThreshold` (defaults 66% / 33%). Override `OnEnterPhase` in a Blueprint or C++ subclass to trigger phase-specific attack patterns.
- **`Components/StaminaComponent`** — stamina pool with regen-after-delay, used to gate combat actions.
- **`Components/CombatComponent`** — light attack, heavy attack, and dodge with a brief invincibility window (toggles `HealthComponent::bIsInvincible` for `DodgeInvincibleDuration`). Each action spends stamina and briefly locks out other actions (`ECombatState`).
- **`Components/PowerUpComponent`** — an array of power-up slots (`PowerUpSlots`, matched by index to input) sharing one 10s cooldown (`CooldownDuration`) between activations. Broadcasts `OnPowerUpActivated`; the actual effect of each power-up isn't implemented yet — bind it in Blueprint or a subclass.
- **`Components/HealthComponent`** — shared damage/death handling used by both the player and the boss, respects `bIsInvincible`.
- **`WanderersDescentGameMode`** — sets `WanderersDescentCharacter` as the default pawn.

All tunable numbers (attack costs, durations, cooldowns, phase thresholds) are `EditDefaultsOnly` so they can be tweaked per-Blueprint without touching code.

## Still to do in the editor (can't be scaffolded as text files)

- **Input**: create an `Input Mapping Context` (e.g. `IMC_Player`) and `Input Action` assets for Move, Look, Jump, LightAttack, HeavyAttack, Dodge, and one per power-up slot. Create a Blueprint subclass of `WanderersDescentCharacter` (e.g. `BP_PlayerCharacter`) and assign these to the corresponding `UPROPERTY`s.
- **GameMode**: either point `DefaultPawnClass` at `BP_PlayerCharacter` via a Blueprint subclass of `WanderersDescentGameMode`, or set it directly on the C++ GameMode's defaults.
- **Art/animation**: character mesh, animation blueprint, weapon meshes, boss mesh — per the proposal's "full detail on hero elements" style.
- **Level design**: hub area + 3-5 themed zones (Nomad Canyons biomes) + Soul Valley boss arena, using the low-poly blockout → trim sheet/lighting workflow described in the proposal.
- **Boss phase content**: subclass `BossCharacter` (or override `OnEnterPhase` in Blueprint) to actually change attack patterns per phase.
- **Power-up effects**: bind `OnPowerUpActivated` to whatever each power-up actually does (buff, AoE, heal, etc.).
