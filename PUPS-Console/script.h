#include "MathTools.h";
#pragma once
#ifndef OFFSET_H
#define OFFSET_H
#endif

#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#endif
#include <stdlib.h>


using namespace std;

class Script {

	DWORD i;
	DWORD glowIdx;
	DWORD health;
	DWORD localPlayerTeamNum;
	DWORD entity_team_num;
	DWORD entity;
	DWORD dwLocalPlayer = 0;
	DWORD LocalPlayer_inCross;
	DWORD LocalPlayer_Team;
	BYTE fFlags = 0;
	DWORD Trigger_EntityBase;
	DWORD Trigger_EntityTeam;
	bool Trigger_EntityDormant;
	DWORD glow_obj;

	struct glow_struct {
		BYTE base[4];
		float red;
		float green;
		float blue;
		float alpha;
		BYTE buff0[16];
		bool occluded;
		bool unoccluded;
		bool full_bloom;
		BYTE buff1[5];
		int glow_style;
	};

	glow_struct gst;

public:

	Script(MemoryManager* mem) {
		this->dwLocalPlayer = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwLocalPlayer);//Получаем LocalPlayer
		this->LocalPlayer_inCross = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_iCrosshairId);
		this->LocalPlayer_Team = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_iTeamNum);
	}

	void Bhop(MemoryManager* mem) {
		this->dwLocalPlayer = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwLocalPlayer);
		fFlags = mem->read<BYTE>(dwLocalPlayer + offset::netvar::m_fFlags);
		if (fFlags & (1 << 0)) {
			mem->write(mem->get_client_base() + offset::signature::dwForceJump, 6);
		}
	}

	void triggerBot(MemoryManager* mem) {
		dwLocalPlayer = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwLocalPlayer);
		LocalPlayer_inCross = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_iCrosshairId);
		LocalPlayer_Team = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_iTeamNum);
		DWORD Trigger_EntityBase = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwEntityList + ((LocalPlayer_inCross - 1) * 0x10));
		DWORD Trigger_EntityTeam = mem->read<DWORD>(Trigger_EntityBase + offset::netvar::m_iTeamNum);
		bool Trigger_EntityDormant = mem->read<bool>(Trigger_EntityBase + offset::signature::m_bDormant);
		if ((LocalPlayer_inCross > 0 && LocalPlayer_inCross <= 64) && (Trigger_EntityBase != NULL) && (Trigger_EntityTeam != LocalPlayer_Team) && (!Trigger_EntityDormant)) {
			Sleep(10);
			mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
			Sleep(10);
			mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
			Sleep(10);
		}
	}

	void wall_hack(MemoryManager* mem) {
		dwLocalPlayer = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwLocalPlayer);
		localPlayerTeamNum = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_iTeamNum);
		glow_obj = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwGlowObjectManager);
		for (i = 0; i < 64; i++) {
			entity = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwEntityList + i * 0x10);
			if (entity != 0x0) {
				glowIdx = mem->read<DWORD>(entity + offset::netvar::m_iGlowIndex);
				entity_team_num = mem->read<DWORD>(entity + offset::netvar::m_iTeamNum);
				if (entity_team_num != localPlayerTeamNum) {
					gst = mem->read<glow_struct>(glow_obj + (glowIdx * 0x38));
					health = mem->read<DWORD>(entity + offset::netvar::m_iHealth);
					gst.blue = 0.0f;
					gst.alpha = 1.0f;
					gst.green = health * 0.01;
					gst.red = health * -0.01 + 1;
					gst.occluded = true;
					gst.unoccluded = false;
					mem->write<glow_struct>(glow_obj + (glowIdx * 0x38), gst);
				}

			}
		}
	}

	void radar_hack(MemoryManager* mem) {
		dwLocalPlayer = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwLocalPlayer);
		localPlayerTeamNum = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_iTeamNum);

		for (DWORD i = 0; i < 64; i++) {
			entity = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwEntityList + i * 0x10);
			if (entity != 0x0) {
				entity_team_num = mem->read<DWORD>(entity + offset::netvar::m_iTeamNum);
				if (entity_team_num != localPlayerTeamNum) {
					BOOL entSpotted = mem->read<BOOL>(entity + offset::netvar::m_bSpotted);
					if (!entSpotted) {
						mem->write<DWORD>(entity + offset::netvar::m_bSpotted, TRUE);
					}
				}
			}

		}

	}

	void aim_bot(MemoryManager* mem) {
		int pLocal = mem->read<int>(mem->get_client_base() + offset::signature::dwLocalPlayer);

		int pEngine = mem->read<int>(mem->get_engine_base() + offset::signature::dwClientState);

		if (!mem->read<int>(pLocal + offset::netvar::m_iHealth)) return;

		if (pEngine) {
			int target = getTarget(pLocal, pEngine, mem); /*get the bes target addr*/
			if (target != -1) {
				//LocalPlayer_inCross = mem->read<DWORD>(pLocal + offset::netvar::m_iCrosshairId);
				//if (LocalPlayer_inCross > 0 && LocalPlayer_inCross <= 64) {
					setViewAngel(pEngine, AngleToTarget(pLocal, target, 8, mem), mem); /*set new angle*/
					//triggerBot(mem);
				//}
				
			}
		}
	}

	/*Get position of bone in the world*/
	Vector getBonePos(int pTargetIn, int bone, MemoryManager* mem) {
		const int matrix = mem->read<int>(pTargetIn + offset::netvar::m_dwBoneMatrix);
		return Vector (
			mem->read<float>(matrix + 0x30 * bone + 0xC),
			mem->read<float>(matrix + 0x30 * bone + 0x1C),
			mem->read<float>(matrix + 0x30 * bone + 0x2C)
		);
	}

	/*Set new ViewAngel*/
	void setViewAngel(DWORD EngineBase, Vector angel, MemoryManager* mem) {
		ClampAngles(angel);
		NormalizeAngles(angel);

		mem->write<Vector>(EngineBase + offset::signature::dwClientState_ViewAngles, angel);
	}

	/*Calc angle to target bone*/
	Vector AngleToTarget(int pLocal, int pTargetIn, int boneIndex, MemoryManager* mem) {
		const Vector position = mem->read<Vector>(pLocal + offset::netvar::m_vecOrigin);
		const Vector vecView = mem->read<Vector>(pLocal + offset::netvar::m_vecViewOffset);

		const Vector myView = position + vecView;
		const Vector aimView = getBonePos(pTargetIn, boneIndex, mem);

		Vector dst = CalcAngle(myView, aimView).ToVector();

		NormalizeAngles(dst);

		return dst;
	}

	/*get the target*/

	int getTarget(int pLocal, int clientState, MemoryManager* mem) {
		int courrentTarget = -1;
		float misDist = 360.f;

		Vector _viewAngles = mem->read<Vector>(clientState + offset::signature::dwClientState_ViewAngles);

		const int playerTeam = mem->read<int>(pLocal + offset::netvar::m_iTeamNum);

		for (int i = 0; i < 64; i++) {
			const int target = mem->read<int>(mem->get_client_base() + offset::signature::dwEntityList + i * 0x10);

			if (target > 0 || target) {
				const int targetHealth = mem->read<int>(target + offset::netvar::m_iHealth);
				BOOL entSpotted = mem->read<BOOL>(target + offset::netvar::m_bSpotted);
				if (entSpotted)
				if (targetHealth > 0 || targetHealth) {
					const int targetTeam = mem->read<DWORD>(target + offset::netvar::m_iTeamNum);

					if (targetTeam != playerTeam || !targetTeam) {
						const bool targetDormant = mem->read<bool>(target + offset::signature::m_bDormant);

						if (!targetDormant) {
							Vector angelToTarget = AngleToTarget(pLocal, target, 8 /*head bone index head_0*/, mem);

							float fov = GetFov(_viewAngles.ToQAngle(), angelToTarget.ToQAngle());

							if (fov < misDist && fov <= 20) /*sort by fov*/
							{
								misDist = fov;
								courrentTarget = target;
							}
						}
					}
				}
			}	
		}
		return courrentTarget;
	}

	void anti_flash(MemoryManager* mem) {
		dwLocalPlayer = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwLocalPlayer);
		localPlayerTeamNum = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_iTeamNum);

		mem->write<DWORD>(dwLocalPlayer + offset::netvar::m_flFlashDuration, 0);

	}

	void debug_function(MemoryManager* mem) {
		dwLocalPlayer = mem->read<DWORD>(mem->get_client_base() + offset::signature::dwLocalPlayer);
		localPlayerTeamNum = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_iTeamNum);

		DWORD tickBase = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_bBombPlanted);
		printf_s("tickBase %u \n", tickBase);
		//DWORD tickBase = mem->read<DWORD>(mem->get_client_base() + offset::signature::m_flSpawnTime);
		//mem->write<DWORD>(dwLocalPlayer + offset::netvar::m_iHealth, 100);

		//Vector position = mem->read<Vector>(dwLocalPlayer + offset::netvar::m_vecOrigin);

		
		/*printf_s(" position x %f \n", position.x);
		printf_s(" position y %f \n", position.y);
		printf_s(" position z %f \n", position.z);*/
		//position.z = position.z + (float)1;
		//mem->write<Vector>(dwLocalPlayer + offset::netvar::m_vecOrigin, position);

		/*DWORD PunchAngle = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_aimPunchAngle);
		DWORD PunchAngleVel = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_aimPunchAngleVel);
		DWORD EyeAnglesX = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_angEyeAnglesX);
		DWORD EyeAnglesY = mem->read<DWORD>(dwLocalPlayer + offset::netvar::m_angEyeAnglesY);
		if (system("CLS")) system("clear");
		printf_s("PunchAngle %u \n", PunchAngle);
		printf_s("PunchAngleVel %u \n", PunchAngleVel);
		printf_s("EyeAnglesX %u \n", EyeAnglesX);
		printf_s("EyeAnglesY %u \n", EyeAnglesY);*/

		//mem->write<DWORD>(dwLocalPlayer + offset::netvar::m_aimPunchAngle, 0);
		//mem->write<DWORD>(dwLocalPlayer + offset::netvar::m_aimPunchAngleVel, 0);

		
	}

};
