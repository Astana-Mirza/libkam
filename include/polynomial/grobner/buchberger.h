#ifndef BUCHBERGER_H
#define BUCHBERGER_H

#include <polynomial/polynom.h>

#include <stdexcept>
#include <algorithm>
#include <set>

// a class to hide s_pol and buchberger_criteria functions
template < typename Polynom >
class Buchberger
{
public:
     static std::vector< Polynom > find_basis_brute_force( const std::vector< Polynom >& pols );
     static std::vector< Polynom > find_basis( const std::vector< Polynom >& pols );

private:
     static Polynom s_pol( const Polynom& f, const Polynom& g );
     static bool buchberger_criteria(
               const std::set< std::pair< size_t, size_t > >& all_pairs,
     		const std::pair< size_t, size_t >& index_pair,
     		const std::vector< Polynom >& current_basis );
};

//-----------------------------------------IMPLEMENTATION------------------------------------------

template < typename Polynom >
Polynom Buchberger< Polynom >::s_pol( const Polynom& f, const Polynom& g ) {
	if ( !f || !g )
     {
          throw std::runtime_error{ "cannot find S-polynomial with a zero" };
     }
	Monom common = lcm( f.leading_monom(), g.leading_monom() );
     auto one = f.leading_coeff() / f.leading_coeff();
     Polynom flt{ { { common / f.leading_monom(), one / f.leading_coeff() } } };
     Polynom glt{ { { common / g.leading_monom(), one / g.leading_coeff() } } };
     return flt * f - glt * g;
}


template < typename Polynom >
std::vector< Polynom > Buchberger< Polynom >::find_basis_brute_force( const std::vector< Polynom >& pols ) {
     std::set< std::pair< size_t, size_t > > all_pairs;
     std::vector< Polynom > current_basis{ pols };
     for ( size_t i = 0; i < current_basis.size() - 1; i++ )
     {
          for ( size_t j = i + 1; j < current_basis.size(); j++ )
          {
               all_pairs.insert( { i, j } );
          }
     }
     while ( !all_pairs.empty() )
     {
          auto it = all_pairs.begin();
          Polynom spol = s_pol( current_basis[ it->first ], current_basis[ it->second ] ).mod( current_basis );
          bool not_found = std::find( current_basis.cbegin(), current_basis.cend(), spol ) == current_basis.cend();
          if ( spol && not_found )
          {
               current_basis.push_back(spol);
               for ( size_t i = 0; i < current_basis.size() - 1; i++ )
               {
                    all_pairs.insert( { i, current_basis.size() - 1 } );
               }
          }
          all_pairs.erase( it );
     }
     return current_basis;
}


template < typename Polynom >
bool Buchberger< Polynom >::buchberger_criteria(
          const std::set< std::pair< size_t, size_t > >& all_pairs,
		const std::pair< size_t, size_t >& index_pair,
		const std::vector< Polynom >& current_basis )
{
     auto pair_lcm = lcm( current_basis[ index_pair.first  ].leading_monom(),
                          current_basis[ index_pair.second ].leading_monom() );

     for ( size_t i = 0; i < index_pair.first; i++ )   // search before first
     {
          auto found_first  = std::find( all_pairs.begin(), all_pairs.end(), std::make_pair( i, index_pair.first  ) );
          auto found_second = std::find( all_pairs.begin(), all_pairs.end(), std::make_pair( i, index_pair.second ) );
          bool div = pair_lcm.is_divisible( current_basis[ i ].leading_monom() );
          if ( found_first == all_pairs.end() && found_second == all_pairs.end() && div )
          {
               return true;
          }
     }
     for ( size_t i = index_pair.first + 1; i < index_pair.second; i++ )   // search between first and second
     {
          auto found_first  = std::find( all_pairs.begin(), all_pairs.end(), std::make_pair( index_pair.first,  i ) );
          auto found_second = std::find( all_pairs.begin(), all_pairs.end(), std::make_pair( i, index_pair.second ) );
          bool div = pair_lcm.is_divisible( current_basis[ i ].leading_monom() );
          if ( found_first == all_pairs.end() && found_second == all_pairs.end() && div )
          {
               return true;
          }
     }
     for ( size_t i = index_pair.second + 1; i < current_basis.size(); i++ )   // search after second
     {
          auto found_first  = std::find( all_pairs.begin(), all_pairs.end(), std::make_pair( index_pair.first,  i ) );
          auto found_second = std::find( all_pairs.begin(), all_pairs.end(), std::make_pair( index_pair.second, i ) );
          bool div = pair_lcm.is_divisible( current_basis[ i ].leading_monom() );
          if ( found_first == all_pairs.end() && found_second == all_pairs.end() && div )
          {
               return true;
          }
     }
     return false;
}


template < typename Polynom >
std::vector< Polynom > Buchberger< Polynom >::find_basis( const std::vector< Polynom >& pols )
{
     std::set< std::pair< size_t, size_t > > all_pairs;
     std::vector< Polynom > current_basis{ pols };
     for ( size_t i = 0; i < current_basis.size() - 1; i++ )
     {
          for ( size_t j = i + 1; j < current_basis.size(); j++ )
          {
               all_pairs.insert( { i, j } );
          }
     }
     while ( !all_pairs.empty() )
     {
          auto it = all_pairs.begin();
          auto mon1 = current_basis[ it->first  ].leading_monom();
          auto mon2 = current_basis[ it->second ].leading_monom();
          Polynom spol = s_pol( current_basis[ it->first ], current_basis[ it->second ] ).mod( current_basis );
          bool not_found = std::find( current_basis.cbegin(), current_basis.cend(), spol ) == current_basis.cend();
          bool not_coprime = lcm( mon1, mon2 ) != mon1 * mon2;
          if ( spol && not_found && not_coprime && !buchberger_criteria( all_pairs, *it, current_basis ) )
          {
               current_basis.push_back(spol);
               for ( size_t i = 0; i < current_basis.size() - 1; i++ )
               {
                    all_pairs.insert( { i, current_basis.size() - 1 } );
               }
          }
          all_pairs.erase( it );
     }
     return current_basis;
}


template < typename Polynom >
void reduce_basis( std::vector< Polynom >& pols ) {
     size_t i = 0;
     do
     {
          std::vector< Polynom > current;                   // divide each polynom by all others
          current.reserve( pols.size() - 1 );
          for ( size_t j = 0; j < i; j++ )
          {
               current.push_back( pols[ j ] );
          }
          for ( size_t j = i + 1; j < pols.size(); j++ )
          {
               current.push_back( pols[ j ] );
          }
		const Polynom& pol = pols[ i ];
		Polynom rem = pol.mod( current );
		if ( rem )
          {
               pols[ i ] = rem / rem.leading_coeff();
               i++;
          }
          else
          {
               pols.erase( pols.begin() + i );
          }
     } while ( i < pols.size() );
}

#endif	// #ifndef BUCHBERGER_H
