#include "easy_image.h"
#include "ini_configuration.h"
#include <fstream>
#include <iostream>
#include <string>
#include "lineUtils.h"
#include "utils.h"
#include "l_parser.h"
#include "3DLineUtils.h"
#include "zBufferUtils.h"
#include "Lighting.h"

using namespace utils;

/**
 * @brief handles fractals on figures if needed
 * @param figures 
 */

void handleFractals(Figures3D& figures){
    Figures3D newFigures;
    for(Figure& f: figures){
        if(f.isFractal){
            generateFractal(f,newFigures,f.nrIterations,f.fractalScale);
        } else {
            newFigures.push_back(f);
        }
    }

    figures = newFigures;
}

/**
 * @brief function which handles 2D generated images
 * @param configuration: input from the ini file
 * @return EasyImage that has to be generated
 */

img::EasyImage systemL2D(const ini::Configuration& configuration){
    // Used values variables/objects
    Lines2D lines;
    LParser::LSystem2D l_sys;
    unsigned int size = configuration["General"]["size"].as_int_or_default(256);
    vector<double> c_rgb = configuration["2DLSystem"]["color"].as_double_tuple_or_die();
    Color c = Color(c_rgb[0],c_rgb[1],c_rgb[2]);
    vector<double>bc_rgb = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();

    // Input stream
    std::ifstream input(configuration["2DLSystem"]["inputfile"].as_string_or_die());
    input >> l_sys;
    input.close();

    // Creation & drawing of lines
    L2DReplacer replacer = L2DReplacer(l_sys,c);
    replacer.drawRecursive(lines,l_sys.get_initiator(),l_sys.get_nr_iterations());
    img::EasyImage img = draw2DLines(lines, size, bc_rgb);
    std::ofstream fout("l_system_test.bmp", std::ios::binary);
    fout << img;
    fout.close();
    return img;
}

/**
 * @brief function which handles all 3D line drawings
 * @param configuration: input from the ini file
 * @return EasyImage that has to be generated
 */

img::EasyImage lineDrawing3D(const ini::Configuration& configuration){
    // Figures Parser
    FiguresParser fp = FiguresParser(configuration);

    // Used variables
    unsigned int size = configuration["General"]["size"].as_int_or_default(256);
    vector<double>bc_rgb = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();

    // Generate figures
    fp.generateFigures();
    Figures3D resultFigs = fp.getFigures3D();

    // Handle fractals
    handleFractals(resultFigs);

    // Generate lines
    Lines2D lines = transform::doProjection(resultFigs);

    // Image creation
    img::EasyImage img = draw2DLines(lines, size, bc_rgb);
    std::ofstream fout("l_system_test.bmp", std::ios::binary);
    fout << img;
    fout.close();
    return img;
}

/**
 * @brief function which handles all Z-Buffering line drawings (3D)
 * @param configuration: input from the ini file
 * @return EasyImage that has to be generated
 */

img::EasyImage zBufferedLineDrawing3D(const ini::Configuration& configuration){
    // Figures Parser
    FiguresParser fp = FiguresParser(configuration);

    // Used variables
    unsigned int size = configuration["General"]["size"].as_int_or_default(256);
    vector<double>bc_rgb = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();

    // Generate figures
    fp.generateFigures();
    Figures3D resultFigs = fp.getFigures3D();

    // Handle fractals
    handleFractals(resultFigs);

    // Generate lines
    Lines2D lines = transform::doProjection(resultFigs, true);

    // Image creation
    img::EasyImage img = draw2DLines(lines, size, bc_rgb, true);
    std::ofstream fout("l_system_test.bmp", std::ios::binary);
    fout << img;
    fout.close();
    return img;
}

/**
 * @brief function which handles all Z-Buffering line drawings with triangles (3D)
 * @param configuration: input from the ini file
 * @return EasyImage that has to be generated
 */

img::EasyImage zBuffering3D(const ini::Configuration& configuration){
    // Figures Parser
    FiguresParser fp = FiguresParser(configuration);

    // Used variables
    unsigned int size = configuration["General"]["size"].as_int_or_default(256);
    vector<double>bc_rgb = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();

    // Generate figures
    fp.generateFigures();
    Figures3D resultFigs = fp.getFigures3D();

    // Handle fractals
    handleFractals(resultFigs);

    // Generate lines
    Lines2D lines = transform::doProjection(resultFigs);

    // Image properties
    double imageX;
    double imageY;

    double d;

    double DCx;
    double DCy;
    double dx;
    double dy;

    utils::getImageInfo(lines,size,imageX,imageY,DCx,DCy,dx,dy,d);
    zBufferUtils::ZBuffer zbuf = zBufferUtils::ZBuffer(roundToInt(imageX),roundToInt(imageY));


    // Image creation
    Color bc;
    bc.red = bc_rgb[0];
    bc.green = bc_rgb[1];
    bc.blue = bc_rgb[2];
    img::EasyImage img(roundToInt(imageX), roundToInt(imageY), convertColorValue(bc));

    // Triangulate faces
    for(Figure& fig: resultFigs){
        vector<Face> newFaces;
        for(Face& f: fig.faces){
            vector<Face> triangulatedFaces = triangulate(f);
            newFaces.insert(newFaces.end(), triangulatedFaces.begin(), triangulatedFaces.end());
        }
        fig.faces = newFaces;
    }
    
    // Lighting
    Lighting::Lights3D lights3D;
    if(configuration["General"]["type"].as_string_or_die() == "LightedZBuffering"){
        Lighting::LightsParser lightParser = Lighting::LightsParser(configuration);
        lightParser.generateLights();
        lights3D = lightParser.getLights();
    }

    // Draw
    for(Figure& fig: resultFigs){
        for(Face& f: fig.faces){
            Color lightedColor = fig.color;
            if(configuration["General"]["type"].as_string_or_die() == "LightedZBuffering"){
                Lighting::applyAmbientLightings(lightedColor,fig.ambientReflection,lights3D);
            }
            img::Color c = convertColorValue(lightedColor);
            zBufferUtils::draw_zbuf_triag(zbuf,img,fig.points.at(f.point_indexes.at(0)),fig.points.at(f.point_indexes.at(1)), fig.points.at(f.point_indexes.at(2)),d,dx,dy,c);
        }
    }

    std::ofstream fout("l_system_test.bmp", std::ios::binary);
    fout << img;
    fout.close();
    return img;
}

