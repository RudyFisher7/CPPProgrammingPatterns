# Reflection

This module implements an object that provides reflection.

See unit test(s) inside test/ for usage examples.

## v0.0.1 Features
- Class type information at runtime using a mapping of the system-specific std::type_info (for use in templates) and a portable StringName.
- Properties with system-specific std::type_info are indexed in a map to allow universal get/set/has operations and getting the list of properties. Properties are indexed using pointers, without duplication, so they update automatically when set.
- Factory methods are created for runtime creation of objects.
- StringName is used for 0(1) name look up (as apposed to 0(N) when using std::string). Note, in order to see performance improvement, the source code should reuse StringName instances instead of making new ones of the same std::string value.