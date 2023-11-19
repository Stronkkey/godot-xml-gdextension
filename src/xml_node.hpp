#ifndef XML_NODE_LIB
#define XML_NODE_LIB

#include <godot_cpp/classes/ref.hpp>

namespace godot {

class XMLNode : public RefCounted {
  GDCLASS(XMLNode, RefCounted)

private:
  String name;
  String content;
  Dictionary attributes;
  TypedArray<XMLNode> children;
  TypedArray<String> node_properties = {};
  bool standalone;

  void initialize_node_properties();

protected:
  static void _bind_methods();
  void _get_property_list(List<PropertyInfo> *r_props) const;
  bool _get(const StringName &p_property, Variant &r_value) const;

public:
  XMLNode();
  ~XMLNode();
  Dictionary _to_dict();
  String _to_string() const;
  bool is_empty() const;

  String get_name() const;
  void set_name(String new_name);

  String get_content() const;
  void set_content(const String &new_content);

  Dictionary get_attributes() const;
  void set_attributes(const Dictionary new_attributes);

  TypedArray<XMLNode> get_children() const;
  void set_children(const TypedArray<XMLNode> new_children);

  bool get_standalone() const;
  void set_standalone(const bool new_standalone);
};
}

#endif // !XML_NODE_LIB
