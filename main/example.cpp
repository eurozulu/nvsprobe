/**
 * example usage of nvsprobe, lists the keys stored in the default partition (nvs),
 * for all the Bluetooth and radio related setting in the 'ESP32Radio' namespace.
 *
 */
#include "NvsProbe.h"
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif


void app_main() {
    NvsProbe nvs{"nvs", "ESP32Radio"};
    std::list<entry_t> entries;
    nvs.keys(&entries);
    for (const entry_t &entry : entries)
    {
        std::cout << entry.key << " :: " << entry.type << std::endl;
    }
}

#ifdef __cplusplus
}
#endif

