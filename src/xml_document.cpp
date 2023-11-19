#include "xml_document.hpp"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void XMLDocument::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_root"), &XMLDocument::get_root);
  ClassDB::bind_method(D_METHOD("set_root", "new_root"), &XMLDocument::set_root);
  ClassDB::add_property("XMLDocument", PropertyInfo(Variant::OBJECT, "root", PROPERTY_HINT_RESOURCE_TYPE, "XMLDocument", PROPERTY_USAGE_NONE), "set_root", "get_root");
}

XMLDocument::XMLDocument() {
}

XMLDocument::~XMLDocument() {}

Ref<XMLNode> XMLDocument::get_root() {
  return root;
}

void XMLDocument::set_root(Ref<XMLNode> new_root) {
  root = new_root;
}

String XMLDocument::_to_string() const {
  if (root.is_valid()) {
    return vformat("<XMLDocument root=%s>", root->_to_string());
  }
  return "<XMLDocument root=[]>";
}
