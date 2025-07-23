/**
 * @brief Instancia as funções template de gerenciamento de buffer para um tipo específico.
 * 
 * Use essa macro sempre que for usar a API de gerenciamento de buffer com um novo tipo.
 * Exemplo: Se for usar o tipo KP::UTILS::Light, chame INSTANTIATE_BUFFER_TYPE(KP::UTILS::Light)
 */
#define INSTANTIATE_BUFFER_TYPE(type) \
    template uint32_t BufferManager::addBuffer<type>(type); \
    template void BufferManager::sendBufferDataToGpu<type>(uint32_t, type)
