#define DOWN_SCALE 2.0

#define MAX_INT_DIGITS 4

#define CHAR_SIZE    float2(8, 12)
#define CHAR_SPACING float2(8, 12)

#define STRWIDTH(c)  (c * CHAR_SPACING.x)
#define STRHEIGHT(c) (c * CHAR_SPACING.y)

#define NORMAL    0
#define INVERT    1
#define UNDERLINE 2

//https://www.shadertoy.com/view/Mt2GWD

/*
--------
-###----
##-##---
##-##---
-###----
#####-#-
##-####-
##--##--
##-###--
-###-##-
--------
--------

00000000
01110000
11011000
11011000
01110000
11111010
11011110
11001100
11011100
01110110
00000000
00000000

//Broken up into 4 8x3 (24 bit) chunks for each component of the float4.
//Hexadecimal is being used to reduce clutter in the code but decimal still works.

00000000
01110000 -> 00000000 01110000 11011000 -> 0x0070D8
11011000

11011000
01110000 -> 11011000 01110000 11111010 -> 0xD870FA
11111010

11011110
11001100 -> 11011110 11001100 11011100 -> 0xDECCDC
11011100

01110110
00000000 -> 01110110 00000000 00000000 -> 0x760000
00000000

float4(0x0070D8,0xD870FA,0xDECCDC,0x760000)
*/

//Automatically generated from the 8x12 font sheet here:
//http://www.massmind.org/techref/datafile/charset/extractor/charset_extractor.htm

