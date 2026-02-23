// Fill out your copyright notice in the Description page of Project Settings.


#include "ASC/ProjectCGameplayTags.h"

namespace ProjectCGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage, "Data.Damage")

	// Input Tag
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Test, "InputTag.Test")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget, "InputTag.SwitchTarget")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dodge, "InputTag.Dodge")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UsePotion, "InputTag.UsePotion")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_NextPotion, "InputTag.NextPotion")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchWeapon, "InputTag.SwitchWeapon")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchMagic, "InputTag.SwitchMagic")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_AshesOfWar, "InputTag.AshesOfWar")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Magic, "InputTag.Magic")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack, "InputTag.LightAttack")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavtAttack, "InputTag.HeavtAttack")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Parry, "InputTag.Parry")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_InterAction, "InputTag.InterAction")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump")


	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle, "InputTag.Toggle")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_TargetLockOn, "InputTag.Toggle.TargetLockOn")

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold, "InputTag.Hold")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold_Guard, "InputTag.Hold.Guard")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Hold_Sprint, "InputTag.Hold.Sprint")

	// Player Tag
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Melee_SwordAttack, "Player.Ability.Melee.SwordAttack")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Melee_HeavyAttack, "Player.Ability.Melee.HeavyAttack")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Melee_GuardCounter, "Player.Ability.Melee.GuardCounter")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Melee_AshesOfWar_TripleThrust, "Player.Ability.Melee.AshesOfWar.TripleThrust")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Melee_AshesOfWar_ShieldRush, "Player.Ability.Melee.AshesOfWar.ShieldRush")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Melee_AshesOfWar_Parry, "Player.Ability.Melee.AshesOfWar.Parry")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Magic_MagicBolt, "Player.Ability.Magic.MagicBolt")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Magic_FireArrow, "Player.Ability.Magic.FireArrow")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Magic_Starfall, "Player.Ability.Magic.Starfall")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLockOn, "Player.Ability.TargetLockOn")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Dodge, "Player.Ability.Dodge")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Execution, "Player.Ability.Execution")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SwitchWeapon, "Player.Ability.SwitchWeapon")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_SwitchMagic, "Player.Ability.SwitchMagic")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_UsePotion, "Player.Ability.UsePotion")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_NextPotion, "Player.Ability.NextPotion")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_ParrySuccess, "Player.Ability.ParrySuccess")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_OpenGraceMenu, "Player.Ability.OpenGraceMenu")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_OpenMessage, "Player.Ability.OpenMessage")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Sprint, "Player.Ability.Sprint")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Portal, "Player.Ability.Portal")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Jump, "Player.Ability.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_EnterBossRoom, "Player.Ability.EnterBossRoom")

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLock, "Player.Status.TargetLock")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Parrying, "Player.Status.Parrying")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_HeavyAttack, "Player.Status.HeavyAttack")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_AshesOfWar_ShieldRush, "Player.Status.AshesOfWar.ShieldRush")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_NearGrace, "Player.Status.NearGrace")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_NearBossGate, "Player.Status.NearBossGate")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_NearPortal, "Player.Status.NearPortal")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_NearMessage, "Player.Status.NearMessage")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_OpenMessage, "Player.Status.OpenMessage")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_AtGrace, "Player.Status.AtGrace")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Sprint, "Player.Status.Sprint")
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_GuardCounterAvailable, "Player.Status.GuardCounterAvailable")

	UE_DEFINE_GAMEPLAY_TAG(Player_Equip_Sword, "Player.Equip.Sword")
	UE_DEFINE_GAMEPLAY_TAG(Player_Equip_Staff, "Player.Equip.Staff")

	UE_DEFINE_GAMEPLAY_TAG(Player_Equip_Magic_MagicBolt, "Player.Equip.Magic.MagicBolt")
	UE_DEFINE_GAMEPLAY_TAG(Player_Equip_Magic_FireArrow, "Player.Equip.Magic.FireArrow")
	UE_DEFINE_GAMEPLAY_TAG(Player_Equip_Magic_Starfall, "Player.Equip.Magic.Starfall")

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_TargetSwitch, "Player.Event.TargetSwitch")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ParrySuccess, "Player.Event.ParrySuccess")
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SpellCast, "Player.Event.SpellCast")

	// Enemy Tag
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee_SwordAttack, "Enemy.Ability.Melee.SwordAttack")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Magic_MagicBolt, "Enemy.Ability.Magic.MagicBolt")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Magic_WaterBall, "Enemy.Ability.Magic.WaterBall")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Executed, "Enemy.Ability.Executed")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_EnterExecutedState, "Enemy.Ability.EnterExecutedState")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Chasing, "Enemy.Ability.Chasing")

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_ExecutionAvailable, "Enemy.Status.ExecutionAvailable")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Executed, "Enemy.Status.Executed")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Chasing, "Enemy.Status.Chasing")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Evade_SpellReactive, "Enemy.Status.Evade.SpellReactive")

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_ExecutionAvailable, "Enemy.Event.ExecutionAvailable")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Executed, "Enemy.Event.Executed")
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Chasing, "Enemy.Event.Chasing")

		
	// Boss Tag
	// Aurora
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_Evade, "Boss.Aurora.Ability.Evade")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_Evade_SpellReactive, "Boss.Aurora.Ability.Evade.SpellReactive")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_RushCombo, "Boss.Aurora.Ability.RushCombo")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_QuadSlash, "Boss.Aurora.Ability.QuadSlash")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_ForwardTripleSlash, "Boss.Aurora.Ability.ForwardTripleSlash")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_JumpComboSlash, "Boss.Aurora.Ability.JumpComboSlash")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_SlashThrustDoubleSlash, "Boss.Aurora.Ability.SlashThrustDoubleSlash")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_WideSlash, "Boss.Aurora.Ability.WideSlash")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_ArrowStorm, "Boss.Aurora.Ability.ArrowStorm")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_PhantomRush, "Boss.Aurora.Ability.PhantomRush")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Aurora_Ability_Frost, "Boss.Aurora.Ability.Frost")
	
	// Grux
	UE_DEFINE_GAMEPLAY_TAG(Boss_Grux_Ability_FourStrike, "Boss.Grux.Ability.FourStrike")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Grux_Ability_Rush, "Boss.Grux.Ability.Rush")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Grux_Ability_Rush2, "Boss.Grux.Ability.Rush2")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Grux_Ability_Roar, "Boss.Grux.Ability.Roar")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Grux_Ability_TripleRush, "Boss.Grux.Ability.TripleRush")
	UE_DEFINE_GAMEPLAY_TAG(Boss_Grux_Ability_UpperRush, "Boss.Grux.Ability.UpperRush")

	// Shared Tag
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_GuardBreak, "Shared.Ability.GuardBreak")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Block, "Shared.Ability.Block")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Block, "Shared.Status.Block")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_BlockSuccess, "Shared.Status.BlockSuccess")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Invincible, "Shared.Status.Invincible")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_DamageTaken, "Shared.Status.DamageTaken")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_MagicDamageTaken, "Shared.Status.MagicDamageTaken")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Death, "Shared.Status.Death")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_SuperArmor, "Shared.Status.SuperArmor")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_KnockdownOnHit, "Shared.Status.KnockdownOnHit")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_InAction, "Shared.Status.InAction")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact, "Shared.Status.HitReact")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_InAir, "Shared.Status.InAir")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Layer_UpperBody, "Shared.Layer.UpperBody")

	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_DamageTaken, "Shared.Event.DamageTaken")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_GuardBreak, "Shared.Event.GuardBreak")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MagicDamageTaken, "Shared.Event.MagicDamageTaken")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Front, "Shared.Event.HitReact.Front")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Back, "Shared.Event.HitReact.Back")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Right, "Shared.Event.HitReact.Right")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Left, "Shared.Event.HitReact.Left")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Block, "Shared.Event.HitReact.Block")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Knockdown_Front, "Shared.Event.HitReact.Knockdown.Front")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact_Knockdown_Back, "Shared.Event.HitReact.Knockdown.Back")
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_Death, "Shared.Event.Death")

	// Item Tag
	UE_DEFINE_GAMEPLAY_TAG(Item_Potion_Health, "Item.Potion.Health")
	UE_DEFINE_GAMEPLAY_TAG(Item_Potion_Mana, "Item.Potion.Mana")

	// Gameplay Cue Tag
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Blood, "GameplayCue.VFX.Blood")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_BloodExplosion, "GameplayCue.VFX.BloodExplosion")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Block, "GameplayCue.VFX.Block")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_BlockBreak, "GameplayCue.VFX.BlockBreak")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_CameraShake, "GameplayCue.VFX.CameraShake")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_MagicCast, "GameplayCue.VFX.MagicCast")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_SkeletonMage_MagicCast, "GameplayCue.VFX.SkeletonMage.MagicCast")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_EnterExecute, "GameplayCue.VFX.EnterExecute")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_StartFinishExecuted, "GameplayCue.VFX.StartFinishExecuted")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_EndFinishExecuted, "GameplayCue.VFX.EndFinishExecuted")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_MagicArrowHit, "GameplayCue.VFX.MagicArrowHit")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_FireArrowHit, "GameplayCue.VFX.FireArrowHit")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_MagicBoltHit, "GameplayCue.VFX.MagicBoltHit")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_WaterBallHit, "GameplayCue.VFX.WaterBallHit")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_StarfallHit, "GameplayCue.VFX.StarfallHit")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Kill, "GameplayCue.VFX.Kill")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Heal_Health, "GameplayCue.VFX.Heal.Health")
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_VFX_Heal_Mana, "GameplayCue.VFX.Heal.Mana")


}

