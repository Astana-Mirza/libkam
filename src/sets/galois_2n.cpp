#include <sets/galois_2n.h>
#include <stdexcept>

Galois2N::Galois2N( const polynom_type& irreducible ) noexcept:
     irr_pol_{ irreducible }, basis_coeffs_{ irr_pol_.size() - 1 } {}


Galois2N::Galois2N( const polynom_type& irreducible, const polynom_type& coeffs ) noexcept:
     irr_pol_{ irreducible }, basis_coeffs_{ coeffs }
{
     basis_coeffs_.resize( irr_pol_.size() - 1 );
}


Galois2N::Galois2N( const polynom_type& irreducible, size_t power ) noexcept:
     irr_pol_{ irreducible }
{
     power %= ( irr_pol_.size() - 1 ) * ( irr_pol_.size() - 1 ) - 1;
     basis_coeffs_ = polynom_type( std::max( power + 1, irr_pol_.size() - 1 ) );
     basis_coeffs_.set( power );	// make polynomial x^power
     pol_mod( basis_coeffs_, irr_pol_ );
     basis_coeffs_.resize( irr_pol_.size() - 1 );
}


Galois2N::Galois2N( const Galois2N& other ) noexcept:
     irr_pol_{ other.irr_pol_ }, basis_coeffs_{ other.basis_coeffs_ } {}


Galois2N::Galois2N( Galois2N&& other ) noexcept
{
     irr_pol_.swap( other.irr_pol_ );
     basis_coeffs_.swap( other.basis_coeffs_ );
}


Galois2N& Galois2N::operator= ( const Galois2N& other ) noexcept
{
     if ( &other != this )
     {
          irr_pol_ = other.irr_pol_;
          basis_coeffs_ = other.basis_coeffs_;
     }
     return *this;
}


Galois2N& Galois2N::operator= ( Galois2N&& other ) noexcept
{
     if ( &other != this )
     {
          irr_pol_.swap( other.irr_pol_ );
          basis_coeffs_.swap( other.basis_coeffs_ );
     }
     return *this;
}


Galois2N& Galois2N::operator+= ( const Galois2N& other )
{
     if ( irr_pol_ != other.irr_pol_ )
     {
          throw std::runtime_error{ "operands are from different fields" };
     }
     basis_coeffs_ ^= other.basis_coeffs_;
     return *this;
}


Galois2N& Galois2N::operator-= ( const Galois2N& other )
{
     return *this += other;
}


Galois2N& Galois2N::operator*= ( const Galois2N& other )
{
     if ( irr_pol_ != other.irr_pol_ )
     {
          throw std::runtime_error{ "operands are from different fields" };
     }
     pol_mul( basis_coeffs_, other.basis_coeffs_ );
     shrink( basis_coeffs_ );
     pol_mod( basis_coeffs_, irr_pol_ );
     basis_coeffs_.resize( irr_pol_.size() - 1 );
     return *this;
}


Galois2N& Galois2N::operator/= ( const Galois2N& other )
{
     if ( irr_pol_ != other.irr_pol_ )
     {
          throw std::runtime_error{ "operands are from different fields" };
     }
     return *this *= other.inv();
}


Galois2N Galois2N::operator- () const {
     return *this;
}


bool Galois2N::operator== ( const Galois2N& other ) const
{
     return ( other.irr_pol_ == irr_pol_ ) && ( other.basis_coeffs_ == basis_coeffs_ );
}


bool Galois2N::operator!= ( const Galois2N& other ) const
{
     return !( *this == other );
}


bool Galois2N::operator> ( const Galois2N& other ) const
{
     if ( irr_pol_ != other.irr_pol_ )
     {
          throw std::runtime_error{ "operands are from different fields" };
     }
     return basis_coeffs_ > other.basis_coeffs_;
}


bool Galois2N::operator< ( const Galois2N& other ) const
{
     if ( irr_pol_ != other.irr_pol_ )
     {
          throw std::runtime_error{ "operands are from different fields" };
     }
     return basis_coeffs_ < other.basis_coeffs_;
}


bool Galois2N::operator>= ( const Galois2N& other ) const
{
     if ( irr_pol_ != other.irr_pol_ )
     {
          throw std::runtime_error{ "operands are from different fields" };
     }
     return basis_coeffs_ >= other.basis_coeffs_;
}


bool Galois2N::operator<= ( const Galois2N& other ) const
{
     if ( irr_pol_ != other.irr_pol_ )
     {
          throw std::runtime_error{ "operands are from different fields" };
     }
     return basis_coeffs_ <= other.basis_coeffs_;
}


