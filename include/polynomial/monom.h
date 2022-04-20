#ifndef MONOM_H
#define MONOM_H

#include <map>
#include <string>

class Monom
{
public:
     Monom( const std::map< std::string, size_t >& vars = {} ) noexcept;
     Monom( const Monom& other ) noexcept;
     Monom( Monom&& other ) noexcept;

     Monom& operator= ( const Monom& other ) noexcept;
     Monom& operator= ( Monom&& other ) noexcept;
     Monom& operator*= ( const Monom& other );
     Monom& operator/= ( const Monom& other );
     bool operator== ( const Monom& other ) const;
     bool operator!= ( const Monom& other ) const;

     void remove_var( const std::string& var );
     void set_deg( const std::string& var, size_t deg );
     size_t var_deg( const std::string& var ) const;
     size_t full_deg() const;
     size_t var_count() const;
     bool is_divisible( const Monom& other ) const;
     const std::map< std::string, size_t >& get_vars() const;

private:
     std::map< std::string, size_t > vars_;
};


Monom operator* ( Monom lhs, const Monom& rhs );
Monom operator/ ( Monom lhs, const Monom& rhs );
Monom pow( const Monom base, size_t exp );
Monom lcm(const Monom& a, const Monom& b);

#endif // #ifndef MONOM_H
