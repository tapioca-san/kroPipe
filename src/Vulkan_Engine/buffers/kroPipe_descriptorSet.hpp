
#ifndef KROPIPE_DESCRIPTORSET_H
#define KROPIPE_DESCRIPTORSET_H

#include "../../kroPipe_depedence.hpp"
#include "kroPipe_uboHandler.hpp"

namespace KP {
namespace ENGINE {


class DescriptorSet{

    public:
    
    void createDescriptorSets(KP::ENGINE::UniformBuffers &uniformBuffers);


};

extern KP::ENGINE::DescriptorSet OBJECT_DescriptorSet;

} //BUFFER
} //KP


#endif//KROPIPE_DESCRIPTORSET_H