img::EasyImage generate_image(const ini::Configuration &configuration)
{
    if(configuration["General"]["type"].as_string_or_die() == "2DLSystem"){
        return systemL2D(configuration);
    } else if(configuration["General"]["type"].as_string_or_die() == "Wireframe"){
        return lineDrawing3D(configuration);
    } else if(configuration["General"]["type"].as_string_or_die() == "ZBufferedWireframe"){
        return zBufferedLineDrawing3D(configuration);
    } else if(configuration["General"]["type"].as_string_or_die() == "ZBuffering" || configuration["General"]["type"].as_string_or_die() == "LightedZBuffering"){
        return zBuffering3D(configuration);
    }
}

int main(int argc, char const* argv[])
{
        srand(time(0));
        int retVal = 0;
        try
        {
                for(int i = 1; i < argc; ++i)
                {
                        ini::Configuration conf;
                        try
                        {
                                std::ifstream fin(argv[i]);
                                fin >> conf;
                                fin.close();
                        }
                        catch(ini::ParseException& ex)
                        {
                                std::cerr << "Error parsing file: " << argv[i] << ": " << ex.what() << std::endl;
                                retVal = 1;
                                continue;
                        }

                        img::EasyImage image = generate_image(conf);
                        if(image.get_height() > 0 && image.get_width() > 0)
                        {
                                std::string fileName(argv[i]);
                                std::string::size_type pos = fileName.rfind('.');
                                if(pos == std::string::npos)
                                {
                                        //filename does not contain a '.' --> append a '.bmp' suffix
                                        fileName += ".bmp";
                                }
                                else
                                {
                                        fileName = fileName.substr(0,pos) + ".bmp";
                                }
                                try
                                {
                                        std::ofstream f_out(fileName.c_str(),std::ios::trunc | std::ios::out | std::ios::binary);
                                        f_out << image;

                                }
                                catch(std::exception& ex)
                                {
                                        std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                                        retVal = 1;
                                }
                        }
                        else
                        {
                                std::cout << "Could not generate image for " << argv[i] << std::endl;
                        }
                }
        }
        catch(const std::bad_alloc &exception)
        {
    		//When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
    		//Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
    		//(Unless of course you are already consuming the maximum allowed amount of memory)
    		//If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
		//mark the test as failed while in reality it just needed a bit more memory
                std::cerr << "Error: insufficient memory" << std::endl;
                retVal = 100;
        }

        cout << "[ENGINE]: DONE" << endl;
        return retVal;


}




//// INTRO Generate Image
//    if(configuration["General"]["type"].as_string_or_die() == "IntroColorRectangle"){
//        unsigned int width = configuration["ImageProperties"]["width"].as_int_or_default(256);
//        unsigned int height = configuration["ImageProperties"]["height"].as_int_or_default(256);
//        img::EasyImage image(width, height);
//        for(unsigned int i = 0; i < width; i++)
//        {
//            for(unsigned int j = 0; j < height; j++)
//            {
//                image(i,j).red = i;
//                image(i,j).green = j;
//                image(i,j).blue = (i+j)%width;
//            }
//        }
//        std::ofstream fout("out_test.bmp", std::ios::binary);
//        fout << image;
//        fout.close();
//        return image;
//    } else if(configuration["General"]["type"].as_string_or_die() == "IntroBlocks"){
//        unsigned int width = configuration["ImageProperties"]["width"].as_int_or_default(256);
//        unsigned int height = configuration["ImageProperties"]["height"].as_int_or_default(256);
//        std::vector<int> rgb1 = convertColorValues(configuration["BlockProperties"]["colorWhite"].as_double_tuple_or_die());
//        std::vector<int> rgb2 = convertColorValues(configuration["BlockProperties"]["colorBlack"].as_double_tuple_or_die());
//        int nrXB = configuration["BlockProperties"]["nrXBlocks"].as_int_or_die();
//        //int nrYB = configuration["BlockProperties"]["nrYBlocks"].as_int_or_die();
//        //bool invertC = configuration["BlockProperties"]["invertColor"].as_bool_or_die();
//
//        int blockSize = floor(width/nrXB);
//
//        img::EasyImage image(width, height);
//        std::vector<int>currentC = rgb1;
//        for(unsigned int i = 0; i < width; i++)
//        {
//            int wB = floor(i/blockSize);
//            for(unsigned int j = 0; j < height; j++)
//            {
//                int hB = floor(j/blockSize);
//                if((wB+hB)%2 == 0){
//                    if(currentC == rgb1){
//                        currentC = rgb2;
//                    } else {
//                        currentC = rgb1;
//                    };
//                }
//
//                image(i,j).red = currentC[0];
//                image(i,j).green = currentC[1];
//                image(i,j).blue = currentC[2];
//            }
//        }
//        img::EasyImage image;
//        std::ofstream fout("out_color.bmp", std::ios::binary);
//        fout << image;
//        fout.close();
//        return image;
//    }
//    return img::EasyImage();