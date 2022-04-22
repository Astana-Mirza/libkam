#ifndef GALOIS_2N_H
#define GALOIS_2N_H

#include <boost/dynamic_bitset.hpp>

// element of a Galois field GF(2^n)
class Galois2N
{
public:
     using polynom_type = boost::dynamic_bitset<uint64_t>;

	explicit Galois2N( const polynom_type& irreducible = {}, const polynom_type& coeffs = {} ) noexcept;
     explicit Galois2N( const polynom_type& irreducible, size_t power ) noexcept;
     Galois2N( const Galois2N& other ) noexcept;
     Galois2N( Galois2N&& other ) noexcept;

     Galois2N& operator=  ( const Galois2N& other ) noexcept;
     Galois2N& operator=  ( Galois2N&& other ) noexcept;
     Galois2N& operator+= ( const Galois2N& other );
     Galois2N& operator-= ( const Galois2N& other );
     Galois2N& operator*= ( const Galois2N& other );
     Galois2N& operator/= ( const Galois2N& other );
     Galois2N operator- () const;  // unary minus
     bool operator== ( const Galois2N& other ) const;
     bool operator!= ( const Galois2N& other ) const;
     bool operator>  ( const Galois2N& other ) const;
     bool operator<  ( const Galois2N& other ) const;
     bool operator>= ( const Galois2N& other ) const;
     bool operator<= ( const Galois2N& other ) const;
     explicit operator bool() const;		// equivalent to *this != 0

     Galois2N inv() const;
     size_t prim_power() const;
     const polynom_type& irreducible_pol() const;
     const polynom_type& coeffs() const;

private:
     polynom_type irr_pol_;        // irreducible polynomial
     polynom_type basis_coeffs_;   // coefficients of a value in power
                                   // basis {a^n-1, ... a, 1}, where
                                   // a is a primitive element
     // erase leading zeroes, polynomial's size will be >= size
     static void shrink( polynom_type& pol, size_t size = 0 );
     static void pol_mod( polynom_type& pol, const polynom_type& divisor );
     static void pol_div( polynom_type& pol, const polynom_type& divisor );
     static void pol_mul( polynom_type& pol, const polynom_type& mul );
};


Galois2N operator+ ( Galois2N a, const Galois2N& b );
Galois2N operator- ( Galois2N a, const Galois2N& b );
Galois2N operator* ( Galois2N a, const Galois2N& b );
Galois2N operator/ ( Galois2N a, const Galois2N& b );
Galois2N pow( const Galois2N& base, size_t exp );

#endif // #ifndef GALOIS_2N_H
