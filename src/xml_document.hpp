#ifndef XML_DOCUMENT_LIB
#define XML_DOCUMENT_LIB

#include "xml_node.hpp"
#include <godot_cpp/classes/ref.hpp>


namespace godot {

class XMLDocument : public RefCounted {
  GDCLASS(XMLDocument, RefCounted);

private:
  Ref<XMLNode> root;

protected:
  static void _bind_methods();

public:
  XMLDocument();
  ~XMLDocument();

  Ref<XMLNode> get_root();
  String _to_string() const;
  void set_root(Ref<XMLNode> new_root);

};
};

#endif // !XML_DOCUMENT_LIB
