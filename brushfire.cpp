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
    int x;
    int y;
    int id ;
    int lastChanged;
    int value;
    bool voroni;
    vector<int> allObjectId;

    Pixel() : x(-1), y(-1), id(-1), lastChanged(-1), value(-1), voroni(false) {};
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
    vector<vector<Pixel> > getAll()
    {
        vector<vector<Pixel> > all;
        all.push_back(top);
        all.push_back(bottom);
        all.push_back(left);
        all.push_back(right);
        return all;
    }
} Walls;

// World Name
const char *fileName = "u.pbm";
vector<int> Colours;


// Globals
BYTE *image;
vector<Pixel> allPixels;
vector<Pixel> voroniPoints;
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

float convertPointsX(float x){
    return (4000*(1-x/128));
}
float convertPointsY(float y){
    return (4000*(y/128));
}

void driveToPoint(vector<Pixel> points)
{
    int curX, curY, curAng;
    VWSetPosition(0, 0, 0);
    VWGetPosition(&curX, &curY, &curAng);

    // find closest veroni point
    Pixel pixel;
    int dist = 10000;

    for (int i = 0; i < points.size(); i++){

        float x = convertPointsX(points.at(i).x) - 500;//4000*(1-(points.at(i).x/128));
        float y = convertPointsY(points.at(i).y) - 3500;//4000*(points.at(i).y/128);
        int newDist = sqrt(x*x+y*y);

        

        if (newDist < dist){
            pixel = points.at(i);
            dist = newDist;
        }
    }
    printf("Pixel: (%i, %i), distFromPlayer; %i\n", pixel.x, pixel.y, dist);
    
    

    return;
}



void readImage()
{
    read_pbm(fileName, &image); // same as last week but you might want to add walls aswell
}


void groupPixel()
{ 
    for (int i = 0; i < 128; i++){
        for (int j = 0; j < 128; j++){
            Pixel newPixel;
            newPixel.x = i;
            newPixel.y = j;
            if (image[i*128+j]){
                

                int hasNeighbours = -1; // stores neighbour id if has neighbour

                // neighbours = connected elements with the current element's value
                if (j > 0){
                    if (image[i * 128 + j - 1])
                    { // pixel to the left
                        hasNeighbours = allPixels.at(i * 128 + j - 1).id;
                    }
                    if (image[(i - 1) * 128 + j - 1])
                    { // pixel to the top and left
                        hasNeighbours = allPixels.at((i - 1) * 128 + j - 1).id;
                    }
                }
                
                
                if (image[(i - 1) * 128 + j]){ // pixel above
                    hasNeighbours = allPixels.at((i - 1) * 128 + j).id;
                }
                if (j < 128 ){
                    if (image[(i - 1) * 128 + j + 1])
                    { // pixel to the top and right
                        hasNeighbours = allPixels.at((i - 1) * 128 + j + 1).id;
                    }
                }      

                if (hasNeighbours == -1){
                    

                    Object newObject;
                    newPixel.id = numberOfObjects;
                    newObject.id = numberOfObjects;
                    newObject.allPixels.push_back(newPixel);
                    allObject.push_back(newObject);
                    numberOfObjects++;
                }
                else{
                    
                    newPixel.id = hasNeighbours;
                    allObject.at(newPixel.id).allPixels.push_back(newPixel);
                }

            }
            allPixels.push_back(newPixel);
        }
    }
    

    for (int i = 0; i < 128; i++){
        for (int j = 0; j < 128; j++) {


            if (image[i * 128+j]){
                
                


                if (image[i * 128 + j + 1] && (allPixels.at(i * 128 + j).id != allPixels.at(i * 128 + j+1).id))
                {
                    
                    int newId = allPixels.at(i * 128 + j).id;
                    int oldId = allPixels.at(i * 128 + j + 1).id;

                    
                    printf("%merging now %i to %i\n", oldId, newId);
                    

                    for (int k = 0; k < allObject.at(oldId).allPixels.size(); k++){

                        int x = allObject.at(oldId).allPixels.at(k).x;
                        int y = allObject.at(oldId).allPixels.at(k).y;

                        allObject.at(oldId).allPixels.at(k).id = newId;
                        allPixels.at(x*128+y).id = newId;
                        allObject.at(newId).allPixels.push_back(allObject.at(oldId).allPixels.at(k));
                       
                    }

                    printf("erasing object %i with size %lu\n", oldId, allObject.at(oldId).allPixels.size());
                    allObject.erase(allObject.begin() + oldId);
                }



            }


        }
    }

    printf("\n\nFound %lu objects now printing \n\n", allObject.size());
    printf("\n\n");
    for (int size = 0; size < allObject.size(); size++)
    {
        printf("Object %i, size = %lu, id = %i\n", size, allObject.at(size).allPixels.size(), allObject.at(size).id);
    }
    printf("\n\n");

    for (int i = 0; i < allObject.size(); i++){ // for each object in allObjects
        for (int j = 0; j < allObject.at(i).allPixels.size(); j++){ // for each pixel in object
            

            int x = allObject.at(i).allPixels.at(j).x;
            int y = allObject.at(i).allPixels.at(j).y;
            LCDArea(y, x, y+1, x+1, Colours.at(i), 1);
        
        }
    }
}

