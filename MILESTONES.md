# Milestones — Wanderer's Descent

**Due:** Saturday, December 5, 2026
**Target: feature-complete by Friday, November 20, 2026** — leaves Thanksgiving weekend (Nov 26–29) plus the following week as a polish/bug-fix buffer before submission, rather than the weekend itself being the finish line. Adjust this assumption if you actually meant "done" *by* the weekend — that would compress Weeks 5–6 below into 5 weeks instead of 6.

Check off items as you go; re-scope the current week (not future ones) if something runs long rather than letting it silently slip.

## Week 1 — Fri Oct 16: Input & movement playable
- [ ] Create `IMC_Player` Input Mapping Context + Input Actions (Move, Look, Jump, LightAttack, HeavyAttack, Dodge, PowerUp slots)
- [ ] Create `BP_PlayerCharacter` (subclass of `WanderersDescentCharacter`), assign all Input Actions
- [ ] Point `WanderersDescentGameMode::DefaultPawnClass` (or a BP subclass) at `BP_PlayerCharacter`
- [ ] Greybox test level; character moves/looks/jumps with a placeholder mesh

## Week 2 — Fri Oct 23: Combat core functional
- [ ] Light/heavy attack + dodge wired to placeholder animation montages
- [ ] Attack hit detection (trace or overlap) that actually calls `HealthComponent::ApplyDamage` on a target
- [ ] Basic stamina bar UI (cost drain + regen visible)
- [ ] Training-dummy test actor with a `HealthComponent` to validate the damage flow end-to-end

## Week 3 — Fri Oct 30: Power-ups & combat feel
- [ ] Implement effects for at least 2–3 power-ups bound to `PowerUpComponent::OnPowerUpActivated`
- [ ] Power-up cooldown UI indicator (10s shared cooldown)
- [ ] Combat juice pass: hit reactions, dodge i-frame visual cue, basic camera feedback
- [ ] Simple non-boss enemy AI to fight against

## Week 4 — Fri Nov 6: Hub + first zone blockout
- [ ] Hub area greybox, connected to at least 1 themed zone
- [ ] Traversal + environmental storytelling pass in that first zone
- [ ] Trim sheet/lighting pass started on the hub (low-poly blockout + trim sheets/lighting per the proposal's style)

## Week 5 — Fri Nov 13: Remaining zones blockout
- [ ] Remaining 2–4 themed zones blocked out and connected to the hub (3–5 zones total)
- [ ] Zone-specific enemy placements
- [ ] Art pass (trim sheets, lighting) on completed zones

## Week 6 — Fri Nov 20: Boss fight & Soul Valley — FEATURE COMPLETE
- [ ] Soul Valley arena built
- [ ] Boss mesh/animations; phase 1–3 attack patterns via `ABossCharacter::OnEnterPhase` overrides
- [ ] Full gameplay loop playable start-to-finish: hub → zones → Soul Valley → boss → win/lose state
- [ ] Full artistic detail pass on hero elements (player character, weapons, boss) per the proposal's style

## Thanksgiving weekend (Nov 26–29): buffer — no new features
Playtest only; log bugs/rough edges for the final week rather than starting new work.

## Week 7 — Fri Dec 4: Final polish
- [ ] Bug triage from Thanksgiving playtest
- [ ] UI/UX polish (health/stamina readability, power-up cooldown clarity)
- [ ] Audio/SFX pass
- [ ] Final playtest, packaging/build check, submission prep

## Dec 5 — Due
