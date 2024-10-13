#include "parameter.h"

template <typename T>
void flash::configParameters::Parameter<T>::loadValue(const YAML::Node &configNode) {
    if (configNode[key]) {
        value = configNode[key].as<T>();
    } else if (defaultValue) {
        value = defaultValue.value();
    } else {
        throw std::runtime_error("Key " + key + " is not defined.");
    }
}
