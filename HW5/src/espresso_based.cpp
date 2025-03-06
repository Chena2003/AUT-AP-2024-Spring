#include "espresso_based.h"

#include <sub_ingredients.h>

void EspressoBased::brew() {
    // to do
}

std::vector<Ingredient*>& EspressoBased::get_ingredients() {
    return ingredients;
}

EspressoBased::EspressoBased() : ingredients(), name() {

}

EspressoBased::EspressoBased(const EspressoBased &esp) :
    name(esp.name) {
    for (const auto& item : esp.ingredients)
        ingredients.push_back(item->clone());
}

EspressoBased& EspressoBased::operator=(const EspressoBased &esp) {
    if (this != &esp) {
        ingredients.clear();
        name = esp.name;

        for (const auto& item : esp.ingredients)
            ingredients.push_back(item->clone());
    }

    return *this;
}

EspressoBased::~EspressoBased() {
    for(const auto& i : ingredients)
        delete i;
    ingredients.clear();
}
