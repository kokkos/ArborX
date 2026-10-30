ArborX code style
=================
The following code style was adopted in ArborX:

- Code format from ``.clang-format`` (with clang-16)
- East const: ``int const* ptr;``
- constexpr West: ``constexpr float zero = 0.f;``
- Camel case template parameters: ``TemplateType``
- Camel case class names: ``ClassName``
- Lower camel case function names: ``functionName``
- Lower case, underscore separated variables: ``variable_name``
- Class members which are private are preceeded by an underscore: ``_private_class_variable``
- Class/struct member type aliases use lower case, underscore separated names: ``using node_type = Node;``
- ``ArborX::``-prefixed Kokkos profiling regions, parallel regions, and views labels

  - ``ArborX::ClassName::view_label`` for class member views
  - ``ArborX::ClassName::function::label`` for local views, profiling regions and parallel regions in class member functions
  - ``ArborX::Function::label`` or ``ArborX::FunctionalArea::label`` everywhere else
