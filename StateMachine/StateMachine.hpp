

#pragma once

// Top of function
#define STATE_MACHINE__BEGIN(state_var) switch(state_var){ case 0:
// Also INIT
#define STATE_MACHINE__RESET(state_var) { state_var = 0; }
#define STATE(state) case state:
#define STATE_MACHINE__CHECKPOINT(state_var, state) state_var = state; STATE(state)
#define STATE_MACHINE__CHECKPOINT_RETURN(state_var, state, return_val) state_var = state; return return_val; STATE(state)
#define STATE_MACHINE__AUTOCHECKPOINT(state_var) STATE_MACHINE__CHECKPOINT(state_var, __LINE__)
#define STATE_MACHINE__AUTOCHECKPOINT_RETURN(state_var, return_val) STATE_MACHINE__CHECKPOINT_RETURN(state_var, __LINE__, return_val)
// This point should never be reached in normal processing.
#define STATE_MACHINE__END(state_var) }  assert(false);

typedef uint16_t StateMachine;

