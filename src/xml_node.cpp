#include "xml_node.hpp"

#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void XMLNode::_bind_methods() {
  ClassDB::bind_method(D_METHOD("get_name"), &XMLNode::get_name);
  ClassDB::bind_method(D_METHOD("set_name", "new_name"), &XMLNode::set_name);
  ClassDB::add_property("XMLNode", PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");

  ClassDB::bind_method(D_METHOD("get_content"), &XMLNode::get_content);
  ClassDB::bind_method(D_METHOD("set_content", "new_content"), &XMLNode::set_content);
  ClassDB::add_property("XMLNode", PropertyInfo(Variant::STRING, "content"), "set_content", "get_content");

  ClassDB::bind_method(D_METHOD("get_attributes"), &XMLNode::get_attributes);
  ClassDB::bind_method(D_METHOD("set_attributes", "new_attributes"), &XMLNode::set_attributes);
  ClassDB::add_property("XMLNode", PropertyInfo(Variant::DICTIONARY, "attributes"), "set_attributes", "get_attributes");

  ClassDB::bind_method(D_METHOD("get_children"), &XMLNode::get_children);
  ClassDB::bind_method(D_METHOD("set_children", "new_children"), &XMLNode::set_children);
  ClassDB::add_property("XMLNode", PropertyInfo(Variant::ARRAY, "children"), "set_children", "get_children");

  ClassDB::bind_method(D_METHOD("get_standalone"), &XMLNode::get_standalone);
  ClassDB::bind_method(D_METHOD("set_standalone", "new_standalone"), &XMLNode::set_standalone);
  ClassDB::add_property("XMLNode", PropertyInfo(Variant::BOOL, "standalone"), "set_standalone", "get_standalone");

  ClassDB::bind_method(D_METHOD("to_dict"), &XMLNode::_to_dict);
  ClassDB::bind_method(D_METHOD("is_empty"), &XMLNode::is_empty);
}

void XMLNode::initialize_node_properties() {
  TypedArray<String> names_to_nodes = {};

  int children_size = children.size();

  for (int i = 0; i < children_size; i++) {
    XMLNode *child = Object::cast_to<XMLNode>(children[i]);
    if (child == nullptr)
      continue;
  
    if (!names_to_nodes.has(child->get_name())) {
      names_to_nodes.append(child->get_name());
    } else {
      names_to_nodes.erase(child->get_name());
    }

  }

  node_properties = names_to_nodes;
}

XMLNode::XMLNode() {
  name = "";
  content = "";
  attributes = {};
  standalone = false;
  initialize_node_properties();
}

XMLNode::~XMLNode() {
}

Dictionary XMLNode::_to_dict() {
  Dictionary output = {};
  Dictionary child_dict = {};

  output["__name__"] = name;
  output["__content__"] = content;
  output["attrs"] = attributes;

  int children_size = children.size();

  for (int i = 0; i < children_size; i++) {
    XMLNode *child = Object::cast_to<XMLNode>(children[i]);
    child_dict[child->get_name()] = child->_to_dict();
  }

  output["children"] = child_dict;

  return output;
}

String XMLNode::_to_string() const {
  return vformat("<XMLNode name=\"%s\" attributes=\"%s\" content=\"%s\" standalone=%s children=%s>",
      name,
      !attributes.is_empty() ? "{...}" : "",
      !content.is_empty() ? "..." : "",
      standalone ? "true" : "false",
      !children.is_empty() ? "[...]" : "[]");
}

bool XMLNode::is_empty() const {
  return (children.is_empty() && attributes.is_empty() && name == "" && content.is_empty()) ? true : false;
}

bool XMLNode::_get(const StringName &p_property, Variant &r_value) const {
  TypedArray<StringName> properties;
  if (properties.has(p_property) || !node_properties.has(String(p_property)))
    return false;

  int children_size = children.size();

  for (int i = 0; i < children_size; i++) {
    XMLNode *child = Object::cast_to<XMLNode>(children[i]);
    if (child->name == p_property)
      return true;
  }

  return false;
}

void XMLNode::_get_property_list(List<PropertyInfo> *r_props) const {
  List<PropertyInfo> property_list;

  int property_size = node_properties.size();

  for (int i = 0; i < property_size; i++) {
    Ref<XMLNode> child = children[i];
    if (child.is_null())
      continue;

    r_props->push_front(PropertyInfo(Variant::STRING, child->name, PROPERTY_HINT_NONE, "", PROPERTY_HINT_NONE, "XMLNode"));
  }
}

String XMLNode::get_name() const {
  return name;
}

void XMLNode::set_name(String new_name) {
  name = new_name;
}

String XMLNode::get_content() const {
  return content;
}

void XMLNode::set_content(const String &new_content) {
  content = new_content;
}

Dictionary XMLNode::get_attributes() const {
  return attributes;
}

void XMLNode::set_attributes(const Dictionary new_attributes) {
  attributes.clear();
  attributes = new_attributes;
}

TypedArray<XMLNode> XMLNode::get_children() const {
  return children;
}

void XMLNode::set_children(const TypedArray<XMLNode> new_children) {
  children.clear();

  TypedArray<XMLNode> final_children;
  for (int i = 0; i < new_children.size(); i++) {
    XMLNode *child = Object::cast_to<XMLNode>(new_children[i]);
    final_children.append(child);
  }

  children = final_children;
  initialize_node_properties();
}

bool XMLNode::get_standalone() const {
  return standalone;
}

void XMLNode::set_standalone(bool new_standalone) {
  standalone = new_standalone;
}
