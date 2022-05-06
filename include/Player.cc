
#ifndef MEGA_PLAYABLE_CHARACTER
#define MEGA_PLAYABLE_CHARACTER

#include <Coordinates.h>

namespace MEGA {

class FirstPersonCharacter {
    public:
        // define the controls and mechanisms for movement
    private:
        //float yaw = 0.f, pitch = 0.f;
        Quanternion rotation;   //
        float x, y, z;  // world position
        
    
}


} // namespace
#endif
