#include <windows.h>
#include <iostream>
using namespace std;

INPUT FabricateInput(LONG fx, LONG fy) {
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = fx;
	Input.mi.dy = fy;
	return Input;
}
// IS ACTUALLY ALT CLICK
bool GetKeyStateShift(char key) {
	if (GetKeyState(VK_MENU) & 0x8000)
	{
		if (GetKeyState(key) & 0x8000)
		{
			return true;
		}
	}
	return false;
}

int main() {
	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	POINT currentPos;
	POINT additivePos;
	bool inGame = false;
	while (true) {
		GetCursorPos(&currentPos);
		// shift k toggles between modes
		if (GetKeyStateShift('K')) {
			inGame = !inGame;
			Sleep(250);
		}
		// ingame cursor logic

					// if alt + right shift, then right click
		if (GetKeyState(VK_MENU) & 0x8000)
		{
			if (GetKeyState(VK_RSHIFT) & 0x8000)
			{
				INPUT Inputs[2] = { 0 };

				Inputs[0].type = INPUT_MOUSE;
				Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

				Inputs[1].type = INPUT_MOUSE;
				Inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

				::SendInput(3, Inputs, sizeof(INPUT));
			}
		}
		else { // left click on rshift
			if (GetKeyState(VK_RSHIFT) & 0x8000)
			{
				INPUT Inputs[2] = { 0 };

				Inputs[0].type = INPUT_MOUSE;
				Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

				Inputs[1].type = INPUT_MOUSE;
				Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

				::SendInput(3, Inputs, sizeof(INPUT));
			}
		}

		if (inGame) {
			// fabricate new ::sendinput parameters for each input 
			// probably able to do it better but /shrug

			if (GetKeyState(VK_UP) & 0x8000)
			{
				double fx = currentPos.x - 0 * (65535.0f / fScreenWidth);
				double fy = currentPos.y - 2 * (65535.0f / fScreenHeight);
				INPUT Input = FabricateInput(fx, fy);
				::SendInput(1, &Input, sizeof(INPUT));
			}
			if (GetKeyState(VK_LEFT) & 0x8000)
			{
				double fx = currentPos.x - 2 * (65535.0f / fScreenWidth);
				double fy = currentPos.y - 0 * (65535.0f / fScreenHeight);
				INPUT Input = FabricateInput(fx, fy);
				::SendInput(1, &Input, sizeof(INPUT));
			}
			if (GetKeyState(VK_DOWN) & 0x8000)
			{
				double fx = currentPos.x - 0 * (65535.0f / fScreenWidth);
				double fy = currentPos.y + 2 * (65535.0f / fScreenHeight);
				INPUT Input = FabricateInput(fx, fy);
				::SendInput(1, &Input, sizeof(INPUT));
			}
			if (GetKeyState(VK_RIGHT) & 0x8000)
			{
				double fx = currentPos.x + 2 * (65535.0f / fScreenWidth);
				double fy = currentPos.y - 0 * (65535.0f / fScreenHeight);
				INPUT Input = FabricateInput(fx, fy);
				::SendInput(1, &Input, sizeof(INPUT));
			}
		}
		//desktop cursor logic
		else {
			// add or remove 5 to cursor positon relative to each directon
			// this lets you move diagonally 
			// again can probably make this 100x better
			additivePos = currentPos;
			if (GetKeyState(VK_UP) & 0x8000)
			{
				additivePos.y -= 15;
			}
			if (GetKeyState(VK_DOWN) & 0x8000)
			{
				additivePos.y += 15;
			}
			if (GetKeyState(VK_LEFT) & 0x8000)
			{
				additivePos.x -= 15;
			}
			if (GetKeyState(VK_RIGHT) & 0x8000)
			{
				additivePos.x += 15;
			}
			// set cursor position to additive pos
			SetCursorPos(additivePos.x, additivePos.y);
		}
		// console log
		if (inGame) {
			cout << "ingame mode" << "\n";
		}
		else {
			cout << "desktop mode" << "\n";
		}
		Sleep(25);
	}

}