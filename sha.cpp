#include <sha.hpp>
class SHA256
{
public:
    SHA256(std::string message)
    {
        Pad(message);
        Update(message);
    }
}