#define ch_spc  float4(0x000000, 0x000000, 0x000000, 0x000000)
#define ch_exc  float4(0x003078, 0x787830, 0x300030, 0x300000)
#define ch_quo  float4(0x006666, 0x662400, 0x000000, 0x000000)
#define ch_hsh  float4(0x006C6C, 0xFE6C6C, 0x6CFE6C, 0x6C0000)
#define ch_dol  float4(0x30307C, 0xC0C078, 0x0C0CF8, 0x303000)
#define ch_pct  float4(0x000000, 0xC4CC18, 0x3060CC, 0x8C0000)
#define ch_amp  float4(0x0070D8, 0xD870FA, 0xDECCDC, 0x760000)
#define ch_apo  float4(0x003030, 0x306000, 0x000000, 0x000000)
#define ch_lbr  float4(0x000C18, 0x306060, 0x603018, 0x0C0000)
#define ch_rbr  float4(0x006030, 0x180C0C, 0x0C1830, 0x600000)
#define ch_ast  float4(0x000000, 0x663CFF, 0x3C6600, 0x000000)
#define ch_crs  float4(0x000000, 0x18187E, 0x181800, 0x000000)
#define ch_com  float4(0x000000, 0x000000, 0x000038, 0x386000)
#define ch_dsh  float4(0x000000, 0x0000FE, 0x000000, 0x000000)
#define ch_per  float4(0x000000, 0x000000, 0x000038, 0x380000)
#define ch_lsl  float4(0x000002, 0x060C18, 0x3060C0, 0x800000)
#define ch_0    float4(0x007CC6, 0xD6D6D6, 0xD6D6C6, 0x7C0000)
#define ch_1    float4(0x001030, 0xF03030, 0x303030, 0xFC0000)
#define ch_2    float4(0x0078CC, 0xCC0C18, 0x3060CC, 0xFC0000)
#define ch_3    float4(0x0078CC, 0x0C0C38, 0x0C0CCC, 0x780000)
#define ch_4    float4(0x000C1C, 0x3C6CCC, 0xFE0C0C, 0x1E0000)
#define ch_5    float4(0x00FCC0, 0xC0C0F8, 0x0C0CCC, 0x780000)
#define ch_6    float4(0x003860, 0xC0C0F8, 0xCCCCCC, 0x780000)
#define ch_7    float4(0x00FEC6, 0xC6060C, 0x183030, 0x300000)
#define ch_8    float4(0x0078CC, 0xCCEC78, 0xDCCCCC, 0x780000)
#define ch_9    float4(0x0078CC, 0xCCCC7C, 0x181830, 0x700000)
#define ch_col  float4(0x000000, 0x383800, 0x003838, 0x000000)
#define ch_scl  float4(0x000000, 0x383800, 0x003838, 0x183000)
#define ch_les  float4(0x000C18, 0x3060C0, 0x603018, 0x0C0000)
#define ch_equ  float4(0x000000, 0x007E00, 0x7E0000, 0x000000)
#define ch_grt  float4(0x006030, 0x180C06, 0x0C1830, 0x600000)
#define ch_que  float4(0x0078CC, 0x0C1830, 0x300030, 0x300000)
#define ch_ats  float4(0x007CC6, 0xC6DEDE, 0xDEC0C0, 0x7C0000)
#define ch_A    float4(0x003078, 0xCCCCCC, 0xFCCCCC, 0xCC0000)
#define ch_B    float4(0x00FC66, 0x66667C, 0x666666, 0xFC0000)
#define ch_C    float4(0x003C66, 0xC6C0C0, 0xC0C666, 0x3C0000)
#define ch_D    float4(0x00F86C, 0x666666, 0x66666C, 0xF80000)
#define ch_E    float4(0x00FE62, 0x60647C, 0x646062, 0xFE0000)
#define ch_F    float4(0x00FE66, 0x62647C, 0x646060, 0xF00000)
#define ch_G    float4(0x003C66, 0xC6C0C0, 0xCEC666, 0x3E0000)
#define ch_H    float4(0x00CCCC, 0xCCCCFC, 0xCCCCCC, 0xCC0000)
#define ch_I    float4(0x007830, 0x303030, 0x303030, 0x780000)
#define ch_J    float4(0x001E0C, 0x0C0C0C, 0xCCCCCC, 0x780000)
#define ch_K    float4(0x00E666, 0x6C6C78, 0x6C6C66, 0xE60000)
#define ch_L    float4(0x00F060, 0x606060, 0x626666, 0xFE0000)
#define ch_M    float4(0x00C6EE, 0xFEFED6, 0xC6C6C6, 0xC60000)
#define ch_N    float4(0x00C6C6, 0xE6F6FE, 0xDECEC6, 0xC60000)
#define ch_O    float4(0x00386C, 0xC6C6C6, 0xC6C66C, 0x380000)
#define ch_P    float4(0x00FC66, 0x66667C, 0x606060, 0xF00000)
#define ch_Q    float4(0x00386C, 0xC6C6C6, 0xCEDE7C, 0x0C1E00)
#define ch_R    float4(0x00FC66, 0x66667C, 0x6C6666, 0xE60000)
#define ch_S    float4(0x0078CC, 0xCCC070, 0x18CCCC, 0x780000)
#define ch_T    float4(0x00FCB4, 0x303030, 0x303030, 0x780000)
#define ch_U    float4(0x00CCCC, 0xCCCCCC, 0xCCCCCC, 0x780000)
#define ch_V    float4(0x00CCCC, 0xCCCCCC, 0xCCCC78, 0x300000)
#define ch_W    float4(0x00C6C6, 0xC6C6D6, 0xD66C6C, 0x6C0000)
#define ch_X    float4(0x00CCCC, 0xCC7830, 0x78CCCC, 0xCC0000)
#define ch_Y    float4(0x00CCCC, 0xCCCC78, 0x303030, 0x780000)
#define ch_Z    float4(0x00FECE, 0x981830, 0x6062C6, 0xFE0000)
#define ch_lsb  float4(0x003C30, 0x303030, 0x303030, 0x3C0000)
#define ch_rsl  float4(0x000080, 0xC06030, 0x180C06, 0x020000)
#define ch_rsb  float4(0x003C0C, 0x0C0C0C, 0x0C0C0C, 0x3C0000)
#define ch_pow  float4(0x10386C, 0xC60000, 0x000000, 0x000000)
#define ch_usc  float4(0x000000, 0x000000, 0x000000, 0x00FF00)
#define ch_a    float4(0x000000, 0x00780C, 0x7CCCCC, 0x760000)
#define ch_b    float4(0x00E060, 0x607C66, 0x666666, 0xDC0000)
#define ch_c    float4(0x000000, 0x0078CC, 0xC0C0CC, 0x780000)
#define ch_d    float4(0x001C0C, 0x0C7CCC, 0xCCCCCC, 0x760000)
#define ch_e    float4(0x000000, 0x0078CC, 0xFCC0CC, 0x780000)
#define ch_f    float4(0x00386C, 0x6060F8, 0x606060, 0xF00000)
#define ch_g    float4(0x000000, 0x0076CC, 0xCCCC7C, 0x0CCC78)
#define ch_h    float4(0x00E060, 0x606C76, 0x666666, 0xE60000)
#define ch_i    float4(0x001818, 0x007818, 0x181818, 0x7E0000)
#define ch_j    float4(0x000C0C, 0x003C0C, 0x0C0C0C, 0xCCCC78)
#define ch_k    float4(0x00E060, 0x60666C, 0x786C66, 0xE60000)
#define ch_l    float4(0x007818, 0x181818, 0x181818, 0x7E0000)
#define ch_m    float4(0x000000, 0x00FCD6, 0xD6D6D6, 0xC60000)
#define ch_n    float4(0x000000, 0x00F8CC, 0xCCCCCC, 0xCC0000)
#define ch_o    float4(0x000000, 0x0078CC, 0xCCCCCC, 0x780000)
#define ch_p    float4(0x000000, 0x00DC66, 0x666666, 0x7C60F0)
#define ch_q    float4(0x000000, 0x0076CC, 0xCCCCCC, 0x7C0C1E)
#define ch_r    float4(0x000000, 0x00EC6E, 0x766060, 0xF00000)
#define ch_s    float4(0x000000, 0x0078CC, 0x6018CC, 0x780000)
#define ch_t    float4(0x000020, 0x60FC60, 0x60606C, 0x380000)
#define ch_u    float4(0x000000, 0x00CCCC, 0xCCCCCC, 0x760000)
#define ch_v    float4(0x000000, 0x00CCCC, 0xCCCC78, 0x300000)
#define ch_w    float4(0x000000, 0x00C6C6, 0xD6D66C, 0x6C0000)
#define ch_x    float4(0x000000, 0x00C66C, 0x38386C, 0xC60000)
#define ch_y    float4(0x000000, 0x006666, 0x66663C, 0x0C18F0)
#define ch_z    float4(0x000000, 0x00FC8C, 0x1860C4, 0xFC0000)
#define ch_lpa  float4(0x001C30, 0x3060C0, 0x603030, 0x1C0000)
#define ch_bar  float4(0x001818, 0x181800, 0x181818, 0x180000)
#define ch_rpa  float4(0x00E030, 0x30180C, 0x183030, 0xE00000)
#define ch_tid  float4(0x0073DA, 0xCE0000, 0x000000, 0x000000)
#define ch_lar  float4(0x000000, 0x10386C, 0xC6C6FE, 0x000000)

