#include "stdafx.h"
#include "SquidCompositor.h"

namespace
{
    inline BYTE ClampToByte(int value)
    {
        if (value < 0)   return 0;
        if (value > 255) return 255;
        return (BYTE)value;
    }
}

bool ComposeSquidImage(CxImage& head, CxImage& body, CxImage& points, CxImage& result)
{
    if (!head.IsValid() || !body.IsValid() || !points.IsValid())
        return false;

    DWORD width  = head.GetWidth();
    DWORD height = head.GetHeight();
    width  = min(width,  body.GetWidth());
    width  = min(width,  points.GetWidth());
    height = min(height, body.GetHeight());
    height = min(height, points.GetHeight());

    if (width == 0 || height == 0)
        return false;

    result.Create(width, height, 24, CXIMAGE_FORMAT_PNG);
    if (!result.IsValid())
        return false;

    for (DWORD y = 0; y < height; y++)
    {
        for (DWORD x = 0; x < width; x++)
        {
            RGBQUAD headColor   = head.GetPixelColor(x, y);
            RGBQUAD bodyColor   = body.GetPixelColor(x, y);
            RGBQUAD pointsColor = points.GetPixelColor(x, y);

            // Step 1: add squid_head and squid_body.
            int r = (int)headColor.rgbRed   + (int)bodyColor.rgbRed;
            int g = (int)headColor.rgbGreen + (int)bodyColor.rgbGreen;
            int b = (int)headColor.rgbBlue  + (int)bodyColor.rgbBlue;

            r = ClampToByte(r);
            g = ClampToByte(g);
            b = ClampToByte(b);

            // Step 2: subtract squid_points from the result of step 1.
            r = r - (int)pointsColor.rgbRed;
            g = g - (int)pointsColor.rgbGreen;
            b = b - (int)pointsColor.rgbBlue;

            RGBQUAD newColor;
            newColor.rgbRed      = ClampToByte(r);
            newColor.rgbGreen    = ClampToByte(g);
            newColor.rgbBlue     = ClampToByte(b);
            newColor.rgbReserved = 0;

            result.SetPixelColor(x, y, newColor);
        }
    }

    return true;
}
