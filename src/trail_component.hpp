#ifndef TRAIL_COMPONENT_ASS
#define TRAIL_COMPONENT_ASS

#include <godot_cpp/classes/line2d.hpp>

namespace godot {

class TrailComponent : public Line2D {
  GDCLASS(TrailComponent, Line2D);
  
public:
  enum ProcessCallback {
    TRAIL_PROCESS_PHYSICS = 0,
    TRAIL_PROCESS_IDLE = 1,
    TRAIL_PROCESS_NONE = 2
  };

protected:
  static void _bind_methods();

private:
  Vector2 previous_position;
  Vector2 previous_velocity;
  bool clean_points;
  bool optimize;
  int max_points;
  ProcessCallback process_callback;
  Node2D *follow_node;

  void process_line();
  void ready();
  void process(double delta);
  void physics_process(double delta);

public:
  TrailComponent();
  ~TrailComponent();

  void set_clean_points(const bool enabled);
  void set_process_callback(const int new_process_callback);
  void set_follow_node(Node2D *new_node_path);
  void set_optimized(const bool optimized);
  void set_max_points(const int new_max_points);
  
  bool get_clean_points() const;
  bool is_optimized() const;
  int get_process_callback() const;
  int get_max_points() const;
  Node2D *get_follow_node();

  void _notification(int p_what);
};
}

#endif // !TRAIL_COMPONENT_ASS
