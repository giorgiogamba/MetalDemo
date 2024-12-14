## Apple/Metal framework and pipeline

All MacOS is based on the CoreFoundation, framework written in C and thus difficult to interact with.
They create a framework written in ObjectiveC to ease interaction, and then tehy created Swift which is another framework
on top of it to wrap objective C functions

Metal has the peculiarity that it doesn't use wrappers, thus we can invoke objectiveC functions directly as in Swift,
this because they are both based on C (they only differ on oop implemetations).
In reality, we have got the C== and ObjkectiveC specific implementations that both call the bridging header
written in pure C

Based on the fact that MACROS are simply aliases for bunch of code, and thus at compile time they are replaced
by the actual code, the INLINE keyword instructs C++ compiler to align the C++ function code. This means that we won't have
direct function calls (which brings to overhead) but we will have the function code replacing the method invokation instead -> no overhead
Working at 60fps it is very important

NOTE: If creating objects from 0, then the memory responbility is on the programmer
