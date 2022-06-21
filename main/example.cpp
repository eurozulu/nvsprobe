/**
 * example usage of nvsprobe, lists the keys stored in the default partition (nvs),
 * for all the Bluetooth and radio related setting in the 'ESP32Radio' namespace.
 *
 */


#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "NvsProbe.h"
#include <iostream>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#define LOG_TAG "Key Scan App"

#ifdef __cplusplus
extern "C" {
#endif

const char * typeToString(nvs_type_t t) {
    switch (t) {
        case NVS_TYPE_U8:
            return "NVS_TYPE_U8";
        case NVS_TYPE_I8:
            return "NVS_TYPE_I8";
        case NVS_TYPE_U16:
            return "NVS_TYPE_U16";
        case NVS_TYPE_I16:
            return "NVS_TYPE_I16";
        case NVS_TYPE_U32:
            return "NVS_TYPE_U32";
        case NVS_TYPE_I32:
            return "NVS_TYPE_I32";
        case NVS_TYPE_U64:
            return "NVS_TYPE_U64";
        case NVS_TYPE_I64:
            return "NVS_TYPE_I64";
        case NVS_TYPE_STR:
            return "NVS_TYPE_STR";
        case NVS_TYPE_BLOB:
            return "NVS_TYPE_BLOB";
        case NVS_TYPE_ANY:
            return "NVS_TYPE_ANY";
        default:
            return "UNKNOWN";
    }
}

void printValue(NvsProbe &nvs, entry_t entry) {
    nvs.setNamespaceName(entry.nameSpc);

    switch (entry.type) {
        case NVS_TYPE_U8:
            break;
        case NVS_TYPE_I8:
            break;
        case NVS_TYPE_U16:
            break;
        case NVS_TYPE_I16:
            break;
        case NVS_TYPE_U32: {
            uint32_t i = 0;
            ESP_ERROR_CHECK(nvs.readItem(entry.key.c_str(), i));
            std::cout << " = " << i;
            break;
        }
        case NVS_TYPE_I32: {
            int32_t i = 0;
            ESP_ERROR_CHECK(nvs.readItem(entry.key.c_str(), i));
            std::cout << " = " << i;
            break;
        }
        case NVS_TYPE_U64:
            break;
        case NVS_TYPE_I64:
            break;
        case NVS_TYPE_STR: {
            size_t ln = 2048;
            char value[ln];
            ESP_ERROR_CHECK(nvs.readString(entry.key.c_str(), value, ln));
            std::cout << " = " << value;
            break;
        }
        case NVS_TYPE_BLOB: {
            size_t ln = 2048;
            uint32_t value;
            ESP_ERROR_CHECK(nvs.readBlob(entry.key.c_str(), &value, ln));
            std::cout << " = " << value;
            break;
        }
        default:
            break;
    }

}
void app_main() {
    esp_log_level_set(LOG_TAG, ESP_LOG_VERBOSE);
    ESP_LOGD(LOG_TAG, "starting nvs probe++++++++++++++++++");
    std::list<entry_t> entries = {};
    NvsProbe nvs{};
    nvs.keys(entries);
    ESP_LOGD(LOG_TAG, "found %d keys", entries.size());

    for (const entry_t &entry : entries) {
        std::cout << " type: " << typeToString(entry.type) << "\t";

        if (!entry.nameSpc.empty()) {
            std::cout << entry.nameSpc << "::";
        }
        std::cout << entry.key;
        std::cout << std::endl;

    }
    printValue(nvs, entries.front());
}

#ifdef __cplusplus
}
#endif