void brushFire()
{
    
    bool changes = true;
    int lastChanged = 0;
    while (changes){
        changes = false;
        for (int i = 0; i < 128; i++){
            for (int j = 0; j < 128; j++){
                if (allPixels.at(i*128+j).id != -1 && (lastChanged != allPixels.at(i*128+j).lastChanged)){
                    
                    int newId = allPixels.at(i * 128 + j).id;
                    

                    if (j > 0 && i > 0){
                       
                        if (allPixels.at((i-1) * 128 + j-1).id == -1){
                            changes = true;
                            allPixels.at((i - 1) * 128 + j - 1).id = newId;
                            allPixels.at((i - 1) * 128 + j - 1).lastChanged = lastChanged;
                        } else if (allPixels.at(i*128+j).id != allPixels.at((i-1) * 128 + j-1).id){
                            
                            allPixels.at(i*128+j).voroni = true;
                            allPixels.at((i-1) * 128 + j-1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i*128+j));
                            voroniPoints.push_back(allPixels.at((i-1) * 128 + j-1));
                            allPixels.at((i - 1) * 128 + j - 1).lastChanged = lastChanged;
                        }
                    }

                    if (j > 0){
                        
                        if (allPixels.at((i) * 128 + j-1).id == -1){
                            changes = true;
                            allPixels.at((i) * 128 + j - 1).id = newId;
                            allPixels.at((i) * 128 + j - 1).lastChanged = lastChanged;
                        } else if (allPixels.at(i*128+j).id != allPixels.at((i) * 128 + j-1).id){
                            
                            allPixels.at(i*128+j).voroni = true;
                            allPixels.at((i) * 128 + j-1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i*128+j));
                            voroniPoints.push_back(allPixels.at((i) * 128 + j-1));
                            
                            allPixels.at((i) * 128 + j - 1).lastChanged = lastChanged;
                        }
                    }

                    if (j > 0 && i < 127){
                        
                        if (allPixels.at((i+1) * 128 + j-1).id == -1){
                            changes = true;
                            allPixels.at((i + 1) * 128 + j - 1).id = newId;
                            allPixels.at((i + 1) * 128 + j - 1).lastChanged = lastChanged;
                        } else if (allPixels.at(i*128+j).id != allPixels.at((i+1) * 128 + j-1).id){
                            
                            allPixels.at(i*128+j).voroni = true;
                            allPixels.at((i+1) * 128 + j-1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i*128+j));
                            voroniPoints.push_back(allPixels.at((i + 1) * 128 + j - 1));
                            allPixels.at((i + 1) * 128 + j - 1).lastChanged = lastChanged;
                        }
                    }

                    if (i > 0){
                        
                        if (allPixels.at((i-1) * 128 + j).id == -1){
                            allPixels.at((i-1) * 128 + j).id = newId;
                            changes = true;
                            allPixels.at((i - 1) * 128 + j).lastChanged = lastChanged;
                        } else if (allPixels.at(i*128+j).id != allPixels.at((i-1) * 128 + j).id){
                            
                            allPixels.at(i*128+j).voroni = true;
                            allPixels.at((i-1) * 128 + j).voroni = true;
                            voroniPoints.push_back(allPixels.at(i*128+j));
                            voroniPoints.push_back(allPixels.at((i - 1) * 128 + j));
                            allPixels.at((i - 1) * 128 + j).lastChanged = lastChanged;
                        }
                    }

                    if (i < 127){
                        
                        if (allPixels.at((i+1) * 128 + j).id == -1){
                            changes = true;
                            allPixels.at((i+1) * 128 + j).id = newId;
                            allPixels.at((i + 1) * 128 + j).lastChanged = lastChanged;
                        } else if (allPixels.at(i*128+j).id != allPixels.at((i+1) * 128 + j).id){
                            
                            allPixels.at(i*128+j).voroni = true;
                            allPixels.at((i+1) * 128 + j).voroni = true;
                            voroniPoints.push_back(allPixels.at(i*128+j));
                            voroniPoints.push_back(allPixels.at((i+1) * 128 + j));
                            allPixels.at((i + 1) * 128 + j).lastChanged = lastChanged;
                        }
                    }

                    if (j < 127 && i > 0){
                        
                        if (allPixels.at((i-1) * 128 + j+1).id == -1){
                            changes = true;
                            allPixels.at((i - 1) * 128 + j + 1).id = newId;
                            allPixels.at((i - 1) * 128 + j + 1).lastChanged = lastChanged;
                        } else if (allPixels.at(i*128+j).id != allPixels.at((i-1) * 128 + j+1).id){
                            
                            allPixels.at(i*128+j).voroni = true;
                            allPixels.at((i-1) * 128 + j+1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i*128+j));
                            voroniPoints.push_back(allPixels.at((i-1) * 128 + j+1));
                            allPixels.at((i - 1) * 128 + j + 1).lastChanged = lastChanged;
                        }
                    }

                    if (j < 127){
                        
                        if (allPixels.at((i) * 128 + j+1).id == -1){
                            changes = true;
                            allPixels.at((i) * 128 + j + 1).id = newId;
                            allPixels.at((i) * 128 + j + 1).lastChanged = lastChanged;
                        } else if (allPixels.at(i*128+j).id != allPixels.at((i) * 128 + j+1).id){
                            
                            allPixels.at(i*128+j).voroni = true;
                            allPixels.at((i) * 128 + j+1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i*128+j));
                            voroniPoints.push_back(allPixels.at((i) * 128 + j+1));
                            allPixels.at((i) * 128 + j + 1).lastChanged = lastChanged;
                        }
                    }

                    if (j < 127 && i < 127){
                        
                        if (allPixels.at((i+1) * 128 + j+1).id == -1){
                            changes = true;
                            allPixels.at((i + 1) * 128 + j + 1).id = newId;
                            allPixels.at((i + 1) * 128 + j + 1).lastChanged = lastChanged;
                        } else if (allPixels.at(i*128+j).id != allPixels.at((i+1) * 128 + j+1).id){
                            
                            allPixels.at(i*128+j).voroni = true;
                            allPixels.at((i+1) * 128 + j+1).voroni = true;
                            voroniPoints.push_back(allPixels.at(i*128+j));
                            voroniPoints.push_back(allPixels.at((i+1) * 128 + j+1));
                            allPixels.at((i + 1) * 128 + j + 1).lastChanged = lastChanged;
                        }
                    }

                }

                

            }

        }
        lastChanged++;
        for (int x = 0; x < 128; x++){
            for (int y = 0; y < 128; y++){
                int id = allPixels.at(x*128+y).id;
                if (allPixels.at(x*128+y).voroni){
                    if (id != -1){
                    LCDArea(y, x, y+1, x+1, Colours.at(6), 1);
                    }
                } else{
                    if (id != -1){
                    LCDArea(y, x, y+1, x+1, Colours.at(id), 1);
                    }
                }
                
                
                
            }
        }
        
    }


}

int main()
{
    Colours.push_back(RED);
    Colours.push_back(GREEN);
    Colours.push_back(CYAN);
    Colours.push_back(MAGENTA);
    Colours.push_back(MAGENTA);
    Colours.push_back(DARKGRAY);
    Colours.push_back(BLACK);
    readImage();
    LCDImageStart(0, 0, IMAGESIZE, IMAGESIZE);
    LCDImageBinary(image);
    // Display image and objects with unique colours
    // display voroni points and lines

    // required path = path

    // drive to goal, following the path

    LCDMenu("FIND", "FIRE", "DRIVE", "EXIT");
    int endSim = 0; // boolean to end sim 0 = false, 1 = true

    do
    {
        switch (KEYRead())
        {
        case KEY1:
            printf("\n\n            Grouping \n\n");
            groupPixel();
            break;
        case KEY2:
            printf("\n\n            Burning \n\n");
            brushFire();
            break;
        case KEY3:
            printf("\n\n            Driving \n\n");
            driveToPoint(voroniPoints);
            break;
        case KEY4:
            endSim = 1;
            break;
        }
    } while (!endSim);

}
 