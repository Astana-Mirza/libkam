#ifndef FINITE_AUTOMATON_H
#define FINITE_AUTOMATON_H

template <
     typename In,        // input type
     typename Out,       // output type
     typename State,     // state type
     typename TransFunc, // transition function
     typename OutFunc    // output function
>
class FiniteAutomaton
{
public:
     FiniteAutomaton( const State& init, const TransFunc& trf, const OutFunc& outf ) noexcept;
     FiniteAutomaton& operator =( const FiniteAutomaton& other ) noexcept;

     State get_state() const { return curr_state_; }
     Out step( const In& word );
     const TransFunc& transform_func() const { return transform_; }
     const OutFunc& output_func() const { return output_; }

protected:
	State curr_state_;
	TransFunc transform_;
	OutFunc output_;
};


// function for auto deducing types TransFunc and OutFunc
template < typename In, typename Out, typename State, typename TransFunc, typename OutFunc >
FiniteAutomaton<In, Out, State, TransFunc, OutFunc> make_automaton
(
          const State& init,
          const TransFunc& trf,
          const OutFunc& outf
)
{
     return { init, trf, outf };
}

//-----------------------------------------IMPLEMENTATION------------------------------------------

template < typename In, typename Out, typename State, typename TransFunc, typename OutFunc >
FiniteAutomaton< In, Out, State, TransFunc, OutFunc >::FiniteAutomaton
(
     const State& init,
     const TransFunc& trf,
     const OutFunc& outf
) noexcept :
curr_state_{ init }, transform_{ trf }, output_{ outf } {}


template < typename In, typename Out, typename State, typename TransFunc, typename OutFunc >
FiniteAutomaton< In, Out, State, TransFunc, OutFunc >&
FiniteAutomaton< In, Out, State, TransFunc, OutFunc >::operator =( const FiniteAutomaton& other ) noexcept
{
	if ( this == &other )
     {
		return *this;
     }
	curr_state_ = other.curr_state_;
	transform_  = other.transform_;
	output_     = other.output_;
	return *this;
}


template < typename In, typename Out, typename State, typename TransFunc, typename OutFunc >
Out FiniteAutomaton< In, Out, State, TransFunc, OutFunc >::step( const In& word )
{
	Out ret = output( word, curr_state_ );
	curr_state_ = transform( word, curr_state_ );
	return ret;
}

#endif // #ifndef FINITE_AUTOMATON_H
