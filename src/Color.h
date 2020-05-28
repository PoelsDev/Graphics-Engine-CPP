//
// Created by thibaultpoels on 04/04/2020.
//

#ifndef ENGINE_COLOR_H
#define ENGINE_COLOR_H

class Color{
    
    /**
     * @brief internal color class for calculations (light, ...)
     */
    
public:
    double red{};
    double green{};
    double blue{};

    /**
     * @brief simple constructor 
     * @param red double
     * @param green double
     * @param blue double
     */
     
    Color(double red, double green, double blue);
    Color()=default;

    /**
     * @brief operators for calculations between lights 
     */
    
    void operator +=(const Color& c);
    Color operator * (const Color& c) const;
};

#endif //ENGINE_COLOR_H
