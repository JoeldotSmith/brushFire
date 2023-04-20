//
#include "eyebot++.h"
#include <vector>
using std::vector;

typedef struct Pixel
{
    int x;
    int y;
    int id;
    int expansion = 0;
    int value;
    bool voroni = false;
    vector<int> allObjectId;
} Pixel;

struct Object
{
    int id;
    vector<Pixel> allPixels;

    void addPixeltoLine(Pixel pixel)
    {
        allPixels.push_back(pixel);
    }

} Object;

struct Walls
{
    vector<Pixel> top;
    vector<Pixel> bottom;
    vector<Pixel> left;
    vector<Pixel> right;

    void addPixeltoTop(Pixel pixel)
    {
        top.push_back(pixel);
    }
    void addPixeltoBottom(Pixel pixel)
    {
        bottom.push_back(pixel);
    }
    void addPixeltoLeft(Pixel pixel)
    {
        left.push_back(pixel);
    }
    void addPixeltoTop(Pixel pixel)
    {
        right.push_back(pixel);
    }
    vector<vector<Pixel>> getAll()
    {
        vector<vector<Pixel>> all;
        all.push_back(top);
        all.push_back(bottom);
        all.push_back(left);
        all.push_back(right);
    }
} Walls;