//Extracts bit b from the given number.
//Shifts bits right (num / 2^bit) then ANDs the result with 1 (mod(result,2.0)).
float extract_bit(float n, float b)
{
    b = clamp(b, -1.0, 24.0);
    return floor(fmod(floor(n / pow(2.0, floor(b))), 2.0));
}

//Returns the pixel at uv in the given bit-packed sprite.
float sprite(float4 spr, float2 size, float2 uv)
{
    uv = floor(uv);

    //Calculate the bit to extract (x + y * width) (flipped on x-axis)
    float bit = (size.x - uv.x - 1.0) + uv.y * size.x;

    //Clipping bound to remove garbage outside the sprite's boundaries.
    bool bounds = all(uv >= float2(0.0,0.0)) && all(uv < size);

    float pixels = 0.0;
    pixels += extract_bit(spr.x, bit - 72.0);
    pixels += extract_bit(spr.y, bit - 48.0);
    pixels += extract_bit(spr.z, bit - 24.0);
    pixels += extract_bit(spr.w, bit - 00.0);

    return bounds ? pixels : 0.0;
}

//Prints a character and moves the print position forward by 1 character width.
float _char(float4 ch, float2 uv, inout float2 print_pos, int TEXT_MODE)
{
    if (TEXT_MODE == INVERT)
    {
        //Inverts all of the bits in the character.
        ch = pow(2.0, 24.0) - 1.0 - ch;
    }
    if (TEXT_MODE == UNDERLINE)
    {
        //Makes the bottom 8 bits all 1.
        //Shifts the bottom chunk right 8 bits to drop the lowest 8 bits,
        //then shifts it left 8 bits and adds 255 (binary 11111111).
        ch.w = floor(ch.w / 256.0) * 256.0 + 255.0;
    }

    float px = sprite(ch, CHAR_SIZE, uv - print_pos);
    print_pos.x += CHAR_SPACING.x;
    return px;
}


//Returns the digit sprite for the given number.
float4 get_digit(float d)
{
    d = floor(d);

    if (d == 0.0) return ch_0;
    if (d == 1.0) return ch_1;
    if (d == 2.0) return ch_2;
    if (d == 3.0) return ch_3;
    if (d == 4.0) return ch_4;
    if (d == 5.0) return ch_5;
    if (d == 6.0) return ch_6;
    if (d == 7.0) return ch_7;
    if (d == 8.0) return ch_8;
    if (d == 9.0) return ch_9;
    return float4(0.0,0.0,0.0,0.0);
}

//Prints out the given number starting at pos.
float print_number(float number, float2 uv, inout float2 print_pos, int TEXT_MODE)
{
    float result = 0.0;

    for (int i = 3; i >= -1; i--)
    {
        float digit = fmod(number / pow(10.0, float(i)), 10.0);

        if (i == -1) //Add a decimal point.
        {
            result += _char(ch_per, uv, print_pos, TEXT_MODE);
        }

        if (abs(number) > pow(10.0, float(i)) || i == 0) //Clip off leading zeros.
        {
            result += _char(get_digit(digit), uv, print_pos, TEXT_MODE);
        }
    }
    return result;
}

float print_integer(float number, int zeros, float2 uv, inout float2 print_pos, int TEXT_MODE)
{
    float result = 0.0;

    for (int i = MAX_INT_DIGITS; i >= 0; i--)
    {
        float digit = fmod(number / pow(10.0, float(i)), 10.0);

        if (abs(number) > pow(10.0, float(i)) || zeros > i || i == 0) //Clip off leading zeros.
        {
            result += _char(get_digit(digit), uv, print_pos, TEXT_MODE);
        }
    }
    return result;
}
