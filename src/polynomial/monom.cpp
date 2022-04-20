#include <polynomial/monom.h>

#include <stdexcept>

Monom::Monom( const std::map< std::string, size_t >& vars ) noexcept
{
     for ( const auto& var : vars )
     {
          if ( var.second )
          {
               vars_.insert( var );
          }
     }
}


Monom::Monom( const Monom& other ) noexcept:
     vars_{ other.vars_ } {}


Monom::Monom( Monom&& other ) noexcept
{
     std::swap( vars_, other.vars_ );
}


Monom& Monom::operator= ( const Monom& other ) noexcept
{
     if ( &other != this )
     {
          vars_ = other.vars_;
     }
     return *this;
}


Monom& Monom::operator= ( Monom&& other ) noexcept
{
     if ( &other != this )
     {
          vars_ = {};
          std::swap( vars_, other.vars_ );
     }
     return *this;
}


Monom& Monom::operator*= ( const Monom& other )
{
     for ( const auto& kv_pair : other.vars_ )
     {
          vars_[ kv_pair.first ] += kv_pair.second;
     }
     return *this;
}


Monom& Monom::operator/= ( const Monom& other )
{
     for ( const auto& kv_pair : other.vars_ )
     {
          auto found = vars_.find( kv_pair.first );
          if ( found == vars_.end() || found->second < kv_pair.second )
          {
               throw std::runtime_error{ "indivisible monomials" };
          }
          found->second -= kv_pair.second;
          if ( found->second == 0 )
          {
               vars_.erase( found );
          }
     }
     return *this;
}


bool Monom::operator== ( const Monom& other ) const
{
     return vars_ == other.vars_;
}


bool Monom::operator!= ( const Monom& other ) const
{
     return vars_ != other.vars_;
}


void Monom::remove_var( const std::string& var )
{
     vars_.erase( var );
}


void Monom::set_deg( const std::string& var, size_t deg )
{
     if ( deg == 0 )
     {
          vars_.erase( var );
     }
     else
     {
          vars_[ var ] = deg;
     }
}


size_t Monom::var_deg( const std::string& var ) const
{
     auto found = vars_.find( var );
     if ( found == vars_.cend() )
     {
          return 0;
     }
     return found->second;
}


size_t Monom::full_deg() const
{
     size_t full = 0;
     for ( const auto& kv_pair : vars_ )
     {
          full += kv_pair.second;
     }
     return full;
}


size_t Monom::var_count() const
{
     return vars_.size();
}


bool Monom::is_divisible( const Monom& other ) const
{
     for ( const auto& kv_pair : other.vars_ )
     {
          auto found = vars_.find( kv_pair.first );
          if ( found == vars_.cend() || found->second < kv_pair.second )
          {
               return false;
          }
     }
     return true;
}


const std::map< std::string, size_t >& Monom::get_vars() const
{
     return vars_;
}


Monom operator* ( Monom lhs, const Monom& rhs ) { return lhs *= rhs; }
Monom operator/ ( Monom lhs, const Monom& rhs ) { return lhs /= rhs; }


Monom pow( const Monom base, size_t exp )
{
     std::map< std::string, size_t > vars = base.get_vars();
     for ( auto& kv_pair : vars )
     {
          kv_pair.second *= exp;
     }
     return Monom{ vars };
}


Monom lcm(const Monom& a, const Monom& b)
{
     std::map< std::string, size_t > vars;
     const auto& vars_a = a.get_vars(), vars_b = b.get_vars();
     for ( const auto& var : vars_a )
     {
          vars[ var.first ] = std::max( var.second, b.var_deg( var.first ) );
     }
     for ( const auto& var : vars_b )
     {
          vars[ var.first ] = std::max( var.second, a.var_deg( var.first ) );
     }
     return Monom{ vars };
}
