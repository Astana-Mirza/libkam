#include <sets/residue.h>

#include <stdexcept>
#include <cmath>

Residue::Residue( uint64_t modulo, int64_t value ) noexcept:
     modulo_{ modulo }
{
     if ( modulo_ <= 1)
     {
          throw std::runtime_error{ "wrong modulo value" };
     }
     if ( value >= 0 )
     {
          value_ = value % modulo_;
     }
     else
     {
          value_ = modulo_ - ( ( -value ) % modulo_ );
     }
}


Residue::Residue( const Residue& other ) noexcept:
     modulo_{ other.modulo_ }, value_{ other.value_ } {}


Residue& Residue::operator= ( const Residue& other ) noexcept
{
     modulo_ = other.modulo_;
     value_ = other.value_;
     return *this;
}


Residue& Residue::operator+= ( const Residue& other )
{
     if ( modulo_ != other.modulo_ )
     {
          throw std::runtime_error{ "different modulo values" };
     }
     value_ = ( value_ + other.value_ ) % modulo_;
     return *this;
}


Residue& Residue::operator-= ( const Residue& other )
{
     if ( modulo_ != other.modulo_ )
     {
          throw std::runtime_error{ "different modulo values" };
     }
     value_ = (value_ + modulo_ - other.value_) % modulo_;
     return *this;
}


Residue& Residue::operator*= ( const Residue& other )
{
     if ( modulo_ != other.modulo_ )
     {
          throw std::runtime_error{ "different modulo values" };
     }
     value_ = (value_ * other.value_) % modulo_;
     return *this;
}


Residue& Residue::operator/= ( const Residue& other )
{
     if ( modulo_ != other.modulo_ )
     {
          throw std::runtime_error{ "different modulo values" };
     }
     value_ = (value_ * other.inv().value_) % modulo_;
     return *this;
}


Residue Residue::operator- () const
{
     Residue ret( modulo_ );
     ret.value_ = (modulo_ - value_) % modulo_;
     return ret;
}


bool Residue::operator== ( const Residue& other ) const
{
     return modulo_ == other.modulo_ && value_ == other.value_;
}


bool Residue::operator!= ( const Residue& other ) const
{
     return !( *this == other );
}


bool Residue::operator< ( const Residue& other ) const
{
     if ( modulo_ != other.modulo_ )
     {
          throw std::runtime_error{ "different modulo values" };
     }
     return value_ < other.value_;
}


bool Residue::operator> ( const Residue& other ) const
{
     if ( modulo_ != other.modulo_ )
     {
          throw std::runtime_error{ "different modulo values" };
     }
     return value_ > other.value_;
}


bool Residue::operator<= ( const Residue& other ) const
{
     if ( modulo_ != other.modulo_ )
     {
          throw std::runtime_error{ "different modulo values" };
     }
     return value_ <= other.value_;
}


bool Residue::operator>= ( const Residue& other ) const
{
     if ( modulo_ != other.modulo_ )
     {
          throw std::runtime_error{ "different modulo values" };
     }
     return value_ >= other.value_;
}


Residue::operator bool() const
{
     return value_ != 0;
}


Residue Residue::inv() const
{
     if ( value_ == 0 )
     {
          throw std::runtime_error{ "division by zero" };
     }
     // extended euclidean algorithm
     uint64_t t    = 0, r    = modulo_;
     uint64_t newt = 1, newr = value_;
     while ( newr != 0 )
     {
          auto quotient = r / newr;
          auto tmp = t - quotient * newt;
          t    = newt;
          newt = tmp;
          tmp  = r - quotient * newr;
          r    = newr;
          newr = tmp;
     }
     if ( r > 1 )
     {
          throw std::runtime_error{ "not invertible element" };
     }
     if ( t > modulo_ ) // unsigned t < 0
     {
          t += modulo_;
     }
     Residue ret( modulo_ );
     ret.value_ = t % modulo_;
     return ret;
}


uint64_t Residue::get_modulo() const
{
     return modulo_;
}


uint64_t Residue::get_value() const
{
     return value_;
}


bool Residue::is_field() const
{
     uint64_t end = std::sqrt( modulo_ );
     for ( uint64_t i = 2; i < end; i++ )
     {
          if ( modulo_ % i )
          {
               return false;
          }
     }
     return true;
}

Residue operator+ ( Residue a, const Residue& b ) { return a += b; }
Residue operator- ( Residue a, const Residue& b ) { return a -= b; }
Residue operator* ( Residue a, const Residue& b ) { return a *= b; }
Residue operator/ ( Residue a, const Residue& b ) { return a /= b; }


Residue pow( const Residue& base, size_t exp )
{
     if ( !base )
     {
          return base;
     }
     Residue result{ base.get_modulo(), 1 };
     for ( size_t i = 0; i < exp; i++ )
     {
          result *= base;
     }
     return base;
}
