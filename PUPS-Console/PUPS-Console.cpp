#include <iostream>
#include <windows.h>
#include <cstdio>
#include <tchar.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <conio.h>
#include "MemoryManager.h"
#include "Offset.h"
#include "script.h"
#include <stdlib.h>
#include <stdio.h>
#include "Authorization.h"





using namespace std;

/*const char logo[][51] = {
	"  _  ___     _          __        __    ___",
	" | |/ / |__ (_) __ _ _ _\\ \\      / /_ _|  _ \\ ___ ",
	" | ' /| '_ \\| |/ _` | '__\\ \\ /\\ / / _` | |_) / _ \\",
	" | . \\| | | | | (_| | |   \\ V  V / (_| |  _ <  __/",
	" |_|\\_\\_| |_|_|\\__,_|_|    \\_/\\_/ \\__,_|_| \\_\\___|"
};*/

void isActive(bool flag, HANDLE h) {
	if (flag) {
		SetConsoleTextAttribute(h, FOREGROUND_GREEN);
		printf_s("Activated \n");
	}
	else {
		SetConsoleTextAttribute(h, FOREGROUND_RED);
		printf_s("Deactivated \n");
	}
	SetConsoleTextAttribute(h, FOREGROUND_INTENSITY);
}


int main() {

	/*Authorization* auth = new Authorization();
	cout << "Exodus version 1.0" << endl;
	cout << "Website http://exodus-cheat.ru" << endl << endl;
	cout << "Please log in to continue.!" << endl;
	do {
		string email;
		string password;
		cout << "Email: ";
		cin >> email;
		cout << "Password: ";
		cin >> password;
	

		auth->setLogin(email);
		auth->setPassword(password);
	} while (!auth->isAuth());
	
	if (auth->getUser() == NULL) return 1;

	User* user = auth->getUser();

	cout << "Welcome "+user->getName() << endl;
	cout << "Subscription date from " + user->subscription->getDateBegining() + " to " + user->subscription->getDateExpiration() << endl << endl;



	//_getch();
	const TCHAR console_name[20] = TEXT("EXODUS");
	SetConsoleTitle(console_name);

	/*for (int i = 0; i < 5; i++) {
		printf_s("%s\n", logo[i]);
	}*/
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	


	MemoryManager* mem = new MemoryManager();
	Script script(mem);
	int key;
	bool is_running = true;
	bool wall_en = false;
	bool bhop_en = false;
	bool trigger_en = false;
	bool radar_hack = false;
	bool aim_en = false;
	bool anti_flash = false;
	//printf("Чит Вонючая Полина Активэйтеддд!");

	printf_s("\nInsert - WallHack\n");
	printf_s("Home - RadarHack\n");
	printf_s("PageUp - TriggerBot\n");
	printf_s("PageDown - AimBot\n");
	printf_s("END - AntiFlash\n");
	printf_s("DEL - Exit\n\n");


	do {
		Sleep(20);
		for (key = 8; key < 255; key++) {
			if (is_running == false) {
				break;
			}
			if (GetAsyncKeyState(key) & 0x8000) {
				switch (key) {
				case VK_INSERT: {
					wall_en = !wall_en;
					Sleep(300);
					printf_s("Wallhack: "); // Wall hack кнопка INSERT
					isActive(wall_en, h);
					break;
				}
				case VK_HOME: {
					/*bhop_en = !bhop_en;
					Sleep(300);
					printf_s("Bhop: %d \n", bhop_en); // Bhop кнопка Home
					break;*/
					radar_hack = !radar_hack;
					printf_s("Radar Hack: ");
					isActive(radar_hack, h);
					Sleep(300);
					break;
				}
				case VK_DELETE: {
					is_running = false; // quit the game with F1
					printf_s("Goodbye :)\n");
					Sleep(300);
					break;
				}
				case VK_PRIOR: {
					trigger_en = !trigger_en;
					//debug = !debug;
					printf_s("Trigger bot: "); // Trigger bot кнопка PgUp
					isActive(trigger_en, h);
					Sleep(300);
					break;
				}
				case VK_NEXT: {
					aim_en = !aim_en;
					printf_s("Aim bot: ");
					isActive(aim_en, h);
					Sleep(300);
					break;
				}
				case VK_END: {
					anti_flash = !anti_flash;
					printf_s("Anti Flash: ");
					isActive(anti_flash, h);
					Sleep(300);
					break;
				}
				default: {
					break;
				}
				}
			}
			

			if (GetKeyState(VK_SPACE) & 0x8000) {
				if (bhop_en) {
					script.Bhop(mem);
				}
			}
			if (GetKeyState(VK_MBUTTON) & 0x8000) {
				if (trigger_en) {
					script.triggerBot(mem);
				}
			}
			if (wall_en) {
				script.wall_hack(mem);
			}
			if (radar_hack) {
				script.radar_hack(mem);
				//script.debug_function(mem);
			}
			if (aim_en) {
				script.aim_bot(mem);
			}
			if (anti_flash) {
				script.anti_flash(mem);
			}
		}
	} while (is_running);


	return 0;
}

