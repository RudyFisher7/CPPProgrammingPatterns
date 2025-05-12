#ifndef KOI_PUB_SUB_SERIALIZATION_H
#define KOI_PUB_SUB_SERIALIZATION_H


#include <cstdint>
#include <format>
#include <type_traits>


namespace KoiPubSub {
    namespace Serialization {

        /**
         * Checks if the system is little endian.
         * @return True if the system is little endian, else false.
         */
        constexpr bool is_little_endian() {
            union {
                const uint16_t WORD {0x01};
                uint8_t BYTES[2u];
            };

            return BYTES[0] == 0x01;
        }


        /**
         * Serializes the given primitive value into a byte array in network byte order (big endian).
         * @tparam T The type to serialize from.
         * @param value The value to serialize.
         * @param destination The destination to serialize the value into. Can be a byte array or part of one. Note that
         * this function doesn't check if there is enought space in the array. The number of bytes serialized is
         * determined by the size of T.
         * @return A pointer to the first byte that is past the end of the value in the array. Like array::end().
         */
        template<typename T>
        uint8_t* primitive_to_network_bytes(T value, uint8_t* destination) {
            static_assert(std::is_scalar<T>::value && !std::is_pointer<T>::value, "Type T must be bool, char, int, float, enum, or a derivation of these scalar types. T must not be a pointer or a non-scalar type.");
            size_t size = sizeof(T);
            uint8_t* result = destination + size;
            auto* value_bytes = reinterpret_cast<uint8_t*>(&value);

            if (is_little_endian()) {
                for (size_t i = 0; i < size; ++i) {
                    destination[i] = value_bytes[size - 1u - i];
                }
            } else {
                for (size_t i = 0; i < size; ++i) {
                    destination[i] = value_bytes[i];
                }
            }

            return result;
        }

        /**
         * Deserializes the given bytes into a primitive type of T.
         * @tparam T The type to deserialize the bytes into.
         * @param bytes The bytes to be deserialized. Note that the size of the array is determined by the size of the
         * type returned.
         * @return The value that was deserialized from the byte array.
         */
        template<typename T>
        T network_bytes_to_primitive(uint8_t* bytes) {
            static_assert(std::is_scalar<T>::value && !std::is_pointer<T>::value, "Type T must be bool, char, int, float, enum, or a derivation of these scalar types. T must not be a pointer or a non-scalar type.");
            T result{};
            size_t size = sizeof(T);
            uint8_t result_buffer[size];

            if (is_little_endian()) {
                for (size_t i = 0; i < size; ++i) {
                    result_buffer[i] = bytes[size - 1u - i];
                }

                result = *reinterpret_cast<T*>(result_buffer);
            } else {
                result = *reinterpret_cast<T*>(bytes);
            }

            return result;
        }
    }
}


#endif //KOI_PUB_SUB_SERIALIZATION_H
