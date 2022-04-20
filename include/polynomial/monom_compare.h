#ifndef MONOM_COMPARE_H
#define MONOM_COMPARE_H

class Monom;

struct LexGreater
{
     bool operator() ( const Monom& lhs, const Monom& rhs ) const;
};


struct InvlexGreater
{
     bool operator() ( const Monom& lhs, const Monom& rhs ) const;
};


struct GrlexGreater
{
     bool operator() ( const Monom& lhs, const Monom& rhs ) const;
};


struct GrevlexGreater
{
     bool operator() ( const Monom& lhs, const Monom& rhs ) const;
};


struct RinvlexGreater
{
     bool operator() ( const Monom& lhs, const Monom& rhs ) const;
};

#endif // #ifndef MONOM_COMPARE_H
