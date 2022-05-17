#ifndef CRYPTO_AUTOMATON_H
#define CRYPTO_AUTOMATON_H

#include <automaton/finite_automaton.h>

// crypto automaton is an automaton with a key
template <
     typename In,        // input type
     typename Out,       // output type
     typename State,     // state type
     typename TransFunc, // transition function
     typename OutFunc,   // output function
     typename KeyType    // key of crypto automaton
>
class CryptoAutomaton : public FiniteAutomaton< In, Out, State, TransFunc, OutFunc >
{
public:
     CryptoAutomaton( const State& init, const KeyType& init_key, const TransFunc& trf, const OutFunc& outf ) noexcept;
     CryptoAutomaton& operator =( const CryptoAutomaton& other ) noexcept;

     Out step( const In& word );
     KeyType get_key() const { return key_; }
private:
     KeyType key_;
};

// function for auto deducing types TransFunc and OutFunc
template <typename In, typename Out, typename KeyType, typename State, typename TransFunc, typename OutFunc >
CryptoAutomaton< In, Out, State, TransFunc, OutFunc, KeyType > make_crypto
(
          const State& init,
          const KeyType& init_key,
          const TransFunc& trf,
          const OutFunc& outf
)
{
     return { init, init_key, trf, outf };
}

//-----------------------------------------IMPLEMENTATION------------------------------------------

template < typename In, typename Out, typename State, typename TransFunc, typename OutFunc, typename KeyType >
CryptoAutomaton< In, Out, State, TransFunc, OutFunc, KeyType >::CryptoAutomaton
(
          const State& init,
          const KeyType& init_key,
          const TransFunc& trf,
          const OutFunc& outf
) noexcept :
FiniteAutomaton< In, Out, State, TransFunc, OutFunc >::FiniteAutomaton( init, trf, outf ), key_{ init_key } {}


template < typename In, typename Out, typename State, typename TransFunc, typename OutFunc, typename KeyType >
CryptoAutomaton< In, Out, State, TransFunc, OutFunc, KeyType >&
CryptoAutomaton< In, Out, State, TransFunc, OutFunc, KeyType >::operator =( const CryptoAutomaton& other ) noexcept
{
     if ( this == &other )
     {
          return *this;
     }
	this->curr_state_ = other.curr_state_;
	this->transform_  = other.transform_;
	this->output_     = other.output_;
	this->key_        = other.key_;
	return *this;
}


template < typename In, typename Out, typename State, typename TransFunc, typename OutFunc, typename KeyType >
Out CryptoAutomaton< In, Out, State, TransFunc, OutFunc, KeyType >::step( const In& word )
{
     Out ret = this->output_( word, this->curr_state_, this->key_ );
     this->curr_state_ = this->transform_( word, this->curr_state_, this->key_ );
     return ret;
}

#endif // #ifndef CRYPTO_AUTOMATON_H
