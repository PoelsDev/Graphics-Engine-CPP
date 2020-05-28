//
// Created by thibaultpoels on 24/05/2020.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "Color.h"
#include "library.h"

namespace Lighting {
    
    class Light
    {
    public:
        //de ambiente licht component
        Color ambientLight;
        //de diffuse licht component
        Color diffuseLight;
        //de diffuse licht component
        Color specularLight;
    };

    typedef std::list<Light> Lights3D;
    
    
    class LightsParser{

        const ini::Configuration& config;
        
        Lights3D lights;
        
        int nrLights;
        
    public:
        
        LightsParser(const ini::Configuration& configuration);
        
        void generateLights();
        
        Light createLight(int lightNr);
        
        Lights3D getLights();
        
    };
    
    void applyAmbientLightings(Color& figureColor, const Color& ambientReflection, const Lights3D& lights);
    
    void applyAmbientLighting(Color& figureColor, const Color& ambientReflection, const Light& light);
    
};


#endif //ENGINE_LIGHT_H
