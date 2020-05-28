//
// Created by thibaultpoels on 24/05/2020.
//

#include "Lighting.h"

namespace Lighting {

    LightsParser::LightsParser(const ini::Configuration &configuration) : config(configuration) {
        
        nrLights = configuration["General"]["nrLights"].as_int_or_die();
        
    }

    void LightsParser::generateLights() {
        
        // Create lights declared in .ini-file
        for(int i = 0; i<nrLights;i++){
            lights.push_back(createLight(i));
        }
        
    }

    Light LightsParser::createLight(int lightNr) {
        string currentLight = "Light" + to_string(lightNr);
        
        Light resultLight;
        vector<double> rgbs = config[currentLight]["ambientLight"].as_double_tuple_or_die();
        resultLight.ambientLight = Color(rgbs[0],rgbs[1],rgbs[2]);
        resultLight.diffuseLight = Color(0,0,0);
        resultLight.specularLight = Color(0,0,0);
        
        return resultLight;
    }

    Lights3D LightsParser::getLights() {
        return lights;
    }

    void applyAmbientLightings(Color& figureColor, const Color& ambientReflection, const Lights3D& lights) {
        for(const Light& light: lights){
            applyAmbientLighting(figureColor,ambientReflection,light);
        }
    }

    void applyAmbientLighting(Color& figureColor, const Color& ambientReflection, const Light& light) {
        figureColor += (light.ambientLight * ambientReflection);
    }
};