# Pub/Sub

This module implements the pub/sub pattern.

See unit test(s) inside test/ for usage examples.

## v0.0.1 Features
- A base Data class provides virtual functions to be overridden for (de)serializing its data to/from byte arrays in network byte order.
- A Callable class provides a way for associating functions with class instances.
- A Server class functions as the mediator/relay/broker. It relays data to the appropriate subscribers when publishers send the data to it.
- Some serialization function templates are provided for networked use cases. They only support fundamental/scalar types.