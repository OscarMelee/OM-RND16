#include "plugin.hpp"

namespace rack {
namespace plugin {

Plugin* pluginInstance;

void init(Plugin* p) {
    pluginInstance = p;
    p->addModel(modelRND16);
}

} // namespace plugin
} // namespace rack
