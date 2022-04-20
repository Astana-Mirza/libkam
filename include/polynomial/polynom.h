#ifndef POLYNOM_H
#define POLYNOM_H

#include <polynomial/monom_compare.h>
#include <polynomial/monom.h>

#include <map>
#include <vector>
#include <string>
#include <stdexcept>

template < typename CoeffType, typename Compare = LexGreater >
class Polynom
{
public:
     using coeff_type = CoeffType;
     using monom_compare = Compare;

     Polynom( std::map<Monom, CoeffType, Compare> terms = {} ) noexcept;
     Polynom( const CoeffType& coeff ) noexcept;
     Polynom( const Polynom& other ) noexcept;
     Polynom( Polynom&& other ) noexcept;

     Polynom& operator= ( const Polynom& other ) noexcept;
     Polynom& operator= ( Polynom&& other ) noexcept;
     Polynom& operator+= ( const Polynom& other );
     Polynom& operator-= ( const Polynom& other );
     Polynom& operator*= ( const Polynom& other );
     Polynom& operator*= ( const CoeffType& coeff );
     Polynom& operator/= ( const CoeffType& coeff );
     Polynom operator- () const;
     explicit operator bool() const;		// equivalent to *this != 0
     bool operator== ( const Polynom& other ) const;
     bool operator!= ( const Polynom& other ) const;

     Polynom mod( const std::vector< Polynom >& divs ) const;
     Polynom subst( const Polynom& pol, const std::string& var ) const;
     const std::map< Monom, CoeffType, Compare >& get_terms() const;
     Polynom leading_term() const;
     Monom leading_monom() const;
     CoeffType leading_coeff() const;

private:
     std::map< Monom, CoeffType, Compare > terms_;
};


template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> operator+
( Polynom< CoeffType, Compare > lhs, const Polynom< CoeffType, Compare >& rhs );

template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> operator-
( Polynom< CoeffType, Compare > lhs, const Polynom< CoeffType, Compare >& rhs );

template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> operator*
( Polynom< CoeffType, Compare > lhs, const Polynom< CoeffType, Compare >& rhs );

template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare > operator*
( const CoeffType& lhs, const Polynom< CoeffType, Compare >& rhs );

template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare > operator*
( Polynom< CoeffType, Compare > lhs, const CoeffType& rhs );

template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> operator/
( Polynom< CoeffType, Compare > lhs, const CoeffType& rhs );

template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> pow( const Polynom<CoeffType, Compare>& base, size_t exp );

//-----------------------------------------IMPLEMENTATION------------------------------------------

