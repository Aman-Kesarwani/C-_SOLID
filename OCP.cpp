// OCP.cpp : Defines the entry point for the console application.
// Open/Closed Principle - Software Entitities (class, methods etc) should be open for extension and closed 
// for modification.

// 14-Jul-2018  Aman Kesarwani	Initial Creation

#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Color {red, green, blue};
enum class Size {small, medium, large};

struct Product {
	string productName;
	Color productColor;
	Size productSize;
};

// ProductFilter is concerned with filtering the product according to some criteria - here criteria is color, size
// everytime requirement changes say different criteria needed then a method has to be added to fulfull that.
// this is what against the OCP. 
struct ProductFilterCriteria {
	typedef vector<Product*> Items;

	Items filter_by_color(Items items, const Color color) {
		Items result;
		for (auto& i : items) {
			if (i->productColor == color) {
				result.push_back(i);
				return result;
			}
		}
	}

	Items filter_by_size(Items items, const Size size) {
		Items filteredItems;
		for (auto& i : items) {
			if (i->productSize == size) {
				filteredItems.push_back(i);
			}
			return filteredItems;
			;
		}
	}

	Items filter_by_size_and_color(Items items, const Size size, const Color color) {
		Items filteredItems;
		for (auto& i : items) {
			if (i->productSize == size && i->productColor == color) {
				filteredItems.push_back(i);
			}
		}
		return filteredItems;
	}
};

// lets convert above code to follow OCP (open closed principle)

template <typename T> struct AndSpecification;

template <typename T> struct Specification
{
	virtual ~Specification() = default;
	virtual bool is_satisfied(T* item) const = 0;
};

template <typename T> AndSpecification<T> operator&& (const Specification<T>& first, const Specification<T>& second) {
	return { first, second };
}

template <typename T> struct Filter
{
	virtual vector<T*> filter(vector<T*> items, Specification<T>& spec) = 0;
};

struct SpecialisedFilter : Filter<Product> {
	virtual vector<Product*> filter(vector<Product*> items, Specification<Product>& spec) override {

		vector<Product*> filteredProducts;
		for (auto&productItem : items) {
			if (spec.is_satisfied(productItem))
				filteredProducts.push_back(productItem);
		}
		return filteredProducts;
	}
};

struct ColorSpecifcation : Specification<Product> {
	Color color;
	ColorSpecifcation(Color color) : color(color) {}

	bool is_satisfied(Product *item) const override {

		return item->productColor == color;
	}
};

struct SizeSpecification : Specification<Product> {
	Size size;

	explicit SizeSpecification(const Size size) : size{ size } {}

	bool is_satisfied(Product *item) const override{
		return item->productSize == size;
	}
};

template <typename T> struct AndSpecification : Specification<T>
{
	const Specification<T>& firstSpecification;
	const Specification<T>& secondSpecification;

	AndSpecification(const Specification<T>& first, const Specification<T>& second) : firstSpecification(first), secondSpecification(second) {}

	bool is_satisfied(T *item) const override {
		return firstSpecification.is_satisfied(item) && secondSpecification.is_satisfied(item);
	}
};

int main()
{
	Product shoes{ "Reebok", Color::red, Size::medium };
	Product tshirt{ "Spykar", Color::red, Size::large };
	Product slippers{ "Sketchers", Color::green, Size::medium };

	const vector<Product*> products{&shoes, &tshirt, &slippers};

	SpecialisedFilter specialisedFilter;
	ColorSpecifcation red(Color::red);

	auto filteredProducts = specialisedFilter.filter(products, red);

	for (auto& product : filteredProducts)
		cout << product->productName << " is Red\n";

	SizeSpecification medium(Size::medium);
	AndSpecification < Product> red_and_medium(red, medium);

	auto spec = red && medium;

	for (auto &product : specialisedFilter.filter(products, spec))
		cout << product->productName << " is Red and Medium\n";

    return 0;
}

