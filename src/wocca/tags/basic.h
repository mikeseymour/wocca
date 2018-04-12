#pragma once
#include <string>

namespace wocca {
namespace tags {

using view = std::string_view;

struct tag {
	explicit operator bool() const {return !value.empty();}

	int key;
	view value;
};

}
}