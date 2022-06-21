//
// Created by Rob GIlham on 16/06/2022.
//

#ifndef NVSPROBE_H
#define NVSPROBE_H

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#define LOG_TAG_PROBE "NVS Probe"
#include <esp_log.h>

#include <list>
#include "nvs_handle.hpp"

typedef esp_err_t ERROR;

typedef nvs_type_t dataType;

typedef struct {
    std::string nameSpc;
    std::string key;
    dataType type;
} entry_t;

class NvsProbe {
public:


    NvsProbe(std::string namespaceName, std::string partition);

    NvsProbe(std::string namespaceName) : NvsProbe(namespaceName, NVS_DEFAULT_PART_NAME) {}

    NvsProbe() : NvsProbe("", NVS_DEFAULT_PART_NAME) {}


    virtual ~NvsProbe();

    std::string partitionName();

    std::string namespaceName();

    void setNamespaceName(std::string name);

    void keys(std::list<entry_t> &entries);

    ERROR readString(const char *key, char *out_str, size_t le);

    ERROR writeString(const char *key, const char *value);

    ERROR readItem(const char *key, int32_t &value);

    ERROR writeItem(const char *key, int32_t value);

    ERROR readItem(const char *key, uint32_t &value);

    ERROR writeItem(const char *key, uint32_t value);

    ERROR readItem(const char *key, uint64_t &value);

    ERROR writeItem(const char *key, uint64_t value);

    ERROR readBlob(const char *key, uint32_t *value, size_t &ln);

    ERROR writeBlob(const char *key, uint32_t *value, size_t ln);

private:
    std::string partition = "";
    std::string namespaceNm = "";

    std::unique_ptr<nvs::NVSHandle> openHandle(nvs::NVSHandle *handle);
};


#endif //NVSPROBE_H