template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >::Polynom( std::map<Monom, CoeffType, Compare> terms ) noexcept
{
     for ( const auto& term : terms )
     {
          if ( term.second )
          {
               terms_.insert( term );
          }
     }
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >::Polynom( const CoeffType& coeff ) noexcept
{
     if ( coeff )
     {
          terms_[ Monom{} ] = coeff;
     }
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >::Polynom( const Polynom< CoeffType, Compare >& other ) noexcept:
     terms_{ other.terms_ } {}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >::Polynom( Polynom< CoeffType, Compare >&& other ) noexcept
{
     std::swap( terms_, other.terms_ );
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >& Polynom< CoeffType, Compare >::operator=
( const Polynom< CoeffType, Compare >& other ) noexcept
{
     if ( &other != this )
     {
          terms_ = other.terms_;
     }
     return *this;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >& Polynom< CoeffType, Compare >::operator=
( Polynom< CoeffType, Compare >&& other ) noexcept
{
     if ( &other != this )
     {
          terms_ = {};
          std::swap( terms_, other.terms_ );
     }
     return *this;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >& Polynom< CoeffType, Compare >::operator+=
( const Polynom< CoeffType, Compare >& other )
{
     for ( const auto& term : other.terms_ )
     {
          auto& value = terms_[ term.first ] += term.second;
          if ( !value )
          {
               terms_.erase( term.first );
          }
     }
     return *this;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >& Polynom< CoeffType, Compare >::operator-=
( const Polynom< CoeffType, Compare >& other )
{
     for ( const auto& term : other.terms_ )
     {
          auto& value = terms_[ term.first ] -= term.second;
          if ( !value )
          {
               terms_.erase( term.first );
          }
     }
     return *this;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >& Polynom< CoeffType, Compare >::operator*=
( const Polynom< CoeffType, Compare >& other )
{
     Polynom< CoeffType, Compare > result;
     for ( const auto& this_term : terms_ )
     {
          for ( const auto& other_term : other.terms_ )
          {
               result.terms_[ this_term.first * other_term.first ] = this_term.second * other_term.second;
          }
     }
     *this = result;
     return *this;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >& Polynom< CoeffType, Compare >::operator*=
( const CoeffType& coeff )
{
     if ( !coeff )
     {
          terms_ = {};
          return *this;
     }
     for (auto iter = terms_.begin(); iter != terms_.end(); ++iter)
     {
          iter->second = iter->second * coeff;
     }
     return *this;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >& Polynom< CoeffType, Compare >::operator/=
( const CoeffType& coeff )
{
     if ( !coeff )
     {
          throw std::runtime_error{ "division by zero" };
     }
     for (auto iter = terms_.begin(); iter != terms_.end(); ++iter)
     {
          iter->second = iter->second / coeff;
     }
     return *this;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare > Polynom< CoeffType, Compare >::operator- () const
{
     auto ret{ *this };
     for (auto iter = ret.terms_.begin(); iter != ret.terms_.end(); ++iter)
     {
          iter->second = -( iter->second );
     }
     return ret;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare >::operator bool() const
{
     return !terms_.empty();
}

template < typename CoeffType, typename Compare >
bool Polynom< CoeffType, Compare >::operator== ( const Polynom& other ) const
{
     return terms_ == other.terms_;
}


template < typename CoeffType, typename Compare >
bool Polynom< CoeffType, Compare >::operator!= ( const Polynom& other ) const
{
     return terms_ != other.terms_;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare > Polynom< CoeffType, Compare >::mod
( const std::vector< Polynom< CoeffType, Compare > >& divs ) const
{
     Polynom< CoeffType, Compare > rem, dividend{ *this };
     while ( dividend )
     {
          size_t i = 0;
          bool had_division = false;
          while ( i < divs.size() && !had_division )
          {
               if ( dividend.leading_monom().is_divisible( divs[ i ].leading_monom() ) )
               {
                    Polynom< CoeffType, Compare > tmp {
                         { { dividend.leading_monom() / divs[i].leading_monom(),
                           dividend.leading_coeff() / divs[i].leading_coeff() } }
                    };
                    dividend -= tmp * divs[i];
                    had_division = true;
               }
               else
               {
                    i++;
               }
          }
          if ( !had_division )
          {
               auto lt = dividend.leading_term();
               dividend -= lt;
               rem += lt;
          }
     }
     return rem;
} // mod


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare > Polynom< CoeffType, Compare >::subst
( const Polynom< CoeffType, Compare >& pol, const std::string& var ) const
{
     if ( terms_.empty() )
     {
          return *this;
     }
     Polynom< CoeffType, Compare > ret;
     for ( std::pair< Monom, size_t > term : terms_ ) {
          size_t deg = term.first.var_deg( var );
          term.first.remove_var( var );
          ret += pow(pol, deg) * Polynom< CoeffType, Compare >{ { { term.first, term.second } } };
     }
     return ret;
}


template < typename CoeffType, typename Compare >
const std::map< Monom, CoeffType, Compare >& Polynom< CoeffType, Compare >::get_terms() const
{
     return terms_;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare > Polynom< CoeffType, Compare >::leading_term() const
{
     if ( !terms_.empty() )
     {
          auto begin = terms_.cbegin();
          return Polynom< CoeffType, Compare >{ { { begin->first, begin->second } } };
     }
     return Polynom< CoeffType, Compare >{};
}


template < typename CoeffType, typename Compare >
Monom Polynom< CoeffType, Compare >::leading_monom() const
{
     if ( !terms_.empty() )
     {
          return terms_.cbegin()->first;
     }
     return Monom{};
}


template < typename CoeffType, typename Compare >
CoeffType Polynom< CoeffType, Compare >::leading_coeff() const
{
     if ( !terms_.empty() )
     {
          return terms_.cbegin()->second;
     }
     return CoeffType{};
}


template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> operator+ ( Polynom< CoeffType, Compare > lhs, const Polynom< CoeffType, Compare >& rhs )
{
     return lhs += rhs;
}


template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> operator- ( Polynom< CoeffType, Compare > lhs, const Polynom< CoeffType, Compare >& rhs )
{
     return lhs -= rhs;
}


template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> operator* ( Polynom< CoeffType, Compare > lhs, const Polynom< CoeffType, Compare >& rhs )
{
     return lhs *= rhs;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare > operator* ( const CoeffType& lhs, const Polynom< CoeffType, Compare >& rhs )
{
     return rhs * lhs;
}


template < typename CoeffType, typename Compare >
Polynom< CoeffType, Compare > operator* ( Polynom< CoeffType, Compare > lhs, const CoeffType& rhs )
{
     return lhs *= rhs;
}


template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> operator/ ( Polynom< CoeffType, Compare > lhs, const CoeffType& rhs )
{
     return lhs /= rhs;
}


template < typename CoeffType, typename Compare >
Polynom<CoeffType, Compare> pow( const Polynom<CoeffType, Compare>& base, size_t exp )
{
     if ( !base )
     {
          return base;
     }
     if ( exp == 0 )
     {
          auto coeff = base.leading_coeff();
          return Polynom<CoeffType, Compare>{ { { Monom{}, coeff / coeff } } }; // one
     }
     auto ret = base;
     for ( size_t i = 1; i < exp; i++ )
     {
          ret *= base;
     }
     return ret;
}


#endif // #ifndef POLYNOM_H
