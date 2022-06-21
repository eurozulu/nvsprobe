//
// Created by Rob GIlham on 16/06/2022.
//


#include "NvsProbe.h"

#include <nvs.h>
#include "nvs_flash.h"

NvsProbe::NvsProbe(std::string namespaceNm, std::string partition) : partition(partition), namespaceNm(namespaceNm) {
    esp_log_level_set(LOG_TAG_PROBE, ESP_LOG_VERBOSE);
    ESP_LOGD(LOG_TAG_PROBE, "initalising nvs flash");

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGD(LOG_TAG_PROBE, "nvs unformatted, erasing flash...");
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_LOGD(LOG_TAG_PROBE, "reinitalising nvs flash");
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    ESP_LOGD(LOG_TAG_PROBE, "nvs flash initalisation complete");
}

NvsProbe::~NvsProbe() {
    nvs_flash_deinit();
}

std::string NvsProbe::partitionName() {
    return this->partition;
}

std::string NvsProbe::namespaceName() {
    return !namespaceNm.empty() ? namespaceNm.c_str() : NULL;
}

void NvsProbe::setNamespaceName(std::string name) {
    this->namespaceNm = name;
}

void NvsProbe::keys(std::list<entry_t> &entries) {
    ESP_LOGD(LOG_TAG_PROBE, "listing keys for partition: %s\tNamespace: %s", partition.c_str(), namespaceNm.c_str());
    const char *namespace_name = !namespaceNm.empty() ? namespaceNm.c_str() : NULL;
    nvs_iterator_t it = nvs_entry_find(partition.c_str(), namespace_name, NVS_TYPE_ANY);
    if (it == NULL) {
        ESP_LOGD(LOG_TAG_PROBE, "+++++++++++++++++++ nothing found +++++++++++++++++++");
    }

    while (it != NULL) {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info); // Can omit error check if parameters are guaranteed to be non-NULL
        ESP_LOGD(LOG_TAG_PROBE, "Namespace: %s\tkey entry: %s", info.namespace_name, info.key);
        entry_t *entry = new entry_t;
        entry->nameSpc.assign(info.namespace_name);
        entry->key.assign(info.key);
        entry->type = info.type;
        entries.push_back(*entry);
        delete entry;
        it = nvs_entry_next(it);
    }
    nvs_release_iterator(it);
}



ERROR NvsProbe::readString(const char *key, char *out_str, size_t le) {
    // Handle will automatically close when going out of scope or when it's reset.
    ERROR err;
    const char *ns_name = !namespaceNm.empty() ? namespaceNm.c_str() : NULL;
    std::shared_ptr<nvs::NVSHandle> handle =
            nvs::open_nvs_handle(ns_name, NVS_READONLY, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->get_string(key, out_str, le);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}

ERROR NvsProbe::writeString(const char *key, const char *value) {
    // Handle will automatically close when going out of scope or when it's reset.
    ERROR err;
    const char *ns_name = !namespaceNm.empty() ? namespaceNm.c_str() : NULL;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(ns_name, NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->set_string(key, value);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}

ERROR NvsProbe::readItem(const char *key, int32_t &value) {
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->get_item(key, value);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->commit();
    if (err != ESP_OK) {
        return err;
    }
    handle.reset();
    return ESP_OK;

}

ERROR NvsProbe::writeItem(const char *key, int32_t value) {
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}

ERROR NvsProbe::readItem(const char *key, uint32_t &value) {
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->get_item(key, value);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}

ERROR NvsProbe::writeItem(const char *key, uint32_t value) {
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->set_item(key, value);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}

ERROR NvsProbe::readBlob(const char *key, uint8_t *data, size_t ln) {
    ESP_LOGD(LOG_TAG_PROBE, "reading blob '%s::%s'", namespaceNm.c_str(), key);
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    ESP_LOGD(LOG_TAG_PROBE, "opened handle");

    // Size the blob
    size_t len = 0;
    err = handle->get_item_size(nvs::ItemType::BLOB, key, len);
    if (err != ESP_OK) {
        return err;
    }
    ESP_LOGD(LOG_TAG_PROBE, "blob size = %d", len);
    if (len > 0) {
        uint32_t* value = (uint32_t*)malloc(len + sizeof(uint32_t));
        err = handle->get_blob(key, value, len);
        if (err != ESP_OK) {
            return err;
        }
        if (ln > len) {
            ln = len
        }
    }
    ESP_LOGD(LOG_TAG_PROBE, "read blob");

    return handle->commit();
}

ERROR NvsProbe::writeBlob(const char *key, uint32_t *value, size_t ln) {
    ERROR err;
    std::shared_ptr<nvs::NVSHandle> handle = nvs::open_nvs_handle(namespaceNm.c_str(), NVS_READWRITE, &err);
    if (err != ESP_OK) {
        return err;
    }
    err = handle->set_blob(key, value, ln);
    if (err != ESP_OK) {
        return err;
    }
    return handle->commit();
}
