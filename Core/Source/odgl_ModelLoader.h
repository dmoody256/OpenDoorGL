#ifndef ODGL_MODEL_LOADER
#define ODGL_MODEL_LOADER


#include "odgl_Model.h"

namespace OpenDoorGL{

    class ModelLoader {

    public:
        static Model* LoadOBJFile(const char * path );
        
    };

}

#endif