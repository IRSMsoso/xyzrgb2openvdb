// xyzrgb2openvdb.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES
//#include <cmath>
#include <iostream>
#include <openvdb\openvdb.h>
#include <fstream>
#include <boost\algorithm\string\classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <vector>
#include <filesystem>



struct Particle {
public:
    float x, y, z;
    int r, g, b;
};


int main()
{
    openvdb::initialize();
    
    std::cout << "Scale: ";
    float scale;
    std::cin >> scale;

    std::cout << "Number of frames to process: ";
    int num_frames = 0;
    std::cin >> num_frames;


    float overlappedPercTotal = 0.0f;


    for (int i = 1; i <= num_frames; i++) {
        std::cout << "Processing frame " << i << std::endl;

        std::vector<Particle> particles;


        std::string openFile = "frames/frame_";
        openFile += std::to_string(i);
        openFile += ".xyzrgb";

        std::cout << "Opening file " << (openFile) << std::endl;

        std::ifstream infile(openFile);

        std::cout << "Processing particles\n";

        float x, y, z;
        int r, g, b;
        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);

            std::vector<std::string> tokens;
            boost::split(tokens, line, boost::is_any_of(" "));

            if (tokens.size() != 6) continue;  // Will happen for first line

            Particle particle;



            particle.x = std::stof(tokens[0]) * scale;
            particle.y = std::stof(tokens[1]) * scale;
            particle.z = std::stof(tokens[2]) * scale;
            particle.r = std::stoi(tokens[3]);
            particle.g = std::stoi(tokens[4]);
            particle.b = std::stoi(tokens[5]);

            particles.push_back(particle);
        }
        /*
        int maxx = 0, maxy = 0, maxz = 0, minx = 0, miny = 0, minz = 0;

        for (int i = 0; i < particles.size(); i++) {
            if (particles[i].x < minx) {
                minx = particles[i].x;
            }
            if (particles[i].y < miny) {
                miny = particles[i].y;
            }
            if (particles[i].z < minz) {
                minz = particles[i].z;
            }
            if (particles[i].x > maxx) {
                maxx = particles[i].x;
            }
            if (particles[i].y > maxy) {
                maxy = particles[i].y;
            }
            if (particles[i].z > maxz) {
                maxz = particles[i].z;
            }
        }
        

        std::cout << "X: " << minx << " -> " << maxx << std::endl;
        std::cout << "Y: " << miny << " -> " << maxy << std::endl;
        std::cout << "Z: " << minz << " -> " << maxz << std::endl;

        */



        std::cout << "Processed " << particles.size() << " particles\n";


        std::cout << "Creating Volume\n";


        openvdb::Vec3fGrid::Ptr color = openvdb::Vec3fGrid::create();
        color->setName("color");

        openvdb::FloatGrid::Ptr density = openvdb::FloatGrid::create();
        density->setName("density");


        int new_values = 0;
        int overlapped_values = 0;


        openvdb::Vec3fGrid::Accessor colorAccess = color->getAccessor();
        openvdb::FloatGrid::Accessor densityAcccess = density->getAccessor();
        for (int i = 0; i < particles.size(); i++) {

            openvdb::Coord curr_coord(particles[i].x, particles[i].y, particles[i].z);

            if (colorAccess.isValueOn(curr_coord)) {
                overlapped_values++;
            }
            else {
                new_values++;
            }

            colorAccess.setValue(curr_coord, openvdb::Vec3i(((float)particles[i].r) / 255.f, ((float)particles[i].g) / 255.f, ((float)particles[i].b) / 255.f));
            densityAcccess.setValue(openvdb::Coord(particles[i].x, particles[i].y, particles[i].z), 1.0f);
        }

        float overlappedPerc = ((double)overlapped_values / ((double)new_values + (double)overlapped_values));
        std::cout << "Overlapped particle to voxel percentage: " << overlappedPerc << std::endl;
        overlappedPercTotal += overlappedPerc;


        openvdb::GridPtrVec grids;
        grids.push_back(color);
        grids.push_back(density);

        std::cout << "Writing to file\n";


        std::filesystem::create_directory("output");


        std::string outputFileName = "output/output";
        outputFileName += std::to_string(i);
        outputFileName += ".vdb";
        openvdb::io::File file(outputFileName);
        file.write(grids);
        file.close();
        std::system("cls");
    }


    std::cout << "Completed!\nAverage Overlapped percentage: " << (overlappedPercTotal / num_frames) << std::endl;
    
    

    std::system("pause");

}