Galois2N::operator bool() const
{
     return basis_coeffs_.any();
}


Galois2N Galois2N::inv() const
{
     if ( basis_coeffs_.none() )
     {
          throw std::runtime_error{ "division by zero" };
     }
     // extended euclidean algorithm
     polynom_type t{ irr_pol_.size() }, newt{ 1, 1 };
     polynom_type r{ irr_pol_ }, newr{ basis_coeffs_ };
     while ( newr.any() )
     {
          shrink( newr );
          shrink( newt );
          auto quotient = r;
          pol_div( quotient, newr );
          auto tmp = quotient;
          pol_mul( tmp, newr );
          tmp.resize( std::max( r.size(), tmp.size() ) );
          r.resize( tmp.size() );
          tmp  = r ^ tmp;
          r    = newr;
          newr = tmp;
          tmp  = quotient;
          pol_mul( tmp, newt );
          tmp.resize( std::max( t.size(), tmp.size() ) );
          t.resize( tmp.size() );
          tmp  = t ^ tmp;
          t    = newt;
          newt = tmp;
     }
     t.resize( irr_pol_.size()-1 );
     return Galois2N{ irr_pol_, t };
}


// returns power n of primitive element a, so: a^n == *this
size_t Galois2N::prim_power() const
{
     if ( basis_coeffs_.none() )
     {
          throw std::runtime_error{ "zero is not in multiplicative group" };
     }
     auto coeffs = basis_coeffs_;
     coeffs.resize( irr_pol_.size() );
     size_t power = coeffs.find_first();
     size_t shift_len = power;
     while ( coeffs.count() > 1 )
     {
          coeffs >>= shift_len;
          coeffs ^= irr_pol_;
          shift_len = coeffs.find_first();
          if ( coeffs.count() > 1 )
          {
               power += shift_len;
          }
     }
     return power;
}


const Galois2N::polynom_type& Galois2N::irreducible_pol() const
{
     return irr_pol_;
}


const Galois2N::polynom_type& Galois2N::coeffs() const
{
     return basis_coeffs_;
}


// remove leading zeroes until pol.size() == size
void Galois2N::shrink( polynom_type& pol, size_t size )
{
     while ( pol.size() > size && !pol.test( pol.size() - 1 ) )
     {
          pol.pop_back();
     }
}


// pol and mul must monic polynomials (leading coeff. == 1)
// this function does "pol %= mul", pol will be monic
void Galois2N::pol_mod( polynom_type& pol, const polynom_type& divisor )
{
     while ( pol.size() >= divisor.size() )
     {
          auto product = divisor;
          product.resize( pol.size() );
          product <<= pol.size() - divisor.size();
          pol ^= product;
          shrink( pol );
     }
}


// pol and mul must monic polynomials (leading coeff. == 1)
// this function does "pol /= mul", pol will be monic
void Galois2N::pol_div( polynom_type& pol, const polynom_type& divisor ) {
	polynom_type result( pol.size() );
	while ( pol.size() >= divisor.size() )
     {
          auto product = divisor;
          auto shift_len = pol.size() - divisor.size();
          result.set( shift_len );
          product.resize( pol.size() );
          product <<= shift_len;
          pol ^= product;
          shrink( pol );
	}
	shrink( result );
	pol = std::move( result );
}


// pol and mul must monic polynomials (leading coeff. == 1)
// this function does "pol *= mul", pol will be monic
void Galois2N::pol_mul( polynom_type& pol, const polynom_type& mul ) {
	if ( pol.none() )
     {
          return;
     }
     polynom_type result( pol.size() + mul.size() - 1 );
     auto pos = mul.find_first();
     while (pos != polynom_type::npos)
     {
          auto component = pol;
          component.resize( pol.size() + mul.size() - 1 );
          component <<= pos;
          result ^= component;
          pos = mul.find_next( pos );
	}
	pol = result;
}

Galois2N operator+ ( Galois2N a, const Galois2N& b ) { return a += b; }
Galois2N operator- ( Galois2N a, const Galois2N& b ) { return a += b; }
Galois2N operator* ( Galois2N a, const Galois2N& b ) { return a *= b; }
Galois2N operator/ ( Galois2N a, const Galois2N& b ) { return a /= b; }


Galois2N pow( const Galois2N& base, size_t exp )
{
     if ( !base )
     {
          return base;
     }
     return { base.irreducible_pol(), base.prim_power() * exp };
}
