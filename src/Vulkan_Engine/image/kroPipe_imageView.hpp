#ifndef KROPIPE_IMAGEVIEW_HPP
#define KROPIPE_IMAGEVIEW_HPP

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {


class ImageView{

public:
    void createImageViews();
    void DestroyImageview();

}; 

extern KP::ENGINE::ImageView OBJECT_imageView;

} 
} 

#endif //KROPIPE_IMAGEVIEW_HPP