//
// Created by thibaultpoels on 24/05/2020.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "Color.h"
#include "library.h"

/**
 * @brief namespace used for Lighting (ambient only)
 */

namespace Lighting {
    
    class Light
    {
        
        /**
         * @brief class used for Light
         */
        
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
        
        /**
         * @brief class used to parse Lights from the ini-files
         */
        

        const ini::Configuration& config;
        
        Lights3D lights;
        
        int nrLights;
        
    public:
        
        /**
         * @brief for documentation on these methods, go to the FiguresParser class which works exactly the same way
         */
        
        LightsParser(const ini::Configuration& configuration);
        
        void generateLights();
        
        Light createLight(int lightNr);
        
        Lights3D getLights();
        
    };
    
    /**
     * @brief functions used to change the figure's color on the lights that were parsed and the ambient reflection they have.
     */
    
    void applyAmbientLightings(Color& figureColor, const Color& ambientReflection, const Lights3D& lights);
    
    void applyAmbientLighting(Color& figureColor, const Color& ambientReflection, const Light& light);
    
};


#endif //ENGINE_LIGHT_H
