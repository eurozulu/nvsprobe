//
// Created by Rob GIlham on 16/06/2022.
//

#ifndef NVSPROBE_H
#define NVSPROBE_H

#include <nvs.h>
#include <string>
#include <list>
#include "nvs_flash.h"
#include "nvs_handle.hpp"

typedef esp_err_t ERROR;

typedef nvs_type_t dataType;

typedef struct {
    const char *key;
    dataType type;
} entry_t;

class NvsProbe{
public:

    NvsProbe(const std::string &partition, const std::string &namespaceNm);

    NvsProbe(const std::string &partition) : NvsProbe(partition, NULL){}

    NvsProbe() : NvsProbe(NULL, NULL){}

    virtual ~NvsProbe();

    std::string partitionName();

    std::string namespaceName();
    void setNamespaceName(std::string name);

    void keys(std::list<entry_t> *entries);

    ERROR readString(const char *key, char* out_str, size_t le);
    ERROR writeString(const char *key, const char* value);

    template<typename T> ERROR get_item(const char *key, T &value);
    template<typename T> ERROR set_item(const char *key, T value);

private:
    std::string partition;
    std::string namespaceNm;
};


#endif //NVSPROBE_H
