// Stub implementations for PyCXX debug functions
// These are required but not provided in the minimal PyCXX distribution

#include "Exception.hxx"
#include "Objects.hxx"

namespace Py
{

// Provide validate() implementation
// In release mode, this does nothing
void Object::validate()
{
    // No-op in release builds
    // In debug builds, this would check reference counts, types, etc.
}

// Provide BaseException constructor
BaseException::BaseException()
{
    // Empty constructor - exception info comes from Python runtime
}

} // namespace Py
