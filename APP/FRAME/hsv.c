#include "hsv.h"

#define HSV_SCAL 100
/*
void rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue, hsv_t *hsv)
{
	float h, s, v, r, g, b, minRGB, maxRGB, deltaRGB;
	r = red / 255.0f;	
	g = green / 255.0f;
	b = blue / 255.0f;
	minRGB = MIN(MIN(r, g), b);
	maxRGB = MAX(MAX(r, g), b);
	deltaRGB = maxRGB - minRGB;

	if (maxRGB != 0.0)
	{
		s = deltaRGB / maxRGB;
	}
	else
	{
		s = 0;
	}
	
	if (deltaRGB == 0.0)
	{
		h = 0.0;
	}
	else
	{
		if (r == maxRGB)
		{
			h = 60 * ((g - b) / deltaRGB + 0.0) / 360;
		}
		if (g == maxRGB)
		{
			h = 60 * ((b - r) / deltaRGB + 2.0) / 360;
		}
		if (b == maxRGB)
		{
			h = 60 * ((r - g) / deltaRGB + 4.0) / 360;
		}
	}
	v = maxRGB;

	hsv->h = h * HSV_SCAL;
	hsv->s = s * HSV_SCAL;
	hsv->v = v * HSV_SCAL;
}
*/
void hsv_to_rgb(hsv_t *hsv, uint8_t *red, uint8_t *green, uint8_t *blue)
{
	data uint16_t tmp;
    data uint8_t r, g, b;
	data uint8_t i, f, p, q, t;

	tmp = hsv->h * 6;
    i = tmp / HSV_SCAL;
	f = tmp - i * HSV_SCAL;
	if (f < 5) f = 0;
    p = hsv->v * (HSV_SCAL - hsv->s) / HSV_SCAL;
    q = hsv->v * (HSV_SCAL - f * hsv->s / HSV_SCAL) / HSV_SCAL;
    t = hsv->v * (HSV_SCAL - (HSV_SCAL - f) * hsv->s / HSV_SCAL) / HSV_SCAL;

    switch (i % 6)
	{
        case 0: r = hsv->v, g = t, b = p; break;
        case 1: r = q, g = hsv->v, b = p; break;
        case 2: r = p, g = hsv->v, b = t; break;
        case 3: r = p, g = q, b = hsv->v; break;
        case 4: r = t, g = p, b = hsv->v; break;
        case 5: r = hsv->v, g = p, b = q; break;
    }

    *red = r * 255 / HSV_SCAL;
    *green = g * 255 / HSV_SCAL;
    *blue = b * 255 / HSV_SCAL;
}

#undef HSV_SCAL
