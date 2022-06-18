//
// Created by Rob GIlham on 16/06/2022.
//

#include "NvsProbe.h"
#include "../../../esp32/esp-idf/components/nvs_flash/src/nvs.hpp"

NvsProbe::NvsProbe(const string &partition, const string &namespaceNm) : partition(partition), namespaceNm(namespaceNm) {
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

NvsProbe::~NvsProbe() {
    nvs_flash_deinit();
}

std::string NvsProbe::partitionName() {
    return this->partition;
}

std::string NvsProbe::namespaceName() {
    return this->namespaceNm;
}

void NvsProbe::setNamespaceName(std::string name) {
    this->namespaceNm = name;
}

void NvsProbe::keys(std::list<entry_t> *entries) {
    nvs_iterator_t it = nvs_entry_find(this->partition.c_str(), this->namespaceNm.c_str(), NVS_TYPE_ANY);
    while(it != NULL) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info); // Can omit error check if parameters are guaranteed to be non-NULL
        std::string name;
        if (strlen(info.namespace_name) > 0) {
            name.append(info.namespace_name);
            name.append(".");
        }
        name.append(info.key);
        (*entries).push_back(entry_t {key: name.c_str(), type: info.type});
        it = nvs_entry_next(it);
    }
    nvs_release_iterator(it);
}

ERROR NvsProbe::readString(const char *key, char* out_str, size_t le) {
    // Handle will automatically close when going out of scope or when it's reset.
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle =
            nvs::open_nvs_handle(this->namespaceNm.c_str(), NVS_READONLY, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->get_string(key, out_str, le);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}

ERROR NvsProbe::writeString(const char *key, const char* value) {
    // Handle will automatically close when going out of scope or when it's reset.
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(this->namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->set_string(key, value);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}

template<typename T>
ERROR NvsProbe::set_item(const char *key, T value) {
    // Handle will automatically close when going out of scope or when it's reset.
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(this->namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->set_item(key, value);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}

template<typename T>
ERROR NvsProbe::get_item(const char *key, T &value) {
    // Handle will automatically close when going out of scope or when it's reset.
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(this->namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->get_item(key, value);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}
