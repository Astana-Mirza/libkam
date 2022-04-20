#ifndef RESIDUE_H
#define RESIDUE_H

#include <cstdint>

class Residue
{
public:
     Residue( uint64_t modulo, int64_t value = 0 );

     Residue& operator=  ( const Residue& other ) noexcept;
     Residue& operator+= ( const Residue& other );
     Residue& operator-= ( const Residue& other );
     Residue& operator*= ( const Residue& other );
     Residue& operator/= ( const Residue& other );
     Residue operator- () const;  // unary minus
     bool operator== ( const Residue& other ) const;
     bool operator!= ( const Residue& other ) const;
     bool operator<  ( const Residue& other ) const;
     bool operator>  ( const Residue& other ) const;
     bool operator<= ( const Residue& other ) const;
     bool operator>= ( const Residue& other ) const;
     explicit operator bool() const;

     Residue inv() const;
     uint64_t get_modulo() const;
     bool is_field() const;

private:
     uint64_t modulo_;
     uint64_t value_;
};


Residue operator+ ( Residue a, const Residue& b );
Residue operator- ( Residue a, const Residue& b );
Residue operator* ( Residue a, const Residue& b );
Residue operator/ ( Residue a, const Residue& b );

#endif // #ifndef RESIDUE_H