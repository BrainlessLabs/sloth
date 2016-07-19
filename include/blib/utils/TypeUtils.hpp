#pragma once

#include <cstdint>
#include <type_traits>
#include <string>

namespace blib {
  namespace bun {
    /////////////////////////////////////////////////
    /// @class IsPersistant
    /// @brief If IsPersistant<Class>::value == true then Class can be persisted.
    /////////////////////////////////////////////////
    template<typename T>
    struct IsPersistant : std::integral_constant<bool, std::is_arithmetic<T>::value> {
    };

    template<>
    struct IsPersistant<std::string> : std::true_type {
    };

    namespace _details {
      /////////////////////////////////////////////////
      /// @class BoxingType
      /// @brief A simple boing type implementation.
      /////////////////////////////////////////////////
      template<typename T>
      struct BoxingType;

      template<>
      struct BoxingType<int> {
        using type = std::int64_t;
      };

      template<>
      struct BoxingType<unsigned int> {
        using type = std::uint64_t;
      };

      template<>
      struct BoxingType<float> {
        using type = double;
      };

      template<>
      struct BoxingType<double> {
        using type = double;
      };

      template<>
      struct BoxingType<char> {
        using type = std::int64_t;
      };

      template<>
      struct BoxingType<unsigned char> {
        using type = std::uint64_t;
      };

      template<>
      struct BoxingType<std::string> {
        using type = std::string;
      };

      /////////////////////////////////////////////////
      /// @fn _E
      /// @brief Method to put quote around strings. This is used for the 
      ///        formation of the Sql String.
      /////////////////////////////////////////////////
      template<typename T>
      inline T _E( const T in_val ) {
        return in_val;
      }

      inline std::string& _E( std::string& in_val ) {
        in_val = "'" + in_val + "'";
        return in_val;
      }
    }
  }
}
