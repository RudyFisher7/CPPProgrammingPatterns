/*
 * MIT License
 *
 * Copyright (c) 2025 Rudy Fisher (kiyasui-hito)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef KOI_PUB_SUB_SERIALIZATION_H
#define KOI_PUB_SUB_SERIALIZATION_H


#include <cstdint>
#include <format>
#include <type_traits>
#include <vector>
#include <iostream>
#include <string>


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
        T network_bytes_to_primitive(const uint8_t* bytes) {
            static_assert(std::is_scalar<T>::value && !std::is_pointer<T>::value, "Type T must be bool, char, int, float, enum, or a derivation of these scalar types. T must not be a pointer or a non-scalar type.");
            T result{};

            if (is_little_endian()) {
                size_t size = sizeof(T);
                uint8_t result_buffer[size];
                for (size_t i = 0; i < size; ++i) {
                    result_buffer[i] = bytes[size - 1u - i];
                }

                result = *reinterpret_cast<T*>(result_buffer);
            } else {
                const T const_result = *reinterpret_cast<const T*>(bytes);
                result = const_result;
            }

            return result;
        }


        /**
         * Base case for getting the number of bytes needed to serialize a variable number and variable types of
         * primitive data into a byte array.
         * @return 0
         */
        constexpr size_t get_number_of_bytes() {
            return 0;
        }


        /**
         * Gets the number of bytes needed to serialize a variable number and variable types of
         * primitive data into a byte array.
         * @return The number of bytes needed to be present in a byte array, as a size type.
         */
        template<typename T, typename ... TArgs>
        constexpr size_t get_number_of_bytes(const T& value, const TArgs& ... args) {
            return sizeof(T) + get_number_of_bytes(args...);
        }


        template<typename T>
        void _to_network_bytes_helper(uint8_t* begin, const T& value) {
        uint8_t* end = primitive_to_network_bytes(value, begin);
    }


        template<typename T, typename ... TArgs>
        typename std::enable_if<(sizeof ... (TArgs) > 0), void>::type
        _to_network_bytes_helper(uint8_t* begin, const T& value, const TArgs& ... args) {
            uint8_t* end = primitive_to_network_bytes(value, begin);
            _to_network_bytes_helper(end, args...);
        }


        /**
         * Serializes the arguments into the given vector. Resizes the vector as needed.
         * @tparam T The type of the first value to serialize.
         * @tparam TArgs The types of the rest of the values to serialize.
         * @param out_result The vector of bytes that contain the serialized data.
         * @param value The first value to serialize.
         * @param args The rest of the values to serialize.
         */
        template<typename T, typename ... TArgs>
        void to_network_bytes(std::vector<uint8_t>& out_result, const T& value, const TArgs& ... args) {
            size_t number_of_bytes = get_number_of_bytes(value, args...);
            out_result.resize(number_of_bytes);
            _to_network_bytes_helper(out_result.data(), value, args...);
        }


        template<typename T>
        std::tuple<T> from_network_bytes(const uint8_t* begin) {
            T value = network_bytes_to_primitive<T>(begin);
            return std::make_tuple(value);
        }


        /**
         * Deserializes the array of bytes pointed to by the given pointer into a tuple of specified types.
         * @warning The size of the array of bytes is not validated.
         * @tparam T The type of the first value to deserialize.
         * @tparam TArgs The types of the rest of the values to deserialize.
         * @param begin A pointer to the beginning of the array.
         * @return A tuple with all values deserialized into their parameterized types.
         */
        template<typename T, typename ... TArgs>
        typename std::enable_if<(sizeof ... (TArgs) > 0), std::tuple<T, TArgs...>>::type
        from_network_bytes(const uint8_t* begin) {
            T value = network_bytes_to_primitive<T>(begin);
            return std::tuple_cat(std::make_tuple(value), from_network_bytes<TArgs...>(begin + sizeof(T)));
        }


        template<typename T>
        void _from_network_bytes_helper(const uint8_t* begin, T& out_value) {
            out_value = network_bytes_to_primitive<T>(begin);
        }

        template<typename T, typename ... TArgs>
        typename std::enable_if<(sizeof ... (TArgs) > 0), void>::type
        _from_network_bytes_helper(const uint8_t* begin, T& out_value, TArgs&... args) {
            out_value = network_bytes_to_primitive<T>(begin);
            _from_network_bytes_helper<TArgs...>(begin + sizeof(T), args...);
        }


        /**
         * Deserializes the arguments from the given array of bytes pointed to by the given pointer. This function
         * Checks to make sure the array of bytes has enough bytes.
         * @note This function populates its type-parameterized arguments directly, so they must not be const.
         * @tparam T The type of the first value to deserialize.
         * @tparam TArgs The types of the rest of the values to deserialize.
         * @param begin A pointer to the beginning of the array.
         * @param end A pointer to the end of the array, past the last value.
         * @param out_value The first value that was deserialized.
         * @param args The rest of the values to deserialize.
         */
        template<typename T, typename ... TArgs>
        bool from_network_bytes(const uint8_t* begin, const uint8_t* end, T& out_value, TArgs&... args) {
            size_t number_of_bytes = get_number_of_bytes(out_value, args...);
            if ((end - begin) < number_of_bytes) {
                return false;
            }

            out_value = network_bytes_to_primitive<T>(begin);
            _from_network_bytes_helper<TArgs...>(begin + sizeof(T), args...);
            return true;
        }
    }
}


#endif //KOI_PUB_SUB_SERIALIZATION_H
