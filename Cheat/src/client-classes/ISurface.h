#pragma once
#include <cstdint>

enum EFontDrawType : std::int32_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE
};

enum EFontFlag
{
	FONT_FLAG_NONE,
	FONT_FLAG_ITALIC = 0x001,
	FONT_FLAG_UNDERLINE = 0x002,
	FONT_FLAG_STRIKEOUT = 0x004,
	FONT_FLAG_SYMBOL = 0x008,
	FONT_FLAG_ANTIALIAS = 0x010,
	FONT_FLAG_GAUSSIANBLUR = 0x020,
	FONT_FLAG_ROTARY = 0x040,
	FONT_FLAG_DROPSHADOW = 0x080,
	FONT_FLAG_ADDITIVE = 0x100,
	FONT_FLAG_OUTLINE = 0x200,
	FONT_FLAG_CUSTOM = 0x400,
	FONT_FLAG_BITMAP = 0x800,
};

class ISurface
{
public:
	void DrawSetColor(int r, int g, int b, int a = 255) noexcept
	{
		using Fn = void(__thiscall*)(ISurface*, int, int, int, int);
		Fn call = (*reinterpret_cast<Fn**>(this))[15];
		return call(this, r, g, b, a);
	}

	void DrawFilledRect(int x, int y, int xx, int yy) noexcept
	{
		using Fn = void(__thiscall*)(ISurface*, int, int, int, int);
		Fn call = (*reinterpret_cast<Fn**>(this))[16];
		return call(this, x, y, xx, yy);
	}

	void DrawOutlinedRect(int x, int y, int xx, int yy) noexcept
	{
		using Fn = void(__thiscall*)(ISurface*, int, int, int, int);
		Fn call = (*reinterpret_cast<Fn**>(this))[18];
		return call(this, x, y, xx, yy);
	}
};