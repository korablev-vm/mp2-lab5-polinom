﻿#ifndef __MENUDESIGN_H__
#define __MENUDESIGN_H__

#include <conio.h>
#include <windows.h>
#include <locale>
#include <iostream>

using namespace std;

enum TColor {
	BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY,
	DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
	LIGHTMAGENTA, YELLOW, WHITE
};
//-----------------------------------------------------------------------------
COORD wherexy(void)
{
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { -1, -1 };
	if (!Console)
		return pos;

	CONSOLE_SCREEN_BUFFER_INFO buf;

	GetConsoleScreenBufferInfo(Console, &buf);
	return buf.dwCursorPosition;
}

//-----------------------------------------------------------------------------
void clrscr(void)
{
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!Console)
		return;

	CONSOLE_SCREEN_BUFFER_INFO buf;
	GetConsoleScreenBufferInfo(Console, &buf);

	WORD Attr;
	DWORD Count;
	COORD pos = buf.dwCursorPosition;
	ReadConsoleOutputAttribute(Console, &Attr, 1, pos, &Count);

	int col = Attr;
	int width = buf.dwSize.X;
	int height = buf.dwSize.Y;

	COORD zpos;
	zpos.X = 0;
	zpos.Y = 0;
	SetConsoleCursorPosition(Console, zpos);

	FillConsoleOutputAttribute(Console, col, width * height, zpos, &Count);
	FillConsoleOutputCharacter(Console, ' ', width * height, zpos, &Count);
}

//-----------------------------------------------------------------------------
void gotoxy(int x, int y)
{
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!Console)
		return;

	COORD pos;
	pos.X = x;
	pos.Y = y;

	SetConsoleCursorPosition(Console, pos);
}

//-----------------------------------------------------------------------------
void textcolor(int color)
{
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!Console)
		return;

	CONSOLE_SCREEN_BUFFER_INFO buf;
	GetConsoleScreenBufferInfo(Console, &buf);

	WORD Attr;
	DWORD Count;
	COORD pos = buf.dwCursorPosition;
	ReadConsoleOutputAttribute(Console, &Attr, 1, pos, &Count);

	int bk_col = Attr / 16;
	int col = color % 16;
	col = col + bk_col * 16;

	SetConsoleTextAttribute(Console, col);

	wchar_t  ch;
	pos = wherexy();
	ReadConsoleOutputCharacter(Console, &ch, 1, pos, &Count);
	wcout << ch;
	gotoxy(pos.X, pos.Y);
}

void hidecursor(void)
{
	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!Console)
		return;

	CONSOLE_CURSOR_INFO buf;
	GetConsoleCursorInfo(Console, &buf);
	buf.bVisible = 0;
	SetConsoleCursorInfo(Console, &buf);
}

void exit_prog()
{
	system("cls");
	cout << "Спасибо за использование программы!" << endl;
	cout << "До свидания" << endl;
	system("pause");
	exit(0);
}

int menu_mode_clear(string* arr, int size)
{
	char ch = 0;
	int choice = 1, i;
	while (ch != 13)
	{
		system("cls");
		for (i = 0; i < size; i++)
		{
			if (i == choice)
			{
				textcolor(LIGHTMAGENTA);
			}
			else
			{
				textcolor(WHITE);
			}
			cout << arr[i] << endl;
		}
		hidecursor();
		textcolor(WHITE);
		ch = _getch();
		if (ch == -32)
			ch = _getch();
		switch (ch)
		{
		case 72:
			choice--;
			break;
		case 80:
			choice++;
			break;
		case 27:
			exit_prog();
		case 37:
			return -1;
		}
		if (choice >= size)
			choice = 1;
		if (choice < 1)
			choice = size - 1;
	}
	return choice;
}

#endif