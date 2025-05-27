#ifndef KROPIPE_IMAGEVIEW_HPP
#define KROPIPE_IMAGEVIEW_HPP

#include "../../kroPipe_depedence.hpp"

#include <vector> // std::vector

namespace KP {
namespace ENGINE {

// Variáveis globais/namespace (declaradas extern)
extern std::vector<VkImage> swapChainImages; // Assinando como extern
extern std::vector<VkImageView> swapChainImageViews; // Assinando como extern

class ImageView{

public:
    // Métodos da classe ImageView
    void createImageViews();
    void DestroyImageview();

}; //CLASS IMAGEVIEW

// Objeto ImageView (declarado extern)
extern KP::ENGINE::ImageView OBJECT_imageView;

} // namespace IMAGEVIEW
} // namespace KP

#endif //KROPIPE_IMAGEVIEW_HPP