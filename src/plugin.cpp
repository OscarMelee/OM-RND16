#include "plugin.hpp"

Plugin* pluginInstance;

void init(rack::plugin::Plugin* p) {
    pluginInstance = p;
    p->addModel(modelRND16);
}
