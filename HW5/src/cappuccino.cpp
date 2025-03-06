#include "cappuccino.h"
#include "sub_ingredients.h"

Cappuccino::Cappuccino() {
    name = "Cappuccino";
    ingredients.push_back(new Espresso(2));
    ingredients.push_back(new Milk(2));
    ingredients.push_back(new MilkFoam(1));
}

// 显示调用基类中的复制构造函数
Cappuccino::Cappuccino(const Cappuccino &cap) :
    EspressoBased(cap) {
    for (const auto& item : cap.side_items)
        side_items.push_back(item->clone());
}

Cappuccino& Cappuccino::operator=(const Cappuccino &cap) {
    // 显示调用基类中的operator=
    EspressoBased::operator=(cap);

    if (this != &cap) {
        side_items.clear();
        for (const auto& item : cap.side_items)
            side_items.push_back(item->clone());
    }

    return *this;
}

// 不需要显示调用基类中的析构函数
Cappuccino::~Cappuccino()
{
    for(const auto& i : side_items)
        delete i;
    side_items.clear();
}

std::string Cappuccino::get_name() {
    return name;
}

double Cappuccino::price() {
    double price = 0;

    for (const auto& item : ingredients)
        price += item->price();

    for (const auto& item : side_items)
        price += item->price();

    return price;
}

void Cappuccino::add_side_item(Ingredient* side) {
    side_items.push_back(side);
}

std::vector<Ingredient*>& Cappuccino::get_side_items() {
    return side_items;
}