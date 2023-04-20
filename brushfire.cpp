//
#include "eyebot++.h"
#include "image.h"
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

typedef struct Object
{
    int id;
    vector<Pixel> allPixels;

    void addPixeltoLine(Pixel pixel)
    {
        allPixels.push_back(pixel);
    }

} Object;

typedef struct Walls
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

// World Name
char *fileName = "u.pbm";

// Globals
BYTE *image;
vector<Pixel> allPixels;
vector<Object> allObject;
int numberOfObjects = 0;

int WORLDSIZE = 4000;
int IMAGESIZE = 128;

vector<int> getReletiveGoalLocation(int x, int y)
{
    int dist = 0; // TODO
    int rot = 0;  // TODO
    vector<int> callBack;
    callBack.push_back(dist);
    callBack.push_back(rot);
    return callBack;
}

void driveToPoint(int x, int y)
{
    // TODO
    return;
}

void LCDPixelBigger(int x, int y, COLOR colour)
{
    LCDArea(x ^ 2, y ^ 2, x ^ 2 + 2, y ^ 2 + 2, colour, 1);
}

void readImage()
{
    read_pbm(fileName, &image); // same as last week but you might want to add walls aswell
}

void getObjectAndRemove(int id)
{ // TODO
    // TODO pops an object from the list
    return;
}

void resolveObject()
{ // TODO
    // TODO may not be necessary, joins adjacent objects such as u object
    return;
}

void groupPixel()
{                    // TODO
                     /*
                     for each pixel
                         if you are occupied
                             check if neighbours are occupied to assign to that object
                             if neighbours are not occupied found new object
                     */
    resolveObject(); // if necessary

    // add walls to object list if necessary
}

void brushFire()
{
    bool changes = true;
    while (changes)
    {
        changes = false;
        // for each row in allPixels
        for (int i = 0; i < (allPixels.size() / IMAGESIZE); i++)

        {
            // for each pixel in row
            for (int j = 0; j < 128; i++)
            {
                if (j > allPixels.size() - IMAGESIZE * floor(allPixels.size() / IMAGESIZE))
                {
                    return
                }
                // if wall or already has a number continue

                // for all other neighbours

                vector<int> allIds;
                Pixel pixel = allPixels[i * IMAGESIZE + j];

                // if neightbour has expansion value
                allIds.push_back(pixel.id);

                if (allIds.size() > 0)
                {
                    allPixels[i * IMAGESIZE + j].expansion = -1; // TODO ID you found
                    changes = true;
                    LCDPixelBigger(pixel.x, pixel.y, LIGHTGRAY);
                }
            }
        }
    }
}