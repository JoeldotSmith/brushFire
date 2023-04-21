/*


BRUSHFIRE for eyeSim

Author Joel Smith


*/
#include "eyebot++.h"
#include "image.h"
#include <vector>
using std::vector;

typedef struct Pixel
{
    int x = -1;
    int y = -1;
    int id = -1;
    int expansion = 0;
    int value = -1;
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
    void addPixeltoRight(Pixel pixel)
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
const char *fileName = "u.pbm";

// Globals
BYTE *image;
vector<Pixel> allPixels;
vector<Object> allObject;
int numberOfObjects = 0;

int WORLDSIZE = 4000;
int IMAGESIZE = 128;

vector<int> getReletiveGoalLocation(int x, int y, int curX, int curY)
{
    int dx = x - curX;
    int dy = y - curY;
    int dist = sqrt(dx * dx + dy * dy);
    int rot = atan2(dy, dx) * 180 / M_PI;
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
{
    for (int i = 0; i < allObject.size(); i++)
    {
        if (allObject.at(i).id == id)
        {
            allObject.erase(allObject.begin() + i);
        }
    }
    return;
}

void resolveObject()
{
    // TODO may not be necessary, joins adjacent objects such as u object
    return;
}

void groupPixel()
{ // TODO
    /*
    for each pixel
        if you are occupied
            check if neighbours are occupied to assign to that object
            if neighbours are not occupied found new object
    */
    // resolveObject(); // if necessary

    // add walls to object list if necessary

    for (int i = 0; i < 128; i++)
    {
        for (int j = 0; j < 128; i++)
        {
            if (image[i * IMAGESIZE + j])
            {
                if (allPixels.at(i * IMAGESIZE + j - 1).id != NULL)
                {
                    allPixels.at(i * IMAGESIZE + j).id = allPixels.at(i * IMAGESIZE + j - 1).id;
                    allObject.at(allPixels.at(i * IMAGESIZE + j).id).allPixels.push_back(allPixels.at(i * IMAGESIZE + j));
                }
                else if (allPixels.at((i - 1) * IMAGESIZE + j).id != NULL)
                {
                    allPixels.at(i * IMAGESIZE + j).id = allPixels.at((i - 1) * IMAGESIZE + j).id;
                    allObject.at(allPixels.at(i * IMAGESIZE + j).id).allPixels.push_back(allPixels.at(i * IMAGESIZE + j));
                }
                else
                {
                    Object newObject;
                    newObject.id = numberOfObjects;
                    numberOfObjects += 1;
                    newObject.allPixels.push_back(allPixels.at(i * IMAGESIZE + j));
                    allObject.push_back(newObject);
                }
            }
        }
    }
    resolveObject();
    // add walls TODO
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
                    return;
                }
                // if wall or already has a number continue

                // for all other neighbours

                vector<int> allIds;
                Pixel pixel = allPixels[i * IMAGESIZE + j];

                // if neightbour has expansion value
                allIds.push_back(pixel.id);

                if (allIds.size() > 0)
                {
                    allPixels[i * IMAGESIZE + j].expansion = allIds.at(allIds.size()); // TODO ID you found
                    changes = true;
                    LCDPixelBigger(pixel.x, pixel.y, LIGHTGRAY);

                    // if two neighbours labeled or neighbour with same expansion
                    //  voroni point so label it
                    if (allIds.size() == 1)
                    {
                        if (allPixels[i * IMAGESIZE + j + 1].expansion == allPixels[i * IMAGESIZE + j].expansion)
                        {
                            pixel.voroni = true;
                        }
                    }
                    else
                    {
                        if (allPixels[i * IMAGESIZE + j + 1].expansion == allPixels[i * IMAGESIZE + j].expansion || allPixels[i * IMAGESIZE + j - 1].expansion == allPixels[i * IMAGESIZE + j].expansion)
                        {
                            pixel.voroni = true;
                        }
                        if (allPixels.at(i * IMAGESIZE + j + 1).value != NULL && allPixels.at(i * IMAGESIZE + j - 1).value != NULL)
                        {
                            pixel.voroni = true;
                        }
                    }
                }
            }
        }
    }
}
int heuristic(Pixel pixel)
{
    return 0; // TODO A* heuristic (euclidean distance)
}

int main()
{
    readImage();
    // Display image and objects with unique colours
    // display voroni points and lines

    // required path = path

    // drive to goal, following the path

    LCDMenu("EXIT", "", "", "");
    KEYWait(KEY1);
}