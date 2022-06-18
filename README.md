# nvsprobe

A simple wrapper to access the non volatile storage of the ESP32.  
NVS is a simple key/value pair store, with pairs grouped into logical 'namespaces'.  
  
vnsprobe offers a simplified interface for accessing and setting those values.  
The interface works using a single namespace, allowing for listing the keys and data types  
as well as setting and retrieving the key values.

### example
This example lists all the keys and their data types of the 'ESP32Radio' namespace  
```
NvsProbe nvs{"nvs", "ESP32Radio"};
std::list<entry_t> entries;
nvs.keys(&entries);
for (const entry_t &entry : entries)
{
std::cout << entry.key << " :: " << entry.type << std::endl;
}
```