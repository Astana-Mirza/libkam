#include <polynomial/monom_compare.h>
#include <polynomial/monom.h>

bool LexGreater::operator() ( const Monom& lhs, const Monom& rhs ) const
{
     auto iter_l = lhs.get_vars().cbegin();
     auto iter_r = rhs.get_vars().cbegin();
     // monom is greater if its first variable is less
	while ( iter_l != lhs.get_vars().cend() && iter_r != rhs.get_vars().cend() )
     {
		int cmp = ( iter_l->first ).compare( iter_r->first );
		if ( cmp != 0 )
          {
			return cmp < 0;
          }
		if ( iter_l->second != iter_r->second )    // degree comprasion
          {
               return iter_l->second > iter_r->second;
          }
		++iter_l;
          ++iter_r;
	}
     if ( iter_l != lhs.get_vars().cend() && iter_r == rhs.get_vars().cend() )
     {
          return true;
     }
     if ( iter_l == lhs.get_vars().cend() && iter_r != rhs.get_vars().cend() )
     {
          return false;
     }
	return false; // equal
}


bool InvlexGreater::operator() ( const Monom& lhs, const Monom& rhs ) const
{
     auto iter_l = lhs.get_vars().crbegin();
     auto iter_r = rhs.get_vars().crbegin();
     // monom is greater if its last variable is greater
     while ( iter_l != lhs.get_vars().crend() && iter_r != rhs.get_vars().crend() )
     {
          int cmp = ( iter_l->first ).compare( iter_r->first );
          if ( cmp != 0 )
          {
               return cmp > 0;
          }
          if ( iter_l->second != iter_r->second )    // degree comprasion
          {
               return iter_l->second > iter_r->second;
          }
          ++iter_l;
          ++iter_r;
     }
     if ( iter_l != lhs.get_vars().crend() && iter_r == rhs.get_vars().crend() )
     {
          return true;
     }
     if ( iter_l == lhs.get_vars().crend() && iter_r != rhs.get_vars().crend() )
     {
          return false;
     }
     return false; // equal
}


bool GrlexGreater::operator() ( const Monom& lhs, const Monom& rhs ) const
{
     size_t deg_l = lhs.full_deg(), deg_r = rhs.full_deg();
     if ( deg_l != deg_r )
     {
          return deg_l > deg_r;
     }
     return LexGreater{}( lhs, rhs );
}


bool GrevlexGreater::operator() ( const Monom& lhs, const Monom& rhs ) const
{
     size_t deg_l = lhs.full_deg(), deg_r = rhs.full_deg();
     if ( deg_l != deg_r )
     {
          return deg_l > deg_r;
     }
     return RinvlexGreater{}( lhs, rhs );
}


bool RinvlexGreater::operator() ( const Monom& lhs, const Monom& rhs ) const
{
     return InvlexGreater{}( rhs, lhs ); // arguments swapped
}
